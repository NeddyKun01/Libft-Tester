# Coverage Table

This table documents the main cases covered for each function. To see the same
information in the terminal:

```sh
make ROOT_DIR=../libft coverage
make ROOT_DIR=../libft explain FUNC=ft_split
```

| Function | Suite | Main cases | Malloc |
| --- | --- | --- | --- |
| `ft_isalpha` | `ctype` | full range, letters, non-letters | no |
| `ft_isdigit` | `ctype` | full range, digits, non-digits | no |
| `ft_isalnum` | `ctype` | full range, letters, digits, symbols | no |
| `ft_isascii` | `ctype` | ASCII limits, out of range values | no |
| `ft_isprint` | `ctype` | printable chars, control chars, limits | no |
| `ft_toupper` | `ctype` | lowercase, uppercase, symbols | no |
| `ft_tolower` | `ctype` | uppercase, lowercase, symbols | no |
| `ft_strlen` | `strings` | empty, normal, whitespace, punctuation, inner `\0` | no |
| `ft_strchr` | `strings` | found, first char, missing, `\0`, wrapped char | no |
| `ft_strrchr` | `strings` | last match, first char, missing, `\0`, wrapped char | no |
| `ft_strncmp` | `strings` | `n = 0`, equal, prefix, positive/negative diffs, unsigned char | no |
| `ft_strnstr` | `strings` | empty needle, zero len, exact fit, too short, missing | no |
| `ft_strlcpy` | `strings` | size zero, size one, full copy, truncation, empty source | no |
| `ft_strlcat` | `strings` | size smaller than dst, truncation, full append, zero size, empty strings | no |
| `ft_strdup` | `strings` | normal string, empty string, copied content | yes |
| `ft_memset` | `memory` | return value, fixed fill, zero size, unsigned char cast | no |
| `ft_bzero` | `memory` | full clear, partial clear, zero size, one byte, surrounding bytes | no |
| `ft_memcpy` | `memory` | return value, fixed copy, zero size, unchanged bytes, offset | no |
| `ft_memmove` | `memory` | forward/backward overlap, zero size, same pointer | no |
| `ft_memchr` | `memory` | first match, NUL byte, missing, zero size, search limit | no |
| `ft_memcmp` | `memory` | equal, zero size, unsigned diff, negative diff, equal prefix | no |
| `ft_calloc` | `memory` | overflow, zero fill, one byte | yes |
| `ft_atoi` | `atoi` | zero, positive, negative, spaces, signs, INT_MIN/MAX, invalid inputs | no |
| `ft_substr` | `string_utils` | normal slice, long len, start out of range, zero len, empty source | yes |
| `ft_strjoin` | `string_utils` | normal, empty left, empty right, both empty | yes |
| `ft_strtrim` | `string_utils` | spaces, custom set, empty set, full trim, no trim, multi-set | yes |
| `ft_split` | `string_utils` | repeated delimiters, start/end delimiters, NULL terminator, tokens | yes |
| `ft_itoa` | `string_utils` | INT_MIN, zero, positive, INT_MAX, negative, selected values | yes |
| `ft_strmapi` | `string_utils` | index transform, empty string, allocated result | yes |
| `ft_striteri` | `string_utils` | in-place change, empty string, index, NULL guards | no |
| `ft_putchar_fd` | `output` | letter, newline, digit, NUL byte | no |
| `ft_putstr_fd` | `output` | text, empty, whitespace, NULL, digits | no |
| `ft_putendl_fd` | `output` | text + newline, empty, inner newline, NULL, digits | no |
| `ft_putnbr_fd` | `output` | INT_MIN, zero, negative, INT_MAX, positive | no |
| `ft_lstnew` | `lists` | content, NULL content, next NULL | yes |
| `ft_lstadd_front` | `lists` | normal prepend, empty list, NULL node, size | setup |
| `ft_lstsize` | `lists` | NULL, one node, multiple nodes, after clear | no |
| `ft_lstlast` | `lists` | NULL, one node, multiple nodes, tail with next NULL | no |
| `ft_lstadd_back` | `lists` | NULL node, empty list, preserves head, links tail | setup |
| `ft_lstdelone` | `lists` | deletion through clear/map paths and custom deleters | no |
| `ft_lstclear` | `lists` | two nodes, resets head, empty list, NULL del | no |
| `ft_lstiter` | `lists` | multiple nodes, one node, NULL function, content mutation | no |
| `ft_lstmap` | `lists` | mapped copy, size, transformed values, `f` failure, cleanup | yes |
