#ifndef S4532390_CAG_DISPLAY_H
#define S4532390_CAG_DISPLAY_H

#define GRID_WIDTH 15
#define GRID_HEIGHT 15

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

extern char cellColour[15];

void s4532390_CAG_Display_Init();

void s4532390_CAG_Display_Task();



#endif