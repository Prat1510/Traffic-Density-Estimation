# SUBTASK2

This folder contains :
1. density.cpp - Contains the code along with explanation as comments
2. makefile
3. this README.md file
4. backgroung.jpg - image which is used as background for finding queue density
5. plot.png - plot of queue density and moving density
6. out.txt - a text file containg values of queue density and moving density at different instants
7. cam.h - a file containing code for angle correction and cropping.

To run the code on Ubuntu/MacOs :
1) First run the make command by using 'make' or 'make all' command. This creates a executable named density.out .

2) Then run the command ./density.out followed by video name as an command line arguement. The input video should be present in the same folder as the executable. 

3) On running this command: 
   a)Three columns of values - "time (in secs), queue density, dynamic density" will begin to be printed on the command line.
   b)It also makes output.txt file containg values of queue density and moving density at different instants.
   c)The program will stop after the video ends. You can also end the program by pressing esc.

4) You can use 'make clean' command to delete the executable and start again if required.

This program basically takes an video as input, and then using opencv functions, it finds the moving density and queue density in the traffic video.
It also makes "output.txt" file containg values of queue density and moving density at different instants. Along with that, it also prints those values
on terminal.

If any wrong command line arguement is provided during executing the 'density.out' executable, then help is provided to the user by printing required instruction and ending the program.  
