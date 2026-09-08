#include "find_ingredience.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct ingredient_file_case {
  char *name;
  char *floc;
  size_t expected;
  uintmax_t expected2;
};

int test_find_ingredients(void) {
  int failures = 0;

  struct ingredient_file_case cases[] = {
      {"Example file", "tests/example.txt", 3, 14}};

  for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    FILE *file = fopen(cases[i].floc, "r");
    if (file == NULL) {
      ++failures;
      fprintf(stderr, "Can't open file \"%s\"\n", cases[i].floc);
    }
    uintmax_t actual2 = 0;
    size_t actual = get_spoiled_ingredience(file, &actual2);

    if (actual != cases[i].expected) {
      ++failures;
      fprintf(stderr, "FAILURE: %s: Expected %zu, found %zu\n", cases[i].name,
              cases[i].expected, actual);
    }
    if (actual2 != cases[i].expected2) {
      ++failures;
      fprintf(stderr,
              "FAILURE: %s: Expected %" PRIuMAX ", found %" PRIuMAX "\n",
              cases[i].name, cases[i].expected2, actual2);
    }
    fclose(file);
  }

  return failures;
}

int main(void) {
  int failures = 0;

  failures += test_find_ingredients();

  if (failures == 0) {
    printf("All tests pass!\n");
    return EXIT_SUCCESS;
  }

  fprintf(stderr, "Found %d failures!\n", failures);
  return EXIT_FAILURE;
}
