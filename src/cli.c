/*
 * cli.c - Menu interativo no terminal.
 *
 * [PARTE 1]                         laco do menu, cadastro e listagem de livros.
 * [COMMIT 1 - ALOCACAO DINAMICA]    menu em tabela; usuarios, emprestimos,
 *                                   devolucoes e buscas sobre a Library dinamica.
 * [COMMIT 2 - ARQUIVOS BINARIOS]    salvar em .bin e exportar .txt.
 */
#include <stdio.h>
#include <string.h>
#include "cli.h"
#include "persist.h"
#include "util.h"

#define LINE_LEN 256

/* ---------- Entrada ---------- */

/* Le uma linha sem o '\n'. Devolve 0 no fim da entrada (EOF). */
static int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    if (!fgets(buf, (int)size, stdin)) {
        buf[0] = '\0';
        return 0;
    }
    if (!strchr(buf, '\n')) {       /* linha maior que o buffer: descarta o resto */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }
    buf[strcspn(buf, "\r\n")] = '\0';
    return 1;
}

static int read_int(const char *prompt, int *out) {
    char buf[32];
    if (!read_line(prompt, buf, sizeof buf)) return 0;
    if (parse_int(buf, out)) return 1;
    printf("Numero invalido.\n");
    return 0;
}

static void report_error(int rc) {
    printf("Erro: %s.\n", lib_strerror(rc));
}

/* ---------- Impressao ---------- */

static void print_book_header(void) {
    printf("%-6s %-35s %-25s %5s %5s\n", "Codigo", "Titulo", "Autor", "Ano", "Disp.");
}

static void print_book(const Book *b) {
    printf("%-6d %-35.35s %-25.25s %5d %5d\n", b->id, b->title, b->author, b->year, b->quantity);
}

static void print_loan_header(void) {
    printf("%-5s %-9s %-6s %-30s %-10s %-10s\n",
           "Id", "Matric.", "Livro", "Titulo", "Emprestimo", "Devolucao");
}

static void print_loan(const Library *lib, const Loan *l) {
    const Book *b = lib_find_book(lib, l->book_id);
    char loaned[16], returned[16];
    date_format(l->loan_date, loaned, sizeof loaned);
    date_format(l->return_date, returned, sizeof returned);
    printf("%-5d %-9d %-6d %-30.30s %-10s %-10s\n", l->id, l->user_reg, l->book_id,
           b ? b->title : "?", loaned, l->return_date ? returned : "em aberto");
}

/* ---------- Livros ---------- */

static void add_book(Library *lib) {
    char title[LINE_LEN], author[LINE_LEN];
    int year, quantity, rc;
    read_line("Titulo: ", title, sizeof title);
    read_line("Autor: ", author, sizeof author);
    if (!read_int("Ano: ", &year) || !read_int("Quantidade disponivel: ", &quantity)) return;
    rc = lib_add_book(lib, title, author, year, quantity);
    if (rc < 0) report_error(rc);
    else printf("Livro cadastrado com codigo %d.\n", rc);
}

static void list_books(Library *lib) {
    size_t i;
    if (lib->n_books == 0) {
        printf("(nenhum livro cadastrado)\n");
        return;
    }
    print_book_header();
    for (i = 0; i < lib->n_books; i++) print_book(&lib->books[i]);
}

/* [PARTE 1] Busca sequencial por trecho do titulo ou do autor. */
static void search_books(Library *lib) {
    char term[LINE_LEN];
    size_t i;
    unsigned long found = 0;
    read_line("Trecho do titulo ou do autor: ", term, sizeof term);
    for (i = 0; i < lib->n_books; i++) {
        const Book *b = &lib->books[i];
        if (str_icontains(b->title, term) || str_icontains(b->author, term)) {
            if (found++ == 0) print_book_header();
            print_book(b);
        }
    }
    printf("%lu livro(s) encontrado(s).\n", found);
}

/* ---------- Usuarios ---------- */

static void add_user(Library *lib) {
    char name[LINE_LEN], course[LINE_LEN];
    int reg, rc;
    if (!read_int("Matricula: ", &reg)) return;
    read_line("Nome: ", name, sizeof name);
    read_line("Curso: ", course, sizeof course);
    rc = lib_add_user(lib, reg, name, course);
    if (rc < 0) report_error(rc);
    else printf("Usuario cadastrado.\n");
}

static void list_users(Library *lib) {
    size_t i;
    if (lib->n_users == 0) {
        printf("(nenhum usuario cadastrado)\n");
        return;
    }
    printf("%-10s %-35s %s\n", "Matricula", "Nome", "Curso");
    for (i = 0; i < lib->n_users; i++) {
        const User *u = &lib->users[i];
        printf("%-10d %-35.35s %s\n", u->registration, u->name, u->course);
    }
}

/* [PARTE 1] Busca sequencial por matricula. */
static void find_user(Library *lib) {
    int reg;
    const User *u;
    if (!read_int("Matricula: ", &reg)) return;
    u = lib_find_user(lib, reg);
    if (!u) printf("Usuario nao encontrado.\n");
    else printf("%d - %s (%s)\n", u->registration, u->name, u->course);
}

/* ---------- Emprestimos ---------- */

static void lend_book(Library *lib) {
    int reg, book_id, rc;
    if (!read_int("Matricula: ", &reg) || !read_int("Codigo do livro: ", &book_id)) return;
    rc = lib_lend(lib, reg, book_id, date_today());
    if (rc < 0) report_error(rc);
    else printf("Emprestimo %d registrado.\n", rc);
}

static void return_book(Library *lib) {
    int reg, book_id, rc;
    if (!read_int("Matricula: ", &reg) || !read_int("Codigo do livro: ", &book_id)) return;
    rc = lib_return(lib, reg, book_id, date_today());
    if (rc < 0) report_error(rc);
    else printf("Devolucao registrada.\n");
}

static void list_loans(Library *lib) {
    size_t i;
    if (lib->n_loans == 0) {
        printf("(nenhum emprestimo registrado)\n");
        return;
    }
    print_loan_header();
    for (i = 0; i < lib->n_loans; i++) print_loan(lib, &lib->loans[i]);
}

/* ---------- Arquivos ---------- */

/* [COMMIT 2] O binario e o formato principal; o texto fica como exportacao. */
static void save_now(Library *lib) {
    if (persist_save_bin(lib, BIN_FILE) == 0) printf("Dados salvos em %s.\n", BIN_FILE);
    else printf("Erro ao salvar os dados.\n");
}

static void export_txt(Library *lib) {
    if (persist_save_txt(lib, "") == 0)
        printf("Exportado para livros.txt, usuarios.txt e emprestimos.txt.\n");
    else
        printf("Erro ao exportar.\n");
}

/* ---------- Menu ---------- */

typedef struct MenuItem {
    const char *label;
    void (*run)(Library *lib);
} MenuItem;

/* [COMMIT 1] Menu em tabela: cada funcionalidade nova e uma linha aqui. */
static const MenuItem MENU[] = {
    {"Cadastrar livro", add_book},
    {"Listar livros", list_books},
    {"Buscar livro por titulo/autor (sequencial)", search_books},
    {"Cadastrar usuario", add_user},
    {"Listar usuarios", list_users},
    {"Buscar usuario por matricula", find_user},
    {"Registrar emprestimo", lend_book},
    {"Registrar devolucao", return_book},
    {"Listar emprestimos", list_loans},
    {"Salvar dados (.bin)", save_now},
    {"Exportar dados para texto (.txt)", export_txt},       /* [COMMIT 2] */
};
#define MENU_LEN ((int)(sizeof MENU / sizeof MENU[0]))

void cli_run(Library *lib) {
    char buf[32];
    int choice, i;
    for (;;) {
        printf("\n== SGB - Biblioteca Universitaria ==\n");
        for (i = 0; i < MENU_LEN; i++) printf("%2d) %s\n", i + 1, MENU[i].label);
        printf(" 0) Sair\n");
        if (!read_line("Escolha: ", buf, sizeof buf)) break;   /* EOF: sai salvando */
        if (!parse_int(buf, &choice) || choice < 0 || choice > MENU_LEN) {
            printf("Opcao invalida.\n");
            continue;
        }
        if (choice == 0) break;
        MENU[choice - 1].run(lib);
    }
}
