#!/bin/sh

autoreconf -f -i || exit 1

if [ -z "$NOCONFIGURE" ]; then
	./configure -C "$@"
fi
