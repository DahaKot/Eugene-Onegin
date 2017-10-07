#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct String {
    unsigned long int len;
    char *begin;
};

char *File_read(int *); //put the text prom file to char's array
int N_str(const char * poem); //count lines in poem
char **P_partition(char * poem, int); //creating char*'s array of pointers on the strings from poem
void Sort(struct String *, int);
//int Comparator_of_strings(const char* first, const char* second); //meaning there are not 2 alike strings
void File_write(struct String *, int); //writing the result in another file

int main() {
    int num_symbs = -1;
    char *poem = File_read(&num_symbs);
    int num_strings = N_str(poem);
    char **s_pointers = P_partition(poem, num_strings);

    struct String *strokes = (struct String *) calloc(num_strings, sizeof(struct String));
    int i = 0;
    for ( ; i < num_strings - 1; i++) {
        strokes[i].begin = s_pointers[i];
        strokes[i].len = s_pointers[i+1] - s_pointers[i];
    }
    strokes[i].begin = s_pointers[i];
    strokes[i].len = num_symbs - (s_pointers[i] - s_pointers[0]);
    //Here the trouble begin
    Sort(strokes, num_strings);

    File_write(strokes, num_strings);

    free(poem);
    free(s_pointers);
    free(strokes);

    return 0;
}

char *File_read(int *n) {
    FILE *f_poem;
    f_poem = fopen("out.txt", "r");
    assert(f_poem != NULL); //with if?

    //finding numbers of characters in the f_poem
    fseek(f_poem, 0, SEEK_END);
    long int num_of_symb = ftell(f_poem);
    *n = num_of_symb;

    //making char's array with text
    char *c_poem = calloc(num_of_symb, sizeof(char));
    rewind(f_poem);
    fread(c_poem, sizeof(char), num_of_symb, f_poem);
    c_poem[num_of_symb] = EOF;

    fclose(f_poem);

    return c_poem;
}

int N_str (const char *poem) {
    int i = 0, Nstrings = 1;
    while (poem[i] != EOF) {
        if (poem[i] == '\n' && poem[i+1] != EOF) {
            Nstrings++;
        }
        i++;
    }

    return Nstrings;
}

char ** P_partition(char *poem, int num_strings) {
    assert(poem != NULL);

    int i = 0, j = 0;
    char **s_pointers = calloc(num_strings, sizeof(char *));
    s_pointers[j] = poem + i;
    j++;
    i++;
    while (poem[i] != EOF) {
        if (poem[i] == '\n' && poem[i+1] != EOF) {
            s_pointers[j] = poem + i + 1;             //remember the beginning of next line
            j++;
        }
        i++;
    }

    return s_pointers;
}

void Sort (struct String *str, int len) {
    int i = 0, j = 0;
    for ( ; i < len; i++) {
        for (j = i; j < len; j++) {
            int l = 0, k = 0;
            while (l < str[i].len && l < str[j].len) {
                if (str[i].begin[l] != str[j].begin[l]) {
                    k = (int) (str[i].begin[l]) - (int) (str[j].begin[l]);
                    break;
                }
                l++;
            }

            if (k > 0) {
                struct String temp = str[j];
                str[j] = str[i];
                str[i] = temp;
            }
        }
    }
}

/*int Comparator_of_strings(const char* line1, const char* line2) {
    int i = 0;
    while (i < strlen(line1) && i < strlen(line2)) {
        if (line1[i] != line2[i]) {
            return ((int) line1[i] - (int) line2[i]);
        }
        i++;
    }
}*/

void File_write(struct String *poem, int num_strings) {
    FILE *eginno;
    eginno = fopen("Sorted.txt", "w");
    assert(eginno != NULL);

    int i = 0;
    while (i < num_strings) {
        int j = 0;
        while (j < poem[i].len) {
            if (j == poem[i].len - 1 && poem[i].begin[j] != '\n') {
                poem[i].begin[j] = '\n';
            }
            fprintf(eginno, "%c", poem[i].begin[j]);
            j++;
        }
        i++;
    }

    fclose(eginno);
}