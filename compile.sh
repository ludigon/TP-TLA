#!/bin/bash

out=$(./golfit) && gcc -x c -g -o output - <<<"$out"
