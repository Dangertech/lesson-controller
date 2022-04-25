IDIR=.
CC=g++
CFLAGS=
CONFLOC=~/.config/lesson-controller
# Default data directory is just the config
# dir for lesson-controller
DATALOC=$(CONFLOC)
BINNAME=lesson

SRC=main.cpp args.cpp lesson.cpp timecalc.cpp
OBJ=main.o args.o lesson.o timecalc.o

SHELL=/bin/bash # Don't change that!

# For steady recompilation of the binary
default: compile link

compile: $(SRC)
	$(CC) -c $(SRC) $(CFLAGS)

link:  $(OBJ)
	$(CC) -o $(BINNAME) $(OBJ) $(CFLAGS)

# For a random user wanting to install the program
install: $(BINNAME)
	cp -f $(BINNAME) /usr/local/bin/
	chmod 755 /usr/local/bin/$(BINNAME)
	mkdir -p $(CONFLOC)
	cp -f ./default/config.def.conf $(CONFLOC)/config.conf
	cp -f ./default/timeframes.def.dat $(DATALOC)/timeframes.dat
	cp -f ./default/lessondata.def.dat $(DATALOC)/lessondata.dat

# For resetting all files
reset: 
	# Resetting the configuration files
	- rm -r $(CONFLOC)
	- rm -r $(DATALOC)
	mkdir $(CONFLOC)
	 
	cp -f ./default/config.def.conf $(CONFLOC)/config.conf
	 
	 
	if [ ! -d $(DATALOC) ]; then \
		mkdir $(DATALOC); \
	fi
	cp -f ./default/timeframes.def.dat $(DATALOC)/timeframes.dat
	cp -f ./default/lessondata.def.dat $(DATALOC)/lessondata.dat
	 

uninstall:
	rm /usr/local/bin/$(BINNAME)

clean:
	rm $(OBJ) $(BINNAME)
