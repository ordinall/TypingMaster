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
    wclear(wind);
    wrefresh(wind);
    delwin(wind);
}

WINDOW *create_newwin(int h, int w, int starty, int startx, char *string) {
    WINDOW *tempwin;
    tempwin = newwin(h, w, starty, startx);
    mvwprintw(tempwin, 0, 0, string);
    wrefresh(tempwin);
    return tempwin;
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
void waitenter() {
    char a = 'a';
    while (a != '\n') {
        a = getch();
    }
}

void createprofile(FILE *profile) {
    struct player temp;
    box(stdscr, 0, 0);
    fseek(profile, 0, SEEK_END);
    stringhcentre(LINES / 2, COLS / 2 - 10, "Enter name:");
    noecho();
    curs_set(1);
    refresh();
    int i = 0;
    keypad(stdscr, TRUE);
    int ch = 'a';
    int cury, curx;
    for (; !(((ch = getch()) == '\n') || i >= 38); i++) {
        getyx(stdscr, cury, curx);
        if (ch == 8) {
            mvprintw(cury, curx - 1, " ");
            move(cury, curx - 1);
        } else {
            temp.name[i] = ch;
            printw("%c", ch);
            refresh();
        }
    }
    temp.name[i] = '\0';
    curs_set(0);
    fwrite(&temp, sizeof(struct player), 1, profile);
    clear();
    refresh();
    profiles(profile);
}

void course(FILE *profile) {
    srand(time(NULL));
    int typingx, typingy;
    int numword = 20;
    system("cls");
    for (int i = 0; i < 2; i++) {
        system("color F0");
        Sleep(8);
        system("color 0F");
        Sleep(80);
    }
    system("color F0");
    initscr();
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    int health = 7;
    int score = 0;
    mvprintw(2, COLS / 2 - 11, "Fast Typing Course       Your Highscore:%d", play.score);
    stringhcentre(5, COLS / 2, "Type the words appearing on the screen, type the bottom most word first");
    stringhcentre(6, COLS / 2, "Dont correct and worry about typing mistakes");
    stringhcentre(7, COLS / 2, "Press Enter to Start");
    mvprintw(LINES - 3, COLS / 2 - 5, "Health: %d", health);
    mvprintw(LINES - 2, COLS / 2 - 5, "Score: %d", score);
    refresh();
    refresh();
    Beep(1600, 150);
    Beep(1500, 150);
    Beep(1000, 250);
    waitenter();
    int starty = 8;
    int spawnpos = COLS / 8;
    int endy = LINES - 4;
    WINDOW *win[3];
    char words[3][21];
    strcpy(words[0], getword());
    strcpy(words[1], getword());
    strcpy(words[2], getword());
    win[0] = create_newwin(1, 20, starty + 1, spawnpos * 2, words[0]);
    win[1] = create_newwin(1, 20, starty + 2, spawnpos * 4, words[1]);
    win[2] = create_newwin(1, 20, starty + 3, spawnpos * 6, words[2]);
    stringhcentre(7, COLS / 2, "                      ");
    mvhline(endy, 0, ACS_HLINE, COLS);
    refresh();
    char ch;
    int charcount = 0;
    int speed = 10;
    for (long long int p = 0;; p++) {
        mvprintw(LINES - 3, COLS / 2 - 5, "Health: %d", health);
        mvprintw(LINES - 2, COLS / 2 - 5, "Score: %d", score);
        refresh();
        int winx, winy;
        if (p == 100)
            speed = 7;
        if (p == 300)
            speed = 5;
        if (p % speed == 0) {
            for (int i = 0; i < 3; i++) {
                getbegyx(win[i], winy, winx);
                if (winy == endy - 1) {
                    wclear(win[i]);
                    wrefresh(win[i]);
                    delwin(win[i]);
                    strcpy(words[i], getword());
                    create_newwin(1, 20, starty + 1, spawnpos * ((i + 1) * 2), words[i]);
                    health--;
                    charcount = 0;
                } else {
                    wclear(win[i]);
                    wrefresh(win[i]);
                    delwin(win[i]);
                    create_newwin(1, 20, winy + 1, winx, words[i]);
                }
            }
        }
        if (health == 0) {
            clear();
            stringhcentre(LINES / 2 - 3, COLS / 2, "GAME OVER!");
            if (play.score < score) {
                stringhcentre(LINES / 2 - 2, COLS / 2, "Congrats, You broke your highscore");
                play.score = score;
                writeready(&profile);
                updateprofile(&profile);
            }
            move(LINES / 2 - 1, COLS / 2 - 8);
            printw("Your Score = %d", score);
            Beep(1000, 120);
            Beep(1500, 150);
            Beep(2000, 500);
            refresh();
            stringhcentre(LINES / 2, COLS / 2, "Press enter to continue");
            refresh();
            waitenter();
            endwin();
            system("cls");
            for (int i = 0; i < 2; i++) {
                system("color 0F");
                Sleep(8);
                system("color F0");
                Sleep(80);
            }
            system("color 0F");
            initscr();
            noecho();
            curs_set(0);
            menu(profile);
        }
        ch = getch();
        if (ch != ERR) {
            int max = -1;
            int maxwin;
            for (int i = 0; i < 3; i++) {
                int x, y;
                getbegyx(win[i], y, x);
                if (y > max) {
                    max = y;
                    maxwin = i;
                }
            }
            if (ch == words[maxwin][charcount]) {
                words[maxwin][charcount] = ' ';
                charcount++;
                score += 10;
                int flag = 0;
                for (int l = 0; words[maxwin][l] != '\0'; l++) {
                    if (words[maxwin][l] != ' ') {
                        flag = 1;
                    }
                }
                if (flag == 1) {
                    int rey, rex;
                    getbegyx(win[maxwin], rey, rex);
                    wclear(win[maxwin]);
                    wrefresh(win[maxwin]);
                    delwin(win[maxwin]);
                    create_newwin(1, 20, rey, rex, words[maxwin]);
                    wrefresh(win[maxwin]);
                    refresh();
                } else {
                    wclear(win[maxwin]);
                    wrefresh(win[maxwin]);
                    delwin(win[maxwin]);
                    strcpy(words[maxwin], getword());
                    create_newwin(1, 20, starty + 1, spawnpos * ((maxwin + 1) * 2), words[maxwin]);
                    charcount = 0;
                }
            }
        }
        for (int i = 0; i < 3; i++) {
            wrefresh(win[i]);
            refresh();
        }

        Sleep(50);
    }
}

void assess(FILE *profile) {
    int typingx, typingy;
    int numword = 20;
    system("cls");
    for (int i = 0; i < 2; i++) {
        system("color F0");
        Sleep(8);
        system("color 0F");
        Sleep(80);
    }
    system("color F0");
    initscr();
    noecho();
    cbreak();
    mvprintw(2, COLS / 2 - 11, "Speed Assessment       Your Highest wpm:%.2f", play.wpm);
    stringhcentre(5, COLS / 2, "Type the words and press Enter after typing all, Dont cheat :)");
    stringhcentre(6, COLS / 2, "Dont correct and worry about typing mistakes");
    stringhcentre(7, COLS / 2, "Press Enter to Start");
    refresh();
    Beep(2000, 150);
    Beep(1500, 150);
    Beep(1000, 500);
    waitenter();
    stringhcentre(7, COLS / 2, "                      ");
    move(9, 1);
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
    printw("TYPE:");
    move(typingy + 1, 1);
    refresh();
    unsigned int starttime = GetTickCount();
    int spacecount = 0;
    keypad(stdscr, TRUE);
    int ch = 'a';
    while (ch != '\n') {
        ch = getch();
        getyx(stdscr, typingy, typingx);
        if (ch == 8) {
            mvprintw(typingy, typingx - 1, " ");
            move(typingy, typingx - 1);
        } else {
            if (typingx > COLS - 20) {
                typingy++;
                move(typingy, 1);
            }
            if (ch == ' ') {
                spacecount++;
            }
            printw("%c", ch);
        }
        refresh();
    }
    keypad(stdscr, TRUE);
    unsigned int endtime = GetTickCount();
    if (!(spacecount > numword - 7 && spacecount < numword + 7)) {
        clear();
        Beep(1300, 100);
        stringhcentre(LINES / 2 - 1, COLS / 2, "Try that again, sorry");
        stringhcentre(LINES / 2, COLS / 2, "Press enter to continue");
        refresh();
        waitenter();
        endwin();
        for (int i = 0; i < 2; i++) {
            system("color 0F");
            Sleep(8);
            system("color F0");
            Sleep(80);
        }
        system("color 0F");
        initscr();
        noecho();
        curs_set(0);
        menu(profile);
    } else {
        float dur = endtime - starttime;
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
        Beep(1000, 120);
        Beep(1500, 150);
        Beep(2000, 500);
        refresh();
    }
    stringhcentre(LINES / 2, COLS / 2, "Press enter to continue");
    refresh();
    waitenter();
    endwin();
    for (int i = 0; i < 2; i++) {
        system("color 0F");
        Sleep(8);
        system("color F0");
        Sleep(80);
    }
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
        clear();
        endwin();
        course(profile);
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