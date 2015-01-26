#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

void raise_argument_error() {
   fprintf(stderr, "Invalid command line arguemnts.\n");
   exit(EXIT_FAILURE);
}

void raise_file_error() {
    fprintf(stderr, "Could not read the file, please make sure it exists.\n");
    exit(EXIT_FAILURE);
}

void raise_not_increasing_error() {
    fprintf(stderr, 
        "Array of indicies is not increasing.  Please sort before passing.\n"
    );
    exit(EXIT_FAILURE);
}

void raise_too_many_fields_error() {
    fprintf(stderr, "Number of fields requested exceeds maximum.\n");
    exit(EXIT_FAILURE);
}

void raise_regex_error(int errcd) {
    if(errcd == REG_BADPAT) {
        fprintf(stderr, "Invalid regular expression.\n");
    }
    else if(errcd == REG_ECTYPE) {
        fprintf(stderr, "Invalid character class in regular expression.\n");
    }
    else if(errcd == REG_EESCAPE) {
        fprintf(stderr, "Invalid traling escape in regular expression.\n");
    }
    else if(errcd == REG_EBRACK) {
        fprintf(stderr, "Unbalanced brackets [] in regular expression.\n");
    }
    else if(errcd == REG_EPAREN) {
        fprintf(stderr, 
            "Unbalanced parenthesies \\(\\) in regular expression.\n"
        );
    }
    else if(errcd == REG_BADBR) {
        fprintf(stderr, "Contents of \\{\\} invalid.\n");
    }
    else if(errcd == REG_ESPACE) {
        fprintf(stderr, "Memory error when compiling regular expression.\n");
    }
    else if(errcd == REG_BADRPT) {
        fprintf(stderr, 
            "?, *, or + not precedded by a valid regular expression.\n"
        );
    }
    else {
        fprintf(stderr, "Unknown error in regular expression.\n");
    }
    exit(EXIT_FAILURE);
}

void raise_eol_error() {
    fprintf(stderr, 
        "End of line encountered without entering requested field.\n"
    );
    exit(EXIT_FAILURE);
}

void raise_oob_match_str_error() {
    fprintf(stderr, 
        "Index error in match string.  Perhaps the field is not well formed?\n"
    );
    exit(EXIT_FAILURE);
}

void raise_invalid_quoting_error() {
    fprintf(stderr,
        "End quote found with no begin quote.\n"
    );
    exit(EXIT_FAILURE);
}
