/* grep:
 *   Ouput lines in a delimited file where a specific field matches
 *   a pattern.
 *
 * usage:
 *   grep <file> <delim> <quoting char> <field idx> <pattern>
 *
 * This program is not meant to be called directly, see instead the interface
 * exposed in csv-grep.
 */
# define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include "errors.h"

#define MAX_FIELD_LEN 200

// Structure for holding command line arguemnts
typedef struct {
    char delim;    // Single char delimiter.
    char quo;      // Single char for quoting.
    char* pattern; // Pattern to match against.
    regex_t regex; // Compiled regular expression object from pattern.
    int field_idx; // Index of field to match pattern against.
    FILE* csv;     // Connection to csv file.
} arguments;


arguments* parse_args(int argc, char* argv[]) {
/* Parse command line arguments into convenient structure. */
    int retcd;  // Return code for regex compile.

    arguments* args = calloc(1, sizeof(arguments));
    args->delim = *argv[2];
    args->quo = *argv[3];
    args->field_idx = atoi(argv[4]);
    args->pattern = argv[5];
    // Compile the pattern into a regular expression object.
    retcd = regcomp(&args->regex, args->pattern, 0);
    if(retcd != 0) {
        printf("Failed to compile regular expression.\n");
        exit(EXIT_FAILURE);
    }
    // Open a connection to the csv file.
    FILE* csv = fopen(argv[1], "r");
    if(csv == NULL) {
        raise_file_error();
    }
    args->csv = csv;
    return args;
}


int match_line(char match_str[], char* line, int line_len, arguments* args) { 
/* Check if a given field in a line of the csv matches a regular expression.
 *
 * Arguments:
 *   match_str: A character array used to buffer the contents of the field to
 *              be matched against.
 *   line: A string containing the contents of the line to be processed.
 *   line_len: The number oc characters in line.
 *   args: Structure containing the parameters recieved from the command line.
 */
    int field_idx = 0;    // Index of current field being processed. 
    int line_p = 0;       // Index into the line.
    int in_quotes = 0;    // Are we in a quoted field?
    char chr;             // Current char being processed.
    char prev_ch = '\0';  // Previous char, to check for escapes.
    int match;            // Did the regex match the line? 

    // Search through the line until we enter the field to match against.
    while(1) {
        chr = line[line_p];
        // Edge case: we are matching against the first field, no need to
        // search through the line.
        if(args->field_idx == 0) {
            break;
        }
        // Search...
        if((chr == args->quo) && prev_ch != '\\') {
            in_quotes = 1 - in_quotes;
        }
        else if((chr == args->delim) && !in_quotes) {
            field_idx++;
        }
        else if(chr == '\n') {
            printf("End of line without encountering field n.\n");
            exit(EXIT_FAILURE);
        }
        // Check if we've enetered the correct field.  If so, increment
        // the line pointer to the first char in that field and stop 
        // searching.
        if(field_idx ==args-> field_idx) {
            line_p++;
            break;
        }
        line_p++;
    }
    // line_p is now the beginning of the field to match against, so
    // extract the string in that field into match_str.
    int i = 0;
    while(1) {
        chr = line[line_p];
        if(((chr == args->delim) && !in_quotes) || (chr == '\n')) {
            break;
        }
        if((chr == args->quo) && prev_ch != '\\') {
            in_quotes = 1 - in_quotes;
        }
        match_str[i] = chr;
        line_p++; i++;
    }
    // Now match_str is the string to match against.
    match = regexec(&args->regex, match_str, 0, NULL, 0);
    return(match != REG_NOMATCH);
}


int main(int argc, char* argv[]) {

    arguments* args = parse_args(argc, argv);
    char* line = NULL;             // Current line in the csv file.
    size_t len = 0;                // Length of the buffer holding the csv line.
    size_t line_len;               // Length of the current line.
    char match_str[MAX_FIELD_LEN]; // Buffer to hold match strings, only 
                                   // allocated once for efficiency.
    int is_match;                  // Is the current line a match?

    // Process csv line by line
    while((line_len = getline(&line, &len, args->csv)) != EOF) {
        is_match = match_line(match_str, line, line_len, args);
        // printf("%i\n", is_match);
        if(is_match) {
            printf(line);
        }
    }
     return 0;
}
