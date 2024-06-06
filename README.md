## Multiple Screw Extruder Analysis: Investigating Pressure and Temperature

For in depth expliantion of project, see /doc/report.pdf

## Tutorial for running the code

This section will give a brief tutorial on how to run the code with the different test cases. 

1. For including the right C++ libraries one needs to be in the right Docker Image. An in-depth guide on how to install Docker and the right pacs-environment, in addition to building and running the environment, is given in the PACS-course Lab session 00. Mark that all the following steps need to be done within the environment. Here is a link to the tutorial on github: https://github.com/pacs-course/pacs-Labs/tree/main/Labs/2024/00-environment_setup
2. The project has to be downloaded and placed within the docker environment. The project can be downloaded by cloning with git from: https://github.com/bangkorkor/PACS-project
3. Do ```module load boost``` in the terminal to load the boost library.
4. Do ```pip install pandas matplotlib``` to install pandas and matplotlib to be used in the plotting script.
5. After completing step 4 all the necessary libraries should be installed, and one can do ```make``` to compile the code. Mark that this has to be done in the terminal in the lowest level of the project directory, this applies for all the remaining steps.
6. For running the code (and also compiling if not already done) do ```make run```. When it comes to running the code with different test cases, it by default runs the test case with parameters from the 2nd line in the data/config.csv file.
7. For running different test cases one can specify what model to run by doing ```./build/main model=7```. This will use the parameters from the 5th line of the data/config.csv file, without having to recompile the code. One can also change whatever parameter using the same technique. See doc/report.pdf/Getpot in section 3.2.1 for more information.
8. The results are shown in the /build folder. If everything is done right you shall see a Pressure_plot.png and Temperature_Plot.png file in the /build folder. 
