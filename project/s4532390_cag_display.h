/**
*********************
*********************
*@file mylib/project/s4532390_cag_display.c
*@author Robert Francis - 45323906
*@date 1/06/2020
*@brief CAG Display library
*REFERENCE:
csse3010_project.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
void s4532390_cag_display_init();
void s4532390_cag_display_task();
*********************
*********************
**/

#ifndef s4532390_CAG_DISPLAY_H
#define s4532390_CAG_DISPLAY_H

//Width and height of the grid
#define GRID_WIDTH 15
#define GRID_HEIGHT 15

//1 for alive, 0 for dead for cells
#define ALIVE 1
#define DEAD 0

#define CELL_BLACK "\e[7;30;40m"
#define CELL_RED	"\e[7;31;41m"
#define CELL_GREEN	"\e[7;32;42m"
#define CELL_YELLOW		"\e[7;33;43m"
#define CELL_BLUE		"\e[7;34;44m"
#define CELL_MAGENTA	"\e[7;35;45m"
#define CELL_CYAN		"\e[7;36;46m"
#define CELL_WHITE 		"\e[7;37;47m"

extern char s4532390_cellColour[15];

void s4532390_cag_display_init();

void s4532390_cag_display_task();

#endif