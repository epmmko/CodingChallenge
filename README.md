Source codes need to be compiled with c++11 standard.
To compile with gcc
1)	Go to the directory that source file is located by
cd src
2)	Compile each file by
g++ -std=c++11 Feature1.cpp Feature1.exe
g++ -std=c++11 Feature2.cpp Feature2.exe
g++ -std=c++11 Feature3.cpp Feature3.exe
g++ -std=c++11 Feature4.cpp Feature4.exe

move the .exe file to the same location as the log.txt (otherwise won't work)
then run .exe file, file by file, by typing

./Feature1.exe

The output file will be produced in the same directory with the .exe file folder
In my computer, i7-4770 CPU @ 3.40 GHz, RAM 16 GB, 64-bit operating system,
I run call exe file in cygwin (unix environment for windows).
Feature 1 took roughly 5 seconds.
Feature 2 took roughly 3 seconds.
Feature 3 took roughly 7 seconds.
Feature 4 took roughly 4 seconds.

I use Visual Studio to write the code and debug.
To get the full speed, try to compile in Unix environment (I did cygwin64 with g++).
I also tested my code in windows environment.
In Windows environment, 
Feature 1 took 113 second
Feature 2 took 423 second
Feature 3 took 234 second
Feature 4 took 550 second

Result Explanation
Feature 1:
