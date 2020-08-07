#!/bin/bash

g++ -g web_test.cpp thread_pool.hpp http_conn.cpp -o web_test -lpthread