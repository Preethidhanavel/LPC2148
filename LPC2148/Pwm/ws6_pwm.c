#include <lpc21xx.h>
#include"lcd1.h"


int main(void)
{
  int value,i;

    PINSEL0 |=0x00000008; /* Configure P0.1 as PWM3 */
    PWMTCR = 0x02; /* Reset and disable counter for PWM */
    PWMPR = 0x1D; /* 29 Prescale Register value  */
    PWMMR0 = 20000; /* Time period f PWM wave, 20msec */
    PWMMR3 = 1000; /* Ton of PWM wave 1 msec */
    PWMMCR = 0x00000002; /* Reset on MR0 match, MR3 match */
    PWMLER = 0x09; /* Latch enable for PWM3 and PWM0 */
    PWMPCR = 0x0800; /* Enable PWM3 and PWM 0, single edge controlled PWM */
    PWMTCR = 0x09; /* Enable PWM and counter */

  while (1)
    {
	   for(i=0; i<1000; i+=10)
	   {
				value=i;
        PWMMR3 = value; 
        PWMLER = 0x08;
				delay_ms(500); 
    }
		}
 }
 

