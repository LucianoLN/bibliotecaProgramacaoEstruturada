/*
 * search.c - Buscas no acervo.
 *
 * [PARTE 1]                         busca sequencial.
 */
#include "search.h"
#include "util.h"

int search_seq_id(const Book *v, size_t n, int id) {
    size_t i;
    for (i = 0; i < n; i++)
        if (v[i].id == id) return (int)i;
    return -1;
}

int search_seq_title(const Book *v, size_t n, const char *title) {
    size_t i;
    for (i = 0; i < n; i++)
        if (str_icmp(v[i].title, title) == 0) return (int)i;
    return -1;
}
