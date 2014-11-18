/* ***********************************************************************
 * file: 		test.c
 * author: 	jacob cook
 * created on: 5/27/2014
 * **********************************************************************/	

#include <string.h>
#include "dbg.h"
#include "test.h"
#include "test_temp.h"
#include "test_torq_rod.h"
#include "types.h"
#include "io.h"

/* main menu directroy options */
#define TEST_NDIRS (4)
const char *dir[TEST_NDIRS] = {"clocks","i2c","AD7991","trods"};
char temp[] = "temp";
char backspace[] = {8,32,8,0};
/* static array definitions */
char MAACS[] = 
"\n\r***##*****##******##*********##********#####*****#####**** \
 \n\r***###***###*****####*******####******##***##***##***##*** \
 \n\r***####*####****##**##*****##**##****##*********####****** \
 \n\r***##*###*##***##****##***##****##***##**********#####**** \
 \n\r***##**#**##***########***########***##*************###*** \
 \n\r***##*****##***##****##***##****##****##***##***##***##*** \
 \n\r***##*****##***##****##***##****##*****#####*****#####**** \
 \n\r\n\r";


/*char user_cmd[CMD_LEN];*/

/* static function protoypes */	
static void process_cmd(char *cmd);
void change_dir(char *arg);

void test_main_menu(void)

{
	char cmd[CMD_LEN];
	dbg_puts(MAACS);
		
	dbg_puts("Welcome to the MAACS flight software test environment\n\r");
	dbg_puts("please press any key to continue...\n\r\n\r");
	dbg_getc();
		
	while(1)
	{
		dbg_puts("maacs$ ");
		get_cmd(cmd, CMD_LEN);
		dbg_puts("\n\r");
		process_cmd(cmd);		
	}
}

static void process_cmd(char *cmd)
{
	char *arg;
	
	/* separate command from argument */
	parse_command(cmd, &arg);

	/* service command */
	if(!strcmp(cmd,"ls"))
	{
		/* list directory contents */
		list_dir((char **)dir, TEST_NDIRS);
	}
	else if(!strcmp(cmd,"cd"))
	{
		/* change directories */
		change_dir(arg);
	}
	else if(!strcmp(cmd,"clear"))
	{
		dbg_puts("\033[2J");
	}
	else	
	{
		/* error message */
		dbg_puts("Command not recognized\n\r");
	}
}

/** list_dir *************************************************************
 * This function lists the passed directory folder and commands.
 ************************************************************************/
void list_dir(char *dir[], int ndir)
{
		int i;

		/* list directory options */
		for(i=0;i<ndir;i++)
		{
			dbg_puts((char *)dir[i]);
			dbg_puts("\n\r");
		}
}		 

/** change_dir ***********************************************************
 * This change directory function is specifically for the main menu and 
 * provides a path down to the device driver and module levels. 
 * 
 * Any new modules should be added to the dir array declared above and a
 * new else if statement added to provide a path the the testing 
 * algorithms.
 ************************************************************************/
void change_dir(char *arg)
{
	if(!strcmp(arg,(char *)dir[0]))
	{
		dbg_puts((char *)dir[0]);
		dbg_puts("\n\r");
	}
	else if(!strcmp(arg,(char *)dir[1]))
	{
		dbg_puts((char *)dir[1]);
		dbg_puts("\n\r");
	}
	else if(!strcmp(arg,(char *)dir[3]))
	{
		trod_dir();
	}
	else if(!strcmp(arg, temp))
	{
		temp_dir();
	}
	else 
	{
	 	dbg_puts("directory not recognized\n\r");
	}
}

/** parse_command ********************************************************
 * This function separates the user input command from the argument. 
 * If the command contains more than one argument it will be handled 
 * at the command excution level.
 ************************************************************************/
void parse_command(char *cmd, char **arg)
{
	char *brk;
	int len;	

	/* break up command and argument strings */
	brk = strchr(cmd,32);
	if(brk != 0)
	{
		*brk++ = 0;
		*arg = brk;
	}
	else 
	{
		len = strlen(cmd);
		*arg = &cmd[len];	
	}
}	

/** get_command **********************************************************
 * This function is used to get a user input from the command line.
 ************************************************************************/
void get_cmd(char *cmd, int len)
{
	unsigned char c;
	unsigned int i = 0;

	c = dbg_getc(); /* get user input */
	/* get input until the user pushes enter or the max length is reached. */
	while(c != 13 && i < len-1)
	{
		if (c==8) /* handle backspace */
		{
			if(i == 0) /* don't back space if we are at the beginning */
			{
				cmd[i] = 0;
			}
			else
			{
				dbg_puts(backspace); /* show backspace on terminal */
				cmd[i--] = 0; /* erase last user input */
			}
		}
		else
		{
			dbg_putc(c); /* echo input */
			cmd[i++] = c; /* save data and increment pointer */
		  cmd[i] = 0; /* move NULL character up */
		}
		c = dbg_getc(); 
	}
}	
