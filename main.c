#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define REGEX_PROTO "(http|https|ssh|ftp|git)"
#define REGEX_SEP_PROTO "://"
#define REGEX_USER "(([[:alnum:]]+)@)?"
#define REGEX_DOMAIN "(([[:alnum:].-])+)"
#define REGEX_PORT "(:([0-9]{2,4}))?"
#define REGEX_CHEMIN "(/([[:alnum:]./-])+)?"
#define REGEX_RESSOURCE "/?(\\?(([[:alnum:]]+=[[:alnum:]]+)&?)+)?"

#define REGEX_WEB "^" REGEX_PROTO REGEX_SEP_PROTO REGEX_USER REGEX_DOMAIN REGEX_PORT REGEX_CHEMIN REGEX_RESSOURCE "$"

char* buffer;
size_t bufsize = 64;

int checkURI(char* str);
regmatch_t* extract(char* in);
char* get(char* in, regmatch_t *pmatch, int index);
char* getDefaultPort(char* procotol);

int main() {
    // Read from stdin
    printf("Entrez votre URI:\n");
    getline(&buffer, &bufsize, stdin);
    
    // Remove \n
    buffer[strlen(buffer)-1] = '\0';
    printf("Analyse de votre URI en cours ... (%s)\n", buffer);

    // Check if the buffer is in correct format
    if (1 == checkURI(buffer)) {
        regmatch_t* reg = extract(buffer);
        char* protocol = get(buffer, reg, 1);

        if (protocol[0] == 'h') {
            char* domain = get(buffer, reg, 4);
            char* port = get(buffer, reg, 7);
            char* chemin = get(buffer, reg, 8);
            char* ressource = get(buffer, reg, 10);

            printf("Le format de notre URI est correct\n");

            printf("Protocol: %s\n", protocol);
            printf("Domain: %s\n", domain);
            printf("Port: %s\n", strlen(port) != 0 ? port : getDefaultPort(protocol));
            printf("Chemin: %s\n", strlen(chemin) != 0 ? chemin : "/");
            printf("Ressource: %s\n", strlen(ressource) != 0 ? ressource : "?");
        } else if (protocol[0] == 's') {
            char* user = get(buffer, reg, 3);
            char* domain = get(buffer, reg, 4);
            char* port = get(buffer, reg, 7);

            char* chemin = get(buffer, reg, 8);
            char* ressource = get(buffer, reg, 10);
            if (strlen(chemin) != 0 || strlen(ressource) != 0) {
                printf("Le format de notre URI est incorrect, merci de la vérifier\n");
                return 1;
            }

            printf("Le format de notre URI est correct\n");

            printf("Protocol: %s\n", protocol);
            printf("User: %s\n", strlen(user) != 0 ? user : "root");
            printf("Domain: %s\n", domain);
            printf("Port: %s\n", strlen(port) != 0 ? port : getDefaultPort(protocol));
        } else if (protocol[0] == 'f') {
            char* user = get(buffer, reg, 3);
            char* domain = get(buffer, reg, 4);
            char* port = get(buffer, reg, 7);
            char* path = get(buffer, reg, 8);
            
            char* ressource = get(buffer, reg, 10);
            if (strlen(ressource) != 0) {
                printf("Le format de notre URI est incorrect, merci de la vérifier\n");
                return 1;
            }

            printf("Le format de notre URI est correct\n");

            printf("Protocol: %s\n", protocol);
            printf("User: %s\n", strlen(user) != 0 ? user : "root");
            printf("Domain: %s\n", domain);
            printf("Port: %s\n", strlen(port) != 0 ? port : getDefaultPort(protocol));
            printf("Path: %s\n", strlen(path) != 0 ? path : "/");
        } else if (protocol[0] == 'g') {
            char* domain = get(buffer, reg, 4);
            char* chemin = get(buffer, reg, 8);

            char* user = get(buffer, reg, 3);
            char* port = get(buffer, reg, 7);
            char* ressource = get(buffer, reg, 10);
            if (strlen(user) != 0 || strlen(port) != 0 || strlen(ressource) != 0) {
                printf("Le format de notre URI est incorrect, merci de la vérifier\n");
                return 1;
            }

            int i = 0;
            for (int a = 0; a < strlen(chemin); a++) {
                if (chemin[a] == '/') i++;
            }
            if (i != 2) {
                printf("Le format de notre URI est incorrect, merci de la vérifier\n");
                return 1;
            }

            printf("Protocol: %s\n", protocol);
            printf("Domain: %s\n", domain);
            printf("Chemin: %s\n", chemin);
        }
    }
    else {
        printf("Le format de notre URI est incorrect, merci de la vérifier\n");
        return 1;
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

char* getDefaultPort(char* procotol) {
    if (procotol[0] == 'h') {
        if (strlen(procotol) == 4) return "80";
        else return "443";
    } else if (procotol[0] == 's') return "22";
    else if (procotol[0] == 'f') return "21";
    return "0";
}