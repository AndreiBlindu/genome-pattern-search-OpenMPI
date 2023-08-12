#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *preprocessing(char *text)
{
    int N = strlen(text);

    char *newString = (char *)malloc(N);
    int j = 0;

    for (int i = 0; i < N; i++)
    {
        if (text[i] != '\n' && (toupper(text[i]) == 'A' || toupper(text[i]) == 'T' || toupper(text[i]) == 'C' || toupper(text[i]) == 'G'))
        {
            newString[j++] = toupper(text[i]);
        }
    }
    newString[j++] = '\0';
    newString = (char *)realloc(newString, j);

    return newString;
}

char *addTermination(char *text)
{
    int N = strlen(text);

    char *newString = (char *)malloc(N + 1);
    strcpy(newString, text);
    newString[N] = '$';

    return newString;
}