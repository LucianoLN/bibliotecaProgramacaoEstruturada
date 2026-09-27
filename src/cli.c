// [COMMIT 2] Implementação do CLI com menu simples
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "storage.h"

// [COMMIT 2] Le uma linha da entrada e remove newline
static void read_line(char *buf, size_t size) {
    if (!fgets(buf, (int)size, stdin)) { buf[0] = '\0'; return; }
    char *nl = strchr(buf, '\n'); if (nl) *nl = '\0';
}

// [COMMIT 2] Menu de interação
void cli_run(void) {
    char buf[256];
    for (;;) {
        printf("\n== Biblioteca ==\n");
        printf("1) Adicionar livro\n2) Listar livros\n3) Salvar para arquivo\n4) Carregar de arquivo\n0) Sair\");
        printf("\nEscolha: ");
        read_line(buf, sizeof(buf));
        if (strcmp(buf, "1") == 0) {
            char title[128], author[128], year_s[16];
            printf("Título: "); read_line(title, sizeof(title));
            printf("Autor: "); read_line(author, sizeof(author));
            printf("Ano: "); read_line(year_s, sizeof(year_s));
            int year = atoi(year_s);
            int id = storage_add_book(title, author, year);
            if (id < 0) printf("Erro ao adicionar livro.\n"); else printf("Livro adicionado com id %d\n", id);
        } else if (strcmp(buf, "2") == 0) {
            storage_list_books();
        } else if (strcmp(buf, "3") == 0) {
            char fname[256];
            printf("Arquivo (ex: livros.txt): "); read_line(fname, sizeof(fname));
            if (storage_save(fname) == 0) printf("Salvo em %s\n", fname); else printf("Erro ao salvar.\n");
        } else if (strcmp(buf, "4") == 0) {
            char fname[256];
            printf("Arquivo (ex: livros.txt): "); read_line(fname, sizeof(fname));
            if (storage_load(fname) == 0) printf("Carregado de %s\n", fname); else printf("Erro ao carregar.\n");
        } else if (strcmp(buf, "0") == 0) {
            storage_free();
            printf("Saindo...\n");
            break;
        } else {
            printf("Opção inválida.\n");
        }
    }
}
