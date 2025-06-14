#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "symbol_table.h"

int main(int argc, char **argv) {
  if (argc != 3) { 
    fprintf(stderr, "Usage: ./assemble <file_in> <file_out> \n");
    return EXIT_FAILURE; 
  }

  const char *input_filename = argv[1];
  const char *output_filename = argv[2];

  return EXIT_SUCCESS;
}
