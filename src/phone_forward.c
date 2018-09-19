/** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Jan Kociniak <jk394348@students.mimuw.edu.pl>
 * @date 11.04.2018
 */

#include "phone_forward.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMBER_ALPHABET_SIZE 12 ///< Makro na rozmiar alfabetu znaków tworzących numer.


bool isValidDigit (int c) {
    return c >= 48 && c <= 59;
}

/** Sprawdza, czy dany napis jest poprawny.
 * @param[in] num  –  Wskaźnik na napis do sprawdzenia.
 * @return Wartość @p true, jeśli @p num wskazuje na poprawny napis
 *         reprezentujący niepusty ciąg składający się z cyfr od 0 do 9.
 *         Wartość @p false, jeśli @p num jest równy NULL lub reprezentuje
 *         pusty napis.
 */
bool isValidNumber(const char* num) {
    if (num != NULL && strcmp(num, "") != 0) {
        size_t keyLength = strlen(num);

        for (size_t i = 0; i < keyLength; i++)
            if (!isValidDigit(num[i]))
                return false;

        return true;
    }

    else
        return false;
}

PhoneFwd phfwdNew(void) {
    PhoneFwd newPhFwd = malloc(sizeof(struct PhoneForward));

    if (newPhFwd != NULL) {
        for (size_t i = 0; i < NUMBER_ALPHABET_SIZE; i++)
            (newPhFwd->children)[i] = NULL;

        newPhFwd->num = NULL;
        newPhFwd->numForward = NULL;
        newPhFwd->numLength = 0;
        newPhFwd->numForwardLength = 0;
    }

    return newPhFwd;
}

void phfwdDelete(PhoneFwd pf) {
    if (pf != NULL) {
        for (int i = 0; i < NUMBER_ALPHABET_SIZE; i++)
            phfwdDelete(pf->children[i]);

        if (pf->num != NULL)
            free(pf->num);

        if (pf->numForward != NULL)
            free(pf->numForward);

        free(pf);
    }
}

bool phfwdAdd(PhoneFwd pf, const char* num1, const char* num2) {
    if (pf == NULL || !isValidNumber(num1) || !isValidNumber(num2))
        return false;

    size_t keyLength = strlen(num1);
    size_t valueLength = strlen(num2);

    //Sprawdzenie czy napisy num1 i num2 są takie same.
    if (keyLength == valueLength) {
        bool equals = true;

        for (size_t i = 0; i < keyLength; i++) {
            if (num1[i] != num2[i]) {
                equals = false;
                break;
            }
        }

        if (equals == true)
            return false;
    }

    PhoneFwd nextNode = pf; //Startowy wierzchołek drzewa prefiksowego.

    for (size_t i = 0; i < keyLength; i++) {
        /* Sprawdzamy, czy w drzewie jest wierzchołek reprezentujący kolejny
         * prefiks num1. Jeśli nie, dodajemy go. */
        if (nextNode->children[num1[i] - 48] == NULL) {
            nextNode->children[num1[i] - 48] = phfwdNew();

            if (nextNode->children[num1[i] - 48] == NULL)
                return false;
        }

        nextNode = nextNode->children[num1[i] - 48];
    }

    /* Sprawdzamy, czy wierzchołek reprezentujący num1 zawiera jakieś
     * przekierowanie. Jeśli tak, usuwamy je.
     * Jeśli nie, uzupełniamy pole nextNode->num.
     */
    if (nextNode->numForward != NULL)
        free(nextNode->numForward);

    else {
        nextNode->num = calloc(keyLength + 1, sizeof(char));

        if (nextNode->num == NULL)
            return false;

        strcpy(nextNode->num, num1);
        nextNode->numLength = keyLength;
    }

    nextNode->numForward = calloc(valueLength+1, sizeof(char));

    if (nextNode->numForward == NULL)
        return false;

    strcpy(nextNode->numForward, num2);
    nextNode->numForwardLength = valueLength;

    return true;
}

void phfwdRemove(PhoneFwd pf, const char* num) {
    if(!isValidNumber(num))
        return;

    size_t keyLength = strlen(num);
    PhoneFwd nextNode = pf;

    /* Wierzchołek reprezentujący najdłuższy prefiks num, który musi zostać w
     * drzewie, to jest najdłuższy taki, że jest prefiksem co najmniej jednego
     * numeru w drzewie różnego od num. */
    PhoneFwd lastToSave = pf;

    /* Jeśli l to długość numeru reprezentowanego przez lastToSave, to
     * lastToSaveNextIndex to l+1-wsza cyfra num. To jest, jeśli lastToSave
     * ma zostać w drzewie, to należy usunąć poddrzewo
     * lastToSave->children[num[lastToSaveNextIndex] - 48]. */
    size_t lastToSaveNextIndex = 0;
    size_t i; //iterator

    for (i = 0; i < keyLength; i++) {
        // Jeśli NULL, to nie ma w drzewie wierzchołka reprezentującego num.
        if (nextNode->children[num[i] - 48] == NULL)
            return;

        else {
            for (size_t j = 0; j < (size_t)num[i] - 48; j++)
                if (nextNode->children[j] != NULL) {
                    lastToSave = nextNode;
                    lastToSaveNextIndex = i;
                    break;
                }

            for (size_t j = (size_t)num[i] - 48 + 1; j < NUMBER_ALPHABET_SIZE; j++) {
                if (nextNode->children[j] != NULL) {
                    lastToSave = nextNode;
                    lastToSaveNextIndex = i;
                    break;
                }
            }

            if (nextNode->numForward != NULL) {
                lastToSave = nextNode;
                lastToSaveNextIndex = i;
            }

            nextNode = nextNode->children[num[i] - 48];
        }
    }

    phfwdDelete(lastToSave->children[num[lastToSaveNextIndex] - 48]);
    lastToSave->children[num[lastToSaveNextIndex] - 48] = NULL;
}

PhoneNum* phnumNew(size_t len) {
    PhoneNum* newPhNum = malloc(sizeof(struct PhoneNumbers));

    if (newPhNum != NULL) {
        newPhNum->length = len;

        if (len > 0) {
            newPhNum->phNums = calloc(len, sizeof(char *));

            if (newPhNum->phNums == NULL) {
                free(newPhNum);
                return NULL;
            }

            for (size_t i = 0; i < len; i++)
                newPhNum->phNums[i] = NULL;
        }

        else
            newPhNum->phNums = NULL;
    }

    return newPhNum;
}

const PhoneNum* phfwdGet(PhoneFwd pf, const char* num) {
    if (pf == NULL)
        return NULL;

    if (!isValidNumber(num))
        return phnumNew(0);

    PhoneNum* numFwd = phnumNew(1);

    if (numFwd == NULL)
        return NULL;

    size_t keyLength = strlen(num);
    size_t longestPrefixLength = 0;

    /* Wskaźnik na napis reprezentujący przekierowanie
     * najdłuzszego prefiksu num. */
    char** prefForward = NULL;
    PhoneFwd nextNode = pf;

    for (size_t i = 0; i<keyLength; i++) {
        nextNode = nextNode->children[num[i] - 48];

        if (nextNode == NULL)
            break;

        else if (nextNode->numForward != NULL) {
            longestPrefixLength = nextNode->numLength;
            prefForward = &(nextNode->numForward);
        }
    }

    if (longestPrefixLength == 0) {
        numFwd->phNums[0] = calloc(keyLength + 1, sizeof(char));

        if (numFwd->phNums[0] == NULL) {
            free(numFwd);
            return NULL;
        }

        strcpy(numFwd->phNums[0], num);

        return numFwd;
    }

    else {
        size_t prefForwardLength = strlen(*prefForward);
        size_t remainderLength = keyLength - longestPrefixLength;

        numFwd->phNums[0] = calloc(prefForwardLength + remainderLength + 1, sizeof(char));

        if (numFwd->phNums[0] == NULL) {
            free(numFwd);
            return NULL;
        }

        strcpy(numFwd->phNums[0], *prefForward);
        strcpy(numFwd->phNums[0] + prefForwardLength, num + longestPrefixLength);

        return numFwd;
    }
}

/** Lista wskaźników na napisy.
 */
struct strList {
    char* num;             ///< Wskaźnik na napis.
    struct strList* next;  ///< Wskaźnik na kolejny element listy (NULL jeśli to ostatni element listy).
};

typedef struct strList* List; ///< Skrócona nazwa na wskaźnik na strukturę @p strList.

/** @brief Dodaje napis do listy.
 * Wymagana jest informacja o długości napisu.
 * @param[in] l           –  Wskaźnik na wskaźnik na początek listy, do której ma być dodany napis.
 * @param[in] numb        –  Wskaźnik na napis, który ma być dodany do listy.
 * @param[in] numbLength  –  Długość napisu, który ma być dodany do listy.
 * @return Wartość @p true, jeśli napis został dodany.
 *         Wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
bool strListAdd(List *l, const char* numb, size_t numbLength) {
    List newElt = malloc(sizeof(struct strList));

    if (newElt == NULL)
        return false;

    else {
        newElt->next = *l;
        newElt->num = calloc(numbLength + 1, sizeof(char));

        if(newElt->num == NULL)
           return false;

        strcpy(newElt->num, numb);
    }

    *l = newElt;

    return true;
}

/** Usuwa listę.
 * @param[in] l  –  Wskaźnik na początek listy do usunięcia.
 */
void delStrList(List l) {
    if (l != NULL) {
        delStrList(l->next);
        free(l->num);
        free(l);
    }
}

/** @brief Znajduje wszystkie przekierowania na dany numer.
 * Znajduje wszystkie przekierowania na dany numer i dodaje je do danej listy, używając funkcji @ref strListAdd.
 * Zlicza znalezione przekierowania. Przekierowania mogą się powtarzać.
 * @param[in] l           –  Wskaźnik na wskaźnik na pierwszy element listy.
 * @param[in] phFwd       –  Wskaźnik na strukturę przechowującą przekierowania;
 * @param[in] numb        –  Wskaźnik na napis reprezentujący numer;
 * @param[in] numbLength  –  Długość numeru;
 * @param[in] howMany     –  Wskaźnik na zmienną przechowującą liczbę znalezionych przekierowań.
 * @return Wartość @p true, jeśli udało się przeszukać całą strukturę przekierowań.
 *         Wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
bool goThroughTrie(List* l, PhoneFwd phFwd, const char* numb, size_t numbLength, size_t* howMany) {
    if (phFwd != NULL) {
        if (phFwd->num != NULL) {
            // Sprawdzamy czy phFwd->num jest prefiksem num.
            if (phFwd->numForwardLength <= numbLength &&
                strncmp(numb, phFwd->numForward, phFwd->numForwardLength) == 0) {

                    char res[phFwd->numLength + numbLength - phFwd->numForwardLength + 1];

                    strcpy(res, phFwd->num);
                    strcpy(res + phFwd->numLength, numb + phFwd->numForwardLength);

                    if (strListAdd(l, res, phFwd->numLength + numbLength - phFwd->numForwardLength))
                        (*howMany)++;

                    else
                        return false;
            }
        }

        for(size_t i = 0; i < NUMBER_ALPHABET_SIZE; i++)
            if (!goThroughTrie(l, phFwd->children[i], numb, numbLength, howMany))
                return false;

        return true;
    }

    return true;
}

/** Komparator napisów. Używa porządku leksykograficznego.
 * @param p1  –  Wskaźnik na pierwszy napis;
 * @param p2  –  Wskaźnik na drugi napis.
 * @return Wartość @p >0, jeśli pierwszy napis jest większy;
 *         Wartość @p 0, jeśli oba napisy są równe;
 *         Wartość @p <0, jeśli pierwszy napis jest mniejszy.
 */
int compare(const void* p1, const void* p2) {
    char* const* s1 = p1;
    char* const* s2 = p2;

    return strcmp(*s1, *s2);
}

const PhoneNum* phfwdReverse(PhoneFwd pf, const char* num) {
    if (pf == NULL)
        return NULL;

    if (!isValidNumber(num))
        return phnumNew(0);

    size_t numLength = strlen(num);
    PhoneNum* revs;
    List revsList = NULL;
    strListAdd(&revsList, num, numLength);
    size_t numOfRevs = 1;

    if(!goThroughTrie(&revsList, pf, num, numLength, &numOfRevs)) {
        delStrList(revsList);
        return NULL;
    }

    revs = phnumNew(numOfRevs);
    List tempNode = revsList;
    List nextNode = NULL;

    /* Zrzucamy numery z listy do tablicy, usuwając przy okazji listę. */
    for (size_t i = 0; i < revs->length; i++) {
        revs->phNums[i] = tempNode->num;

        if (tempNode->next != NULL) {
            nextNode = tempNode->next;
        }

        free(tempNode);
        tempNode = nextNode;
    }

    qsort(revs->phNums, numOfRevs, sizeof(char*), compare);

    size_t numOfRevsWithoutRepeat = 1;

    char* previous = revs->phNums[0];

    for (size_t i = 1; i < revs->length; i++)
        if (strcmp(previous, revs->phNums[i]) != 0) {
            numOfRevsWithoutRepeat++;
            previous = revs->phNums[i];
        }

    PhoneNum* revsFinal = phnumNew(numOfRevsWithoutRepeat);

    revsFinal->phNums[0] = revs->phNums[0];
    previous = revs->phNums[0];
    size_t j = 1; //iterator dla revsFinal->phNums


    for (size_t i = 1; i < revs->length; i++) {
        if (strcmp(previous, revs->phNums[i]) != 0) {
            revsFinal->phNums[j] = revs->phNums[i];
            previous = revs->phNums[j];
            j++;
        }


        /* Jeśli napis się powtórzył, to usuwamy jego duplikat z tablicy, żeby
         * można było potem bezpiecznie usunąć całą tablicę WSKAŹNIKÓW(!). */
        else
            free(revs->phNums[i]);
    }

    free(revs->phNums);
    free(revs);

    return revsFinal;
}

/** Funkcja implementująca szybkie potęgowanie liczb całkowitych nieujemnych.
 * @param x  –  Podstawa.
 * @param n  –  Wykładnik.
 * @return Wynik x^n.
 */
static size_t fastPow(size_t x, size_t n) {
    size_t res = 1;

    while (n > 0) {
        if (n & 1)
            res = res * x;

        x *= x;
        n >>= 1;
    }

    return res;
}

/** @brief Drzewo prefiksowe z informacją o długości prefiksu.
 *  Pole leaf określa, czy prefiks w danym węźle drzewa jest przechodni czy
 *  określa prawdziwy prefiks.
 */
struct prefixTree {
    struct prefixTree* children[NUMBER_ALPHABET_SIZE]; /**< Tablica wskaźników na pochodne
                                                              prefiksy dłuższe o jedną cyfrę. */
    bool leaf;     ///< Czy dany węzeł reprezentuje prawdziwy prefiks.
    size_t length; ///< Długość prefiksu.
};

typedef struct prefixTree* prefTree; /**< Skrócona nazwa na wskaźnik na strukturę
                                          @p prefixTree */

/** Funkcja zwracająca wskaźnik na nową, pustą strukturę @p prefTree.
 * @return Wskaźnik na nową strukturę @p prefTree. NULL jeśli nie udało się
 *         zaalokować pamięci.
 */
prefTree newPrefTree (void) {
    prefTree t = malloc(sizeof(struct prefixTree));
    if (t != NULL) {
        for (size_t i = 0; i < NUMBER_ALPHABET_SIZE; i++)
            t->children[i] = NULL;

        t->leaf = false;
        t->length = 0;
    }

    return t;
}

/** Dodaje prefiks @p str do danego drzewa prefiksowego.
 * @param t    –  Wskaźnik na drzewo.
 * @param str  –  Wskaźnik na napis do dodania.
 * @return Wartość @p true, jeśli udało się dodać napis.
 *         Wartość @p false w przeciwnym wypadku.
 */
bool prefTreeAdd (prefTree t, const char* str) {
    prefTree nextNode = t;
    size_t keyLength = strlen(str);

    for (size_t i = 0; i < keyLength; i++) {
        /* Sprawdzamy, czy w drzewie jest wierzchołek reprezentujący kolejny
         * prefiks num1. Jeśli nie, dodajemy go. */
        if (nextNode->children[str[i] - 48] == NULL) {
            nextNode->children[str[i] - 48] = newPrefTree();

            if (nextNode->children[str[i] - 48] == NULL)
                return false;
        }

        else if (nextNode->children[str[i] - 48]->leaf)
            return true;

        nextNode = nextNode->children[str[i] - 48];
    }

    nextNode->leaf = true;
    nextNode->length = keyLength;

    return true;
}

/** @brief Funkcja zliczająca nietrywialne numery.
 * Zlicza na podstawie prefiksów z danego drzewa prefiksowego oraz parametrów.
 * @param pf              –  Wskaźnik na drzewo prefiksowe.
 * @param cnt             –  Wskaźnik na zmienną przechowującą wynik.
 * @param maxLength       –  Długość nietrywialnego numeru.
 * @param possibleDigits  –  Zbiór możliwych cyfr nietrywialnego numeru.
 */
void prefTreeCount (prefTree pf, size_t* cnt, const size_t maxLength,
                    const size_t possibleDigits) {
    if (pf != NULL) {
        if (pf->leaf) {
            size_t diff = maxLength - pf->length;
            *cnt += fastPow(possibleDigits, diff);

            return;
        }

        for (size_t i = 0; i < NUMBER_ALPHABET_SIZE; i++)
            prefTreeCount(pf->children[i], cnt, maxLength, possibleDigits);
    }
}

/** Usuwa drzewo prefiksowe.
 * @param pf  –  Wskaźnik na drzewo prefiksowe.
 */
void prefTreeDel (prefTree pf) {
    if (pf != NULL) {
        for (int i = 0; i < NUMBER_ALPHABET_SIZE; i++)
            prefTreeDel(pf->children[i]);

        free(pf);
    }
}

/** @brief Funkcja dodająca nietrywialne prefiksy do drzewa prefiksowego.
 * Przez nietrywialne prefiksy rozumiemy nietrywialne numery krótsze niż
 * dana długość.
 * @param pf              –  Wskaźnik na strukturę przechowującą przekierowania.
 * @param digits          –  Wskaźnik na tablicę przechowującą informację,
 *                           które cyfry są możliwe w nietrywialnych prefiksach.
 * @param maxLen          –  Długość nietrywialnego numeru.
 * @param possibleDigits  –  Moc zbioru możliwych liczb nietrywialnego numeru.
 * @param prefixes        –  Wskaźnik na drzewo prefiksowe.
 */
void phfwdNTC (PhoneFwd pf, const bool* digits, size_t maxLen,
               const size_t possibleDigits, prefTree prefixes) {
    if (pf != NULL) {
        if (pf->numForward != NULL) {
            bool isValidNumber = true;
            size_t i = 0;

            while (pf->numForward[i] != 0 && i < maxLen) {
                if (!digits[pf->numForward[i] - 48]) {
                    isValidNumber = false;
                    break;
                }

                i++;
            }

            if (isValidNumber && pf->numForward[i] == 0) {
                prefTreeAdd(prefixes, pf->numForward);
            }
        }

        for (size_t j = 0; j < NUMBER_ALPHABET_SIZE; j++)
            phfwdNTC(pf->children[j], digits, maxLen, possibleDigits, prefixes);
    }
}


size_t phfwdNonTrivialCount(PhoneFwd pf, const char* set, size_t len) {
    if (pf == NULL || set == NULL || len == 0)
        return 0;

    size_t setLen = strlen(set);

    if (setLen == 0)
        return 0;

    bool digits[12];
    size_t i = 0;
    for (; i < 12; i++)
        digits[i] = false;

    size_t digitsRead = 0; //Ile cyfr ze zbioru już wczytaliśmy.
    i = 0;

    while (i < setLen && digitsRead < 13) {
        if (isValidDigit(set[i]) && !digits[set[i] - 48]) {
            digits[set[i] - 48] = true;
            digitsRead++;
        }

        i++;
    }

    size_t res = 0;
    prefTree prefixes = newPrefTree();

    phfwdNTC(pf, digits, len, digitsRead, prefixes);
    prefTreeCount(prefixes, &res, len, digitsRead);
    prefTreeDel(prefixes);

    return res;
}


void phnumDelete(const PhoneNum* pnum) {
    if (pnum != NULL) {
        for (size_t i = 0; i < pnum->length; i++)
            free(pnum->phNums[i]);

        free(pnum->phNums);
        free((void*)pnum);
    }
}

const char* phnumGet(const PhoneNum* pnum, size_t idx) {
    if (pnum == NULL)
        return NULL;

    else if (idx > pnum->length - 1 || pnum->length == 0)
        return NULL;

    else {
        return pnum->phNums[idx];
    }
}






