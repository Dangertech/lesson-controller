IDIR=.
CC=g++
CFLAGS=
CONFLOC=~/.config/lesson-controller
# Default data directory is just the config
# dir for lesson-controller
DATALOC=$(CONFLOC)
BINNAME=lesson

SHELL=/bin/bash # Don't change that!

# For steady recompilation of the binary
default: main.cpp args.cpp lesson.cpp timecalc.cpp
	$(CC) -o $(BINNAME) main.cpp timecalc.cpp lesson.cpp args.cpp -I $(IDIR) $(CFLAGS)

# For a random user wanting to install the program
install: main.cpp args.cpp lesson.cpp timecalc.cpp
	$(CC) -o $(BINNAME) main.cpp timecalc.cpp lesson.cpp args.cpp -I $(IDIR) $(CFLAGS)
	mkdir $(CONFLOC)
	# Here should be the lesson --create commands later on
	# to create default configuration locations
	touch $(CONFLOC)/config.conf
	touch $(DATALOC)/timeframes.dat
	touch $(DATALOC)/lessondata.dat

# For resetting all files
reset: 
	# Resetting the configuration files
	- rm -r $(CONFLOC)
	- rm -r $(DATALOC)
	mkdir $(CONFLOC)
	# Replace with lesson --create-config
	touch $(CONFLOC)/config.conf
	 
	if [ ! -d $(DATALOC) ]; then \
		mkdir $(DATALOC); \
	fi
	# Replace with lesson --create-timeframes and --create-lessondata
	touch $(DATALOC)/timeframes.dat 
	touch $(DATALOC)/lessondata.dat
