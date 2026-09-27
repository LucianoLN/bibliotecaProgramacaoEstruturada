// [COMMIT 3] Implementação de helpers para `Book`
#include <string.h>
#include "book.h"

// [COMMIT 3] Inicializa os campos de `Book`
void book_init(Book *b, int id, const char *title, const char *author, int year) {
    b->id = id;
    b->year = year;
    strncpy(b->title, title ? title : "", BOOK_TITLE_LEN - 1);
    b->title[BOOK_TITLE_LEN - 1] = '\0';
    strncpy(b->author, author ? author : "", BOOK_AUTHOR_LEN - 1);
    b->author[BOOK_AUTHOR_LEN - 1] = '\0';
}
