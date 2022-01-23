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
	sudo cp -f $(BINNAME) /usr/local/bin/
	sudo chmod 755 /usr/local/bin/$(BINNAME)
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
	sudo rm /usr/local/bin/$(BINNAME)
