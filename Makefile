cflags = -lncurses
execname = a.out
objects = driver.cpp ncurseracer.hpp WordParser.cpp 
$(execname) : $(objects)
	g++ -o $@ $(objects) $(cflags)
