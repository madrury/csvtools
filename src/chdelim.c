#include <stdlib.h>
#include <stdio.h>
#include "errors.h"

void show_usage() {
    printf("usage:\n");
    printf("  csv-chdelim <file> <old delim> <new delim> <grouping char>\n");
}

void show_help() {
    printf("chdelim: Change the delimiter in a delimited file.\n");
    show_usage();
}

int main(int argc, char* argv[]){

    if(argc == 2) {
        show_help();
        exit(EXIT_SUCCESS);
    }
    else if(argc != 5) {
        show_usage();
        raise_argument_error();
    }

    FILE* csv = fopen(argv[1], "r");
    if(csv == NULL) {
        raise_file_error();
    }

    char old_delim = *argv[2];
    char new_delim = *argv[3];
    char quote_char = *argv[4];

    char ch;
    int inside_quote = 0;
    while((ch = getc(csv)) != EOF) {
        if(ch == quote_char) {
            inside_quote = 1 - inside_quote;
            putchar(ch);
        }
        else if((ch == old_delim) && (inside_quote == 0)) {
            putchar(new_delim);
        }
        else {
            putchar(ch);
        }
    }
    exit(EXIT_SUCCESS);
}
