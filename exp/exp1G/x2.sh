#!/bin/sh
mkdir 0-1
cd 0-1
date
../exp2 1 ../0/ ../1/
date
cd ..
mkdir 2-3
cd 2-3
../exp2 1 ../2/ ../3/
date
cd ..
date
mkdir 4-5
cd 4-5
../exp2 1 ../4/ ../5/
date
cd ..
mkdir 6-7
cd 6-7
../exp2 1 ../6/ ../7/
date
cd ..
mkdir 8-9
cd 8-9
date
../exp2 1 ../8/ ../9/
date
cd ..
mkdir 10-11
cd 10-11
../exp2 1 ../10/ ../11/
date
cd ..
date
mkdir 12-13
cd 12-13
../exp2 1 ../12/ ../13/
date
cd ..
mkdir 14-15
cd 14-15
../exp2 2 ../14/ ../15/
date
cd ..
mkdir 0-3
cd 0-3
../exp2 1 ../0-1/ ../2-3/
date
cd ..
date
mkdir 4-7
cd 4-7
../exp2 1 ../4-5/ ../6-7/
date
cd ..
mkdir 8-11
cd 8-11
../exp2 1 ../8-9/ ../10-11/
date
cd ..
date
mkdir 12-15
cd 12-15
../exp2 2 ../12-13/ ../14-15/
date
cd ..
mkdir 0-7
cd 0-7
../exp2 1 ../0-3/ ../4-7/
date
cd ..
mkdir 8-15
cd 8-15
../exp2 2 ../8-11/ ../12-15/
date
cd ..
mkdir 0-15
cd 0-15
../exp2 2 ../0-7/ ../8-15/
date
cd ..
