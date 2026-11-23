/*
 * book.c - Funcoes do modelo Book.
 *
 * [PARTE 1]                         book_init.
 * [COMMIT 1 - ALOCACAO DINAMICA]    inicializa quantity.
 */
#include <string.h>
#include "book.h"
#include "util.h"

void book_init(Book *b, int id, const char *title, const char *author,
               int year, int quantity) {
    memset(b, 0, sizeof *b);        /* sem lixo de memoria nos arquivos binarios */
    b->id = id;
    b->year = year;
    b->quantity = quantity;
    str_copy(b->title, title, BOOK_TITLE_LEN);
    str_copy(b->author, author, BOOK_AUTHOR_LEN);
}
