This folder contains :
i) cam.cpp - Contains the code along with explanation as comments
ii) makefile
iii) empty.jpg
iv) traffic.jpg
v) this README.md file

To run the code on Ubuntu/MacOs :
1)First run the make command by using 'make' or 'make all' command. This creates a executable named cam.out .
2)Then run the command ./cam.out with both the images, one by one, by taking them as an command line arguement. Example: to run the program for empty.jpg run the command :- './cam.out empty' . 
3) On running this command, a window with the image(in grayscale) pops us. 
	a)Now select the 4 required points in anti-clockwise fashion starting from the left-top side. 
	b)Now the window automatically closes(as soon as 4th point is clicked) and next window with the projected image automatically opens up(for MacOs you have to press any key for the next window to appear).
	c)Now again press any key for this window to close and next window with the cropped image to appear. Press any key close this window thus ending the program.
4) Now you can see the projected and the cropped image saved for both the input image inside the same folder.
5) You can use 'make clean' command to delete the executable and start again if required. 

This program basically takes an image as input and first converts it to grayscale then corrects the angle so as to obtain a top view, this image is called projected image. Then it crops the required part of the image which was selected by the user. If any wrong command line arguement is provided during executing the 'cam.out' executable, then help is provided to the user by printing required instruction and ending the program.  