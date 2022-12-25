#include "file_ops.h"
#include <stdio.h>
#include <stdlib.h>

void write_to_file(char *filename, char *string) {
    FILE *outfile = fopen(filename, "w");
    fprintf(outfile, "%s", string);
    fclose(outfile);
}

char *read_from_file(char *filename) {
    FILE *infile = fopen(filename, "r");

    fseek(infile, 0, SEEK_END);
    size_t size = ftell(infile);
    rewind(infile);
    char *buffer = malloc(size + 1);
    fread(buffer, size, 1, infile);
    buffer[size] = '\0';

    fclose(infile);
    return buffer;
}
