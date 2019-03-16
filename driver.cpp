#include <sys/ioctl.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>

void wprintcenter(WINDOW* window, std::string chars, int passedRow);
void wprintcenter(WINDOW* window, std::string chars);
WINDOW* createWindow(int wrows, int wcolumns);
void delete_win(WINDOW* window);
void wcolorprintcenter(WINDOW* window, std::string chars, int correctIndex, int incorrectIndex);
double gameHandler(WINDOW* window, std::string &chars);
void clearline(WINDOW* window, int row, const int COLUMN_PADDING);
void wcolorprint(WINDOW* window, std::string chars, int row, int column, int pairNo);
 
int winrows = 0;
int wincolumns = 0;

int main() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    winrows = w.ws_row;
    wincolumns = w.ws_col;
    initscr();
    curs_set(0); 
    noecho();
    WINDOW* window = createWindow(20, 50);
    wprintcenter(window, "NCURSERACER");
    wprintcenter(window, "A game by Siddarth Pillai and Anand Madathil", 12);
    wprintcenter(window, "PRESS  RETURN", 15);
    int x = 0;
    while(x != 10)
        x = wgetch(window);
    delete_win(window);
    WINDOW* gameWindow = createWindow(30, 100);
    std::string lel = "";
    for(int i = 0; i < 500; i++) {
        lel += (i < 251 ? "a" : "b");
    }
    lel += "end";
    double wpm = gameHandler(gameWindow, lel);
    endwin();
    std::cout << "Your Words per Minute: " << wpm << std::endl;
    return 0;

}

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
    start_color();
    getmaxyx(window, rows, columns); // get rows and columns of printing window
    // text scroller
    for(int j = incorrectIndex - (ROWS_BEFORE_SCROLL - 1) * (columns - COLUMN_PADDING * 2); 
        (j / (columns - COLUMN_PADDING * 2)) > 0; j -= columns - COLUMN_PADDING * 2) {
        incorrectIndex -= columns - COLUMN_PADDING * 2;
        incorrectIndex = incorrectIndex < 0 ? 0 : incorrectIndex;
        correctIndex -= columns - COLUMN_PADDING * 2; 
        correctIndex = correctIndex < 0 ? 0 : correctIndex;
        chars.erase(0, columns - COLUMN_PADDING * 2); // trim already typed info
    }
    
    while(displayRow < MAX_TEXT_ROW) {
        std::string cur = "";
        if(chars.size() > columns - COLUMN_PADDING) {
            cur = chars.substr(0, columns - COLUMN_PADDING * 2); 
            chars.erase(0, columns - COLUMN_PADDING * 2);
        } else if(displayRow < MAX_TEXT_ROW) { 
            cur = chars;
            chars.clear(); // charstream complete
        }
        clearline(window, displayRow + TEXT_TOP_PADDING, COLUMN_PADDING);
        // green section
        init_pair(1, COLOR_WHITE, COLOR_GREEN);
        wcolorprint(window, cur.substr(0, correctIndex), TEXT_TOP_PADDING + displayRow, COLUMN_PADDING, 1);
        init_pair(2, COLOR_WHITE, COLOR_RED);
        wcolorprint(window, cur.substr(correctIndex > (int)cur.size() ? (int)cur.size() : correctIndex, incorrectIndex), 
            TEXT_TOP_PADDING + displayRow, COLUMN_PADDING + correctIndex, 2);
        init_pair(3, COLOR_BLACK, COLOR_WHITE);
        std::string endsection = cur.substr(incorrectIndex > (int)cur.size() ? (int)cur.size() : incorrectIndex, (int)cur.size());
        while(cur.size() > 0 && (int)endsection.size() < columns - COLUMN_PADDING * 2 - incorrectIndex) {
            endsection += " ";
        }
        wcolorprint(window, endsection,
            TEXT_TOP_PADDING + displayRow, COLUMN_PADDING + incorrectIndex, 3);
        displayRow++;
        correctIndex -= columns - COLUMN_PADDING;
        correctIndex = correctIndex > 0 ? correctIndex : 0;
        incorrectIndex -= columns - COLUMN_PADDING;
        incorrectIndex = incorrectIndex > 0 ? incorrectIndex : 0;
        // get the substring till the end of the string and convert to char*
    }
}

void wcolorprint(WINDOW* window, std::string chars, int row, int column, int pairNo) {
    wattron(window, COLOR_PAIR(pairNo));
    mvwprintw(window, row, column, chars.c_str());
    wattroff(window, COLOR_PAIR(pairNo));
}

double gameHandler(WINDOW* window, std::string &chars) { 
    // INITIALIZE YOUR VARIABLES PLEASE
    double wpmCount = 0.0;
    const int CLOCK_SPEED = 5; // in milliseconds
    int currentIndex = 0, incorrectBegin = 0, msSinceStart = 0, begin = 0;
    nodelay(window, true);
    use_default_colors();

    while(currentIndex < (int)chars.size()) {
        wcolorprintcenter(window, chars, currentIndex, incorrectBegin);
        if(begin) msSinceStart += CLOCK_SPEED;
        if(msSinceStart > 0 && begin) {
            wpmCount =  (float)currentIndex / (5 * (float)msSinceStart / 60000);
            std::string WPM = "Words Per Minute: " + std::to_string(wpmCount);
            wprintcenter(window, WPM.substr(0, 25), 20);
        }
        init_pair(6, -1, -1);
        bkgd(COLOR_PAIR(6));
        redrawwin(stdscr);
        int nextchar = wgetch(window);
        if(begin == 0 && nextchar == ERR) continue;
        else begin = 1;
        if(incorrectBegin <= currentIndex && 
            nextchar == (int)chars[currentIndex]) incorrectBegin = ++currentIndex;
        else if(nextchar == 127 && (currentIndex > 0 || incorrectBegin > 0)) { // ASCII 127= backspace
            if(incorrectBegin > currentIndex) incorrectBegin--;
            else incorrectBegin = --currentIndex;
        } else if(nextchar != 127 && nextchar != ERR && incorrectBegin < (int)chars.size()) {
            if(incorrectBegin < currentIndex) incorrectBegin = currentIndex + 1;
            else incorrectBegin++;
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(CLOCK_SPEED));
    }
    return wpmCount;
}
