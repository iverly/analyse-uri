#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define REGEX_PROTO "(https?)"
#define REGEX_SEP_PROTO "://"
#define REGEX_DOMAIN "([[:alnum:].-])+"

#define REGEX_WEB "^" REGEX_PROTO REGEX_SEP_PROTO REGEX_DOMAIN "$"

int checkURI(char* str);

int main() {
    char* buffer;
    size_t bufsize = 64;

    // Read from stdin
    printf("Entrez votre URI:\n");
    getline(&buffer ,&bufsize, stdin);
    
    // Remove \n
    buffer[strlen(buffer)-1] = '\0';
    printf("Analyse de votre URI en cours ... (%s)\n", buffer);

    // Check if the buffer is in correct format
    if (checkURI(buffer)) {
        printf("Le format de notre URI est correct\n");
    }
    else {
        printf("Le format de notre URI est incorrect, merci de la vérifier\n");
    }
    
    return 0;
}

int checkURI(char* str) {
    regex_t regex;
    int reti;

    reti = regcomp(&regex, REGEX_WEB, REG_NOSUB | REG_EXTENDED);
    if (reti) {
        printf("Impossible de compiler la regex\n");
        exit(1);
    }

    reti = regexec(&regex, str, 0, NULL, 0);
    regfree(&regex);
    return !reti;
}