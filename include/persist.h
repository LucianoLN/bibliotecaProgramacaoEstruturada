/*
 * persist.h - Gravacao e leitura dos dados em arquivo.
 *
 * [PARTE 1]                         storage_save/storage_load (texto, so livros).
 * [COMMIT 1 - ALOCACAO DINAMICA]    texto para livros, usuarios e emprestimos.
 * [COMMIT 2 - ARQUIVOS BINARIOS]    arquivo binario unico (formato principal);
 *                                   o texto vira exportacao/migracao.
 */
#ifndef PERSIST_H
#define PERSIST_H

#include "storage.h"

#define PERSIST_ERROR   (-1)
#define PERSIST_NO_FILE (-2)

/* [COMMIT 2] Arquivo de dados principal. */
#define BIN_FILE "biblioteca.bin"

/* [COMMIT 2] Grava cabecalho + os tres vetores com fwrite. Escreve primeiro
 * em "<path>.tmp" e so depois troca, para nao corromper o arquivo anterior.
 * Devolve 0 ou PERSIST_ERROR. */
int persist_save_bin(const Library *lib, const char *path);

/* [COMMIT 2] Substitui o conteudo de lib pelo do arquivo binario. Devolve o
 * total de registros, PERSIST_NO_FILE ou PERSIST_ERROR (lib fica intacta). */
int persist_load_bin(Library *lib, const char *path);

/* Grava livros.txt, usuarios.txt e emprestimos.txt (prefix = pasta/prefixo
 * dos nomes, "" para a pasta atual). Devolve 0 ou PERSIST_ERROR. */
int persist_save_txt(const Library *lib, const char *prefix);

/* Substitui o conteudo de lib pelo dos arquivos texto. Aceita tambem o
 * livros.txt da Parte 1 (id|ano|titulo|autor). Devolve o total de registros,
 * PERSIST_NO_FILE ou PERSIST_ERROR; em erro, lib fica intacta. */
int persist_load_txt(Library *lib, const char *prefix);

#endif /* PERSIST_H */
