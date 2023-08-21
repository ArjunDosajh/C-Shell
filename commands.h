#ifndef __COMMANDS_H
#define __COMMANDS_H

// create an array of string which contains all the possible commands

void Warp(char *strtok_state);
void Echo(char *strtok_state);
void Sleep(char *strtok_state);
void execute_command(char *command, int isAnd);
void tokeniseCommands(char *input);

#endif