#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define REGEX_PROTO "(http|https|ssh|ftp)"
#define REGEX_SEP_PROTO "://"
#define REGEX_DOMAIN "(([[:alnum:].-])+)"

#define REGEX_WEB "^" REGEX_PROTO REGEX_SEP_PROTO REGEX_DOMAIN "$"

char* buffer;
size_t bufsize = 64;

int checkURI(char* str);
regmatch_t* extract(char* in);
char* get(char* in, regmatch_t *pmatch, int index);

int main() {
    // Read from stdin
    printf("Entrez votre URI:\n");
    getline(&buffer, &bufsize, stdin);
    
    // Remove \n
    buffer[strlen(buffer)-1] = '\0';
    printf("Analyse de votre URI en cours ... (%s)\n", buffer);

    // Check if the buffer is in correct format
    if (1 == checkURI(buffer)) {
        printf("Le format de notre URI est correct\n");

        regmatch_t* reg = extract(buffer);
        char* protocol = get(buffer, reg, 1);
        char* domain = get(buffer, reg, 2);

        printf("Protocol: %s\n", protocol);
        printf("Domain: %s\n", domain);
    }
    else {
        printf("Le format de notre URI est incorrect, merci de la vérifier\n");
    }
    
    return 0;
}

int checkURI(char* str) {
    regex_t preg;
    int reti;

    reti = regcomp(&preg, REGEX_WEB, REG_EXTENDED);
    if (0 != reti) {
        printf("Impossible de compiler la regex\n");
        exit(1);
    }

    reti = regexec(&preg, str, 0, NULL, 0);
    regfree(&preg);
    return !reti;
}

regmatch_t* extract(char* in) {
    regex_t preg;
    size_t nmatch = 0;
    regmatch_t *pmatch = NULL;
    int reti;

    reti = regcomp(&preg, REGEX_WEB, REG_EXTENDED);
    if (0 != reti) {
        printf("Impossible de compiler la regex\n");
        exit(1);
    }

    nmatch = preg.re_nsub;
    pmatch = malloc (sizeof (*pmatch) * nmatch);

    reti = regexec(&preg, in, nmatch, pmatch, 0);
    regfree(&preg);
    return pmatch;
}

char* get(char* in, regmatch_t *pmatch, int index) {
    char *out = NULL;
    int start = pmatch[index].rm_so;
    int end = pmatch[index].rm_eo;
    size_t size = end - start;
        
    out = malloc (sizeof(char) * (size + 1));
    if (out) {
        strncpy(out, &in[start], size);
        out[size] = '\0';
        return out;
    }
    
    return NULL;
}