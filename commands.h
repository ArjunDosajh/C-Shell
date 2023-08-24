#ifndef __COMMANDS_H
#define __COMMANDS_H

// create an array of string which contains all the possible commands

void proclore(char *strtok_state);
void printPastEvents(char *strtok_state);
void executePastEvent(char *strtok_state);
void Seek(char *strtok_state);
void Peek(char *strtok_state);
void Clear();
void Warp(char *strtok_state);
void Echo(char *strtok_state);
void Sleep(char *strtok_state);
int execute_command(char *command, int isAnd);
void tokeniseCommands(char *input);

#endif