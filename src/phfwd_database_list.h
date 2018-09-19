/** @file
 * Specyfikacja jednokierunkowej listy baz przekierowań i funkcji z nią związanych.
 *
 * @author Jan Kociniak <jk394348@students.mimuw.edu.pl>
 * @date 01.06.2018
 */

#ifndef TELEFONY_PHFWD_DATABASE_LIST_H
#define TELEFONY_PHFWD_DATABASE_LIST_H

#include "phone_forward.h"

/** @brief Lista baz przekierowań.
 *  Lista jest jednokierunkowa.
 */
struct phFwdDatabaseList {
    char* id;                        ///< Wskaźnik na identyfikator tablicy.
    PhoneFwd database;               ///< Wskaźnik na bazę przekierowań.
    struct phFwdDatabaseList* next;  ///< Wskaźnik na następną komórkę listy.
};

/** Skrócona nazwa dla wskaźnika na komórkę listy baz przekierowań.
 */
typedef struct phFwdDatabaseList* dtbList;

/** @brief Dodaje bazę do listy.
 * Dodaje bazę o identyfikatorze @p id do listy @p l. Po dodaniu znajduje się
 * ona na czele listy. Funkcja ta nie sprawdza czy baza o danym identyfikatorze
 * jest już w liście.
 * @param[in,out] l  –  Wskaźnik na wskaźnik na pierwszą komórkę listy;
 * @param[in] id     –  Wskaźnik na napis reprezentujący identyfikator bazy.
 * @return Wartość @p true, jeśli baza została dodana.
 *         Wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
bool addDtb (dtbList* l, const char* id);

/** @brief Usuwa bazę z listy.
 * Usuwa bazę o identyfikatorze @p id z listy @p l.
 * @param[in,out] l  –  Wskaźnik na wskaźnik na pierwszą komórkę listy;
 * @param[in] id     –  Wskaźnik na napis reprezentujący identyfikator bazy.
 * @return Wartość @p true, jeśli baza została poprawnie usunięta.
 *         Wartość @p false, jeśli w liście nie ma bazy o danym identyfikatorze.
 */
bool removeDtb (dtbList* l, const char* id);

/** @brief Usuwa listę.
 * Usuwa listę wskazywaną przez @p l. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in,out] l  –  Wskaźnik na wskaźnik na pierwszą komórkę listy;
 */
void removeDtbList (dtbList l);

/** Sprawdza, czy baza o podanym identyfikatorze jest w liście.
 * @param[in,out] l  –  Wskaźnik na wskaźnik na pierwszą komórkę listy;
 * @param[in] id     –  Wskaźnik na napis reprezentujący identyfikator bazy.
 * @return Wartość @p true, jeśli baza o podanym identyfikatorze jest w liście.
 *         Wartość @p false w przeciwnym wypadku.
 */
bool dtbExists (dtbList l, const char* id);

/** Zwraca bazę o podanym identyfikatorze z listy.
 * @param[in,out] l  –  Wskaźnik na wskaźnik na pierwszą komórkę listy;
 * @param[in] id     –  Wskaźnik na napis reprezentujący identyfikator bazy.
 * @return Wskaźnik na bazę o podanym identyfikatorze lub NULL, jeśli bazy
 *         o takim identyfikatorze nie ma w liście.
 */
dtbList getDtb (dtbList l, const char* id);

#endif //TELEFONY_PHFWD_DATABASE_LIST_H
