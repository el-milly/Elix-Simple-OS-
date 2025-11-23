#include "../../include/drivers/keyboard.h"
#include "../../include/kernel/io.h"
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_CMD_PORT 0x64
#define KEY_BUFFER_SIZE 256

static char key_buffer[KEY_BUFFER_SIZE];
static int buffer_start = 0;
static int buffer_end = 0;
static int buffer_count = 0;

static void buffer_add(char c) {
    if (buffer_count >= KEY_BUFFER_SIZE) {
        buffer_start = (buffer_start + 1) % KEY_BUFFER_SIZE;
        buffer_count--;
    }
    
    key_buffer[buffer_end] = c;
    buffer_end = (buffer_end + 1) % KEY_BUFFER_SIZE;
    buffer_count++;
}

static char buffer_read(void) {
    if (buffer_count == 0) {
        return 0;
    }

    char c = key_buffer[buffer_start];
    buffer_start = (buffer_start + 1) % KEY_BUFFER_SIZE;
    buffer_count--;
    return c;
}

static char scan_code_to_char(uint8_t scan_code) {
    static const char scan_code_map[] = {
        0,  0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,  0,
        'q','w','e','r','t','y','u','i','o','p','[',']',0,   0,  'a','s',
        'd','f','g','h','j','k','l',';','\'','`',0,   '\\','z','x','c','v',
        'b','n','m',',','.','/',0,   '*',0,   ' ',0,   0,   0,   0,   0,   0,   0
    };

    if (scan_code < sizeof(scan_code_map)) {
        return scan_code_map[scan_code];
    }
    return 0;
}

void keyboard_handler(void) {
    if(inb(KEYBOARD_STATUS_PORT) & 0x01){
        uint8_t scan_code = inb(KEYBOARD_DATA_PORT);

        if (scan_code < 0x80){
            char character = scan_code_to_char(scan_code);
            if(character !=0){
                buffer_add(character);
            }
        }
    }
}

void keyboard_init(void) {
    buffer_start = 0;
    buffer_end = 0;
    buffer_count = 0;
    
}

char keyboard_read_char(void) {
    return buffer_read();
}

int keyboard_has_data(void) {
    return buffer_count > 0;
}