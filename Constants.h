//
// Created by dasch on 08.10.2017.
//

#ifndef EUGENE_ONEGIN_CONSTANTS_H
#define EUGENE_ONEGIN_CONSTANTS_H

#define POISON_N -1
#define FILE_NOT_FOUND 0
#define EMPTY_FILE 1
#define NOT_ENOUGH_MEMORY 2
#define UNEXPECTED_EOF 3
#define FILE_READ_ERR 4
#define BAD_POINTER 5
#define Error_here(ER)\
switch(ER) {\
    case (FILE_NOT_FOUND): {\
        printf("No such file. Try again.");\
        break;\
    }\
    case (EMPTY_FILE): {\
        printf("File choosed is emty. No text to sort.");\
        break;\
    }\
    case (NOT_ENOUGH_MEMORY): {\
        printf("There is not enough memory. Sorry :(");\
        break;\
    }\
    case (UNEXPECTED_EOF): {\
        printf("Unexpected end of file.");\
        break;\
    }\
    case (FILE_READ_ERR): {\
        printf("File reading error.");\
        break;\
    }\
    case (BAD_POINTER): {\
        printf("Function's got invalid pointer.");\
        break;\
    }\
    default: {\
        printf("Ooops! Some error...");\
    }\
}

#endif //EUGENE_ONEGIN_CONSTANTS_H

struct String {
    unsigned long int len;//inizialization?
    char *begin;
};

//void Error_here (int err_code);                         //tell if there is a problem and what problem it is by err_code
char *File_read (int *n);                               //put the text from file to char's array, counting num of symbols
unsigned int N_str (const char *poem);                  //count lines in poem
char **P_partition (char *poem, unsigned int n_str);    //creating char*'s array of pointers on the strings from poem
int Comparator (const void *S1, const void *S2);        //compare two lines s1 and s2
int Sort (struct String *, unsigned int);               //qsort() with some checks
int File_write (struct String *, unsigned int);         //writing the result in another file