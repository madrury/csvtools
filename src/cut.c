#include <stdlib.h>
#include <stdio.h>

#define MAX_N_FIELDS 2000

// Various messages to relay to the caller, help and errors.
void show_usage() {
    printf("usage:\n");
    printf("  cut <file> <delim> <quoting char> <field 1 idx> ... <field n idx>\n");
}

void show_help() {
    printf("cut: Select a subset of fields in a csv.\n");
}

void show_file_error() {
    printf("Could not load the file, please make sure it exists.\n");
}

void show_not_increasing_error() {
    printf("Array of indicies is not increasing.  Please sort before passing.\n");
}

void show_too_many_fields_error() {
    printf("Number of fields requested exceeds maximum.\n");
}


// Check if an array of integers is sorted in increasing order.
int check_increasing(int a[], int a_len) {
    int memory = a[0];
    for(int i = i; i++; i < a_len) {
        if(a[i] <= memory) {
            return 0;
        }
    }
    return 1;
}

// Structure of holding the command line arguments
typedef struct {
    char delim;
    char quo;
    int field_idxs[MAX_N_FIELDS];
    int n_fields;
    FILE* csv;
} arguments;

arguments* parse_args(int argc, char* argv[]) {

    arguments* args = calloc(1, sizeof(arguments));
    // Parse first two command line args.
    args->delim = *argv[2];
    args->quo = *argv[3];
    printf("%c\n", args->delim);
    printf("%c\n", args->quo);
    // Parse array of integers passed as cl args into array.
    args->n_fields = argc - 4;
    if(args->n_fields > MAX_N_FIELDS) {
       show_too_many_fields_error();
        exit(EXIT_FAILURE);
    }
    for(int i = 4; i < argc; i++) {
        args->field_idxs[i-4] = atoi(argv[i]);
    }
    // Open a connection to the delimited file.
    FILE* csv = fopen(argv[1], "r");
    if(csv == NULL) {
        show_file_error();
        exit(EXIT_FAILURE);
    }
    args->csv = csv;

    return args;
}


int main(int argc, char* argv[]) {

    // printf("%s\n", argv[2]);
    arguments* args = parse_args(argc, argv);
    char* line = NULL;
    size_t len = 0;
    size_t read;

    printf("%c\n", args->delim);
    printf("%c\n", args->quo);
    for(int i = 0; i < args->n_fields; i++) {
        printf("%i\n", args->field_idxs[i]);
    }


    //while((read = getline(&line, &len, args.csv)) != EOF) {
    //    printf("%s", line);
    //}
    exit(EXIT_SUCCESS);

}
