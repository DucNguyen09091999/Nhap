#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "device-reader.h"

void parse_event_handler(char *events, char *result)
{
    char *last = NULL;
    char *token = NULL;

    token = strtok(events, " ");
    while( token != NULL ) {
        if (strstr(token, "event")) {
            last = token;
            break;
        }
        token = strtok(NULL, " ");
    }

    memcpy(result, last, sizeof(result));
}

void get_keyboard_event(char *buf, size_t size)
{
    FILE *fp = NULL;
    ssize_t read = 0;
    char *line = NULL;
    size_t len = 0;
    char foundEvent[10] = {0};
    bool foundKeyboard = false;


    if ((fp = fopen("/proc/bus/input/devices", "r")) == NULL) {
        fprintf(stderr, "Unable to read /proc/bus/input/devices");
        exit(EXIT_FAILURE);
    }


    while ((read = getline(&line, &len, fp)) != -1) {
        // Remove \n character
        line[strlen(line) - 1] = '\0';
        if (strstr(line, "keyboard") != NULL) {
            foundKeyboard = true;
        }
        if (foundKeyboard && strstr(line, "H: Handlers") != NULL) {
            parse_event_handler(line, foundEvent);
            free(line);
            snprintf(buf, size, "/dev/input/%s", foundEvent);
            return;
        }
    }
    free(line);
    buf = NULL;
}

// Only support some keys because I'm too lazy T_T
char input_event_to_ascii(struct input_event ie) {
    switch (ie.code) {
    case KEY_ESC: return '\x1b';
    case KEY_1: return '1';
    case KEY_2: return '2';
    case KEY_3: return '3';
    case KEY_4: return '4';
    case KEY_5: return '5';
    case KEY_6: return '6';
    case KEY_7: return '7';
    case KEY_8: return '8';
    case KEY_9: return '9';
    case KEY_0: return '0';
    case KEY_MINUS: return '-';
    case KEY_EQUAL: return '=';
    case KEY_BACKSPACE: return '\x08';
    case KEY_TAB: return '\t';
    case KEY_Q: return 'q';
    case KEY_W: return 'w';
    case KEY_E: return 'e';
    case KEY_R: return 'r';
    case KEY_T: return 't';
    case KEY_Y: return 'y';
    case KEY_U: return 'u';
    case KEY_I: return 'i';
    case KEY_O: return 'o';
    case KEY_P: return 'p';
    case KEY_A: return 'a';
    case KEY_S: return 's';
    case KEY_D: return 'd';
    case KEY_F: return 'f';
    case KEY_G: return 'g';
    case KEY_H: return 'h';
    case KEY_J: return 'j';
    case KEY_K: return 'k';
    case KEY_L: return 'l';
    case KEY_Z: return 'z';
    case KEY_X: return 'x';
    case KEY_C: return 'c';
    case KEY_V: return 'v';
    case KEY_B: return 'b';
    case KEY_N: return 'n';
    case KEY_M: return 'm';
    }
    return '-';
}

void debug_input_event(struct input_event ie) {
    printf("sec: %ld\n", ie.__sec);
    printf("usec: %ld\n", ie.__usec);
    printf("type: %d\n", ie.type);
    printf("code: %d\n", ie.code);
    printf("value: %d\n", ie.value);
    puts("\n");
}