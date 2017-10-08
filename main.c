#include <stdio.h>
#include <stdlib.h>
#include "Constants.h"

struct String {
    unsigned long int len;//inizialization?
    char *begin;
};

void Error_here (int err_code);                         //tell if there is a problem and what problem it is by err_code
char *File_read (int *n);                               //put the text from file to char's array, counting num of symbols
int N_str (const char *poem);                           //count lines in poem
char **P_partition (char *poem, unsigned int n_str);    //creating char*'s array of pointers on the strings from poem
int Comparator (const void *S1, const void *S2);        //compare two lines s1 and s2
int Sort (struct String *, unsigned int);               //qsort() with some checks
int File_write (struct String *, int);                  //writing the result in another file

int main() {
    int num_symbs = POISON_N;
    char *poem = File_read(&num_symbs);
    if (poem == NULL) {                                 //wrong file reading/writing in poem
        free(poem);
        return 0;
    }

    int num_strings = N_str(poem);
    if (num_strings == POISON_N) {                      //poem is NULL
        free(poem);
        return 0;
    }
    else if (num_strings == 0) {                        //almost emty file
        free(poem);
        Error_here(EMPTY_FILE);
        return 0;
    }
    unsigned int num_strs = (unsigned int) num_strings;

    char **s_pointers = P_partition(poem, num_strs);
    if (s_pointers == NULL) {                           //s_pointers is NULL
        free(poem);
        free(s_pointers);
        return 0;
    }

    struct String *strokes = (struct String *) calloc(num_strs, sizeof(struct String));
    if (strokes == NULL) {                              //not enough memory to create strokes
        Error_here(NOT_ENOUGH_MEMORY);
        free(s_pointers);
        free(strokes);
        free(poem);
        return 0;
    }
    int i = 0;                                          //filling strokes with pointers on their beggings from s_pointers
    for ( ; i < num_strings - 1; i++) {                 //and lens as substraction the poniter from next pointer
        strokes[i].begin = s_pointers[i];//FUNCTION?
        strokes[i].len = s_pointers[i+1] - s_pointers[i];
    }
    strokes[i].begin = s_pointers[i];                   //for the last len we haven't s_pointers[i+1]
    strokes[i].len = num_symbs - (s_pointers[i] - s_pointers[0]);

    int check = Sort(strokes, num_strs);
    if (check == POISON_N) {                            //Sort()'s errors
        free(s_pointers);
        free(strokes);
        free(poem);
        return 0;
    }

    check = File_write(strokes, num_strings);
    if (check == POISON_N) {                            //File_write()'s errors
        free(s_pointers);
        free(strokes);
        free(poem);
        return 0;
    }

    free(poem);
    free(s_pointers);
    free(strokes);
    return 0;
}

void Error_here(int err_code) {
    switch(err_code) {
        case (FILE_NOT_FOUND): {
            printf("No such file. Try again.");
            break;
        }
        case (EMPTY_FILE): {
            printf("File choosed is emty. No text to sort.");
            break;
        }
        case (NOT_ENOUGH_MEMORY): {
            printf("There is not enough memory. Sorry :(");
            break;
        }
        case (UNEXPECTED_EOF): {
            printf("Unexpected end of file.");
            break;
        }
        case (FILE_READ_ERR): {
            printf("File reading error.");
            break;
        }
        case (BAD_POINTER): {
            printf("Function's got invalid pointer.");
            break;
        }
        default: {
            printf("Ooops! Some error...");
        }
    }
}

char *File_read(int *n) {
    if (n == NULL) {
        Error_here(BAD_POINTER);
        return NULL;
    }

    FILE *f_poem;
    f_poem = fopen("Onegin.txt", "r");
    if (f_poem == NULL) {
        Error_here(FILE_NOT_FOUND);
        fclose(f_poem);
        return NULL;
    }

    fseek(f_poem, 0, SEEK_END);                         //finding numbers of characters in the f_poem
    unsigned int num_of_symb = (unsigned int) ftell(f_poem);
    if (num_of_symb <= 0) {
        Error_here(EMPTY_FILE);
        fclose(f_poem);
        return NULL;
    }
    *n = num_of_symb;

    char *c_poem = (char *) calloc(num_of_symb, sizeof(char));
    if (c_poem == NULL) {                               //making char's array with text
        Error_here(NOT_ENOUGH_MEMORY);
        fclose(f_poem);
        free(c_poem); //?
        return NULL;
    }
    rewind(f_poem);
    fread(c_poem, sizeof(char), num_of_symb, f_poem);
    c_poem[num_of_symb] = EOF;

    fclose(f_poem);

    return c_poem;
}

int N_str (const char *poem) {
    if (poem == NULL) {
        Error_here(BAD_POINTER);
        return POISON_N;
    }
    int i = 0, Nstrings = 0;
    while (poem[i] != EOF) {
        if (i == 0 && poem[1] == EOF && poem[i] != '\b' && poem[i] != '\n' && poem[i] != '\t') {
            Nstrings++;                             //in case of file " EOF" Nstrings = 0; else Nstrings > 0
        }
        if (poem[i] == '\n' && poem[i+1] != EOF) {
            Nstrings++;
        }
        i++;
    }

    return Nstrings;
}

char **P_partition(char *poem, unsigned int n_str) {
    if (poem == NULL) {
        Error_here(BAD_POINTER);
        return (char **) NULL;
    }
    if (n_str <= 0) {
        Error_here(EMPTY_FILE);
        return NULL;
    }

    char **s_pointers = calloc(n_str, sizeof(char *));
    if (s_pointers == NULL) {
        Error_here(NOT_ENOUGH_MEMORY);
        free(s_pointers);
        return NULL;
    }

    int i = 0, j = 0;                               //i - position of the symbol, j - iterator in s_pointers (number of the string)
    s_pointers[j] = poem + i;
    j++;
    i++;
    while (poem[i] != EOF) {
        if (poem[i] == '\n' && poem[i+1] != EOF) {
            s_pointers[j] = poem + i + 1;           //remember the beginning of next line
            j++;
        }
        i++;
    }

    return s_pointers;
}

int Comparator (const void *S1, const void *S2) {
    if (S1 == NULL || S2 == NULL) {
        Error_here(BAD_POINTER);
        return 0;
    }

    const struct String *s1 = S1;
    const struct String *s2 = S2;

    int l = 0, k = 0;
    while (l < s1->len && l < s2->len) {
        if (s1->begin[l] != s2->begin[l]) {
            k = (int) (s1->begin[l]) - (int) (s2->begin[l]);
            break;
        }
        l++;
    }

    return  k;
}

int Sort (struct String *str, unsigned int len) {
    if (str == NULL) {
        Error_here(BAD_POINTER);
        return POISON_N;
    }
    if (len <= 0) {
        Error_here(EMPTY_FILE);
        return POISON_N;
    }

    qsort(str, len, sizeof(struct String), Comparator);

    return 0;
}

int File_write(struct String *poem, int num_strings) {
    if (poem == NULL) {
        Error_here(BAD_POINTER);
        return POISON_N;
    }
    if (num_strings <= 0) {
        Error_here(EMPTY_FILE);
        return POISON_N;
    }

    FILE *eginno;
    eginno = fopen("Sorted.txt", "w");
    if (eginno == NULL) {
        Error_here(FILE_NOT_FOUND);
        fclose(eginno);
        return POISON_N;
    }

    int i = 0, j = 0;
    while (i < num_strings) {                               //string by string
        j = 0;                                              //symbol by symbol
        while (j < poem[i].len) {                           //print the sorted poem
            if (j == poem[i].len - 1 && poem[i].begin[j] != '\n') {
                poem[i].begin[j] = '\n';
            }
            fprintf(eginno, "%c", poem[i].begin[j]);
            j++;
        }
        i++;
    }

    fclose(eginno);
    return 0;
}