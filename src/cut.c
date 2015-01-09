/* cut:
 *   Output a subset of columns in a csv.
 *
 * usage:
 *   cut <file> <delim> <quoting char> <field 1 idx> ... <field n idx>
 *
 * This program is not intended to be called directly, see instead the
 * interface exposed in csv-cut.
 */
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"

#define MAX_N_FIELDS 2000
#define MAX_LINE_LEN 2000*50

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

// Structure for holding the command line arguments
typedef struct {
    char delim;                   // Single char delimiter.
    char quo;                     // Single char for quoting.
    int field_idxs[MAX_N_FIELDS]; // Integer array contianing indices of fields
                                  // to output.
    int n_fields;                 // Total number of fields to output.
    FILE* csv;                    // Connection to csv file.
} arguments;


arguments* parse_args(int argc, char* argv[]) {
/* Parse the command line argument array into a convienent structure. */
    FILE* csv;

    arguments* args = calloc(1, sizeof(arguments));
    args->delim = *argv[2];
    args->quo = *argv[3];
    // Parse sequence of integers passed as command line arguments into an
    // integer array.
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
    if(strcmp(argv[1], "stdin") == 0) {
        csv = stdin;
    }
    else {
        csv = fopen(argv[1], "r");
    }
    if(csv == NULL) {
        raise_file_error();
    }
    args->csv = csv;
    return args;
}


void process_line(char outline[], char* line, int len, arguments* args) {
/* A state machine for subsetting a single line of a csv.
 *
 * Arguments:
 *   outline: A character array used as a return value.  Data in the fields
 *            selected from the file will be written to this array, which will
 *            be available to the caller after the function returns.
 *   line: A string containing the contents of the line to be processed.
 *   len: The number of chars in line.
 *   args: Structure containing the parameters recieved from the command line.
 */
    int outline_p = 0;               // Current index into outline array.
    int field_idxs_p = 0;            // Current index into field_idxs array.
    int field_idx = 0;               // Index of current field being processed. 
    int next_field = args->field_idxs[0];  // The next field to be output.
    int in_output_field = 0;         // Are we in a field intended for output?
    int in_quotes = 0;               // Are we in a quoted field?
    char chr;                        // Current char being processed.
    char prev_ch = '\0';             // Previous char, to check for escapes.
    
    // Is the first field an output field?
    if(next_field == 0) {
        in_output_field = 1;
    }

    // Search through the line for the requested fields and write their
    // contents into a buffer.
    for(int i = 0; i < len; i++) {
        chr = line[i];
        // Is this a quoting char?
        // If so: 
        //   - Change state of machine to inside quotes if this quote char
        //     is not escaped. 
        //   - Check if we are in an output field, if so, output the quoting
        //     char because we are outputing the field.
        if(chr ==args->quo) {
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
        else if((chr == args->delim) && !in_quotes) {
            // Wrap up if we are in an output field
            if(in_output_field) {
                outline[outline_p++] = chr;
                next_field = args->field_idxs[++field_idxs_p];
                in_output_field = 0;
            }
            // Increment what field we are looking for.
            field_idx++;
            if(field_idx == next_field) {
                in_output_field = 1;
            }
        }
        // Is this just a boring old char or a delimiter in a quoted field?
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
        process_line(outline, line, line_len, args);
        printf("%s", outline);
    }

    free(args);
    exit(EXIT_SUCCESS);
}
