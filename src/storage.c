// [COMMIT 3] Implementação do armazenamento em memória e persistência simples
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

static Book *books = NULL;
static size_t books_count = 0;
static size_t books_capacity = 0;
static int next_id = 1;

// [COMMIT 3] Internal: ensure capacity
static void ensure_capacity(void) {
    if (books_count >= books_capacity) {
        size_t newcap = books_capacity ? books_capacity * 2 : 8;
        Book *nb = realloc(books, newcap * sizeof(Book));
        if (!nb) return;
        books = nb;
        books_capacity = newcap;
    }
}

// [COMMIT 3] Adiciona livro
int storage_add_book(const char *title, const char *author, int year) {
    ensure_capacity();
    if (books_count >= books_capacity) return -1;
    book_init(&books[books_count], next_id, title, author, year);
    books_count++;
    return next_id++;
}

// [COMMIT 3] Lista livros
void storage_list_books(void) {
    if (books_count == 0) {
        printf("(nenhum livro cadastrado)\n");
        return;
    }
    for (size_t i = 0; i < books_count; ++i) {
        Book *b = &books[i];
        printf("%d: %s — %s (%d)\n", b->id, b->title, b->author, b->year);
    }
}

// [COMMIT 4] Salva em arquivo simples
int storage_save(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return -1;
    for (size_t i = 0; i < books_count; ++i) {
        Book *b = &books[i];
        // formato: id|year|title|author\n (id é redundante)
        fprintf(f, "%d|%d|%s|%s\n", b->id, b->year, b->title, b->author);
    }
    fclose(f);
    return 0;
}

// [COMMIT 4] Carrega de arquivo simples (substitui em memória)
int storage_load(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return -1;
    // reset
    books_count = 0;
    next_id = 1;
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        // trim newline
        char *nl = strchr(line, '\n'); if (nl) *nl = '\0';
        int id=0, year=0;
        char title[BOOK_TITLE_LEN];
        char author[BOOK_AUTHOR_LEN];
        // parse simple format
        char *p1 = strchr(line, '|');
        if (!p1) continue;
        *p1 = '\0'; id = atoi(line);
        char *p2 = strchr(p1+1, '|'); if (!p2) continue; *p2='\0'; year = atoi(p1+1);
        char *p3 = strchr(p2+1, '|'); if (!p3) continue; *p3='\0';
        strncpy(title, p2+1, BOOK_TITLE_LEN-1); title[BOOK_TITLE_LEN-1]='\0';
        strncpy(author, p3+1, BOOK_AUTHOR_LEN-1); author[BOOK_AUTHOR_LEN-1]='\0';
        ensure_capacity();
        book_init(&books[books_count], id, title, author, year);
        books_count++;
        if (id >= next_id) next_id = id + 1;
    }
    fclose(f);
    return 0;
}

// [COMMIT 3] Libera recursos
void storage_free(void) {
    free(books);
    books = NULL;
    books_count = books_capacity = 0;
    next_id = 1;
}
