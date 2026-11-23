/*
 * util.h - Auxiliares de texto, numeros e datas (somente libc).
 *
 * [COMMIT 1 - ALOCACAO DINAMICA]    textos, inteiros e datas.
 * [COMMIT 6 - ATRASOS E MULTA]      leitura de datas e contas com dias.
 */
#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <time.h>

/* Copia src para dst (tamanho size), sempre terminando em '\0'. */
void str_copy(char *dst, const char *src, size_t size);

/* Compara ignorando maiusculas/minusculas (<0, 0, >0 como strcmp). */
int str_icmp(const char *a, const char *b);

/* 1 se term aparece dentro de text, ignorando maiusculas/minusculas. */
int str_icontains(const char *text, const char *term);

/* Converte texto em int validando tudo; devolve 1 em sucesso, 0 se invalido. */
int parse_int(const char *s, int *out);

/* Datas ficam fixadas ao meio-dia, entao a diferenca entre duas e sempre
 * um numero inteiro de dias. (time_t)-1 = data inexistente (ex.: 31/02). */
time_t date_make(int day, int month, int year);
time_t date_today(void);

/* Conversao para o inteiro aaaammdd usado nos arquivos texto (0 = sem data). */
int date_to_ymd(time_t t);
time_t date_from_ymd(int ymd);

/* Escreve a data como dd/mm/aaaa; t == 0 vira "-". */
void date_format(time_t t, char *buf, size_t size);

/* [COMMIT 6] Le "dd/mm/aaaa"; devolve 1 se a data existe, 0 se invalida. */
int date_parse(const char *s, time_t *out);
/* [COMMIT 6] Soma dias a uma data. */
time_t date_add_days(time_t t, int days);
/* [COMMIT 6] Dias inteiros de `from` ate `to` (negativo se to vem antes). */
int date_diff_days(time_t from, time_t to);

#endif /* UTIL_H */
