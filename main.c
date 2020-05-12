#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *buffer;
    size_t bufsize = 64;

    // Read from stdin
    printf("Entrez votre URI:\n");
    getline(&buffer ,&bufsize, stdin);
    
    // Remove \n
    buffer[strlen(buffer)-1] = '\0';
    printf("Analyse de votre URI en cours ... (%s)\n", buffer);

    return 0;
}