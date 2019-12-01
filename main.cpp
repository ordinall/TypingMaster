#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#pragma comment(lib, "lib/pdcurses.lib")
#include <windows.h>
#include <time.h>
#undef MOUSE_MOVED
#include "include/curses.h"

int wordcountfile = -1;
void profiles(FILE *profile);

struct player
{
    char name[40];
    int wpm = 0;
    int score = 0;
} play;

char *getword(FILE *fp)
{
    fseek(fp, 0, SEEK_SET);
    char *ch = (char *)malloc(100);
    int temp = 1;
    if (wordcountfile == -1)
    {
        while (temp != EOF)
        {
            temp = fscanf(fp, "%s", ch);
            wordcountfile++;
        }
    }
    fseek(fp, 0, SEEK_SET);
    int ran = rand() % (wordcountfile);
    for (int j = 1; j <= ran; j++)
    {
        fscanf(fp, "%s", ch);
    }
    return ch;
}

void destroy_win(WINDOW *wind)
{
    wborder(wind, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wclear(wind);
    wrefresh(wind);
    delwin(wind);
}

void stringhcentre(int y, int x, char *tempword)
{
    mvprintw(y, x - strlen(tempword) / 2, "%s", tempword);
}

void delprofile(char *filename, int recnum)
{
    FILE *fp = fopen(filename, "r+");
    FILE *ftemp = fopen("tempo", "w+");
    struct player temp;
    for (int i = 0; fread(&temp, sizeof(struct player), 1, fp) != NULL; i++)
    {
        if (i == recnum)
        {
        }
        else
        {
            fwrite(&temp, sizeof(struct player), 1, ftemp);
        }
    }
    fclose(fp);
    fclose(ftemp);
    remove(filename);
    rename("tempo", filename);
    FILE *profile = fopen("profile.dat", "r+");
    profiles(profile);
}

void createprofile(FILE *profile)
{
    struct player temp;
    box(stdscr, 0, 0);
    fseek(profile, 0, SEEK_END);
    stringhcentre(LINES / 2, COLS / 2 - 10, "Enter name:");
    echo();
    refresh();
    char a;
    int i = 0;
    for (; !(((a = getch()) == '\n') || i >= 38); i++)
    {
        temp.name[i] = a;
    }
    temp.name[i] = '\0';
    noecho();
    fwrite(&temp, sizeof(struct player), 1, profile);
    clear();
    refresh();
    profiles(profile);
}

void profiles(FILE *profile)
{
    stringhcentre(2, COLS / 2, "PROFILES");
    stringhcentre(3, COLS / 2, "press DEL key to delete a profile");
    WINDOW *prowin = newwin(LINES - 10, COLS / 2 - 10, 5, COLS / 2 - 24);
    box(prowin, 0, 0);
    box(stdscr, 0, 0);
    refresh();
    wrefresh(prowin);
    fseek(profile, 0, SEEK_SET);
    int totallist = 0;
    for (;; totallist++)
    {
        fread(&play, sizeof(struct player), 1, profile);
        mvwprintw(prowin, totallist + 2, 3, "%s", play.name);
        if (feof(profile))
        {
            break;
        }
    }
    mvwprintw(prowin, totallist + 2, 3, "\r                                             ");
    mvwprintw(prowin, totallist + 2, 3, "MAKE A NEW PROFILE");
    box(prowin, 0, 0);
    wrefresh(prowin);
    refresh();
    if (totallist > 14)
    {
        wprintw(prowin, "\rPROFILE LIMIT EXCEEDED, DELETE A PROFILE");
        wrefresh(prowin);
    }
    int select = 0;
    keypad(stdscr, TRUE);
    while (1)
    {
        mvwprintw(prowin, select + 2, 2, ">");
        wrefresh(prowin);
        switch (getch())
        {
        case KEY_UP:
            mvwprintw(prowin, select + 2, 2, " ");
            select--;
            if (select == -1)
            {
                select = totallist;
            }
            break;
        case KEY_DOWN:
            mvwprintw(prowin, select + 2, 2, " ");
            select++;
            if (select > totallist)
            {
                select = 0;
            }
            break;
        case '\n':
            if (select == totallist)
            {
                delwin(prowin);
                keypad(stdscr, FALSE);
                createprofile(profile);
            }
            break;
        case KEY_DC:
            delwin(prowin);
            keypad(stdscr, FALSE);
            fclose(profile);
            delprofile("profile.dat", select);
            break;
        }
    }

    refresh();
    wrefresh(prowin);
}

int main()
{
    srand(time(NULL));
    FILE *wordp = fopen("glo.txt", "r");
    initscr();
    noecho();
    curs_set(0);
    int wy, wx;
    box(stdscr, 0, 0);
    stringhcentre(LINES / 2 - 3, COLS / 2, "Welcome to Typing Master 0.0.1 Alpha");
    stringhcentre(LINES / 2 - 2, COLS / 2, "Developed by Abdullah Abid (19K-1042) SE-A");
    stringhcentre(LINES / 2 - 1, COLS / 2, "Press Enter to Continue");
    refresh();
    char a = 'a';
    while (a != '\n')
    {
        a = getch();
    }
    FILE *profile = fopen("profile.dat", "r+");
    if (profile == NULL)
    {
        profile = fopen("profile.dat", "w+");
    }
    clear();
    box(stdscr, 0, 0);
    profiles(profile);
    endwin();
}
/*
        ch = getch();
        getbegyx(hello,wy,wx);
        if(wy == 1){
            wy++;
        }
        
*/
