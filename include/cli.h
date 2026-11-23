/*
 * cli.h - Menu interativo no terminal.
 *
 * [PARTE 1]                         cli_run.
 * [COMMIT 1 - ALOCACAO DINAMICA]    recebe a Library em vez de usar globais.
 */
#ifndef CLI_H
#define CLI_H

#include "storage.h"

/* Executa o menu ate o usuario escolher sair (ou a entrada acabar). */
void cli_run(Library *lib);

#endif /* CLI_H */
