#!/bin/sh

set -eu

needle=${1:-./needle}
case $needle in
    /*) ;;
    *) needle="$(pwd)/${needle#./}" ;;
esac

test_directory=$(mktemp -d)
trap 'rm -rf "$test_directory"' EXIT HUP INT TERM

mkdir -p "$test_directory/sub" "$test_directory/.git"
printf 'alpha needle omega\nplain\nneedle again\n' > "$test_directory/a.txt"
printf 'nested needle\n' > "$test_directory/sub/b.txt"
printf 'hidden needle\n' > "$test_directory/.git/ignored.txt"
printf 'binary needle\0data\n' > "$test_directory/binary.dat"

output=$($needle --color=never -n needle "$test_directory/a.txt")
printf '%s' "$output" | grep -Fq '1:alpha needle omega'
printf '%s' "$output" | grep -Fq '3:needle again'

output=$($needle --color=never -f needle "$test_directory/a.txt")
[ "$(printf '%s\n' "$output" | grep -c needle)" -eq 1 ]

output=$($needle --color=never -rn needle "$test_directory")
printf '%s' "$output" | grep -Fq 'nested needle'
printf '%s' "$output" | grep -Fq 'alpha needle omega'
if printf '%s' "$output" | grep -Fq 'hidden needle'; then
    printf 'recursive search entered .git\n' >&2
    exit 1
fi
if printf '%s' "$output" | grep -Fq 'binary needle'; then
    printf 'binary file was searched\n' >&2
    exit 1
fi

output=$(env -u NO_COLOR "$needle" --color=always needle "$test_directory/a.txt")
printf '%s' "$output" | grep -Fq "$(printf '\033[31m')"

output=$(NO_COLOR=1 $needle --color=always needle "$test_directory/a.txt")
if printf '%s' "$output" | grep -Fq "$(printf '\033[')"; then
    printf 'NO_COLOR did not disable ANSI escapes\n' >&2
    exit 1
fi

if $needle absent "$test_directory/a.txt"; then
    printf 'no-match search returned success\n' >&2
    exit 1
else
    status=$?
    [ "$status" -eq 1 ]
fi

if $needle needle "$test_directory/missing" >/dev/null 2>&1; then
    printf 'missing file returned success\n' >&2
    exit 1
else
    status=$?
    [ "$status" -eq 2 ]
fi

printf 'all needle tests passed\n'
