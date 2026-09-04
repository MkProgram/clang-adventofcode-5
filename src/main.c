#include "find_ingredience.h"
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

  size_t count = get_spoiled_ingredience(file);

  fclose(file);

  printf("Fresh ingredients: %zu\n", count);

  return EXIT_SUCCESS;
}
