#include <stdio.h>
#pragma comment(lib,"lib/pdcurses.lib")
#include <windows.h>
#undef MOUSE_MOVED
#include "include/curses.h"

void destroy_win(WINDOW *local_win){
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wclear(local_win);
	wrefresh(local_win);
	delwin(local_win);
}
WINDOW *create_newwin(int height, int width, int starty, int startx){	WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);
    mvwprintw(local_win,1,1,"HENLO :)");
	wrefresh(local_win);
	return local_win;
}


int main(){
    char ch;
    int wy,wx;
    initscr();
    noecho();
    cbreak();
    start_color();
    init_pair(1,COLOR_BLACK,COLOR_RED);
    attron(COLOR_PAIR(1));
    for(int i=0;i < COLS;i++){
        for(int k=0;k<LINES;k++){
            mvprintw(k,i," ");
        }
    }
    curs_set(0);
    WINDOW *hello = newwin(3,10,LINES/2,COLS/2);
    //wattron(hello,COLOR_PAIR(1));
    box(hello,0,0);
    box(stdscr,0,0);
    mvwprintw(hello,'1','1'," ");
    refresh();
    wrefresh(hello);
    while(1){
        ch = getch();
        getbegyx(hello,wy,wx);
        if(wy == 1){
            wy++;
        }
        if(wy == LINES-4){
            wy--;
        } 
        if(wx == 1){
            wx++;
        }
        if(wx == COLS - 11){
            wx--;
        }
        if(ch == 'w'){
            destroy_win(hello);
            hello = create_newwin(3,10,wy-1,wx);
        }
        if(ch == 'd'){
            destroy_win(hello);
            hello = create_newwin(3,10,wy,wx+1);
        }
        if(ch == 'a'){
            destroy_win(hello);
            hello = create_newwin(3,10,wy,wx-1);
        }
        if(ch == 's'){
            destroy_win(hello);
            hello = create_newwin(3,10,wy+1,wx);
        }
     /*   attron(COLOR_PAIR(1));
        for(int i=0;i < COLS;i++){
            for(int k=0;k<LINES;k++){
                mvprintw(k,i," ");
            }
        }*/
        refresh();
        wrefresh(hello);        
        
    }
    getch();
    endwin();
}
