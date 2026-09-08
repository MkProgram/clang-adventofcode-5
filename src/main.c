#include "find_ingredience.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  puts("Finding spoiled ingredience\n");
  const char *floc = "puzzle.txt";

  FILE *file = fopen(floc, "r");

  if (file == NULL) {
    fprintf(stderr, "Could not process file: %s", floc);
    return EXIT_FAILURE;
  }

  uintmax_t fresh_count = 0;
  size_t count = get_spoiled_ingredience(file, &fresh_count);

  fclose(file);

  printf("Fresh ingredients: %zu\n", count);
  printf("Amount of total fresh ingredients: %" PRIuMAX "\n", fresh_count);

  return EXIT_SUCCESS;
}
