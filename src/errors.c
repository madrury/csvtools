#include <stdlib.h>
#include <stdio.h>

void raise_argument_error() {
   printf("Invalid command line arguemnts.\n");
   exit(EXIT_FAILURE);
}

void raise_file_error() {
    printf("Could not load the file, please make sure it exists.\n");
    exit(EXIT_FAILURE);
}

void raise_not_increasing_error() {
    printf("Array of indicies is not increasing.  Please sort before passing.\n");
    exit(EXIT_FAILURE);
}

void raise_too_many_fields_error() {
    printf("Number of fields requested exceeds maximum.\n");
    exit(EXIT_FAILURE);
}
