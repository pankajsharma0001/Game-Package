Library required for the project!!

https://www.raylib.com

if any problem occur, you can also visit the GitHub,
https://github.com/pankajsharma0001/Game-Package

Here is the instruction to run the project.
If MingGw is installed then first uninstall it because it cause problem to run.
MingGw is already contained in the make file.

1. First install the raylib library.

2. Update System Environment Variables:
	C:\raylib\w64devkit\bin

3. Navigate to Source Directory:
	Open a terminal and navigate to the following path:
	C:\raylib\raylib\src

4. Compile Raylib:
	In the terminal, run the following command:
	mingw32-make PLATFORM=PLATFORM_DESKTOP

5. Locate Compiled Library:
	After the command executes successfully, a new file named libraylib.a will be created.

6. Replace Existing Library:
	Copy the newly created libraylib.a file and replace the existing libraylib.a file in the lib folder within your project directory.

7. Run the Program:
	To compile and run the program, execute the following command in the terminal:  make
	

