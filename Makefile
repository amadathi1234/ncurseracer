cflags = -lncurses
execname = a.out
objects = driver.cpp
$(execname) : $(objects)
	g++ -o $@ $(objects) $(cflags)
