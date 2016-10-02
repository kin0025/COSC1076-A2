#!/usr/bin/env bash
echo "[31m =====================================COMPILING=======================================================[0m"
make clean
make

./ppd stock.dat invalid_coins1.dat
exit_code=$?
if [ "$exit_code" -ne 1 ]; then
	echo "[31mTest Failed[0m"
fi
./ppd stock.dat invalid_coins2.dat
exit_code=$?
if [ "$exit_code" -ne 1 ]; then
	echo "[31mTest Failed[0m"
fi
./ppd stock.dat invalid_coins3.dat
exit_code=$?
if [ "$exit_code" -ne 1 ]; then
	echo "[31mTest Failed[0m"
fi
./ppd stock.dat invalid_coins4.dat
exit_code=$?
if [ "$exit_code" -ne 1 ]; then
	echo "[31mTest Failed[0m"
fi

./ppd invalid_stock1.dat coins.dat
exit_code=$?
if [ "$exit_code" -ne 1 ]; then
	echo "[31mTest Failed[0m"
fi
./ppd invalid_stock2.dat coins.dat
exit_code=$?
if [ "$exit_code" -ne 1 ]; then
	echo "[31mTest Failed[0m"
fi
./ppd invalid_stock3.dat coins.dat
exit_code=$?
if [ "$exit_code" -ne 1 ]; then
	echo "[31mTest Failed[0m"
fi
./ppd invalid_stock4.dat coins.dat
exit_code=$?
if [ "$exit_code" -ne 1 ]; then
	echo "[31mTest Failed[0m"
fi

echo "[31m =====================================WorkingProgram=======================================================[0m"

./ppd stock.dat coins.dat

exit_code=$?
if [ "$exit_code" -ne 0 ]; then
	echo "[31mTest Failed[0m"
fi