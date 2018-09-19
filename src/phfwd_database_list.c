/** @file
 * Implementacja jednokierunkowej listy baz przekierowań i funkcji z nią związanych.
 *
 * @author Jan Kociniak <jk394348@students.mimuw.edu.pl>
 * @date 01.06.2018
 */

#include "phfwd_database_list.h"
#include "string.h"

bool addDtb (dtbList* l, const char* id) {
    dtbList newElt = malloc(sizeof(struct phFwdDatabaseList));

    if (newElt == NULL)
        return false;

    else {
        newElt->next = *l;
        newElt->id = calloc(strlen(id) + 1, sizeof(char));

        if (newElt->id == NULL) {
            free(newElt);
            return false;
        }

        newElt->database = phfwdNew();

        if (newElt->database == NULL) {
            free(newElt->id);
            free(newElt);
            return false;
        }

        strcpy(newElt->id, id);
    }

    *l = newElt;
    return true;
}

bool removeDtb (dtbList* l, const char* id) {
    dtbList head = *l;
    dtbList prev = *l;

    if (head != NULL) {
        if (strcmp(head->id, id) == 0) {
            free(head->id);
            phfwdDelete(head->database);

            *l = head->next;
            free(head);

            return true;
        }

        else {
            head = head->next;

            while (head != NULL) {
                if (strcmp(head->id, id) == 0) {
                    free(head->id);
                    phfwdDelete(head->database);

                    prev->next = head->next;
                    free(head);

                    return true;
                }

                else {
                    prev = head;
                    head = head->next;

                }
            }

            return false;
        }
    }

    else
        return false;
}

bool dtbExists (dtbList l, const char* id) {
    dtbList head = l;

    while (head != NULL) {
        if (strcmp(head->id, id) == 0)
            return true;

        head = head->next;
    }

    return false;
}

void removeDtbList (dtbList l) {
    if (l != NULL) {
        removeDtbList(l->next);
        free(l->id);
        phfwdDelete(l->database);
        free(l);
    }
}

dtbList getDtb (dtbList l, const char* id) {
    dtbList head = l;

    while (head != NULL) {
        if (strcmp(head->id, id) == 0)
            return head;

        head = head->next;
    }

    return NULL;
}

