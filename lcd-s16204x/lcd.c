#define F_CPU 1000000UL
#include <util/delay.h>
#include <inttypes.h>
#include <avr/io.h>

#include "lcd.h"

/* It's not tested yet. */
char lcd_read_byte(void)
{
    char b = 0;
    
    SET_IO(0);
    READ_DB(b);

    return b;    
}

void lcd_write_byte(char b)
{
    /* Configure port for output. */
    SET_IO(1);
    /* Write one byte. */
    WRITE_DB(b);
    /* Configure port for HiZ input. */
    SET_IO(0);

    return;
}

/* BUG:
   It's impossible to put symbol into address
   greater than 16(40 + 1).
*/
void _lcd_put_char(char c, uint8_t addr)
{
    /* Attention:
       One line contains 40 symbols,
       thus for putting symbol in second line,
       addr = 40 + addr.
    */
    SET_RS(0);
    _delay_ms(5);
    lcd_write_byte(C_SET_DDRAM_POS(addr));
    SET_RS(1);
    _delay_ms(5);
    lcd_write_byte(c);

    return;
}

void lcd_put_char(char c, uint8_t addr)
{
    /* addr must be: 0 <= addr < 32. */
    if(addr < 0 || addr > 31)
        return;

    if(addr > 15) {
        addr = 40 + (addr - 16);
    }

    _lcd_put_char(c, addr);

    return;
}

/* Receives null-terminating string
   and puts it to the screen.
*/
void lcd_put_string(char *s, uint8_t addr)
{
    uint8_t i;
    
    for(i = 0; s[i] != '\0'; i++) {
        lcd_put_char(s[i], addr + i);
    }

    return;
}

/* Like the lcd_put_string but with delaying in ms. */
void lcd_put_string_delay(char *s, uint8_t addr, double delay)
{
    uint8_t i;
    for(i = 0; s[i] != '\0'; i++) {
        _delay_ms(delay);
        lcd_put_char(s[i], addr + i);
    }

    return;
}

void lcd_clear_screen(void)
{
    SET_RS(0);
    _delay_ms(5);
    lcd_write_byte(C_SCREEN_CLEAR);

    return;
}

void lcd_init(void)
{
    /* Set lines of the E and RS to output state and 0 level. */
    SET_O_STATE_E_RS(0);

    /* Init sequence recommended by manufacturer. */
    _delay_ms(15);
    SET_RS(0); /* Select IR. */
    lcd_write_byte(C_INTERFACE_8B1L);   
    _delay_ms(5);
    lcd_write_byte(C_INTERFACE_8B1L);
    _delay_us(100);
    lcd_write_byte(C_INTERFACE_8B1L);
    /* End of init sequence. */

    /* LCD configurations. */
    /* 2-line mode. */
    _delay_ms(5);
    lcd_write_byte(C_INTERFACE_4B2L);
    /* Clear screen. */
    _delay_ms(5);
    lcd_write_byte(C_DISPLAY_REST);
    /* Direction left to right. */
    _delay_ms(5);
    lcd_write_byte(C_ENTRYMODE_I1D0S0);
    
    return;
}

int main(void)
{    
    lcd_init();

    while(1) {
        lcd_put_string_delay("GNU is not Unix", 0, 200);
        _delay_ms(200);
        lcd_clear_screen();
    }
    
    return 0;
}
