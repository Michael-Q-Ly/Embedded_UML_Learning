#ifndef LCD_H_
#define LCD_H_
#include "main.h"

extern void lcd_clear                   ( void ) ;
extern void lcd_print_char              ( char c ) ;
extern void lcd_scroll_left             ( void ) ;
extern void lcd_scroll_right            ( void ) ;
extern void lcd_set_cursor              ( int r, int c ) ;
extern void lcd_no_auto_scroll          ( void ) ;
extern void lcd_begin                   ( void ) ;
extern void lcd_move_cursor_R_to_L      ( void ) ;
extern void lcd_move_cursor_L_to_R      ( void ) ;
extern void lcd_cursor_off              ( void ) ;
extern void lcd_blinkoff                ( void ) ;
extern void lcd_insert_number           ( int num ) ;
extern void lcd_print_string            ( String s ) ;

#endif  /* LCD_H_ */