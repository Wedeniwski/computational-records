#!/bin/sh
date
mkdir 8
cd 8; ../exp $1 8 32; cd ..
date
mkdir 9
cd 9; ../exp $1 9 32; cd ..
date
mkdir 10
cd 10; ../exp $1 10 32; cd ..
date
mkdir 11
cd 11; ../exp $1 11 32; cd ..
date
mkdir 8-9
cd 8-9
../exp 1 ../8/ ../9/
date
../exp 3 ../8/ ../9/
date
cd ..
mkdir 10-11
cd 10-11
../exp 1 ../10/ ../11/
date
../exp 3 ../10/ ../11/
date
cd ..
mkdir 8-11
cd 8-11
../exp 1 ../8-9/ ../10-11/
date
../exp 3 ../8-9/ ../10-11/
date
../exp $1 32 32
cd ..
