/*
 * persist.c - Gravacao e leitura dos dados em arquivo.
 *
 * [PARTE 1]                         texto "id|ano|titulo|autor".
 * [COMMIT 1 - ALOCACAO DINAMICA]    texto para os tres cadastros, carregado num
 *                                   Library temporario (falha nao apaga nada).
 */
#include <stdio.h>
#include <string.h>
#include "persist.h"
#include "util.h"

#define TXT_BOOKS "livros.txt"
#define TXT_USERS "usuarios.txt"
#define TXT_LOANS "emprestimos.txt"
#define MAX_FIELDS 5
#define LINE_LEN 512

static FILE *open_file(const char *prefix, const char *name, const char *mode) {
    char path[512];
    snprintf(path, sizeof path, "%s%s", prefix, name);
    return fopen(path, mode);
}

int persist_save_txt(const Library *lib, const char *prefix) {
    FILE *fb = open_file(prefix, TXT_BOOKS, "w");
    FILE *fu = open_file(prefix, TXT_USERS, "w");
    FILE *fl = open_file(prefix, TXT_LOANS, "w");
    int ok = fb && fu && fl;
    size_t i;

    for (i = 0; ok && i < lib->n_books; i++) {
        const Book *b = &lib->books[i];
        ok = fprintf(fb, "%d|%d|%d|%s|%s\n", b->id, b->year, b->quantity, b->title, b->author) > 0;
    }
    for (i = 0; ok && i < lib->n_users; i++) {
        const User *u = &lib->users[i];
        ok = fprintf(fu, "%d|%s|%s\n", u->registration, u->name, u->course) > 0;
    }
    for (i = 0; ok && i < lib->n_loans; i++) {
        const Loan *l = &lib->loans[i];
        ok = fprintf(fl, "%d|%d|%d|%d|%d\n", l->id, l->book_id, l->user_reg,
                     date_to_ymd(l->loan_date), date_to_ymd(l->return_date)) > 0;
    }
    if (fb && fclose(fb) != 0) ok = 0;
    if (fu && fclose(fu) != 0) ok = 0;
    if (fl && fclose(fl) != 0) ok = 0;
    return ok ? 0 : PERSIST_ERROR;
}

/* Quebra a linha nos '|' (alterando-a). Devolve o numero de campos, 0 para
 * linha vazia e max + 1 quando sobram campos (linha invalida). */
static int split_fields(char *line, char **fields, int max) {
    int n = 0;
    char *p = line;
    line[strcspn(line, "\r\n")] = '\0';
    if (*line == '\0') return 0;
    while (n < max) {
        fields[n++] = p;
        p = strchr(p, '|');
        if (!p) return n;
        *p++ = '\0';
    }
    return max + 1;
}

/* Cada leitor interpreta uma linha ja quebrada; devolve 1 se valida. */
static int parse_book(char **f, int n, Book *b) {
    /* 5 campos = formato atual; 4 = livros.txt da Parte 1, sem quantidade. */
    if (n == 5 && parse_int(f[0], &b->id) && parse_int(f[1], &b->year) &&
        parse_int(f[2], &b->quantity)) {
        book_init(b, b->id, f[3], f[4], b->year, b->quantity);
        return 1;
    }
    if (n == 4 && parse_int(f[0], &b->id) && parse_int(f[1], &b->year)) {
        book_init(b, b->id, f[2], f[3], b->year, 1);
        return 1;
    }
    return 0;
}

static int parse_user(char **f, int n, User *u) {
    if (n != 3 || !parse_int(f[0], &u->registration)) return 0;
    str_copy(u->name, f[1], sizeof u->name);
    str_copy(u->course, f[2], sizeof u->course);
    return 1;
}

static int parse_loan(char **f, int n, Loan *l) {
    int loan_ymd, return_ymd;
    if (n != 5 || !parse_int(f[0], &l->id) || !parse_int(f[1], &l->book_id) ||
        !parse_int(f[2], &l->user_reg) || !parse_int(f[3], &loan_ymd) ||
        !parse_int(f[4], &return_ymd))
        return 0;
    l->loan_date = date_from_ymd(loan_ymd);
    l->return_date = date_from_ymd(return_ymd);
    return l->loan_date != (time_t)-1 && l->return_date != (time_t)-1;
}

/* Le um arquivo linha a linha para dentro de tmp. Linhas invalidas sao
 * avisadas e puladas. Devolve 0, PERSIST_NO_FILE ou PERSIST_ERROR. */
static int load_file(Library *tmp, const char *prefix, const char *name) {
    char line[LINE_LEN];
    char *fields[MAX_FIELDS];
    int line_no = 0, rc = 0;
    FILE *f = open_file(prefix, name, "r");
    if (!f) return PERSIST_NO_FILE;

    while (rc == 0 && fgets(line, sizeof line, f)) {
        int n = split_fields(line, fields, MAX_FIELDS);
        int valid;
        Book b;
        User u;
        Loan l;
        line_no++;
        if (n == 0) continue;
        if (strcmp(name, TXT_BOOKS) == 0)
            valid = parse_book(fields, n, &b) && (rc = lib_push_book(tmp, &b)) == 0;
        else if (strcmp(name, TXT_USERS) == 0)
            valid = parse_user(fields, n, &u) && (rc = lib_push_user(tmp, &u)) == 0;
        else
            valid = parse_loan(fields, n, &l) && (rc = lib_push_loan(tmp, &l)) == 0;
        if (!valid && rc == 0)
            fprintf(stderr, "Aviso: %s linha %d ignorada (formato invalido).\n", name, line_no);
    }
    if (ferror(f)) rc = PERSIST_ERROR;
    fclose(f);
    return rc < 0 ? PERSIST_ERROR : 0;
}

int persist_load_txt(Library *lib, const char *prefix) {
    static const char *const names[] = {TXT_BOOKS, TXT_USERS, TXT_LOANS};
    Library tmp;
    int found = 0, rc = 0;
    size_t i;

    lib_init(&tmp);
    for (i = 0; i < sizeof names / sizeof names[0] && rc != PERSIST_ERROR; i++) {
        rc = load_file(&tmp, prefix, names[i]);
        if (rc == 0) found = 1;
    }
    if (rc == PERSIST_ERROR || !found) {
        lib_free(&tmp);
        return rc == PERSIST_ERROR ? PERSIST_ERROR : PERSIST_NO_FILE;
    }
    lib_free(lib);
    *lib = tmp;
    return (int)(lib->n_books + lib->n_users + lib->n_loans);
}
