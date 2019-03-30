#include <sys/ioctl.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>
#include <ctime>


void delete_win(WINDOW* window) {
    wclear(window);
    wrefresh(window);
    delwin(window);
}

WINDOW* createWindow(int wrows, int wcolumns) {
    WINDOW* window = newwin(wrows, wcolumns, (winrows - wrows) / 2, 
        (wincolumns - wcolumns) / 2); // create the window
    box(window, 0, 0); // create the box and border    
    wrefresh(window); // refresh the window
    return window; // return it
}

void wprintcenter(WINDOW* window, std::string chars, int passedRow) {
    int rows, columns;
    getmaxyx(window, rows, columns); // get rows and columns
    if((int)chars.size() >= columns) chars = chars.substr(0, columns);
    init_pair(4, COLOR_WHITE, -1);
    wattron(window, COLOR_PAIR(4));
    mvwprintw(window, passedRow, (columns - (int)chars.size()) / 2, chars.c_str()); // print at center
    wattroff(window, COLOR_PAIR(4));
    wrefresh(window);
}

void wprintcenter(WINDOW* window, std::string chars) {
    int rows, columns;
    getmaxyx(window, rows, columns); // get rows and columns
    wprintcenter(window, chars, rows / 2);
    wrefresh(window);
}

void clearline(WINDOW* window, int row, const int COLUMN_PADDING) {
    int rows = 0, columns = 0;
    getmaxyx(window, rows, columns);
    std::string clearline = "";
    for(int i = 0; i < columns - COLUMN_PADDING; i++) {
        clearline += " ";
    }
    mvwprintw(window, row, (columns - (int)clearline.size()) / 2,
        clearline.c_str());
}

void wcolorprintcenter(WINDOW* window, std::string chars, int correctIndex, int incorrectIndex) {
    int rows = 0, columns = 0;
    int displayRow = 0;
    const int COLUMN_PADDING = 2;
    const int MAX_TEXT_ROW = 7;
    const int TEXT_TOP_PADDING = 1; 
    const int ROWS_BEFORE_SCROLL = 1;
}
