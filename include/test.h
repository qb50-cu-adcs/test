/* ***********************************************************************
 * file: 		test.h
 * author:	jacob cook
 * created on:	5/27/2014
 * 
 * TODO file description
 * ***********************************************************************
 */

#ifndef __TEST_H
#define __TEST_H

#define CMD_LEN (50)


/* function prototypes */
void test_main_menu(void);
void list_dir(char *dir[], int ndir);
void get_cmd(char *cmd, int len);
void parse_command(char *cmd, char **arg);

/* module dir function protos */
void temp_dir(void);

#endif
