#include "file_ops.h"

void write_to_file(char *filename, char *string) {
    FILE *outfile = fopen(filename, "w");
    fprintf(outfile, "%s", string);
    fclose(outfile);
}

char *read_from_file(char *filename) {
    char *buffer = malloc(256);
    FILE *infile = fopen(filename, "r");
    fscanf(infile, "%s", buffer);
    fclose(infile);

    return buffer;
}
