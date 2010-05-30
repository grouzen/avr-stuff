#ifndef __LCD_H__
#define __LCD_H__

/* Commands. */
#define C_INTERFACE_8B2L 0x38
#define C_INTERFACE_8B1L 0x30
#define C_INTERFACE_4B2L 0x28
#define C_INTERFACE_4B1L 0x20
#define C_SCROLL_R       0x1e
#define C_SCROLL_L       0x18
#define C_MOVE_HOME      0x2
#define C_MOVE_R         0x14
#define C_MOVE_L         0x10
#define C_CURSOR_UNDER   0x0e
#define C_CURSOR_BLOCK   0x0f
#define C_CURSOR_INVIS   0x0c
#define C_DISPLAY_BLANK  0x08
#define C_DISPLAY_REST   0x0c
#define C_SCREEN_CLEAR   0x01
#define C_SET_DDRAM_POS(addr) (0x80 + (addr))
#define C_SET_CGRAM_POS(addr) (0x40 + (addr))
#define C_ENTRYMODE_I0D1S0 0x04
#define C_ENTRYMODE_I0D1S1 0x05
#define C_ENTRYMODE_I1D0S0 0x06
#define C_ENTRYMODE_I1D0S1 0x07

/* Must be changed for each board. */
#ifdef XSCIENCE_ATM8
#define DB0 -1   /* 7 */
#define DB1 -1   /* 8 */
#define DB2 -1   /* 9 */
#define DB3 -1   /* 10 */
#define DB4 PC0  /* 11 */
#define DB5 PC1  /* 12 */
#define DB6 PC2  /* 13 */
#define DB7 PC3  /* 14 */

#define RS  PD2  /* 4 */
#define RW  -1   /* 5 */
#define E   PB0  /* 6 */

#define SET_IO(v)                               \
    do {                                        \
        DDRC = ((v) << DDC0) | ((v) << DDC1) |  \
            ((v) << DDC2) | ((v) << DDC3);      \
        if(!(v)) {                              \
            PORTC = (1 << PC0) | (1 << PC1) |   \
                (1 << PC2) | (1 << PC3);        \
        }                                       \
    } while(0)

#define SET_O_STATE_E_RS(v)                      \
    do {                                         \
        DDRB = (1 << E);                         \
        DDRD = (1 << RS);                        \
        PORTB = ((v) << E);                      \
        PORTD = ((v) << RS);                     \
    } while(0)

#define SET_E(v) (PORTB = ((v) << E))
#define SET_RS(v) (PORTD = ((v) << RS))
#define SET_RW(v)

#define WRITE_DB(byte)                                                  \
    do {                                                                \
        PORTC = ((byte) >> 4);                                          \
        SET_E(1);                                                       \
        SET_E(0);                                                       \
        PORTC = (byte);                                                 \
        SET_E(1);                                                       \
        SET_E(0);                                                       \
    } while(0)

#define READ_DB(byte)                           \
    do {                                        \
        SET_RW(1);                              \
        SET_E(1);                               \
        (byte) = PORTC;                         \
        SET_E(0);                               \
        SET_E(1);                               \
        (byte) |= (PORTC >> 4);                 \
        SET_E(0);                               \
        SET_RW(0);                              \
    } while(0)

#endif /* XSCIENCE_ATM8 */

char lcd_read_byte(void);
void lcd_write_byte(char);
void lcd_put_char(char, uint8_t);
void lcd_init(void);

#endif /* __LCD_H__ */
