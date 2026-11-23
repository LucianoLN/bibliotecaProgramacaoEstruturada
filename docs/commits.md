# Commit History

## Commit 1 — Scaffold (iniciado)
- Data: (work in progress)
- Descrição: Criada a estrutura de pastas e arquivos iniciais:
  - src/main.c
  - include/project.h
  - Makefile
  - .gitignore
  - docs/commits.md
- Nota: Próxima etapa — implementar o esqueleto do programa (`Commit 2`).

## Commit 2 — Main & CLI
- Data: (work in progress)
- Descrição: Implementado `src/main.c` e `src/cli.c` com loop interativo básico.

## Commit 3 — Data structures & storage
- Descrição: Adicionadas `include/book.h`, `src/book.c`, `include/storage.h`, `src/storage.c`.

## Commit 4 — Persistence
- Descrição: Implementado `storage_save` e `storage_load` para arquivo texto simples.

## Commit 5 — Docs and polish
- Descrição: `README.md` e marcações de commits adicionadas aos arquivos.

---

# Parte 2

Cada commit compila e roda sozinho; os trechos de código levam a tag do commit
que os introduziu (ex.: `/* [COMMIT 4] ... */`).

| # | Mensagem | Objetivo | Arquivos |
|---|---|---|---|
| 1 | `feat(memoria): migra acervo, usuarios e emprestimos para alocacao dinamica` | Vetores dinâmicos com `malloc`/`realloc`/`free` e `lib_free`; completa usuários, empréstimos e busca sequencial; corrige o erro de compilação do menu | `storage.*`, `persist.*`, `search.*`, `util.*`, `book.*`, `cli.*`, `main.c`, `Makefile`, `tests/` |
| 2 | `feat(arquivos): persistencia em arquivo binario com migracao dos .txt` | `biblioteca.bin` com gravação segura; leitura dos `.txt` da Parte 1 | `persist.*`, `main.c`, `cli.c` |
| 3 | `feat(ordenacao): implementa Insertion Sort e Selection Sort manuais` | Dois algoritmos de ordenação por código ou título | `sort.*`, `book.*`, `cli.c` |
| 4 | `feat(busca): implementa pesquisa binaria por codigo e titulo` | Busca binária com verificação de ordenação | `search.*`, `cli.c` |
| 5 | `feat(historico): historico completo de emprestimos por usuario` | Extra 1: histórico por usuário | `cli.c` |
| 6 | `feat(multa): controle de atrasos e multa por dia de atraso` | Extra 2: prazo, atraso e multa | `fine.*`, `util.*`, `cli.c` |
| 7 | `feat(relatorio): relatorio de desempenho com clock()` | Benchmark de ordenações e buscas, exibido e exportado | `report.*`, `cli.c`, `docs/relatorio_desempenho.txt` |
| 8 | `refactor(cli): menu agrupado, validacoes e documentacao da Parte 2` | Menu por seções, validação de ano, README e este arquivo | `cli.c`, `storage.c`, `README.md`, `docs/commits.md` |

Para ver o que mudou em cada etapa: `git log --oneline` e `git show <hash>`.
