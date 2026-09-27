// [COMMIT 4] Teste simples para storage
#include <stdio.h>
#include <stdlib.h>
#include "storage.h"

int main(void) {
    // adicionar livros
    storage_add_book("Livro A", "Autor X", 2001);
    storage_add_book("Livro B", "Autor Y", 1999);

    printf("Lista atual:\n");
    storage_list_books();

    // salvar
    if (storage_save("tests_out.txt") != 0) {
        printf("Falha ao salvar\n");
        return 1;
    }

    // limpar e carregar
    storage_free();
    if (storage_load("tests_out.txt") != 0) {
        printf("Falha ao carregar\n");
        return 1;
    }

    printf("Lista após reload:\n");
    storage_list_books();
    storage_free();
    return 0;
}
