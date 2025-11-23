#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_init(void);
char keyboard_read_char(void);
int keyboard_has_data(void);
void keyboard_handler(void);

#endif