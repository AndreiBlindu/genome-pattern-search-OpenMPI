#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store data of a rotation
struct rotation
{
    int index;
    char *suffix;
};

// Compares the rotations and
// sorts the rotations alphabetically
int cmpfunc(const void *x, const void *y)
{
    struct rotation *rx = (struct rotation *)x;
    struct rotation *ry = (struct rotation *)y;
    return strcmp(rx->suffix, ry->suffix);
}

// Takes text to be transformed and its length as
// arguments and returns the corresponding suffix array
int *computeSuffixArray(char *inputText, int textLength)
{
    // Array of structures to store rotations and
    // their indexes
    struct rotation *suff = (rotation *)malloc(textLength * sizeof(rotation));

    // Structure is needed to maintain old indexes of
    // rotations after sorting them
    for (int i = 0; i < textLength; i++)
    {
        suff[i].index = i;
        suff[i].suffix = (inputText + i);
    }

    // Sorts rotations using comparison
    // function defined above
    qsort(suff, textLength, sizeof(struct rotation),
          cmpfunc);

    // Stores the indexes of sorted rotations
    int *suffixArr = (int *)malloc(textLength * sizeof(int));
    for (int i = 0; i < textLength; i++)
        suffixArr[i] = suff[i].index;

    free(suff); // avoid memory leak
    // Returns the computed suffix array
    return suffixArr;
}

// Takes suffix array and its size
// as arguments and returns the
// Burrows - Wheeler Transform of given text
char *findLastChar(char *inputText,
                   int *suffixArr, int n)
{
    // Iterates over the suffix array to find
    // the last char of each cyclic rotation
    char *bwtArr = (char *)malloc(n * sizeof(char));
    int i;
    for (i = 0; i < n; i++)
    {
        // Computes the last char which is given by
        // inputText[(suffixArr[i] + n - 1) % n]
        int j = suffixArr[i] - 1;
        if (j < 0)
            j = j + n;

        bwtArr[i] = inputText[j];
    }

    bwtArr[i] = '\0';

    // Returns the computed Burrows - Wheeler Transform
    return bwtArr;
}

// how many times a character c appeared before an index in a string
int rank(char *str, char c, int index)
{
    int count = 0;
    for (int j = index - 1; j >= 0; j--)
    {
        if (str[j] == c)
        {
            count++;
        }
    }
    return count;
}

char *sortString(char *str, char *sorted)
{
    strcpy(sorted, str);
    char temp;

    int i, j;
    int n = strlen(sorted);

    for (i = 0; i < n - 1; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (sorted[i] > sorted[j])
            {
                temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }

    return sorted;
}

int countNucleotide(char *str, char nucleotide)
{
    int count = 0;
    int len = strlen(str);

    for (int i = 0; i < len; i++)
    {
        if (str[i] == nucleotide)
        {
            count++;
        }
    }

    printf("# %c : %d\n", nucleotide, count);
    return count;
}

int getNucleotideIndex(char nucleotide)
{
    switch (nucleotide)
    {
    case 'A':
        return 0;
    case 'C':
        return 1;
    case 'G':
        return 2;
    case 'T':
        return 3;
    default:
        return 4;
    }
}

// Took inspiration from https://mr-easy.github.io/2019-12-19-burrows-wheeler-alignment-part-1/
int bwtSearch(char *bwtArr, int *suffixArr, int bwtLength, char *pattern, int patternLength, int startIndex)
{
    int countA = countNucleotide(bwtArr, 'A');
    int countC = countNucleotide(bwtArr, 'C');
    int countG = countNucleotide(bwtArr, 'G');
    int countT = countNucleotide(bwtArr, 'T');

    int bands[5];

    bands[0] = 1;
    bands[1] = bands[0] + countA;
    bands[2] = bands[1] + countC;
    bands[3] = bands[2] + countG;
    bands[4] = bwtLength;

    int bandStart = bands[getNucleotideIndex(pattern[patternLength - 1])];
    int bandEnd = bands[getNucleotideIndex(pattern[patternLength - 1]) + 1];

    for (int i = (patternLength - 2); i >= 0; i--)
    {
        if (bandStart == bandEnd)
            return -1; // no match

        char currentChar = pattern[i];
        int rankTop = rank(bwtArr, currentChar, bandStart);
        int rankBottom = rank(bwtArr, currentChar, bandEnd);

        bandStart = bands[getNucleotideIndex(currentChar)] + rankTop;
        bandEnd = bands[getNucleotideIndex(currentChar)] + rankBottom;
    }

    printf("Suffix index: %d\n", bandStart);

    return (suffixArr[bandStart] + startIndex);
}