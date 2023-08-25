## How to run
```
make
./a.out
```

## Description

### ```main.c```
- This file keeps the terminal running in an infinite loop

### ```commands.c```
- This file contains the major part of the project
- Most commands are executed in this file itself and some commands which require a large amount of code have their separate files

### ```peek.c```
- `peek` command lists all the files and directories in the specified directories in lexicographic order (default peek does not show hidden files).

### ```seek.c```
- `seek` command looks for a file/directory in the specified target directory (or current if no directory is specified). It returns a list of relative paths (from target directory) of all matching files/directories (files in green and directories in blue) separated with a newline character.

### ```prompt.c```
- This file prints a new prompt int the shell

### ```system_commands.c```
- This file handles the execution of commands usign execvp. 
- It also handles the execution of these commands in the background

### ```global_variables.c```
- All the global variables have been initialised in this file

## Disclaimer
I have used chatgpt and copilot for some minor and repetitive code implementations. I am aware of the entire logic and implementation of the project.

## Assumptions
