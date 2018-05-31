#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>

int strcicmp(char const *a, char const *b);

int main(int argc, char *argv[])
{
    char *windowsPath = argv[1];
    char **tokens = NULL;
    int n_tokens = 0;
    struct dirent *de;

    printf("Windows Path: %s\n", windowsPath);

    char *token = strtok(windowsPath, "\\");
    while(token) {
        tokens = realloc(tokens, sizeof (char*) * ++n_tokens);
        if(tokens == NULL) exit(-1);

        tokens[n_tokens-1] = token;

        token = strtok(NULL, "\\");
    }

    char *base = "/mnt";

    for(int i=0; i<n_tokens; i++) {
        DIR *dr = opendir(base);

        char* token;
        if(strchr(tokens[i],':') != NULL) {
            int size = strlen(tokens[i]);
            token = malloc(size-1);
            strncpy(token, tokens[i], size-1);
            token[size-1] = '\0';
        }
        else {
            token = tokens[i];
        }

        char *bestMatch = NULL;
        while ((de = readdir(dr)) != NULL) {
            if(strcmp(de->d_name, token) == 0) {
                bestMatch = de->d_name;
                break;
            } else if(strcicmp(de->d_name,token) == 0) {
                bestMatch = de->d_name;
            }
        }

        if(bestMatch == NULL) exit(-1);
        else {
            char *newBase = malloc(strlen(base)+strlen(bestMatch)+2);
            newBase[0] = '\0';
            strcat(newBase, base);
            strcat(newBase, "/");
            strcat(newBase, bestMatch);

            base = newBase;
        }

        closedir(dr);
    }

    printf("Unix Path: %s\n",base);

    return 0;
}

int strcicmp(char const *a, char const *b)
{
    for (;; a++, b++) {
        int d = tolower(*a) - tolower(*b);
        if (d != 0 || !*a)
            return d;
    }
}
