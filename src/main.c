// [COMMIT 2] Implement main program skeleton
// Arquivo: src/main.c

#include <stdio.h>
#include "project.h"
#include "cli.h"

// [COMMIT 2] Inicializa a aplicação (placeholder)
void init_app(void) {
  // future initialization (config, logging, etc.)
}

int main(void) {
  // [COMMIT 2] Ponto de entrada do programa
  init_app();
  printf("Biblioteca Programacao Estruturada - Iniciando (Commit 2)\n");
  cli_run();
  // [COMMIT 5] limpeza final ficará aqui se necessário
  return 0;
}
