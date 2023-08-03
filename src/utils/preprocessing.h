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
        if (text[i] == '$') // necessary for bwt
        {
            newString[j++] = '$';
        }
    }

    return newString;
}