/*
 * book.c - Funcoes do modelo Book.
 *
 * [PARTE 1]                         book_init.
 * [COMMIT 1 - ALOCACAO DINAMICA]    inicializa quantity.
 */
#include "book.h"
#include "util.h"

void book_init(Book *b, int id, const char *title, const char *author,
               int year, int quantity) {
    b->id = id;
    b->year = year;
    b->quantity = quantity;
    str_copy(b->title, title, BOOK_TITLE_LEN);
    str_copy(b->author, author, BOOK_AUTHOR_LEN);
}
