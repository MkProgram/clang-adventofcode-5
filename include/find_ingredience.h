#ifndef FIND_INGREDIENCE_H
#define FIND_INGREDIENCE_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
  uintmax_t start;
  uintmax_t end;
} IdRange;

size_t get_spoiled_ingredience(FILE *file);

#endif // !FIND_INGREDIENCE_H
