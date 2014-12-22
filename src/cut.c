# define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>

#define MAX_N_FIELDS 2000
#define MAX_LINE_LEN 2000*50

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
    for(int i = 1; i < a_len; i++) {
        //printf("%i\n", a[i]);
        if(a[i] <= memory) {
            return 0;
        }
        memory = a[i];
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
    // Parse array of integers passed as cl args into array.
    args->n_fields = argc - 4;
    if(args->n_fields > MAX_N_FIELDS) {
       show_too_many_fields_error();
       exit(EXIT_FAILURE);
    }
    for(int i = 4; i < argc; i++) {
        //printf("%i\n", atoi(argv[i]));
        args->field_idxs[i-4] = atoi(argv[i]);
        //printf("%i\n", args->field_idxs[i-4]);
    }
    if(!check_increasing(args->field_idxs, args->n_fields)) {
        show_not_increasing_error();
        exit(EXIT_FAILURE);
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

void process_line(char newline[], char* line, int len, int field_idxs[], int n_fields, char delim, char quo) {
    int newline_p = 0;  // Pointer into newline array
    int field_idxs_p = 0;  // Pointer into filed_idxs array
    int next_field = field_idxs[0];  // What is the next filed to output?
    int field_n = 0;  // What field are we currently looking at?
    int in_output_field = 0;
    int in_quotes = 0;
    char chr;
    
    // Is the first field an output field?
    if(next_field == 0) {
        in_output_field = 1;
    }

    for(int i = 0; i < len; i++) {
        chr = line[i];
        // Is this a quoting char?
        //    If so, change state of machine to inside quotes, and check if we
        //    are in an output field.
        if(chr == quo) {
            in_quotes = 1 - in_quotes;
            if(in_output_field) {
                newline[newline_p++] = chr;
            }
        }
        // Is this a delimiter?
        //
        else if((chr == delim) && !in_quotes) {
            // Wrap up if we are in an output field
            if(in_output_field) {
                newline[newline_p++] = chr;
                next_field = field_idxs[++field_idxs_p];
                in_output_field = 0;
            }
            // Increment what field we are looking for.
            field_n++;
            if(field_n == next_field) {
                in_output_field = 1;
            }
        }
        else {
            if(in_output_field) {
                newline[newline_p++] = chr;
            }
        }
    }
    // At the end, add a newline if necessary.
    if(newline[newline_p - 1] != '\n') {
        newline[newline_p - 1] = '\n';
    }
    // Add the null byte to terminate the string.
    newline[newline_p] = '\0';
}

int main(int argc, char* argv[]) {

    // printf("%s\n", argv[2]);
    arguments* args = parse_args(argc, argv);
    char* line = NULL;
    char newline[MAX_LINE_LEN]; 
    size_t len = 0;
    size_t line_len;

    while((line_len = getline(&line, &len, args->csv)) != EOF) {
        process_line(newline, line, line_len, args->field_idxs, args->n_fields, args->delim, args->quo);
        printf("%s", newline);
    }
    exit(EXIT_SUCCESS);
}
