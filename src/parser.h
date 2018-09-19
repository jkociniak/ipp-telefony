/** @file
 * Specyfikacja parsera do interfejsu tekstowego operacji na numerach telefonów.
 *
 * @author Jan Kociniak <jk394348@students.mimuw.edu.pl>
 * @date 01.06.2018
 */
#ifndef TELEFONY_PARSER_H
#define TELEFONY_PARSER_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "dynamic_string.h"
#include "phfwd_database_list.h"
#include "phone_forward.h"

/** @brief Funkcja parsująca dane wejściowe poprzez standardowe wejście.
 * Funkcja przetwarza dane do pierwszej możliwej operacji i ją wykonuje,
 * lub pomija całe wejście jeśli do końca są tylko białe znaki. Jeśli
 * dane są niepoprawne składniowo lub do wykonania danej operacji podane są
 * niepoprawne dane wypisuje błąd. Jeśli w trakcie przetwarzania danych nastąpi
 * niespodziewany koniec także wypisuje błąd.
 * @param[in,out] pos      –  Wskaźnik na zmienną zawierającą numer przetwarzanego
 *                            znaku.
 * @param[in,out] buffer   –  Wskaźnik na strukturę @p dynamicString będącą
 *                            buforem danych.
 * @param[in,out] dtblist  –  Wskaźnik na pierwszą komórkę listy baz przekierowań.
 * @param[in,out] current  –  Wskaźnik na aktualnie używaną bazę przekierowań.
 * @return Wartość @p true, jeśli udało się poprawnie sparsować pewną operację.
 *         Wartość @p false, jeśli gdzieś wystąpił błąd.
 */
bool parseExpression (size_t* pos, dynStr buffer,
                      dtbList* dtblist, dtbList* current);

#endif //TELEFONY_PARSER_H
