# Coverage Table

Esta tabela documenta os casos principais cobertos por cada função. Para ver a
mesma informação no terminal:

```sh
make ROOT_DIR=../libft coverage
make ROOT_DIR=../libft explain FUNC=ft_split
```

| Função | Suite | Casos principais | Malloc |
| --- | --- | --- | --- |
| `ft_isalpha` | `ctype` | range completo, letras, não letras | não |
| `ft_isdigit` | `ctype` | range completo, dígitos, não dígitos | não |
| `ft_isalnum` | `ctype` | range completo, letras, dígitos, símbolos | não |
| `ft_isascii` | `ctype` | limites ASCII, fora de range | não |
| `ft_isprint` | `ctype` | imprimíveis, control chars, limites | não |
| `ft_toupper` | `ctype` | minúsculas, maiúsculas, símbolos | não |
| `ft_tolower` | `ctype` | maiúsculas, minúsculas, símbolos | não |
| `ft_strlen` | `strings` | vazia, normal, whitespace, pontuação, `\0` interno | não |
| `ft_strchr` | `strings` | encontrado, primeiro char, ausente, `\0`, char wrapped | não |
| `ft_strrchr` | `strings` | último match, primeiro char, ausente, `\0`, char wrapped | não |
| `ft_strncmp` | `strings` | `n = 0`, igual, prefixo, diferenças positivas/negativas, unsigned char | não |
| `ft_strnstr` | `strings` | needle vazia, len zero, exact fit, curto demais, ausente | não |
| `ft_strlcpy` | `strings` | size zero, size one, cópia total, truncamento, source vazia | não |
| `ft_strlcat` | `strings` | size menor que dst, truncamento, append total, size zero, strings vazias | não |
| `ft_strdup` | `strings` | string normal, vazia, conteúdo copiado | sim |
| `ft_memset` | `memory` | retorno, fill fixo, size zero, cast unsigned char | não |
| `ft_bzero` | `memory` | clear total, parcial, size zero, um byte, bytes vizinhos | não |
| `ft_memcpy` | `memory` | retorno, cópia fixa, size zero, bytes inalterados, offset | não |
| `ft_memmove` | `memory` | overlap forward/backward, size zero, mesmo ponteiro | não |
| `ft_memchr` | `memory` | primeiro match, byte NUL, ausente, size zero, limite de busca | não |
| `ft_memcmp` | `memory` | igual, size zero, diff unsigned, diff negativa, prefixo igual | não |
| `ft_calloc` | `memory` | overflow, zero fill, um byte | sim |
| `ft_atoi` | `atoi` | zero, positivo, negativo, espaços, sinais, INT_MIN/MAX, inválidos | não |
| `ft_substr` | `string_utils` | slice normal, len grande, start fora, len zero, source vazia | sim |
| `ft_strjoin` | `string_utils` | normal, left vazia, right vazia, ambas vazias | sim |
| `ft_strtrim` | `string_utils` | espaços, set custom, set vazio, trim total, sem trim, multi-set | sim |
| `ft_split` | `string_utils` | delimitadores repetidos, início/fim, terminador NULL, tokens | sim |
| `ft_itoa` | `string_utils` | INT_MIN, zero, positivo, INT_MAX, negativo, valores selecionados | sim |
| `ft_strmapi` | `string_utils` | transform por índice, string vazia, resultado alocado | sim |
| `ft_striteri` | `string_utils` | alteração in-place, string vazia, índice, guards NULL | não |
| `ft_putchar_fd` | `output` | letra, newline, dígito, byte NUL | não |
| `ft_putstr_fd` | `output` | texto, vazia, whitespace, NULL, dígitos | não |
| `ft_putendl_fd` | `output` | texto + newline, vazia, newline interno, NULL, dígitos | não |
| `ft_putnbr_fd` | `output` | INT_MIN, zero, negativo, INT_MAX, positivo | não |
| `ft_lstnew` | `lists` | content, NULL content, next NULL | sim |
| `ft_lstadd_front` | `lists` | prepend normal, lista vazia, node NULL, tamanho | setup |
| `ft_lstsize` | `lists` | NULL, um nó, vários nós, depois de clear | não |
| `ft_lstlast` | `lists` | NULL, um nó, vários nós, tail com next NULL | não |
| `ft_lstadd_back` | `lists` | node NULL, lista vazia, preserva head, liga tail | setup |
| `ft_lstdelone` | `lists` | deleção via clear/map e deleters custom | não |
| `ft_lstclear` | `lists` | dois nós, reset head, lista vazia, del NULL | não |
| `ft_lstiter` | `lists` | vários nós, um nó, função NULL, mutação de conteúdo | não |
| `ft_lstmap` | `lists` | cópia mapeada, tamanho, valores transformados, falha de `f`, cleanup | sim |
