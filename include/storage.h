// [COMMIT 3] Interface para armazenamento em memória e persistência
#ifndef STORAGE_H
#define STORAGE_H

#include "book.h"

// [COMMIT 3] Adiciona um livro ao armazenamento em memória. Retorna id.
int storage_add_book(const char *title, const char *author, int year);

// [COMMIT 3] Lista os livros atuais para saída padrão
void storage_list_books(void);

// [COMMIT 4] Persiste livros em arquivo texto (um por linha). Retorna 0 em sucesso
int storage_save(const char *filename);

// [COMMIT 4] Carrega livros de arquivo texto. Retorna 0 em sucesso
int storage_load(const char *filename);

// [COMMIT 3] Libera recursos alocados pelo armazenamento
void storage_free(void);

#endif // STORAGE_H
