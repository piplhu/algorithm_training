#!/bin/bash
gcc -g test.c db.c -o test -std=c99 -L./ -lapue
