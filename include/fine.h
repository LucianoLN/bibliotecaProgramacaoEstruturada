/*
 * fine.h - Prazo de devolucao, atraso e multa dos emprestimos.
 *
 * [COMMIT 6 - ATRASOS E MULTA]      regras de prazo e multa por dia.
 */
#ifndef FINE_H
#define FINE_H

#include <stddef.h>
#include <time.h>
#include "storage.h"

#define LOAN_DAYS 7                 /* prazo de devolucao em dias */
#define FINE_CENTS_PER_DAY 100      /* R$ 1,00 por dia de atraso (em centavos: sem erro de double) */

/* Data limite para devolver sem multa. */
time_t loan_due_date(const Loan *l);

/* Dias de atraso: ate a devolucao, ou ate `today` se ainda estiver aberto. */
int loan_days_late(const Loan *l, time_t today);

/* Multa em centavos (0 se no prazo). */
int loan_fine_cents(const Loan *l, time_t today);

/* Escreve centavos como "R$ 12,50". */
void fine_format(long cents, char *buf, size_t size);

#endif /* FINE_H */
