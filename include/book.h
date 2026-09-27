// [COMMIT 3] Definição da estrutura `Book` e helpers públicos
#ifndef BOOK_H
#define BOOK_H

#define BOOK_TITLE_LEN 128
#define BOOK_AUTHOR_LEN 128

typedef struct Book {
    int id;
    char title[BOOK_TITLE_LEN];
    char author[BOOK_AUTHOR_LEN];
    int year;
} Book;

// [COMMIT 3] Funções utilitárias para `Book`
void book_init(Book *b, int id, const char *title, const char *author, int year);

#endif // BOOK_H
