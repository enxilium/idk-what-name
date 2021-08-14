// including thingys I need
#include <ncurses.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

//I kinda need this for the winsize
struct winsize win_size;
int y;
int x;
WINDOW *win;
static char buffer[] = "foobar";

// Random function for choosing if something is white or black
int noise_gen(int density);
int moores_hood(int y_pos, int x_pos);
void bomb(char *message);

int main()
{
    //getting density
    int density;
    scanf("%i", &density);

    //starting the screen
    initscr();

    FILE *oldfile;
    FILE *newfile;

    //This is how I get the size getyx doesn't work well lol
    ioctl(0, TIOCGWINSZ, &win_size);
    y = win_size.ws_row;
    x = win_size.ws_col;

    win = newwin(y, x, 0, 0);
    if (win == NULL)
        bomb("Unable to create window\n");
    // hiding the thingy, cursor visablity
    curs_set(0);
    refresh();

    //actually making the noise now
    for (int y_pos = 0; y_pos < y; y_pos++)
    {
        for (int x_pos = 0; x_pos < x; x_pos++)
        {
            if (noise_gen(density) == 0)
            {
                mvwprintw(win, y_pos, x_pos, "#");
            }
            else
            {
                mvwprintw(win, y_pos, x_pos, " ");
            }
        }
    }

    // //Get position, go up, if wall add to int, then check all sides
    // int walled = 0;
    // // if(mvinch(y_pos + 1, x_pos) == '#'){
    // //     walled++;
    // // }
    // if (mvinch(3, 3) == '#')
    // {
    //     walled++;
    // }
    int r;
    wrefresh(win);
    oldfile = fopen("OLD.txt", "w+");
    if (oldfile == NULL)
        bomb("Error creating file\n");

    /* write the window's data */
    r = putwin(win, oldfile);
    if (r == ERR)
        addstr("Error putting window to disk\n");
    else
        addstr("Window put to disk\n");

    getch();

    wrefresh(win);
    refresh();

    //closing window
    int walled;
    for (int i = 0; i < 10; i++)
    {
        getch();
        for (int y_pos = 0; y_pos < y; y_pos++)
        {
            for (int x_pos = 0; x_pos < x; x_pos++)
            {
                if (noise_gen(density) == 0)
                {
                    walled = moores_hood(y_pos, x_pos);
                }
            }
        }
    }
    mvprintw(0, 0, "The end is nye");
    getch();



/*
You know what?
Screw this, Ima try another method
Have 3 windows, 
1 old, 1 new, and one being shown (We need the 3rd one so stuff stays realitively simple)
switch which one is being shown when doing the analyaztion and printing
solves all the issues
*/


    getch();
    endwin();

    //displaying debug info
    printf("diemensions: %i, %i \n", x, y);
    printf("is 3,3 walled? %i ( 1 = yes, 0 = no) \n", walled);
}

int noise_gen(int density)

{

    int lower = 0, upper = 100;
    int num = (rand() % (upper - lower + 1)) + lower;
    if (num > density)
    {
        num = 1;
    }
    else
    {
        num = 0;
    }
    return num;
}
int moores_hood(int y_pos, int x_pos)
{
    int walled = 0;
    //perpendicular stuff
    // X 1 X
    // 4 Y 3
    // X 2 X
    if (mvinch(y_pos + 1, x_pos) == '#')
    {
        walled++;
    }
    if (mvinch(y_pos - 1, x_pos) == '#')
    {
        walled++;
    }
    if (mvinch(y_pos, x_pos + 1) == '#')
    {
        walled++;
    }
    if (mvinch(y_pos, x_pos - 1) == '#')
    {
        walled++;
    }

    // Adjacent
    //  4 X 1
    //  X X X
    //  2 X 3
    if (mvinch(y_pos + 1, x_pos + 1) == '#')
    {
        walled++;
    }
    if (mvinch(y_pos - 1, x_pos - 1) == '#')
    {
        walled++;
    }
    if (mvinch(y_pos - 1, x_pos + 1) == '#')
    {
        walled++;
    }
    if (mvinch(y_pos + 1, x_pos - 1) == '#')
    {
        walled++;
    }

    //finally switching it, time to go experimental
    if (walled < 4)
    {
        mvprintw(y_pos, x_pos, "#");
    }
    else
    {
        mvprintw(y_pos, x_pos, " ");
    }
    return walled;
}

void bomb(char *message)
{
    endwin();
    puts(message);
    exit(1);
}
