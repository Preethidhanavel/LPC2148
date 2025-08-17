#include <lpc214x.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "timer.h"

uint8_t alarm, flag;  // Flags for alarm and second tick interrupt

// RTC Interrupt Service Routine
__irq void RTC_ISR(void)
{
    if (ILR & 0x01)         // If second tick interrupt occurred
    {
        flag = 1;           // Set second tick flag
        ILR = ILR | 0x01;   // Clear interrupt
    }
    if (ILR & 0x02)         // If alarm interrupt occurred
    {
        alarm = 1;          // Set alarm flag
        ILR = ILR | 0x02;   // Clear interrupt
    }
    VICVectAddr = 0;        // Acknowledge interrupt in VIC
}

// RTC time structure
typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day_of_month;
    uint8_t day_of_week;
    uint16_t day_of_year;
    uint8_t month;
    uint16_t year;
} RTC_Time;

// Set RTC current time
void RTC_Set_Time(RTC_Time Set_Time)
{
    SEC = Set_Time.seconds;
    MIN = Set_Time.minutes;
    HOUR = Set_Time.hours;
    DOM = Set_Time.day_of_month;
    DOW = Set_Time.day_of_week;
    DOY = Set_Time.day_of_year;
    MONTH = Set_Time.month;
    YEAR = Set_Time.year;
}

// Set RTC alarm time
void RTC_Set_Alarm_Time(RTC_Time Alarm_Time)
{
    ALSEC = Alarm_Time.seconds;
    ALMIN = Alarm_Time.minutes;
    ALHOUR = Alarm_Time.hours;
    ALDOM = Alarm_Time.day_of_month;
    ALDOW = Alarm_Time.day_of_week;
    ALDOY = Alarm_Time.day_of_year;
    ALMON = Alarm_Time.month;
    ALYEAR = Alarm_Time.year;
}

// Read current RTC time
RTC_Time RTC_Get_Time(void)
{
    RTC_Time time;
    time.seconds = SEC;
    time.minutes = MIN;
    time.hours = HOUR;
    time.day_of_month = DOM;
    time.day_of_week = DOW;
    time.day_of_year = DOY;
    time.month = MONTH;
    time.year = YEAR;
    return time;
}

int main(void)
{
    /* Variables for setting and reading time */
    RTC_Time set_time, alarm_time, current_time;
    char timestr[30], datestr[30];
    uint8_t i;

    alarm = 0;
    flag = 0;
    IO0DIR = 0x00000010;      // Set P0.4 as output (LED)
    uart0_config();            // Initialize UART0

    /* RTC initialization */
    ILR = 0x0;                 // Clear RTC interrupt flags
    CCR = 0x02;                 // Reset RTC counters
    CCR = 0x00;
    CIIR = 0x00;                // Disable increment interrupts
    AMR = 0x00;                 // No masking of alarm registers
    PREINT = 0x01C8;            // Prescaler integer value for 15MHz PCLK
    PREFRAC = 0x61C0;           // Prescaler fractional value for 15MHz PCLK

    /* VIC setup for RTC interrupt */
    VICVectAddr0 = (unsigned) RTC_ISR;
    VICVectCntl0 = 0x0000002D; // Assign RTC to vector 0
    VICIntEnable = 0x00002000; // Enable RTC interrupt
    VICIntSelect = 0x00000000; // Select IRQ

    /* Set current time */
    set_time.seconds = 0;
    set_time.minutes = 25;
    set_time.hours = 11;
    set_time.day_of_month = 15;
    set_time.day_of_week = 3;
    set_time.day_of_year = 196;
    set_time.month = 7;
    set_time.year = 2025;

    RTC_Set_Time(set_time);

    CIIR = 0x01; // Enable second tick interrupt

    /* Set alarm time */
    alarm_time.seconds = 15;
    alarm_time.minutes = 25;
    alarm_time.hours = 11;
    alarm_time.day_of_month = 6;
    alarm_time.day_of_week = 5;
    alarm_time.day_of_year = 279;
    alarm_time.month = 10;
    alarm_time.year = 2017;
    RTC_Set_Alarm_Time(alarm_time);

    CCR = 0x01; // Enable RTC clock
    ILR = 0x03; // Enable RTC interrupts
    IO0CLR = 0x00000010; // Turn off LED initially

    /* Main loop */
    while(1)
    {
        if(alarm == 1)   // If alarm occurred
        {
            current_time = RTC_Get_Time();
            sprintf(timestr,"Alarm!!!: %d:%d:%d \r\n",current_time.hours,current_time.minutes,current_time.seconds);
            uart_string(timestr); // Print alarm time

            // Blink LED 10 times
            for(i = 0; i < 10; i++)
            {
                IO0SET = 0x00000010;
                delay_ms(300);
                IO0CLR = 0x00000010;
                delay_ms(300);
            }
            alarm = 0; // Reset alarm flag
        }

        if(flag == 1)   // If second tick occurred
        {
            current_time = RTC_Get_Time();
            sprintf(timestr,"Time: %d:%d:%d ",current_time.hours,current_time.minutes,current_time.seconds);
            sprintf(datestr,"Date: %d/%d/%d \r\n",current_time.day_of_month,current_time.month,current_time.year);
            uart_string(timestr); // Print current time
            uart_string(datestr); // Print current date
            flag = 0; // Reset second tick flag
        }
    }
}
