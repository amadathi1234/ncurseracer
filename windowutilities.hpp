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
#ifndef WINDOWUTILITIES_HPP_
#define WINDOWUTILITIES_HPP_
#endif

extern int winrows;
extern int wincolumns;

void wprintcenter(WINDOW* window, std::string chars, int passedRow);
void wprintcenter(WINDOW* window, std::string chars);
WINDOW* createWindow(int wrows, int wcolumns);
void delete_win(WINDOW* window);
void wcolorprintcenter(WINDOW* window, std::string chars, int correctIndex, int incorrectIndex);
void clearline(WINDOW* window, int row, const int COLUMN_PADDING);
void wcolorprint(WINDOW* window, std::string chars, int row, int column, int pairNo);
