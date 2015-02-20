/* chdelim:
 *   Change the delimiter in delimited file.
 *
 * usage:
 *   chdelim <file> <current delim> <new delim> <grouping char>
 *
 * This program is not intended to be called directly, see instead the 
 * interface exposed in csv-chdelim.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"

int main(int argc, char* argv[]){
    FILE* csv;

    if(strcmp(argv[1], "stdin") == 0) {
        csv = stdin;
    }
    else {
        csv = fopen(argv[1], "r");
    }

    if(csv == NULL) {
        raise_file_error();
    }

    char old_delim = *argv[2];
    char new_delim = *argv[3];
    char quote_char = *argv[4];

    // Go through the stream char by char and change the delimiters
    // that are not inside of quoting pairs.
    char ch;
    char prev_ch = '\0';
    int inside_quote = 0;

    while((ch = getc(csv)) != EOF) {
        if(ch == quote_char) {
            // Check if this quote is escaped.
            if(prev_ch != '\\') {
                inside_quote = 1 - inside_quote;
            }
            putchar(ch);
        }
        else if((ch == old_delim) && (inside_quote == 0)) {
            putchar(new_delim);
        }
        else {
            putchar(ch);
        }
        prev_ch = ch;
    }
    exit(EXIT_SUCCESS);
}
