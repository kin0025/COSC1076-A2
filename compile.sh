#!/usr/bin/env bash
echo "[31m =====================================COMPILING
=======================================================[0m"
make clean
make
./ppd stock.dat coins.dat