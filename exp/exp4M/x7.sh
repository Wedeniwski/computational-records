#!/bin/sh
date
mkdir 28
cd 28; ../exp $1 28 32; cd ..
date
mkdir 29
cd 29; ../exp $1 29 32; cd ..
date
mkdir 30
cd 30; ../exp $1 30 32; cd ..
date
mkdir 31
cd 31; ../exp $1 31 32; cd ..
date
mkdir 28-29
cd 28-29
../exp 1 ../28/ ../29/
date
../exp 3 ../28/ ../29/
date
cd ..
mkdir 30-31
cd 30-31
../exp 1 ../30/ ../31/
date
../exp 3 ../30/ ../31/
date
cd ..
mkdir 28-31
cd 28-31
../exp 1 ../28-29/ ../30-31/
date
../exp 3 ../28-29/ ../30-31/
date
../exp $1 32 32
cd ..
cd 24-27
../exp 4 ./ ../28-31/exp.tmp0
../exp 5 ./ ./
cd ..
cd 20-23
../exp 4 ./ ../24-27/exp.tmp
../exp 5 ./ ./
cd ..
cd 16-19
../exp 4 ./ ../20-23/exp.tmp
../exp 5 ./ ./
cd ..
cd 12-15
../exp 4 ./ ../16-19/exp.tmp
../exp 5 ./ ./
cd ..
cd 8-11
../exp 4 ./ ../12-15/exp.tmp
../exp 5 ./ ./
cd ..
cd 4-7
../exp 4 ./ ../8-11/exp.tmp
../exp 5 ./ ./
cd ..
cd 0-3
../exp 4 ./ ../4-7/exp.tmp
../exp 5 ./ ./
../exp $1
cd ..
