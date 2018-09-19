/** @file
 * Specyfikacja parsera do interfejsu tekstowego operacji na numerach telefonów.
 *
 * @author Jan Kociniak <jk394348@students.mimuw.edu.pl>
 * @date 01.06.2018
 */

#include "parser.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* WSZYSTKIE FUNKCJE NA BIEŻĄCO AKTUALIZUJĄ PARAMETR POS, W KTÓRYM PRZECHOWYWANY *
 * JEST NUMER AKTUALNIE PRZETWARZANEGO ZNAKU.                                    */

/** @brief Funkcja wypisująca błąd składniowy znaku na pozycji @p pos.
 * Błąd wypisywany jest na wejście diagnostyczne.
 * @param[in,out] pos  –  Numer znaku który spowodował błąd.
 */
void syntaxError (size_t pos) {
    fprintf(stderr, "ERROR %zu\n", pos);
}
/** @brief Funkcja wypisująca błąd wykonania operatora na pozycji @p pos.
 * Błąd wypisywany jest na wejście diagnostyczne.
 * @param[in,out] pos  –  Numer pozycji operatora który spowodował błąd.
 * @param[in] op   –  Wskaźnik na napis reprezentujący operator.
 */
void execError (size_t pos, const char* op) {
    fprintf(stderr, "ERROR %s %zu\n", op, pos);
}

/** @brief Funkcja wypisująca błąd spowodowany niespodziewanym końcem danych.
 * Błąd wypisywany jest na wejście diagnostyczne.
 */
void eofError (void) {
    fprintf(stderr, "ERROR EOF\n");
}

/** @brief Funkcja pomijająca białe znaki.
 * Pomija wszystkie białe znaki z wejścia dopóki nie napotka niebiałego znaku.
 * @param[in,out] pos  –  Aktualny numer znaku na którym stanęło przetwarzanie danych.
 */
void skipWhiteChars (size_t* pos) {
    int c;
    do {
        c = getchar();
        (*pos)++;
    } while (isspace(c) != 0);

    ungetc(c, stdin);
    (*pos)--;
}

/** @brief Funkcja pomijająca wszystko co zawiera się w komentarzu.
 * Należy użyć po uprzednim sprawdzeniu że na wejściu pojawiły się dwa znaki '$'
 * z rzędu. Jeśli dane wejściowe się skończą zanim na wejściu znowu pojawi się
 * znak '$' dwa razy z rzędu, wypisuje błąd @ref eofError.
 * @param[in,out] pos  –  Aktualny numer znaku na którym stanęło przetwarzanie danych.
 * @return Wartość @p true, jeśli udało się pominąć komentarz bez błędu.
 *         Wartość @p false w przeciwnym wypadku.
 */
bool skipComment (size_t* pos) {
    int c;

    while (true) {
        do {
            c = getchar();
            (*pos)++;

            if (c == EOF) {
                eofError();
                return false;
            }
        } while (c != '$');

        c = getchar();
        (*pos)++;

        if (c == EOF) {
            eofError();
            return false;
        }

        if (c == '$')
            return true;
    }
}

/** Funkcja parsująca token odpowiadający operatorowi NEW lub DEL.
 * @param[in,out] pos     –  Aktualny numer znaku na którym stanęło przetwarzanie danych.
 * @param[in,out] buffer  –  Wskaźnik na bufor danych typu @p dynamicString.
 * @return Wartość 0, jeśli napotkano jakikolwiek błąd.
 *         Wartość 1, jeśli sparsowany token odpowiada operatorowi NEW.
 *         Wartość 2, jeśli sparsowany token odpowiada operatorowi DEL.
 */
int validNEWorDEL (size_t* pos, dynStr buffer) {
    size_t startPos = *pos + 1;
    dynStrReset(buffer);
    char c = (char)getchar();
    (*pos)++;

    while (c != EOF && buffer->used < 4) {
        if (c == '$') {
            syntaxError(startPos);
            return 0;
        }

        if (!dynStrAdd(buffer, c)) {
            fprintf(stderr, "MEMORY ERROR\n");
            return false;
        }

        c = (char)getchar();
        (*pos)++;
    }

    if (buffer->used < 4) {
        syntaxError(startPos);
        return 0;
    }

    ungetc(c, stdin);
    (*pos)--;

    if (strcmp(buffer->str, "NEW") == 0)
        return 1;

    else if (strcmp(buffer->str, "DEL") == 0)
        return 2;

    else {
        syntaxError(startPos);
        return 0;
    }
}


/** @brief Funkcja parsująca token odpowiadający identyfikatorowi bazy.
 * Identyfikator nie może być NEW lub DEL. Musi zaczynać się od litery.
 * @param[in,out] pos     –  Aktualny numer znaku na którym stanęło przetwarzanie danych.
 * @param[in,out] buffer  –  Wskaźnik na bufor danych typu @p dynamicString.
 * @return Wartość @p true, jeśli poprawnie sparsowano identyfikator.
 *         Wartość @p false w przeciwnym wypadku.
 */
bool getID (size_t* pos, dynStr buffer) {
    size_t startPos = *pos + 1;
    dynStrReset(buffer);
    char c = (char)getchar();
    (*pos)++;

    while (isalnum(c) != 0 && c != EOF) {
        if (!dynStrAdd(buffer, c)) {
            fprintf(stderr, "MEMORY ERROR\n");
            return false;
        }

        c = (char)getchar();
        (*pos)++;
    }

    ungetc(c, stdin);
    (*pos)--;

    if (buffer->used == 1) {
        syntaxError(startPos);
        return false;
    }

    else if (strcmp(buffer->str, "NEW") == 0 ||
             strcmp(buffer->str, "DEL") == 0 ||
             isdigit(buffer->str[0]) != 0) {
        syntaxError(startPos);

        return false;
    }

    return true;
}

/** Funkcja parsująca token odpowiadający numerowi.
 * @param[in,out] pos     –  Aktualny numer znaku na którym stanęło przetwarzanie danych.
 * @param[in,out] buffer  –  Wskaźnik na bufor danych typu @p dynamicString.
 * @return Wartość @p true, jeśli poprawnie sparsowano numer.
 *         Wartość @p false w przeciwnym wypadku.
 */
bool getNum (size_t* pos, dynStr buffer) {
    size_t startPos = *pos + 1;
    dynStrReset(buffer);
    char c = (char)getchar();
    (*pos)++;

    while (isValidDigit(c) != 0 && c != EOF) {
        if (!dynStrAdd(buffer, c)) {
            fprintf(stderr, "MEMORY ERROR\n");
            return false;
        }

        c = (char) getchar();
        (*pos)++;
    }

    if (c == EOF) {
        eofError();
        return false;
    }

    ungetc(c, stdin);
    (*pos)--;

    if (buffer->used == 1) {
        syntaxError(startPos);
        return false;
    }

    return true;
}

/** Funkcja sprawdzająca czy kolejne dwa znaki są poprawnym początkiem komentarza.
 * @param[in,out] pos     –  Aktualny numer znaku na którym stanęło przetwarzanie danych.
 * @return Wartość -1, jeśli wystąpił dokładnie jeden znak '$' a po nim coś innego.
 *         Wartość 1, jeśli pierwszy znak jest różny od '$'.
 *         Wartość -0, jeśli obydwa znaki to '$'.
 */
int isValidComment (size_t* pos) {
    size_t startPos = (*pos) + 1;
    int c = getchar();
    (*pos)++;

    if (c == '$') {
        int d = getchar();
        (*pos)++;

        if (d != '$') {
            syntaxError(startPos);
            return -1;
        }

        else
            return 0;
    }

    else {
        ungetc(c, stdin);
        (*pos)--;
        return 1;
    }
}

bool parseExpression (size_t* pos, dynStr buffer,
                      dtbList* dtblist, dtbList* current) {

    size_t startPos;
    int c, flag;

    // Pomijamy wszelkie białe znaki i komentarze z początku wejścia.
    do {
        startPos = *pos;
        skipWhiteChars(pos);

        flag = isValidComment(pos);

        if (flag == 0) {
            if (!skipComment(pos))
                return false;
        }
        else if (flag == -1)
            return false;

    } while ((*pos) != startPos);

    c = getchar();
    (*pos)++;

    // Jeśli wejście się zakończyło bez jakichkolwiek niepomijalnych danych.
    if (c == EOF) {
        ungetc(c, stdin);
        (*pos)--;
        return true;
    }

    // Każde poprawne wejście zaczyna się od litery, cyfry lub znaku '?'.
    if (isalpha(c) == 0 && isValidDigit(c) == 0 && c != '?' && c != '@') {
        syntaxError(*pos);
        return false;
    }

    if (c == '?') {
        size_t opPos = *pos;

        // Pomijamy wszelkie białe znaki i komentarze z początku wejścia.
        do {
            startPos = *pos;
            skipWhiteChars(pos);

            flag = isValidComment(pos);

            if (flag == 0) {
                if (!skipComment(pos))
                    return false;
            }

            else if (flag == -1)
                return false;
        } while ((*pos) != startPos);

        /* Po znaku '?' i dowolnej (może być zerowej) liczbie białych znaków
           oraz komentarzy musi nastąpić niezerowej długości numer. */
        if (getNum(pos, buffer)) {

            /* Wszelkie operacje na numerach przy nieustawionej bazie przekierowań
             * są błędne. */
            if (*current == NULL) {
                execError(opPos, "?");
                return false;
            }

            const PhoneNum* phfwds = phfwdReverse((*current)->database, buffer->str);

            // Sprawdzenie czy nie udało się zaalokować pamięci.
            if (phfwds == NULL) {
                execError(opPos, "?");
                return false;
            }

            else {
                size_t id = 0;

                while (phnumGet(phfwds, id) != NULL)
                    printf("%s\n", phnumGet(phfwds, id++));

                phnumDelete(phfwds);

                return true;
            }
        }

        else
            return false;
    }

    else if (c == '@') {
        size_t opPos = *pos;

        // Pomijamy wszelkie białe znaki i komentarze z początku wejścia.
        do {
            startPos = *pos;
            skipWhiteChars(pos);

            flag = isValidComment(pos);

            if (flag == 0) {
                if (!skipComment(pos))
                    return false;
            }

            else if (flag == -1)
                return false;
        } while ((*pos) != startPos);

        /* Po znaku '@' i dowolnej (może być zerowej) liczbie białych znaków
           oraz komentarzy musi nastąpić niezerowej długości numer. */
        if (getNum(pos, buffer)) {

            /* Wszelkie operacje na numerach przy nieustawionej bazie przekierowań
             * są błędne. */
            if (*current == NULL) {
                execError(opPos, "@");
                return false;
            }

            size_t len;
            len = strlen(buffer->str);

            if (len < 12)
                len = 0;
            else
                len = len - 12;

            printf("%zu\n", phfwdNonTrivialCount((*current)->database, buffer->str, len));

            return true;
        }

        else
            return false;
    }

    else if (isValidDigit(c) != 0) {
        ungetc(c, stdin);
        (*pos)--;

        /* Jeśli pierwszym znakiem wejścia jest cyfra, to jedynym poprawnym
         * tokenem jest numer. */
        if (!getNum(pos, buffer))
            return false;

        char num1[buffer->used];
        strcpy(num1, buffer->str);

        // Pomijamy wszelkie białe znaki i komentarze z początku wejścia.
        do {
            startPos = *pos;
            skipWhiteChars(pos);

            flag = isValidComment(pos);

            if (flag == 0) {
                if(!skipComment(pos))
                    return false;
            }

            else if (flag == -1)
                return false;
        } while ((*pos) != startPos);

        c = getchar();
        (*pos)++;

        if (c == EOF) {
            eofError();
            return false;
        }

        // Po poprawnie wczytanym numerze musi nastąpić operator '>' lub '?'
        if (c == '>') {
            size_t opPos = *pos;

            // Pomijamy wszelkie białe znaki i komentarze z początku wejścia.
            do {
                startPos = *pos;
                skipWhiteChars(pos);

                flag = isValidComment(pos);

                if (flag == 0) {
                    if(!skipComment(pos))
                        return false;
                }

                else if (flag == -1)
                    return false;
            } while ((*pos) != startPos);

            /* Po znaku '>' musi nastąpić pewna liczba białych znaków oraz
             * komentarzy, po czym poprawny numer. */
            if (getNum(pos, buffer)) {
                char num2[buffer->used];
                strcpy(num2, buffer->str);

                /* Wszelkie operacje na numerach przy nieustawionej bazie przekierowań
                 * są błędne. */
                if (*current == NULL) {
                    execError(opPos, ">");
                    return false;
                }

                if (phfwdAdd((*current)->database, num1, num2))
                    return true;

                else  {
                    execError(opPos, ">");
                    return false;
                }
            }

            else
                return false;
        }

        else if (c == '?') {

            /* Wszelkie operacje na numerach przy nieustawionej bazie przekierowań
             * są błędne. */
            if (*current == NULL) {
                execError(*pos, "?");
                return false;
            }

            const PhoneNum* phfwd = phfwdGet((*current)->database, num1);

            if (phfwd == NULL) {
                execError(*pos, "?");
                return false;
            }

            else {
                printf("%s\n", phnumGet(phfwd, 0));
                phnumDelete(phfwd);
                return true;
            }
        }

        else {
            syntaxError(*pos);
            return false;
        }
    }

    ungetc(c, stdin);
    (*pos)--;

    /* Wejście nie zaczyna się operatorem ani cyfrą, więc jedyną poprawną opcją
     * jest operator NEW lub DEL. */
    int newOrDel = validNEWorDEL(pos, buffer);

    if (newOrDel != 0) {
        size_t oldPos = (*pos);

        // Pomijamy wszelkie białe znaki i komentarze z początku wejścia.
        do {
            startPos = *pos;
            skipWhiteChars(pos);

            flag = isValidComment(pos);

            if (flag == 0) {
                if (!skipComment(pos))
                    return false;
            }

            else if (flag == -1)
                return false;
        } while ((*pos) != startPos);

        c = getchar();
        (*pos)++;

        if (c == EOF) {
            eofError();
            return false;
        }

        /* Jeśli po NEW pojawi się operator, to błąd składni bo pojawił się
         * nowy token, niepoprawny. */
        if (c == '?' || c == '>') {
            syntaxError(*pos);
            return false;
        }

        ungetc(c, stdin);
        (*pos)--;

        // Po operatorze NEW/DEL musi być spacja.
        if ((*pos) == oldPos) {
            syntaxError((*pos) - 2);
            return false;
        }

        if (newOrDel == 1) {
            if (getID(pos, buffer)) {
                // Jeśli dana baza istnieje, to zmieniamy na nią wskaźnik na aktualną.
                if (dtbExists(*dtblist, buffer->str)) {
                    *current = getDtb(*dtblist, buffer->str);
                    return true;
                }

                // Jeśli nie, dodajemy ją i dopiero wtedy zmieniamy wskaźnik.
                else {
                    if (!addDtb(dtblist, buffer->str)) {
                        fprintf(stderr, "MEMORY ERROR\n");
                        return false;
                    }

                    *current = *dtblist;
                    return true;
                }
            }

            else
                return false;
        }

        else {
            c = getchar();
            (*pos)++;
            bool succeed;

            // Musimy sprawdzić, czy będzie DEL identyfikator czy DEL numer.
            int type = 0; //1 - num, 2 - id

            if (isValidDigit(c) != 0) {
                ungetc(c, stdin);
                (*pos)--;
                succeed = getNum(pos, buffer);
                type = 1;
            }

            else if (isalpha(c) != 0) {
                ungetc(c, stdin);
                (*pos)--;
                succeed = getID(pos, buffer);
                type = 2;
            }

            else if (c == EOF) {
                eofError();
                succeed = false;
            }

            else {
                syntaxError(*pos);
                succeed = false;
            }

            if (!succeed)
                return false;

            else {
                if (type == 1) {
                    if (*current != NULL) {
                        phfwdRemove((*current)->database, buffer->str);
                        return true;
                    }

                    /* Wszelkie operacje na numerach przy nieustawionej bazie przekierowań
                     * są błędne. */
                    else {
                        execError(oldPos - 2, "DEL");
                        return false;
                    }
                }

                else {
                    /* Sprawdzamy czy aktualna baza danych będzie usuwana.
                     * Jeśli tak, to usuwamy wskaźnik na nią.   */
                    if (*current != NULL) {
                        if (strcmp(buffer->str, (*current)->id) == 0)
                            *current = NULL;
                    }

                    if (removeDtb(dtblist, buffer->str)) {
                        return true;
                    }

                    else {
                        execError(oldPos - 2, "DEL");
                        return false;
                    }
                }
            }
        }
    }

    else
        return false;
}




