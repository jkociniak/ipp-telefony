/** @file
 * Główny plik zapewniający interfejs tekstowy do operacji na numerach.
 *
 * @author Jan Kociniak <jk394348@students.mimuw.edu.pl>
 * @date 01.06.2018
 */

#include <string.h>
#include "phfwd_database_list.h"
#include "phone_forward.h"
#include "stdio.h"
#include "parser.h"


/** Główna funkcja parsująca dane wejściowe.
 * @return Wartość 0, gdy bezbłędnie przetworzono całe dane wejściowe.
 *         Wartość 1, gdy gdzieś wystąpił błąd.
 */
int main (void) {
    // INICJALIZACJA
    size_t position = 0;
    size_t* pos = &position;

    dynStr buffer = dynStrInit();

    dtbList dtb = NULL;
    dtbList* dtblist = &dtb;

    dtbList temp = NULL;
    dtbList* current = &temp;

    int error = 0;

    // ZAPĘTLENIE PARSOWANIA
    int c = getchar();
    (*pos)++;

    while (c != EOF) {
        ungetc(c, stdin);
        (*pos)--;

        if (!parseExpression(pos, buffer, dtblist, current)) {
            error = 1;
            break;
        }

        c = getchar();
        (*pos)++;
    }

    // DEALOKACJA PAMIĘCI
    dynStrDelete(buffer);
    removeDtbList(dtb);

    return error;
}