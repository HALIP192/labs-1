#!/bin/bash

sudo ./server 0< input_ser 1> output_ser &
./client 0< output_ser 1> input_ser &
#./buttons 1>output_ser

