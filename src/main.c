/*
 * main.c - Ponto de entrada do SGB (Sistema de Gerenciamento de Biblioteca).
 *
 * [PARTE 1]                         inicia o menu.
 * [COMMIT 1 - ALOCACAO DINAMICA]    carrega os dados, salva ao sair e libera
 *                                   toda a memoria antes de terminar.
 */
#include <stdio.h>
#include "cli.h"
#include "persist.h"

int main(void) {
    Library lib;
    int n, status = 0;

    lib_init(&lib);
    n = persist_load_txt(&lib, "");
    if (n == PERSIST_ERROR) {
        /* Sair sem salvar: gravar agora sobrescreveria os arquivos com dados vazios. */
        fprintf(stderr, "Erro ao ler os arquivos de dados; nada foi alterado.\n");
        return 1;
    }
    if (n > 0) printf("%d registro(s) carregado(s).\n", n);

    cli_run(&lib);

    if (persist_save_txt(&lib, "") != 0) {
        fprintf(stderr, "Erro ao salvar os dados!\n");
        status = 1;
    } else {
        printf("Dados salvos. Ate logo!\n");
    }
    lib_free(&lib);
    return status;
}
