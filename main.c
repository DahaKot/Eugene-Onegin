#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <assert.h>

char *File_read(); //put the text prom file to char's array
int N_str(const char * poem); //count lines in poem
char **P_partition(char * poem, int); //creating char*'s array of pointers on the strings from poem
void Sort(char ** , int);
//int Comparator_of_strings(const char* first, const char* second); //meaning there are not 2 alike strings
void File_write(char *); //writing the result in another file

int main() {
    char *poem = File_read();
    int num_strings = N_str(poem);
    char **s_pointers = P_partition(poem, num_strings);

    //Here the trouble begin
    Sort(s_pointers, num_strings);

    free(poem);
    free(s_pointers);

    return 0;
}

char *File_read() {
    FILE *f_poem;
    f_poem = fopen("Onegin.txt", "r");
    assert(f_poem != NULL); //with if?

    //finding numbers of characters in the f_poem
    fseek(f_poem, 0, SEEK_END);
    long int num_of_symb = ftell(f_poem);

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

void Sort (char **pointers, int len) {
    char **result = calloc(len, sizeof(char *));
    int i = 0, j = 1;
    for ( ; i < len; i++) {
        for ( ; j < len; j++) {
            int l = 0, k = 0;
            while ((l < strlen(pointers[i])) && (l < strlen(pointers[j]))) {
                if (*(pointers[i]) != *(pointers[j])) {
                    k = (int) (*(pointers[i])) - (int) (*(pointers[j]));
                    break;
                }
                l++;
            }

            if (k > 0) {
                result[i] = pointers[j];
            }
            else {
                result[i] = pointers[i];
            }
        }
    }

    for (i = 0; i < len; i++) {
        File_write(result[i]);
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

void File_write(char * poem) {
    FILE *eginno;
    eginno = fopen("Sorted.txt", "a");
    assert(eginno != NULL);

    int i = 0;
    while (poem[i] != '\n' && poem[i] != EOF) {
        fprintf(eginno, "%c", poem[i]);
        i++;
    }

    fclose(eginno);
}