#include <stdio.h>

int main() {
FILE *filepointer = fopen("test.txt", "r");
int charac = getc(filepointer);

while (charac != EOF) {
putchar(charac);
charac = getc(filepointer); }

if (feof(filepointer))
{ printf("\n Program has reached End-of-file"); }
else
{ printf("\n There was a problem"); }
fclose(filepointer);
getchar();
return 0;
}