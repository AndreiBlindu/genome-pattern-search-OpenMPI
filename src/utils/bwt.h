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
int *computeSuffixArray(char *input_text, int len_text)
{
    // Array of structures to store rotations and
    // their indexes
    struct rotation *suff = (rotation *)malloc(len_text * sizeof(rotation));

    // Structure is needed to maintain old indexes of
    // rotations after sorting them
    for (int i = 0; i < len_text; i++)
    {
        suff[i].index = i;
        suff[i].suffix = (input_text + i);
    }

    // Sorts rotations using comparison
    // function defined above
    qsort(suff, len_text, sizeof(struct rotation),
          cmpfunc);

    // Stores the indexes of sorted rotations
    int *suffix_arr = (int *)malloc(len_text * sizeof(int));
    for (int i = 0; i < len_text; i++)
        suffix_arr[i] = suff[i].index;

    // Returns the computed suffix array
    return suffix_arr;
}

// Takes suffix array and its size
// as arguments and returns the
// Burrows - Wheeler Transform of given text
char *findLastChar(char *input_text,
                   int *suffix_arr, int n)
{
    // Iterates over the suffix array to find
    // the last char of each cyclic rotation
    char *bwt_arr = (char *)malloc(n * sizeof(char));
    int i;
    for (i = 0; i < n; i++)
    {
        // Computes the last char which is given by
        // input_text[(suffix_arr[i] + n - 1) % n]
        int j = suffix_arr[i] - 1;
        if (j < 0)
            j = j + n;

        bwt_arr[i] = input_text[j];
    }

    bwt_arr[i] = '\0';

    // Returns the computed Burrows - Wheeler Transform
    return bwt_arr;
}

// how many times character c appeared before index i in string s
int rank(char *s, char c, int i)
{
    int count = 0;
    for (int j = i - 1; j >= 0; j--)
    {
        if (s[j] == c)
        {
            count++;
        }
    }
    return count;
}

char *sortString(char *s, char *sorted)
{
    strcpy(sorted, s);
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

int countNucleotide(char *string, char nucleotide)
{
    int count = 0;
    int len = strlen(string);

    for (int i = 0; i < len; i++)
    {
        if (string[i] == nucleotide)
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
int bwtSearch(char *bwt_arr, int *suffix_arr, int bwt_len, char *pattern, int pattern_len, int start_index)
{
    int countA = countNucleotide(bwt_arr, 'A');
    int countC = countNucleotide(bwt_arr, 'C');
    int countG = countNucleotide(bwt_arr, 'G');
    int countT = countNucleotide(bwt_arr, 'T');

    int bands[5];

    bands[0] = 1;
    bands[1] = bands[0] + countA;
    bands[2] = bands[1] + countC;
    bands[3] = bands[2] + countG;
    bands[4] = bwt_len;

    int band_start = bands[getNucleotideIndex(pattern[pattern_len - 1])];
    int band_end = bands[getNucleotideIndex(pattern[pattern_len - 1]) + 1];
    // printf("%d\n", band_start);
    // printf("%d\n\n", band_end);

    for (int i = (pattern_len - 2); i >= 0; i--)
    {
        if (band_start == band_end)
            return -1; // no match

        char currentChar = pattern[i];
        int rank_top = rank(bwt_arr, currentChar, band_start);
        int rank_bottom = rank(bwt_arr, currentChar, band_end);
        // printf("%d\n", rank_top);
        // printf("%d\n\n", rank_bottom);

        band_start = bands[getNucleotideIndex(currentChar)] + rank_top;
        band_end = bands[getNucleotideIndex(currentChar)] + rank_bottom;
        /*printf("%d\n", band_start);
        printf("%d\n\n", band_end);

        printf("%d\n", i);
        printf("############\n");*/
    }

    printf("Suffix index: %d\n", band_start);

    return (suffix_arr[band_start] + start_index);
}