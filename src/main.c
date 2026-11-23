/*
 * main.c - Ponto de entrada do SGB (Sistema de Gerenciamento de Biblioteca).
 *
 * [PARTE 1]                         inicia o menu.
 * [COMMIT 1 - ALOCACAO DINAMICA]    carrega os dados, salva ao sair e libera
 *                                   toda a memoria antes de terminar.
 * [COMMIT 2 - ARQUIVOS BINARIOS]    usa o .bin; sem ele, migra os .txt da Parte 1.
 */
#include <stdio.h>
#include "cli.h"
#include "persist.h"

int main(void) {
    Library lib;
    int n, status = 0;

    lib_init(&lib);
    n = persist_load_bin(&lib, BIN_FILE);
    if (n == PERSIST_NO_FILE) {
        /* [COMMIT 2] Primeira execucao da Parte 2: aproveita os .txt antigos. */
        n = persist_load_txt(&lib, "");
        if (n > 0) printf("Dados em texto migrados; serao gravados em %s.\n", BIN_FILE);
    }
    if (n == PERSIST_ERROR) {
        /* Sair sem salvar: gravar agora sobrescreveria os arquivos com dados vazios. */
        fprintf(stderr, "Erro ao ler os arquivos de dados; nada foi alterado.\n");
        return 1;
    }
    if (n > 0) printf("%d registro(s) carregado(s).\n", n);

    cli_run(&lib);

    if (persist_save_bin(&lib, BIN_FILE) != 0) {
        fprintf(stderr, "Erro ao salvar os dados!\n");
        status = 1;
    } else {
        printf("Dados salvos. Ate logo!\n");
    }
    lib_free(&lib);
    return status;
}
