#include <sys/ioctl.h>
#include <ncurses.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>
#include <utility>
#include "ncurseracer.hpp"

void wprintcenter(WINDOW* window, std::string chars, int passedRow);
void wprintcenter(WINDOW* window, std::string chars);
WINDOW* createWindow(int wrows, int wcolumns);
void delete_win(WINDOW* window);
void wcolorprintcenter(WINDOW* window, std::string chars, int correctIndex, int incorrectIndex);
std::pair<double, float> gameHandler(WINDOW* window, std::string &chars);
void clearline(WINDOW* window, int row, const int COLUMN_PADDING);
void wcolorprint(WINDOW* window, std::string chars, int row, int column, int pairNo);
int difficultyHandler(WINDOW* window);
int postGameHandler(WINDOW* window, float wpm, float accuracy);
int mainGame();

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
    wprintcenter(window, "A game by Anand Madathil", 12);
    wprintcenter(window, "PRESS  RETURN", 15);
    int x = 0;
    while(x != 10)
        x = wgetch(window);
    delete_win(window); 
    while(!mainGame()) {};
    endwin();
    std::cout << "Thank you so much for playing Ncurseracer!" << std::endl;
    return 0;
}

int mainGame() {
    start_color();
    use_default_colors();
    WINDOW* difficultyWindow = createWindow(30, 70);
    int diff = difficultyHandler(difficultyWindow);
    WINDOW* gameWindow = createWindow(30, 100);
    std::string patharray[] {"wordsEasy.txt", "wordsMedium.txt", "wordsHard.txt", "wordsHard.txt"};
    WordParser parser(patharray[diff]);
    std::string a = parser.getString(diff == 3 ? 500 : 60);
    std::pair<double, float> myPair = gameHandler(gameWindow, a);
    delete_win(gameWindow);
    WINDOW* postGameWindow = createWindow(20, 70);
    return postGameHandler(postGameWindow, myPair.first, myPair.second);
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
    getmaxyx(window, rows, columns); // get rows and columns of printing window
    // text scroller
        
    std::string cur = "";
    std::stringstream streamer;
    streamer << chars;
    std::string nextWord = "";
    int charsExtracted = 0;
    int charSpace = columns - COLUMN_PADDING * 2;
    int curCol = 0;
    int spacing = 0;
    int incor = incorrectIndex;
    while(displayRow < MAX_TEXT_ROW) {
        cur = "";
        while(cur.size() + nextWord.size() < charSpace) {
            cur += (cur.size() ? " " : "") + nextWord;  
            if(!streamer.eof()) {
                streamer >> nextWord;
            }
            else {
                nextWord = ""; 
                break;
            } 
        }
        curCol += charSpace;
        spacing += charSpace - cur.size();
        
        if(nextWord != "" && curCol < incor + spacing + 1) {
            incorrectIndex -= cur.size();
            incorrectIndex = std::max(0, incorrectIndex);
            correctIndex -= cur.size();
            correctIndex = std::max(0, correctIndex);   
            continue;
        }         
        
        clearline(window, displayRow + TEXT_TOP_PADDING, COLUMN_PADDING);
        
        // make space black
        init_pair(10, COLOR_BLACK, COLOR_BLACK);
        std::string whitespace = "";
        for(int i = 0; i < columns - COLUMN_PADDING * 2; i++) {
            whitespace += " ";
        }

        wcolorprint(window, whitespace, TEXT_TOP_PADDING + displayRow, COLUMN_PADDING, 10);
        // green section
        init_pair(1, COLOR_WHITE, COLOR_GREEN);
        int greenSpace = correctIndex > cur.size() ? cur.size() : correctIndex;
        wcolorprint(window, cur.substr(0, greenSpace), TEXT_TOP_PADDING + displayRow, COLUMN_PADDING, 1);
        init_pair(2, COLOR_WHITE, COLOR_RED);
        wcolorprint(window, cur.substr(correctIndex > (int)cur.size() ? (int)cur.size() : correctIndex, incorrectIndex), 
            TEXT_TOP_PADDING + displayRow, COLUMN_PADDING + correctIndex, 2);

        init_pair(3, COLOR_BLACK, COLOR_WHITE);
        std::string endsection = cur.substr(incorrectIndex > (int)cur.size() ? (int)cur.size() : incorrectIndex, (int)cur.size());
        wcolorprint(window, endsection,
            TEXT_TOP_PADDING + displayRow, COLUMN_PADDING + incorrectIndex, 3);
        displayRow++;
        correctIndex -= cur.size(); //columns - COLUMN_PADDING;
        correctIndex = correctIndex > 0 ? correctIndex : 0;
        incorrectIndex -= cur.size();
        incorrectIndex = incorrectIndex > 0 ? incorrectIndex : 0;
        // get the substring till the end of the string and convert to char*
    }
}

void wcolorprint(WINDOW* window, std::string chars, int row, int column, int pairNo) {
    wattron(window, COLOR_PAIR(pairNo));
    mvwprintw(window, row, column, chars.c_str());
    wattroff(window, COLOR_PAIR(pairNo));
}

int difficultyHandler(WINDOW* window) {
    keypad(window, TRUE);
    init_color(COLOR_YELLOW, 1000, 543, 0);
    wprintcenter(window, "Choose your difficulty:", 8);
    wprintcenter(window, "(use the left and right arrows to move the cursor, enter to confirm)", 9);
    wprintcenter(window, "EASY         MEDIUM          HARD", 18);
    wprintcenter(window, "             ~ZEN~               ", 19);
    int x = 0;
    int currentMode = 1;
    while(x != 10) {
        switch(currentMode) {
            case 0: {
                std::string easy = "EASY";
                const char* easydesc = "No capital letters or punctuation. Easy-peasy!";
                init_pair(10, COLOR_WHITE, COLOR_GREEN);
                wcolorprint(window, easy, 18, 18, 10);
                wprintcenter(window, easydesc, 12);
                break;
            }
            case 1: {
                std::string medium = "MEDIUM";
                const char* meddesc = "Capitals are now in the mix though you are still";
                const char* meddesc2 = "spared punctuation. Shouldn't be too hard!";
                init_pair(11, COLOR_WHITE, COLOR_YELLOW);
                wcolorprint(window, medium, 18, 31, 11);
                wprintcenter(window, meddesc, 12);
                wprintcenter(window, meddesc2, 13);
                break;
            }
            case 2: {
                std::string hard = "HARD";
                const char* harddesc = "Capitals and punctuation are all";
                const char* harddesc2 = "fair game at this difficulty. Tread carefully!";
                init_pair(12, COLOR_WHITE, COLOR_RED);
                wcolorprint(window, hard, 18, 47, 12);
                wprintcenter(window, harddesc, 12);
                wprintcenter(window, harddesc2, 13);
                break;
            }
            case 3: {
                std::string zen = "~ZEN~";
                const char* zendesc = "UNLEASH. INFINITE. ZEN.";
                init_pair(20, COLOR_BLUE, COLOR_WHITE);
                wcolorprint(window, zen, 19, 31, 20); 
                wprintcenter(window, zendesc, 12);
            }
            default:
                break;
        }
        x = wgetch(window);
        wprintcenter(window, "EASY         MEDIUM          HARD", 18);
        wprintcenter(window, "             ~ZEN~               ", 19);
        clearline(window, 12, 2);
        clearline(window, 13, 2);
        switch(x) {
            case KEY_LEFT:
                currentMode = currentMode == 0 ? 0 : currentMode - 1;
                break;
            case KEY_RIGHT:
                currentMode = currentMode == 3 ? 3 : currentMode + 1;
                break;
            default:
                break;
        }
    }
    return currentMode;
}

std::pair<double, float> gameHandler(WINDOW* window, std::string &chars) { 
    keypad(window, FALSE);
    // INITIALIZE YOUR VARIABLES PLEASE
    double wpmCount = 0.0;
    float accuracy = 0.0;
    int currentIndex = 0, incorrectBegin = 0, msSinceStart = 0, begin = 0;
    nodelay(window, true);
    // prepare string
    int ROWS;
    int COLUMNS;
    getmaxyx(window, ROWS, COLUMNS);
    COLUMNS -= 4;
    int colNo = 1;
    std::stringstream streamer;
    std::string repChars = chars;
    streamer << chars;
    chars = "";
    std::string next = "";
    int extraSpace = 1;
    while(!streamer.eof()) {
        streamer >> next;
        if(chars.size() + next.size() + extraSpace >= colNo * COLUMNS) {
            extraSpace = COLUMNS * colNo - chars.size(); // add extra space to bring offset back to 0
            colNo++;
        } else if(chars.size()) chars += " ";
        chars += next;
    }
    wprintcenter(window, "Press 'ESC' to quit into the post-game menu", 25);    
    time_t startMS = 0;
    float secondsSinceStart = 0;     
    int keystrokes = 0;
    int inaccurateKeys = 0;
    while(currentIndex < (int)chars.size()) {
   
        wcolorprintcenter(window, repChars, currentIndex, incorrectBegin);
        if(keystrokes) {
            accuracy = (float)(keystrokes - inaccurateKeys) * 100/keystrokes;
            wprintcenter(window, "Accuracy: " + 
                    std::to_string(accuracy).substr(0, 5) + "%%", 21);
        }
        if(begin) secondsSinceStart = difftime(time(NULL), startMS);
        if(begin) {
            float minutes = secondsSinceStart / 60;
            if(minutes > 0) wpmCount = (float)currentIndex / (5 * minutes);
            std::string WPM = "Words Per Minute: " + std::to_string(wpmCount).substr(0, 5);
            wprintcenter(window, WPM, 20);
        }
        init_pair(6, -1, -1);
        bkgd(COLOR_PAIR(6));
        redrawwin(stdscr);
        int nextchar = wgetch(window);
        if(begin == 0 && nextchar == ERR) { 
            startMS = time(NULL);            
            continue;
        }
        else if(nextchar != 27) {
            begin = 1;
        } else {
            nextchar = wgetch(window);
            if(nextchar == ERR) // this prevents the arrow keys from triggering an exit
                return {wpmCount, accuracy};
        }
        
        if(nextchar != ERR && nextchar != 127) {
            keystrokes++;    
        }
        if(nextchar != ERR && nextchar != 127 && (incorrectBegin > currentIndex || nextchar != (int)chars[currentIndex])) {
            inaccurateKeys++;
        }
        
        if(incorrectBegin <= currentIndex && 
            nextchar == (int)chars[currentIndex]) incorrectBegin = ++currentIndex;
        else if(nextchar == 127 && (currentIndex > 0 || incorrectBegin > 0)) { // ASCII 127= backspace
            if(incorrectBegin > currentIndex) incorrectBegin--;
            else incorrectBegin = --currentIndex;
        } else if(nextchar != 127 && nextchar != ERR && incorrectBegin < (int)chars.size()) {
            if(incorrectBegin < currentIndex) incorrectBegin = currentIndex + 1;
            else incorrectBegin++;
        }
    }
    std::pair<double, float> val = {wpmCount, accuracy}; 
    return val;
}

int postGameHandler(WINDOW* window, float wpm, float accuracy) {
    keypad(window, TRUE);
    std::string WPMindic = "Your words per minute: " + std::to_string(wpm).substr(0,5);
    wprintcenter(window, WPMindic, 5);
    wprintcenter(window, "Your keystroke accuracy: " + std::to_string(accuracy).substr(0,5) + "%%", 6);
    std::string title = "Would you like to go back to the difficulty menu?";
    wprintcenter(window, title, 10);
    wprintcenter(window, "(use the left and right arrow keys to move and enter to confirm)", 11);
    wprintcenter(window, "Yes, take me back!                   No, I want to stop playing!", 17);
    init_pair(12, COLOR_WHITE, COLOR_GREEN);
    wcolorprint(window,"Yes, take me back!", 17, 3,12);
    int x = 0;
    int curOpt = 0;
    const int COLUMN_PADDING = 2;
    while(x != 10) {
        x = wgetch(window);
        switch(x) {
            case KEY_LEFT:
                curOpt = 0;
                clearline(window, 17, COLUMN_PADDING);
                wprintcenter(window, "Yes, take me back!                   No, I want to stop playing!", 17);
                wcolorprint(window,"Yes, take me back!", 17, 3,12);
                break;
            case KEY_RIGHT:
                curOpt = 1;
                init_pair(13, COLOR_WHITE, COLOR_RED);
                clearline(window, 17, COLUMN_PADDING);
                wprintcenter(window, "Yes, take me back!                   No, I want to stop playing!", 17);
                wcolorprint(window, "No, I want to stop playing!", 17, 40, 13);
                break;
            default:
                break;        
        }
    }
    delete_win(window);
    return curOpt;
}
