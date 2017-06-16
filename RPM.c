/*
 * RPM.c
 *
 * Created: 2017-06-15 오전 9:41:09
 * Author: Continue
 */

#include <mega128.h>
#asm
    .equ __lcd_port = 0x1b
#endasm
#include <lcd.h>
#include <stdio.h>

char sbuf[16];
char nbuf[16];
char Dir;
unsigned int Encoder;
unsigned int t_cnt, RPM;

volatile unsigned long revTick;      // Ticks per revolution
volatile unsigned long revCtr;           // Total elapsed revolutions


interrupt [TIM1_CAPT] void timer_capt_isr(void){
	revTick = ICR1;  
	TCNT1 = 0;       
	revCtr++;  
}

interrupt [TIM1_OVF] void timer0_ovf_isr(void){ 
	revTick = 0; 
}

void main(void)
{
    DDRA = 0xff;
    DDRF = 0x01;
	DDRD = 0x00;
    DDRB.6 = 1;
    DDRB.7 = 1;

	TCCR1A = 0;   
	TCCR1B = 68; 
	TCCR1C = 0;  
	TIMSK = 33; 

	TCNT1 = 0;    

    #asm("sei");

    PORTF = 0x01;
    PORTB.6 = 0;
    PORTB.7 = 1;

    lcd_init(16);
    lcd_gotoxy(0,0);
    lcd_puts("Dir:");  //Dir이 0이면 시계 반대, 1이면 시계방향

    lcd_gotoxy(0,1);
    lcd_puts("RPM:");
    while (1){
		if (revTick > 0){
			RPM = 3456000 / revTick;
		} else{
			RPM = 0;
		}
        lcd_gotoxy(5,0);
        sprintf(sbuf, "%d", Dir);
        lcd_puts(sbuf);
        lcd_gotoxy(5,1);
        sprintf(nbuf, "%3d", RPM);
        lcd_puts(nbuf);
    }
}