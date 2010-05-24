#ifndef __LCD_H__
#define __LCD_H__

/* Must be changed for each board. */
#ifdef XSCIENCE_ATM8
#define DB0 NULL /* 7 */
#define DB1 NULL /* 8 */
#define DB2 NULL /* 9 */
#define DB3 NULL /* 10 */
#define DB4 PC0  /* 11 */
#define DB5 PC1  /* 12 */
#define DB6 PC2  /* 13 */
#define DB7 PC3  /* 14 */

#define RS  PD2  /* 4 */
#define RW  NULL /* 5 */
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

#define WRITE_DB(byte)                                                  \
    do {                                                                \
        PORTC = ((byte) >> 4);                                          \
        SET_E(1);                                                       \
        SET_E(0);                                                       \
        PORTC = (byte);                                                 \
        SET_E(1);                                                       \
        SET_E(0);                                                       \
    } while(0)

#endif /* XSCIENCE_ATM8 */
#endif /* __LCD_H__ */
