# needle

`needle` searches for literal text in files and directories. Matching paths,
line numbers and occurrences are colorized when output is connected to a
terminal.

## build and test

```sh
make
make check
make test
```

Install for the current user:

```sh
make install PREFIX="$HOME/.local"
```

## usage

```sh
needle pattern file.txt
needle -n pattern file.txt
needle -r pattern directory
needle -rnf pattern directory
```

Options:

- `-n`: show line numbers.
- `-r`: search directories recursively; defaults to the current directory when
  no path is provided.
- `-f`: stop after the first match.
- `--color=auto|always|never`: control ANSI colors.
- `-h`, `--help`: show usage.

Colors default to `auto`, so redirected output remains plain text. Set the
standard `NO_COLOR` environment variable to disable colors completely.

Exit status follows the grep convention: `0` when a match is found, `1` when
there are no matches, and `2` when an error occurs.
