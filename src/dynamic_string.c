/** @file
 * Implementacja rozszerzającej się w miarę potrzeb tablicy znaków oraz
 * funkcji z nią związanych.
 *
 * @author Jan Kociniak <jk394348@students.mimuw.edu.pl>
 * @date 01.06.2018
 */
#include "dynamic_string.h"

dynStr dynStrInit (void) {
    dynStr newDynStr = malloc(sizeof(struct dynamicString));

    if (newDynStr != NULL) {
        newDynStr->str = malloc(sizeof(char));

        if (newDynStr->str == NULL)
            return NULL;

        newDynStr->str[0] = '\0';
        newDynStr->size = 1;
        newDynStr->used = 1;
    }

    return newDynStr;
}

bool dynStrAdd (dynStr str, char c) {
    if (str->used == str->size) {
        str->size *= 2;
        void *strNew = realloc(str->str, str->size);

        if (strNew == NULL)
            return false;

        str->str = strNew;
    }

    str->str[str->used - 1] = c;
    str->str[(str->used)++] = '\0';
    return true;
}

void dynStrReset (dynStr str) {
    if (str != NULL) {
        free(str->str);
        str->str = malloc(sizeof(char));

        str->str[0] = '\0';
        str->size = 1;
        str->used = 1;
    }
}

void dynStrDelete (dynStr str) {
    if (str != NULL) {
        free(str->str);
        free(str);
    }
}