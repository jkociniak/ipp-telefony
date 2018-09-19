/** @file
 * Specyfikacja rozszerzającej się w miarę potrzeb tablicy znaków oraz
 * funkcji z nią związanych.
 *
 * @author Jan Kociniak <jk394348@students.mimuw.edu.pl>
 * @date 01.06.2018
 */

#ifndef TELEFONY_DYNAMIC_STRING_H
#define TELEFONY_DYNAMIC_STRING_H

#include <stdlib.h>
#include <stdbool.h>

/** @brief Rozszerzająca się w miarę potrzeb tablica znaków.
 *  Struktura ta zapewnia, że napis przechowywany w tablicy jest zakończony
 *  znakiem '/0'. "Pusta" tablica zawiera tylko ten znak.
 */
struct dynamicString {
    char* str;     ///< Wskaźnik na początek tablicy znaków.
    size_t size;   ///< Zmienna przechowująca maksymalny rozmiar tablicy.
    size_t used;   ///< Zmienna przechowująca aktualną liczbę znaków w tablicy.
};

/** Skrócona nazwa na wskaźnik na strukturę @p dynamicString.
 */
typedef struct dynamicString* dynStr;

/** @brief Zwraca wskaźnik na nową, pustą strukturę @p dynamicString.
 * Alokuje strukturę @p dynamicString, którą trzeba potem zwolnić używając
 * funkcji @ref dynStrDelete.
 * @return Wskaźnik na nową strukturę @p dynamicString lub NULL, jeśli
 *         nie udało się zaalokować pamięci.
 */
dynStr dynStrInit (void);

/** @brief Dodaje znak na koniec tablicy.
 * Jeśli tablica jest zapełniona, realokuje pamięć na dwukrotnie większy obszar.
 * @param[in,out] str  –   Wskaźnik na strukturę @p dynamicString;
 * @param[in] c        –   Znak do dodania.
 * @return Wartość @p true, jeśli udało się dodać znak.
 *         Wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
bool dynStrAdd (dynStr str, char c);

/** @brief Resetuje tablicę do początkowego stanu.
 * Zwalnia całą pamięć przeznaczoną na przechowywanie znaków i alokuje obszar
 * potrzebny na przechowanie jednego znaku. Resetuje liczniki @p size i @p used
 * do wartości 1. Nic nie robi, jeśli wskaźnik ma wartość NULL.
 * @param[in] str  –  Wskaźnik na strukturę @p dynamicString.
 */
void dynStrReset (dynStr str);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p str. Nic nie robi, jeśli wskaźnik ma
 * wartość NULL.
 * @param[in] str  –  Wskaźnik na strukturę @p dynamicString.
 */
void dynStrDelete (dynStr str);

#endif //TELEFONY_DYNAMIC_STRING_H
