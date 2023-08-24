#ifndef __PROMPT_H
#define __PROMPT_H

extern char home_directory[], host_name[], system_name[], current_working_direcotry[], previous_working_directory[];
extern int bgIndex;


int init_terminal();
void prompt();

#endif