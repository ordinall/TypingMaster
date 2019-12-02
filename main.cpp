#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib, "lib/pdcurses.lib")
#include <time.h>
#include <windows.h>
#undef MOUSE_MOVED
#include "include/curses.h"

int wordcountfile = -1;
FILE *wordp;

void profiles(FILE *profile);
void menu(FILE *profile);

struct player {
    char name[40];
    float wpm = 0;
    int score = 0;
} play;

void writeready(FILE **profile) {
    fseek(*profile, -(sizeof(struct player)), SEEK_CUR);
}

char *getword() {
    fseek(wordp, 0, SEEK_SET);
    char *ch = (char *)malloc(100);
    int temp = 1;
    if (wordcountfile == -1) {
        while (temp != EOF) {
            temp = fscanf(wordp, "%s", ch);
            wordcountfile++;
        }
    }
    fseek(wordp, 0, SEEK_SET);
    int ran = rand() % (wordcountfile);
    for (int j = 1; j <= ran; j++) {
        fscanf(wordp, "%s", ch);
    }
    return ch;
}

void destroy_win(WINDOW *wind) {
    wborder(wind, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wclear(wind);
    wrefresh(wind);
    delwin(wind);
}

void stringhcentre(int y, int x, char *tempword) {
    mvprintw(y, x - strlen(tempword) / 2, "%s", tempword);
}

void delprofile(char *filename, int recnum) {
    FILE *fp = fopen(filename, "r+");
    FILE *ftemp = fopen("tempo", "w+");
    struct player temp;
    for (int i = 0; fread(&temp, sizeof(struct player), 1, fp) != NULL; i++) {
        if (i == recnum) {
        } else {
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

void updateprofile(FILE **profile) {
    fwrite(&play, sizeof(struct player), 1, *profile);
}

void createprofile(FILE *profile) {
    struct player temp;
    box(stdscr, 0, 0);
    fseek(profile, 0, SEEK_END);
    stringhcentre(LINES / 2, COLS / 2 - 10, "Enter name:");
    echo();
    refresh();
    char a;
    int i = 0;
    for (; !(((a = getch()) == '\n') || i >= 38); i++) {
        temp.name[i] = a;
    }
    temp.name[i] = '\0';
    noecho();
    fwrite(&temp, sizeof(struct player), 1, profile);
    clear();
    refresh();
    profiles(profile);
}

void assess(FILE *profile) {
    int typingx, typingy;
    int numword = 6;
    system("cls");
    for (int i = 0; i < 3; i++) {
        system("color F0");
        Sleep(150);
        system("color 0F");
        Sleep(150);
    }
    system("color F0");
    initscr();
    mvprintw(2, COLS / 2 - 8, "Speed Assessment      Your Highest wpm:%f", play.wpm);
    refresh();
    move(4, 1);
    for (int i = 0; i < numword; i++) {
        printw("%s ", getword());
        getyx(stdscr, typingy, typingx);
        if (typingx > COLS - 20) {
            typingy++;
            move(typingy, 1);
        }
        Sleep(30);
        refresh();
    }
    typingy = typingy + 4;
    move(typingy, 1);
    printw("Type all these words and press Enter after doing so, Dont cheat :)");
    move(typingy + 1, 1);
    refresh();
    unsigned int starttime = GetTickCount();
    int spacecount = 0;
    char ch = 'a';
    while (ch != '\n') {
        ch = getch();
        getyx(stdscr, typingy, typingx);
        if (typingx > COLS - 20) {
            typingy++;
            move(typingy, 1);
        }
        if (ch == ' ') {
            spacecount++;
        }
    }
    unsigned int endtime = GetTickCount();
    if (spacecount != numword - 1) {
        clear();
        move(LINES / 2 - 1, COLS / 2 - 5);
        printw("You cheated :(");
        refresh();
        Sleep(5000);
        endwin();
        system("color 0F");
        initscr();
        noecho();
        curs_set(0);
        menu(profile);
    } else {
        int dur = endtime - starttime;
        float sec = dur / 1000;
        float minute = sec / 60;
        float wpm = numword / minute;
        clear();

        if (play.wpm < wpm) {
            stringhcentre(LINES / 2 - 2, COLS / 2, "Congrats, You broke your highscore");
            play.wpm = wpm;
            writeready(&profile);
            updateprofile(&profile);
        }
        move(LINES / 2 - 1, COLS / 2 - 8);
        printw("Your Wpm = %.2f", wpm);
        refresh();
    }
    Sleep(5000);
    endwin();
    system("color 0F");
    initscr();
    noecho();
    curs_set(0);
    menu(profile);
}

void menu(FILE *profile) {
    clear();
    box(stdscr, 0, 0);
    refresh();
    stringhcentre(LINES / 2 - 5, COLS / 2 - 2, "SELECT A MODE");
    stringhcentre(LINES / 2 - 4, COLS / 2 - 2, "press q to quit");
    int arr[2][2] = {{LINES / 2 - 1, COLS / 2 - 7},
                     {LINES / 2 + 1, COLS / 2 - 7}};
    mvprintw(arr[0][0], arr[0][1], "Typing course");
    mvprintw(arr[1][0], arr[0][1], "Assessment");
    mvprintw(arr[0][0], arr[0][1] - 1, ">");
    int ch = 'a';
    int select = 0;
    keypad(stdscr, TRUE);
    while ((ch = getch()) != '\n') {
        if (ch == KEY_UP || ch == KEY_DOWN) {
            mvprintw(arr[select][0], arr[0][1] - 1, " ");
            select = !select;
            mvprintw(arr[select][0], arr[0][1] - 1, ">");
            refresh();
        }
        if (ch == 'q' || ch == 'Q') {
            endwin();
            exit(0);
        }
    }
    if (select) {
        keypad(stdscr, FALSE);
        clear();
        endwin();
        assess(profile);
    } else {
        keypad(stdscr, FALSE);
        /////////////////////////////////////////////////////////////////////////////////////////to be continued
    }
}

void profiles(FILE *profile) {
    clear();
    stringhcentre(2, COLS / 2, "PROFILES");
    stringhcentre(3, COLS / 2, "press DEL key to delete a profile");
    WINDOW *prowin = newwin(LINES - 10, COLS / 2 - 10, 5, COLS / 2 - 24);
    box(prowin, 0, 0);
    box(stdscr, 0, 0);
    refresh();
    wrefresh(prowin);
    fseek(profile, 0, SEEK_SET);
    int totallist = 0;
    for (;; totallist++) {
        fread(&play, sizeof(struct player), 1, profile);
        mvwprintw(prowin, totallist + 2, 3, "%s", play.name);
        if (feof(profile)) {
            break;
        }
    }
    mvwprintw(prowin, totallist + 2, 3, "\r                                             ");
    mvwprintw(prowin, totallist + 2, 3, "MAKE A NEW PROFILE");
    box(prowin, 0, 0);
    wrefresh(prowin);
    refresh();
    if (totallist > 14) {
        stringhcentre(LINES - 4, COLS / 2, "PROFILE LIMIT EXCEEDED, DELETE A PROFILE");
        wrefresh(prowin);
        refresh();
    }
    int select = 0;
    keypad(stdscr, TRUE);
    while (1) {
        mvwprintw(prowin, select + 2, 2, ">");
        wrefresh(prowin);
        switch (getch()) {
            case KEY_UP:
                mvwprintw(prowin, select + 2, 2, " ");
                select--;
                if (select == -1) {
                    select = totallist;
                }
                break;
            case KEY_DOWN:
                mvwprintw(prowin, select + 2, 2, " ");
                select++;
                if (select > totallist) {
                    select = 0;
                }
                break;
            case '\n':
                if (select == totallist) {
                    destroy_win(prowin);
                    keypad(stdscr, FALSE);
                    createprofile(profile);
                } else {
                    keypad(stdscr, FALSE);
                    clear();
                    destroy_win(prowin);
                    refresh();
                    fseek(profile, 0, SEEK_SET);
                    for (int k = 0; k <= select; k++) {
                        fread(&play, sizeof(struct player), 1, profile);
                    }
                    menu(profile);
                    exit(0);
                }
                break;
            case KEY_DC:
                destroy_win(prowin);
                keypad(stdscr, FALSE);
                fclose(profile);
                delprofile("profile.dat", select);
                break;
        }
    }

    refresh();
    wrefresh(prowin);
}

int main() {
    srand(time(NULL));
    wordp = fopen("glo.txt", "r");
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
    while (a != '\n') {
        a = getch();
    }
    FILE *profile = fopen("profile.dat", "r+");
    if (profile == NULL) {
        profile = fopen("profile.dat", "w+");
    }
    clear();
    box(stdscr, 0, 0);
    profiles(profile);
    endwin();
}
