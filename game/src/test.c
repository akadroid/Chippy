#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"

int main()
{
    FILE *file;
    file = fopen("ibm.ch8", "rb");
    if (file == NULL) {
        printf("Error while opening file\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
	long bufsize = ftell(file);
	char *buffer = (char *) calloc(bufsize + 1, sizeof(char));
	rewind(file);

    size_t len = fread(buffer, sizeof(char), bufsize, file);
	if (ferror(file) != 0) {
		fputs("Error reading file", stderr);
		exit(69);
	} else {
		buffer[len++] = '\0';
	}
	fclose(file);

    long memory[4000];

    for (long i = 0; i < bufsize; i++) {
        memory[i] = buffer[i];
        printf("%i", memory[i]);
    }
    free(buffer);
    printf("hello");
    return 0;
}