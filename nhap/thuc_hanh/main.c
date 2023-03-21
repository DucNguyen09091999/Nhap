#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
void sig_handler(int num)
{
    printf("Khong the ket thuc truoc khi hoan\n");
    exit(EXIT_SUCCESS);
}

int main()
{
    if(signal(SIGINT, sig_handler) == SIG_ERR) {
        fprintf(stderr, "Cannot handle SIGINT\n");
        exit(EXIT_FAILURE);
	}

    while (1)
    {
        /* code */
    }
    

    return 0;
}