#include <sys/ioctl.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>

void wprintcenter(WINDOW* window, const char* chars, int passedRow);
void wprintcenter(WINDOW* window, const char* chars);
WINDOW* createWindow(int wrows, int wcolumns);
void delete_win(WINDOW* window);
void wcolorprintcenter(WINDOW* window, std::string chars, int correctIndex, int incorrectIndex);
double gameHandler(WINDOW* window, std::string &chars);

int winrows = 0;
int wincolumns = 0;

int main() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    winrows = w.ws_row;
    wincolumns = w.ws_col;
    initscr();
    noecho();
    WINDOW* window = createWindow(20, 50);
    wprintcenter(window, "NCURSERACER");
    wprintcenter(window, "PRESS  RETURN", 15);
    int x = 0;
    while(x != 10)
        x = wgetch(window);
    delete_win(window);
    WINDOW* body = createWindow(30, 100);
    std::string lel =  "The quick brown fox jumps over the lazy dog";
    double wpm = gameHandler(body, lel);
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

void wprintcenter(WINDOW* window, const char* chars, int passedRow) {
    int rows, columns;
    getmaxyx(window, rows, columns); // get rows and columns
    mvwprintw(window, passedRow, (columns - strlen(chars)) / 2, chars); // print at center
    wrefresh(window);
}

void wprintcenter(WINDOW* window, const char* chars) {
    int rows, columns;
    getmaxyx(window, rows, columns); // get rows and columns
    wprintcenter(window, chars, rows / 2);
    wrefresh(window);
}

void wcolorprintcenter(WINDOW* window, std::string chars, int correctIndex, int incorrectIndex) {
    int rows, columns;
    getmaxyx(window, rows, columns); // get rows and columns of printing
    start_color();
    
    // green section
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, rows / 2, (columns - (int)chars.size()) / 2, 
        chars.substr(0, correctIndex).c_str()); 
    // get the substring till the end of correct chars and convert to char*
    wattroff(window, COLOR_PAIR(1));
    init_pair(2, COLOR_WHITE, COLOR_RED);
    wattron(window, COLOR_PAIR(2));
    mvwprintw(window, rows / 2, (columns - (int)chars.size()) / 2 + correctIndex, 
        chars.substr(correctIndex, incorrectIndex).c_str());
    // get the substring till the end of incorrect chars and convert to char*
    wattroff(window, COLOR_PAIR(2));
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    wattron(window, COLOR_PAIR(3)); 
    mvwprintw(window, rows / 2, (columns - (int)chars.size()) / 2 + incorrectIndex, 
        chars.substr(incorrectIndex, (int)chars.size()).c_str());
    wattroff(window, COLOR_PAIR(3));
    // get the substring till the end of the string and convert to char*
    wrefresh(window);
}

double gameHandler(WINDOW* window, std::string &chars) { 
    // INITIALIZE YOUR VARIABLES PLEASE
    double wpmCount = 0.0;
    const int CLOCK_SPEED = 5;
    int currentIndex = 0, incorrectBegin = 0, msSinceStart = 0, begin = 0;
    nodelay(window, true);
    while(currentIndex < (int)chars.size()) {
        wcolorprintcenter(window, chars, currentIndex, incorrectBegin);
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
        
        msSinceStart += CLOCK_SPEED;
        if(msSinceStart > 0) {
            wpmCount =  (float)currentIndex / (5 * (float)msSinceStart / 60000);
            std::string WPM = "Words Per Minute: " + std::to_string(wpmCount);
            wprintcenter(window, WPM.c_str(), 20);
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(CLOCK_SPEED));
    }
    return wpmCount;
}
