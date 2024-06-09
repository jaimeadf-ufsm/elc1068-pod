#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

bool is_size(char *str);

bool parse_bool_or_exit(char *str);
int parse_int_or_exit(char *str);
long long parse_long_long_or_exit(char *str);
long long parse_size_or_exit(char *str);

#endif