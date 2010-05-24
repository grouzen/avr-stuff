#define F_CPU 1000000UL
#include <util/delay.h>
#include <inttypes.h>
#include <avr/io.h>

#include "lcd.h"

void lcd_write_byte(char b)
{
#if 0
    /* As the BF flag is not checked. Delay about 5 ms. */
    _delay_ms(5);
#endif
    
    /* Configure port for output. */
    SET_IO(1);

    /* Write one byte. */
    WRITE_DB(b);

    /* Configure port for HiZ input. */
    SET_IO(0);

    return;
}

void lcd_init(void)
{
    /* Set lines of the E and RS to output state and 0 level. */
    SET_O_STATE_E_RS(0);

    /* Init sequence recommended by manufacturer. */
    _delay_ms(15);
    SET_RS(0); /* Select IR. */
    lcd_write_byte(0x30);   
    _delay_ms(5);
    lcd_write_byte(0x30);
    _delay_us(100);
    lcd_write_byte(0x30);
    /* End of init sequence. */

    /* LCD configurations. */
    /* 2-line mode. */
    _delay_ms(5);
    lcd_write_byte(0x28);
    /* Clear screen. */
    _delay_ms(5);
    lcd_write_byte(0x0c);
    /* Direction left to right. */
    _delay_ms(5);
    lcd_write_byte(0x06);
    
    return;
}

int main(void)
{    
    lcd_init();

    SET_RS(1);
    _delay_ms(5);
    lcd_write_byte(0x6c);
    _delay_ms(5);
    lcd_write_byte(0x6f);
    _delay_ms(5);
    lcd_write_byte(0x6f);
    _delay_ms(5);
    lcd_write_byte(0x73);
    _delay_ms(5);
    lcd_write_byte(0x65);
    _delay_ms(5);
    lcd_write_byte(0x72);
    _delay_ms(5);
    lcd_write_byte(0x73);
    _delay_ms(5);
    lcd_write_byte(0x40);

    return 0;
}
