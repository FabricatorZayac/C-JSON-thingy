#ifndef FILE_OPS_H_
#define FILE_OPS_H_

#include <stdio.h>
#include <stdlib.h>

void write_to_file(char *filename, char *string);
char *read_from_file(char *filename);

#endif // FILE_OPS_H_
