#!/bin/sh
sh x1.sh $1 | tee -a x1.log
sh x2.sh $1 | tee -a x2.log
sh x3.sh $1 | tee -a x3.log
