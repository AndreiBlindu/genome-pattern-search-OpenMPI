#include <stdlib.h>
#include <string.h>

char *preprocessing(char *text)
{
    int N = strlen(text);

    char *newString = (char *)malloc(N);
    int j = 0;

    for (int i = 1; i < N; i++)
    {
        if (text[i] != '\n' && (text[i] == 'A' || text[i] == 'T' || text[i] == 'C' || text[i] == 'G'))
        {
            newString[j++] = text[i];
        }
    }

    return newString;
}