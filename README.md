# Libft Tester

Um tester C++ para projetos `libft` da 42, focado em:

- cobertura por funcao;
- output compacto e legivel;
- isolamento com `fork`;
- deteccao de `SEGV`, `BUS`, `ABRT`, `FPE` e `TIMEOUT`;
- simulacao de falhas de `malloc`;
- modo de leaks com `valgrind`;
- suporte a terminais sem cor com `NO_COLOR=1`;
- filtros por suite ou funcao;
- output JSON para scripts e CI;
- comando de versao para identificar builds do tester.

## Como usar

A partir da raiz do repositório do tester:

```sh
make
```

Ou a partir da pasta pai, se o repositório estiver numa subpasta chamada `tester`:

```sh
make -C tester
```

Tambem podes passar argumentos pelo Makefile:

```sh
make ARGS="--only ft_split"
make ARGS="--suite memory --verbose"
```

## Opcoes

```sh
./libft_tester --help
./libft_tester --version
./libft_tester --list
./libft_tester --only ft_split
./libft_tester --suite memory
./libft_tester --timeout 5000
./libft_tester --verbose
./libft_tester --quiet
./libft_tester --json
./libft_tester --no-color
```

Atalhos pelo Makefile:

```sh
make help
make list
make verbose
make json
make ci
```

Para JSON limpo a partir da raiz:

```sh
make --no-print-directory -C tester json ARGS="--only ft_strlen"
```

## CI

```sh
make ci
make ci ARGS="--only ft_strlen"
```

O alvo `ci` usa JSON sem cores, para o output ser estavel em GitHub Actions,
GitLab CI ou scripts locais.

## Leaks

```sh
make leaks
```

O modo de leaks corre sem `fork`, para o `valgrind` analisar diretamente as
funcoes testadas e nao o mecanismo interno do runner.

Tambem aceita filtros:

```sh
make leaks ARGS="--only ft_split"
make leaks ARGS="--suite lists"
```

## Sem cores

```sh
NO_COLOR=1 make
```

## Estados

- `OK`: teste normal passou.
- `MOK`: expectativa relacionada com `malloc` passou.
- `NOK`: teste normal falhou.
- `MNOK`: expectativa relacionada com `malloc` falhou.
- `SEGV`: a suite caiu com segmentation fault.
- `BUS`: a suite caiu com bus error.
- `ABRT`: a suite abortou.
- `FPE`: a suite caiu com erro aritmetico.
- `TIMEOUT`: a suite demorou demasiado tempo.

## Comportamento

O tester nao apaga automaticamente a `libft.a` depois de correr. Isto evita
corridas e problemas em CI ou quando varios comandos sao executados em paralelo.
Para limpar tudo, usa:

```sh
make fclean
```

## Estrutura

- `include/tester.hpp`: runner, asserts, output e helpers.
- `include/test_modules.hpp`: declaracoes das suites.
- `include/malloc_fail.hpp`: controlo das falhas simuladas de `malloc`.
- `src/*_tests.cpp`: testes separados por familia.
- `src/malloc_fail.cpp`: wrapper de `malloc`.
- `src/main.cpp`: registo e execucao das suites.

## Nota

Este tester evita testar comportamento indefinido do C quando isso nao e exigido
pelo subject. Por exemplo, nao testa chamadas como `ft_strlen(NULL)`.
