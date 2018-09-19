/** @file
 * Interfejs klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Marcin Peczarski <marpe@mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 09.04.2018
 */

#ifndef __PHONE_FORWARD_H__
#define __PHONE_FORWARD_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define NUMBER_ALPHABET_SIZE 12 ///< Makro na rozmiar alfabetu znaków tworzących numer.

/** @brief Struktura przechowująca przekierowania numerów telefonów.
 * Struktura jest w formie drzewa prefiksowego dowolnej długości ciągów
 * cyfr od 0,1,2,3,4,5,6,7,8,9,:,;.
 */
struct PhoneForward {
    struct PhoneForward* children[NUMBER_ALPHABET_SIZE]; /**< Tablica wskaźników na pochodne
                                                              prefiksy dłuższe o jedną cyfrę. */
    char* num;                         /**< Prefiks w tym wierzchołku.
                                            Jeśli NULL, to temu prefiksowi
                                            nie zostało przypisane
                                            przekierowanie. */
    char* numForward;                  ///< Przekierowanie prefiksu.
    size_t numLength;                  ///< Długość prefiksu.
    size_t numForwardLength;           ///< Długość przekierowania.
};

typedef struct PhoneForward* PhoneFwd; /**< Skrócona nazwa dla wskaźnika
                                            na strukturę @p PhoneForward. */


/** Struktura przechowująca ciąg (w formie tablicy) numerów telefonów.
 */
struct PhoneNumbers {
    char** phNums; ///< Tablica wskaźników na wskaźniki na napisy.
    size_t length; ///< Długość tablicy @p phNums.
};

/** Skrócona nazwa dla struktury @p PhoneNumbers.
 */
typedef struct PhoneNumbers PhoneNum;


/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowań.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         zaalokować pamięci.
 */
PhoneFwd phfwdNew(void);


/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pf. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pf – wskaźnik na usuwaną strukturę.
 */
void phfwdDelete(PhoneFwd pf);


/** @brief Dodaje przekierowanie.
 * Dodaje przekierowanie wszystkich numerów mających prefiks @p num1, na numery,
 * w których ten prefiks zamieniono odpowiednio na prefiks @p num2. Każdy numer
 * jest swoim własnym prefiksem. Jeśli wcześniej zostało dodane przekierowanie
 * z takim samym parametrem @p num1, to jest ono zastępowane.
 * @param[in] pf   – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num1 – wskaźnik na napis reprezentujący prefiks numerów
 *                   przekierowywanych;
 * @param[in] num2 – wskaźnik na napis reprezentujący prefiks numerów, na które
 *                   jest wykonywane przekierowanie.
 * @return Wartość @p true, jeśli przekierowanie zostało dodane.
 *         Wartość @p false, jeśli wystąpił błąd, np. podany napis nie
 *         reprezentuje numeru, oba podane numery są identyczne lub nie udało
 *         się zaalokować pamięci.
 */
bool phfwdAdd(PhoneFwd pf, const char* num1, const char* num2);


/** @brief Usuwa przekierowania.
 * Usuwa wszystkie przekierowania, w których parametr @p num jest prefiksem
 * parametru @p num1 użytego przy dodawaniu. Jeśli nie ma takich przekierowań
 * lub napis nie reprezentuje numeru, nic nie robi.
 *
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący prefiks numerów.
 */
void phfwdRemove(PhoneFwd pf, const char* num);


/** @brief Wyznacza przekierowanie numeru.
 * Wyznacza przekierowanie podanego numeru. Szuka najdłuższego pasującego
 * prefiksu. Wynikiem jest co najwyżej jeden numer. Jeśli dany numer nie został
 * przekierowany, to wynikiem jest ten numer. Jeśli podany napis nie
 * reprezentuje numeru, wynikiem jest pusty ciąg. Alokuje strukturę
 * @p PhoneNumbers, która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się zaalokować pamięci.
 */
const PhoneNum* phfwdGet(PhoneFwd pf, const char* num);


/** @brief Wyznacza przekierowania na dany numer.
 * Wyznacza wszystkie przekierowania na podany numer. Wynikowy ciąg zawiera też
 * dany numer. Wynikowe numery są posortowane leksykograficznie i nie mogą się
 * powtarzać. Jeśli podany napis nie reprezentuje numeru, wynikiem jest pusty
 * ciąg. Alokuje strukturę @p PhoneNumbers, która musi być zwolniona za pomocą
 * funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się zaalokować pamięci.
 */
const PhoneNum* phfwdReverse(PhoneFwd pf, const char* num);


/** @brief Oblicza liczbę nietrywialnych numerów.
 * Oblicza liczbę nietrywialnych numerów długości @p len zawierających tylko
 * cyfry, które znajdują się w napisie @p set.
 * @param pf  –  wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param set  –  wskaźnik na napis reprezentujący zbiór cyfr.
 * @param len  –  długość numeru.
 * @return Liczba nietrywialnych numerów o podanej długości i zbiorze cyfr.
 * Jeśli wskaźnik @p pf ma wartość NULL, @p set ma wartość NULL, @p set jest
 * pusty, @p set nie zawiera żadnej cyfry lub parametr @p len jest równy zeru,
 * wynikiem jest zero.
 */
size_t phfwdNonTrivialCount(PhoneFwd pf, const char* set, size_t len);


/** @brief Tworzy nową strukturę typu @p PhoneNumbers.
 * Tworzy nową strukturę niezawierającą żadnych numerów.
 * @param[in] len  –  długość tablicy w tworzonej strukturze.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         zaalokować pamięci.
 */
PhoneNum* phnumNew(size_t len);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pnum. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pnum – wskaźnik na usuwaną strukturę.
 */
void phnumDelete(const PhoneNum* pnum);


/** @brief Udostępnia numer.
 * Udostępnia wskaźnik na napis reprezentujący numer. Napisy są indeksowane
 * kolejno od zera.
 * @param[in] pnum – wskaźnik na strukturę przechowującą ciąg napisów;
 * @param[in] idx  – indeks napisu.
 * @return Wskaźnik na napis. Wartość NULL, jeśli wskaźnik @p pnum ma wartość
 *         NULL lub indeks ma za dużą wartość.
 */
const char* phnumGet(const PhoneNum* pnum, size_t idx);

/** @brief Sprawdza, czy c jest prawidłowym znakiem reprezentującym cyfrę.
 * Zależy od definicji cyfry.
 * @param c  –  Znak do sprawdzenia.
 * @return Wartość @p true, jeśli podany znak jest cyfrą.
 *         Wartość @p false w przeciwnym wypadku.
 */
bool isValidDigit (int c);

#endif /* __PHONE_FORWARD_H__ */
