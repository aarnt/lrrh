#!/bin/sh
hash=$(git --git-dir="$(dirname $0)/.git" rev-parse --short HEAD 2>/dev/null)
branch=$(git --git-dir="$(dirname $0)/.git" rev-parse --abbrev-ref HEAD | sed -r 's/.*[^0-9A-Za-z-]([0-9A-Za-z-]*)$/\1/g')

if [ -n "$hash" ] || [ -n "$branch" ]
then
	printf '+g%s.%s' "$hash" "$branch"
fi
echo
