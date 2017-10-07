#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <assert.h>

#define Strings_num 4

int Comarator_of_strings(const void* first, const void* second) {
    return (strcmp((const char*) first, (const char*) second));
}

int main() {
    //read from file, remembering pointer on every string
    //the beginning of new string is symbol after '\n'
    FILE *poem;
    //poem = fopen("C:/Users/dasch/Documents/MIPT/Technotrek/Eugene Onegin/cmake-build-debug/Onegin.txt", "r");
    poem = fopen("Onegin.txt", "r");
    assert(poem != NULL);

    char **strings[Strings_num]; //arrays of pointers NEEDS DECLARATION
    //We should change the type FILE* to char*
    char *The_poem = NULL;
    fseek(poem, 0, SEEK_END); //сместились в конец файла
    long int num_of_symb = ftell(poem); //нашли количество символов в файле
    The_poem = calloc(num_of_symb, sizeof(char));
    rewind(poem);
    fread(The_poem, sizeof(char), num_of_symb, poem); //преобразовали FILE* in char*

    int i = 0, string = 0;
    strings[string] = &The_poem;
    for ( ; i < num_of_symb-1; i++) {
        if (The_poem[i] == '\\' && The_poem[i+1] == 'n') {
            i += 2;
            strings[string] = &The_poem + i;
        }
    }

    int str_position = 1;
    i = 0;

    while (The_poem[i] != EOF) { //filling strings with pointers on strings
        if (The_poem[i] == '\n') {
            strings[str_position] = The_poem+i;
            str_position++;
        }
        i++;
    }

    //To look for quick sort
    qsort(strings, (size_t) str_position, sizeof(char *), Comarator_of_strings);

    fclose(poem);

    return 0;
}