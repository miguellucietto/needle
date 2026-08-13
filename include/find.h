#ifndef find_H_
#define find_H_

#include <stdio.h>


char **get_file_lines(FILE *);
int find_in(const char *needle, const char *file);

#endif /* find_H_ */
