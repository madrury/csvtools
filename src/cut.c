# define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include "errors.h"

#define MAX_N_FIELDS 2000
#define MAX_LINE_LEN 2000*50


// Structure for holding the command line arguments
typedef struct {
    char delim;
    char quo;
    int field_idxs[MAX_N_FIELDS];
    int n_fields;
    FILE* csv;
} arguments;


// Messages for the user.
void show_usage() {
    printf("usage:\n");
    printf("  cut <file> <delim> <quoting char> <field 1 idx> ... <field n idx>\n");
}
void show_help() {
    printf("cut: Select a subset of fields in a csv.\n");
}


int check_increasing(int a[], int a_len) {
/* Check if an array of integers is sorted in increasing order. */
    int memory = a[0];
    for(int i = 1; i < a_len; i++) {
        if(a[i] <= memory) {
            return 0;
        }
        memory = a[i];
    }
    return 1;
}


arguments* parse_args(int argc, char* argv[]) {
/* Parse the command line argument array into a convienent structure. */
    arguments* args = calloc(1, sizeof(arguments));
    
    // Single char arguments
    args->delim = *argv[2];
    args->quo = *argv[3];

    // Parse array of integers passed as cl args into iinteger array.
    args->n_fields = argc - 4;
    if(args->n_fields > MAX_N_FIELDS) {
       raise_too_many_fields_error();
    }
    for(int i = 4; i < argc; i++) {
        args->field_idxs[i-4] = atoi(argv[i]);
    }
    if(!check_increasing(args->field_idxs, args->n_fields)) {
        raise_not_increasing_error();
    }

    // Open a connection to the delimited file.
    FILE* csv = fopen(argv[1], "r");
    if(csv == NULL) {
        raise_file_error();
    }
    args->csv = csv;

    return args;
}

void process_line(char outline[], char* line, int len, 
                  int field_idxs[], int n_fields, char delim, char quo) {
/* A state machine for subsetting a single line of a csv.
 *
 * Arguments:
 *   outline: A character array used as a return value.  Data in the fields
 *            selected from the file will be written to this array, which will
 *            be available to the caller after the function returns.
 *   line: A string containing the contents of the line to be processed.
 *   len: The number of chars in line.
 *   field_idxs: An integer array containing the indexes for the requested
 *               fields, guarenteed to be in strictly increasing order.
 *   n_fields: The length of field_idxs.
 *   delim: The delimiting character in the file.
 *   quo: the quoting character in the file.
 */
    int outline_p = 0;               // Current index into outline array.
    int field_idxs_p = 0;            // Current index into field_idxs array.
    int field_idx = 0;               // Index of current field being processed. 
    int next_field = field_idxs[0];  // The next field to be output.
    int in_output_field = 0;         // Are we in a field intended for output?
    int in_quotes = 0;               // Are we in a quoted field?
    char chr;                        // Current char being processed.
    char prev_ch = '\0';             // Previous char, to check for escapes.
    
    // Is the first field an output field?
    if(next_field == 0) {
        in_output_field = 1;
    }

    for(int i = 0; i < len; i++) {
        chr = line[i];
        // Is this a quoting char?
        // If so: 
        //   - Change state of machine to inside quotes if this quote char
        //     is not escaped. 
        //   - Check if we are in an output field, if so, output the quoting
        //     char because we are outputing the field.
        if(chr == quo) {
            if(prev_ch != '\\') {
                in_quotes = 1 - in_quotes;
            }
            if(in_output_field) {
                outline[outline_p++] = chr;
            }
        }
        // Is this a delimiter outside of a quoted field?
        // If so:
        //   - If we are currently in an output field, then this is the end
        //     of it, so output the delimiter and change state to not an 
        //     output field.
        //   - Increment the state tracking the next field to look for.
        //   - If the subsequent field is an output field, set the state to 
        //     reflect this.
        else if((chr == delim) && !in_quotes) {
            // Wrap up if we are in an output field
            if(in_output_field) {
                outline[outline_p++] = chr;
                next_field = field_idxs[++field_idxs_p];
                in_output_field = 0;
            }
            // Increment what field we are looking for.
            field_idx++;
            if(field_idx == next_field) {
                in_output_field = 1;
            }
        }
        // Is this just a boring old char, or a delimiter in a quoted field?
        // If so:
        //   - If we are in an output field, output the char.
        else {
            if(in_output_field) {
                outline[outline_p++] = chr;
            }
        }
    }
    // At the end, add a outline if necessary.
    if(outline[outline_p - 1] != '\n') {
        outline[outline_p - 1] = '\n';
    }
    // Add the null byte to terminate the string.
    outline[outline_p] = '\0';
}


int main(int argc, char* argv[]) {

    arguments* args = parse_args(argc, argv);
    char* line = NULL;           // Current line in the csv file.
    char outline[MAX_LINE_LEN];  // Data to write out for the current line.
    size_t len = 0;              // Length of the buffer holding the csv line.
    size_t line_len;             // Length of the current line.

    // Process the csv line by line.
    while((line_len = getline(&line, &len, args->csv)) != EOF) {
        process_line(outline, line, line_len, 
                     args->field_idxs, args->n_fields, args->delim, args->quo);
        printf("%s", outline);
    }
    exit(EXIT_SUCCESS);
}
