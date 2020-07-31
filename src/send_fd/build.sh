#!/bin/bash

gcc common.c openfile.c -o openfile
gcc common.c server.c -o server
gcc common.c client.c -o client
