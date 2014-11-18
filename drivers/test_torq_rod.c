/* ***********************************************************************
 * file: 		test_torq_rod.c
 * author: 	jacob cook
 * created on: 10/29/2014
 * 
 * **********************************************************************/	

#include <string.h>
#include "dbg.h"
#include "test.h"
#include "types.h"
#include "io.h"
#include "global.h"
#include "torque_rod.h"

/* current directory */
static char cdir[] = "maacs/torq_rod$ ";

/* command list */
#define NCMDS (3)
const static char *cmd_list[NCMDS] = {"on","off","torq"};
static char dir_cmd[CMD_LEN];
static int exit_dir;

/* static function protorypes */
static void process_cmd(char *cmd);
static void cmd1_handler(char *arg);
static void cmd2_handler(char *arg);
static void cmd3_handler(char *arg);


/** temp_dir *************************************************************
 * top level function of the directory, shouldn't need to change anything
 * in here when making a new module. The new function handler name 
 * should be added to test.h.
 ************************************************************************/
void trod_dir(void)
{
	char *cmd = dir_cmd;
	exit_dir = 0; /* reset exit condition */
	while(!exit_dir)
	{
		dbg_puts(cdir);
		get_cmd(cmd, CMD_LEN);
		dbg_puts("\n\r");
		process_cmd(cmd);
	}
}

/** process_cmd **********************************************************
 * This is a static function ment to handle the spacific module testing
 * functions.  New commands should be added into the else if statement
 * it handle any additional functionality required for the test.
 ************************************************************************/
static void process_cmd(char *cmd)
{
	char *arg;
	
	/* separate command from argument */
	parse_command(cmd, &arg);

	if(!strcmp(cmd,(char *)cmd_list[0]))
	{
		/* command 1 */
		cmd1_handler(arg);
	}
	else if(!strcmp(cmd,(char *)cmd_list[1]))
	{
		/* command 2 */
		cmd2_handler(arg);
	}
	else if(!strcmp(cmd,(char *)cmd_list[2]))
	{
		/* command 3 */
		cmd3_handler(arg);
	}
	else if(!strcmp(cmd,"ls"))
	{
		list_dir((char **)cmd_list, NCMDS);
	}
	else if(!strcmp(cmd,"cd"))
	{
		/* for now we can only go backward */
		if(!strcmp(arg,".."))
		{
			exit_dir = 1;
		}
		else dbg_puts("Directory not Recognized\n\r");
	}
	else	
	{
		/* error message */
		dbg_puts("Command not recognized\n\r");
	}
}

/** cmd1_handler *********************************************************
 * the following function handles the command input from the user and 
 * executes the desired action. the variable arg is passed in as a raw 
 * string and may need further parsing with in the function to execute 
 * correctly.  It should not be assumes it is the desired form.
 ************************************************************************/
static void cmd1_handler(char *arg)
{
	/* torque rod on */
	PRINT("%s\n\r",arg);
}

/** cmd2_handler *********************************************************
 * the following function handles the command input from the user and 
 * executes the desired action. the variable arg is passed in as a raw 
 * string and may need further parsing with in the function to execute 
 * correctly.  It should not be assumes it is the desired form.
 ************************************************************************/
static void cmd2_handler(char *arg)
{
	/* torque rod off */
	PRINT("%s\n\r",arg);
	
}

/** cmd3_handler *********************************************************
 * the following function handles the command input from the user and 
 * executes the desired action. the variable arg is passed in as a raw 
 * string and may need further parsing with in the function to execute 
 * correctly.  It should not be assumes it is the desired form.
 ************************************************************************/
static void cmd3_handler(char *arg)
{
	/* set torq */
	PRINT("%s\n\r",arg);
}

