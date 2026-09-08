#include "find_ingredience.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

IdRange create_range_from_line(char *line) {
  uintmax_t start = 0;
  uintmax_t end = 0;

  if (sscanf(line, "%" SCNuMAX "-%" SCNuMAX, &start, &end) != 2) {
    fprintf(stderr, "Can't find range in line %s\n", line);
    return (IdRange){0, 0};
  }

  return (IdRange){start, end};
}

bool ingredient_is_fresh(char *line, IdRange *fresh_ranges,
                         size_t range_count) {
  char *endptr;
  uintmax_t id = strtoumax(line, &endptr, 10);
  for (size_t i = 0; i < range_count; ++i) {
    if (id >= fresh_ranges[i].start && id <= fresh_ranges[i].end) {
      return true;
    }
  }
  return false;
}

int compare_ranges(const void *a, const void *b) {

  const IdRange *range_a = (const IdRange *)a;
  const IdRange *range_b = (const IdRange *)b;

  if (range_a->start < range_b->start)
    return -1;
  if (range_a->start > range_b->start)
    return 1;
  return 0;
}

bool merge_ranges(IdRange *ranges, size_t *count) {

  size_t merge_count = 0;

  if (*count == 0) {
    return false;
  }

  for (size_t i = 1; i < *count; i++) {
    if (ranges[i].start <= ranges[merge_count].end) {
      uintmax_t start = ranges[i].start < ranges[merge_count].start
                            ? ranges[i].start
                            : ranges[merge_count].start;
      uintmax_t end = ranges[i].end > ranges[merge_count].end
                          ? ranges[i].end
                          : ranges[merge_count].end;
      ranges[merge_count] = (IdRange){start, end};
      continue;
    }
    ranges[++merge_count] = ranges[i];
  }
  *count = merge_count + 1;

  return true;
}

uintmax_t print_fresh_ingredient_count(IdRange *ranges, size_t count) {
  uintmax_t fresh_count = 0;
  for (size_t i = 0; i < count; i++) {
    fresh_count += ranges[i].end - ranges[i].start + 1;
  }
  return fresh_count;
}

size_t get_spoiled_ingredience(FILE *file, uintmax_t *out) {

  char *line = NULL;
  IdRange *fresh_ranges = NULL;
  size_t line_cap = 0;
  size_t range_cap = 0;
  size_t count = 0;
  size_t fresh_count = 0;
  bool ranges_gathered = false;

  while (getline(&line, &line_cap, file) != -1) {
    line[strcspn(line, "\n")] = '\0';
    if (ranges_gathered) {

      if (ingredient_is_fresh(line, fresh_ranges, count)) {
        fresh_count++;
      }

      continue;
    }

    if (strlen(line) == 0) {
      ranges_gathered = true;
      qsort(fresh_ranges, count, sizeof(*fresh_ranges), compare_ranges);
      merge_ranges(fresh_ranges, &count);
      *out = print_fresh_ingredient_count(fresh_ranges, count);
      continue;
    }
    if (count == range_cap) {
      range_cap = range_cap == 0 ? 4 : range_cap * 2;
      IdRange *grown = realloc(fresh_ranges, range_cap * sizeof(*fresh_ranges));
      if (grown == NULL) {
        fprintf(stderr, "Unable to allocate enough memory after %d\n",
                (int)count);
        return 0;
      }
      fresh_ranges = grown;
    }
    fresh_ranges[count] = create_range_from_line(line);

    ++count;
  }

  if (line != NULL) {
    free(line);
  }

  printf("Create %d fresh ingredient ranges\n", (int)count);
  free(fresh_ranges);

  return fresh_count;
}
