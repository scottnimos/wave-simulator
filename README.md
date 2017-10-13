# Pivot Maritime Wave Simulation
### Project Requirements
* Visual Studio 2017
* [UE4 with WaveWorks](https://github.com/NvPhysX/UnrealEngine/tree/WaveWorks-4.16)


## Installation Guide
There are 2 parts required to be able to run and edit this WaveWorks Project:

### Part 1: Setting up the Unreal Engine 4 with WaveWorks.

### Part 2: Accessing and cloning the repository of this project.
 
### Part 1:
Waveworks Installation and setup with Unreal Engine 4:
Step By Step Installation of Waveworks
 
Step 1:
Visit https://developer.nvidia.com/gameworks-source-github, create a GitHub account and Nvidia Developer account as instructed. Then enter your GitHub username at the bottom of the EULA. Wait for confirmation that your accounts have been linked (this may take upto an hour).
 
Step 2:
Access the Github NVIDIA Waveworks Repository while logged into your Github account which is linked to the NVIDIA Developer account. (https://github.com/NvPhysX/UnrealEngine/tree/WaveWorks)
 
You will see the picture below if you are not logged into an account which is linked to NVIDIA. Please confirm you are logged in to Github, if you still can’t access the repository then the most likely issue is that the NVIDIA Account has not yet been linked to your Github account. Confirm that Step 2 has been done correctly and try the link again.

With the Github Repository opened you can now view the remaining steps if you scroll down the page to the README.md file. However, the steps will be covered in detail for simplicity also.
 
Step 3:
Install Github for Windows and fork and clone the GitHub repository. Then switch forks to the WaveWorks-4.16 branch.
 
Step 4:
Install Visual Studios 2015 with C++ support installed.
 
Step 5:
Open the WaveWorks folder on your computer and run Setup.bat
 
Step 6:
Run the GenerateProjectFiles.bat
 
Step 7:
Open the UE4.sln file in Visual Studio
 
Step 8:
Press F5 in Visual Studio to run in debug mode, then build the files that are needed.
 
### Part 2:

Accessing and cloning the repository of this project.
 
On GitHub, navigate to the main page of the repository at https://github.com/scottnimos/wave-simulator
Under the repository name, click Clone or download.
In the Clone with HTTPs section, click to copy the clone URL for the repository.
Open Git Bash or Git Shell.
Change the current working directory to the location where you want the cloned directory to be made.
Example:
Type, git clone and then paste the URL you copied in Step 2.
Press Enter. Your local clone will be created.
Unreal Engine can now be launched and you can select the directory that you just cloned.

When it states that it is a different version, all you need to do is convert in place.


### Authors of code
All blueprints under Content/BP has been edited by both Scott Nimos and Nathan Mangan,
with all the C++ code both of us have worked on the BuoyancyManager and FloatingSphere.
Throughout the year we have adapted a pair programming style.

__:octocat: How to contribute (Assuming you have cloned the project)__
* git pull
* git add --all
* git commit -am "WHOOP :+1:"
* git push origin master
