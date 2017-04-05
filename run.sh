#!/bin/env bash
g++ -std=c++11 ./src/Feature1.cpp -o ./log_input/Feature1.exe
g++ -std=c++11 ./src/Feature2.cpp -o ./log_input/Feature2.exe
g++ -std=c++11 ./src/Feature3.cpp -o ./log_input/Feature3.exe
g++ -std=c++11 ./src/Feature4.cpp -o ./log_input/Feature4.exe
./log_input/Feature1.exe
./log_input/Feature2.exe
./log_input/Feature3.exe
./log_input/Feature4.exe
mv ./log_input/blocked.txt ./log_output/blocked.txt
mv ./log_input/hosts.txt ./log_output/hosts.txt
mv ./log_input/resources.txt ./log_output/resources.txt
mv ./log_input/hours.txt ./log_output/hours.txt
