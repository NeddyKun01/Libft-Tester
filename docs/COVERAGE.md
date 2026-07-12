# Coverage Table

This table is generated from `tester/src/coverage.cpp`.

```sh
make ROOT_DIR=../libft build
./libft_tester --coverage
./libft_tester --explain ft_split
./libft_tester --coverage-md > docs/COVERAGE.md
```

| Function | Suite | Group | Main cases | Malloc |
| --- | --- | --- | --- | --- |
| `ft_isalpha` | `ctype` | classification | full signed/unsigned char range, letters, non-letters | none |
| `ft_isdigit` | `ctype` | classification | full signed/unsigned char range, digits, non-digits | none |
| `ft_isalnum` | `ctype` | classification | full signed/unsigned char range, letters, digits, symbols | none |
| `ft_isascii` | `ctype` | classification | full signed/unsigned char range, ASCII limits, out of range | none |
| `ft_isprint` | `ctype` | classification | full signed/unsigned char range, printable limits, controls | none |
| `ft_toupper` | `ctype` | conversion | full signed/unsigned char range, lowercase, uppercase, symbols | none |
| `ft_tolower` | `ctype` | conversion | full signed/unsigned char range, uppercase, lowercase, symbols | none |
| `ft_strlen` | `strings` | string length | empty string, normal text, whitespace, punctuation, early NUL, seeded random strings | none |
| `ft_strchr` | `strings` | string search | found, first char, missing, NUL terminator, empty string, wrapped char, stops at NUL, seeded random chars | none |
| `ft_strrchr` | `strings` | string search | last match, first char, missing, NUL terminator, empty string, wrapped char, stops at NUL, seeded random chars | none |
| `ft_strncmp` | `strings` | string compare | zero length, equal, prefix, negative diff, positive diff, shorter string, unsigned chars, seeded random signs | none |
| `ft_strnstr` | `strings` | bounded search | empty needle, zero length, exact fit, too short, start match, missing, partial end, seeded random offsets | none |
| `ft_strlcpy` | `strings` | bounded copy | size zero, size one, full copy, truncation, empty source, return length, buffer bytes | none |
| `ft_strlcat` | `strings` | bounded append | size smaller than dst, truncation, full append, size zero, empty dst, empty src, buffer bytes | none |
| `ft_strdup` | `strings` | allocation copy | normal text, empty string, copied content | malloc failure |
| `ft_memset` | `memory` | memory write | return pointer, fixed fill, zero size, unsigned char cast, seeded random sizes/values | none |
| `ft_bzero` | `memory` | memory zero | full clear, partial clear, zero size, one byte, surrounding bytes | none |
| `ft_memcpy` | `memory` | memory copy | return pointer, fixed copy, zero size, unchanged bytes, offset copy, seeded random bytes | none |
| `ft_memmove` | `memory` | overlap copy | return pointer, forward overlap, backward overlap, zero size, same pointer | none |
| `ft_memchr` | `memory` | memory search | first match, NUL byte, missing, zero size, stops before later match, seeded random values | none |
| `ft_memcmp` | `memory` | memory compare | equal buffers, zero size, unsigned diff, negative diff, equal prefix, seeded random signs | none |
| `ft_calloc` | `memory` | zero allocation | overflow protection, zero-filled blocks, one byte allocation | malloc failure |
| `ft_atoi` | `atoi` | conversion | zero, positive, negative, whitespace, signs, INT_MAX, INT_MIN, invalid prefixes, trailing text | none |
| `ft_substr` | `string_utils` | allocation substring | basic slice, long length, out of range, zero length, empty source | malloc failure |
| `ft_strjoin` | `string_utils` | allocation join | normal join, empty left, empty right, both empty | malloc failure |
| `ft_strtrim` | `string_utils` | allocation trim | spaces, custom set, empty set, full trim, no trim, multi-character set | malloc failure |
| `ft_split` | `string_utils` | allocation split | spaces, repeated delimiters, leading/trailing delimiters, empty tokens avoided, NULL terminator | malloc failure across allocations |
| `ft_itoa` | `string_utils` | integer to string | INT_MIN, zero, positive, INT_MAX, negative, selected values | malloc failure |
| `ft_strmapi` | `string_utils` | mapped string | index-aware transform, empty string, allocated result | malloc failure |
| `ft_striteri` | `string_utils` | in-place iteration | uppercase transform, empty string, index use, NULL function, NULL string guard | none |
| `ft_putchar_fd` | `output` | fd output | letter, newline, digit, NUL byte size/content | none |
| `ft_putstr_fd` | `output` | fd output | normal text, empty string, whitespace, NULL string, digits | none |
| `ft_putendl_fd` | `output` | fd output | normal text with newline, empty string, inner newline, NULL string, digits | none |
| `ft_putnbr_fd` | `output` | fd output | INT_MIN, zero, negative, INT_MAX, positive | none |
| `ft_lstnew` | `lists` | list allocation | content pointer, NULL content, next initialized to NULL | malloc failure |
| `ft_lstadd_front` | `lists` | list mutation | normal prepend, empty list, NULL new node, size after prepend | setup allocations |
| `ft_lstsize` | `lists` | list query | NULL list, one node, multiple nodes, after clear | none |
| `ft_lstlast` | `lists` | list query | NULL list, single node, multiple nodes, next NULL on tail | none |
| `ft_lstadd_back` | `lists` | list mutation | NULL new node, empty list, preserves head, links tail, last node | setup allocations |
| `ft_lstdelone` | `lists` | list deletion | covered through clear/map deletion paths and custom deleters | none |
| `ft_lstclear` | `lists` | list deletion | two nodes, resets head, empty list, NULL deleter guard | none |
| `ft_lstiter` | `lists` | list iteration | multiple nodes, single node, NULL function guard, content mutation | none |
| `ft_lstmap` | `lists` | list mapping | mapped copy, size, transformed values, f failure cleanup, NULL list | malloc failure and cleanup |
