#ifndef shared_H_
#define shared_H_

#include <stdbool.h>


char *strdup(const char *);


int get_flags(int argc, char ** argv);
bool has_flag(char);
bool is_flag(const char *);
int warn_invalid_flags(const char *);


#endif /* shared_H_ */
