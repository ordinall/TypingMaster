#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#pragma comment(lib,"lib/pdcurses.lib")
#include <windows.h>
#include <time.h>
#undef MOUSE_MOVED
#include "include/curses.h"

char * getword(FILE * fp){
    fseek(fp, 0, SEEK_SET);
    char * ch = (char *)malloc(100);
    int i=0;
    int temp=1;
    while(temp != EOF){
       // if(ch == '\n'){
            temp = fscanf(fp,"%s",ch);
            i++;
      //  }
      //  ch=fgetc(fp);
    }
    
    fseek(fp, 0, SEEK_SET);
    int ran = rand() % (i);
    for(int j=1;j<=ran;j++){
        fscanf(fp,"%s",ch);
    }
   /* ch ='a';
    int pos=ftell(fp);
    printf("%d",ftell(fp));
    int len=0;
    for(;ch != '\n';len++){
        ch = fgetc(fp);
    }
    fseek(fp,pos,SEEK_SET); */
   // char word[100];
    //fgets(word,len-1,fp);
    //for(int i=0;i<len;i++){
    //    *(word+i)=fgetc(fp);
    //}
    //*(word+len)='\0';

   // return word;
   return ch;
}

void destroy_win(WINDOW *wind){
	wborder(wind, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wclear(wind);
	wrefresh(wind);
	delwin(wind);
}

WINDOW *create_newwin(int h, int w, int y, int x,int i){	
    WINDOW *wind;
	wind = newwin(h, w, y, x);
    if(i==1){
	    box(wind, 0 , 0);
    }
	wrefresh(wind);
	return wind;
}


int main(){
    srand(time(NULL));
    while(1){
        FILE * fp = fopen("glo.txt","r");
        Sleep(100);
        char * haha=getword(fp);
        printf("%s\n",haha);
        free(haha);
        fclose(fp);
    }
    getchar();

}
  /*  char ch;
    int wy,wx;
    initscr();
    noecho();
    cbreak();
   // start_color();
   // init_pair(1,COLOR_BLACK,COLOR_RED);
   // attron(COLOR_PAIR(1));
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
        attron(COLOR_PAIR(1));
        for(int i=0;i < COLS;i++){
            for(int k=0;k<LINES;k++){
                mvprintw(k,i," ");
            }
        }
        refresh();
        wrefresh(hello);        
        
    }
    getch();
    endwin();
}
*/
