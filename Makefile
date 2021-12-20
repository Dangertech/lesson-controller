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
	touch $(CONFLOC)/config.conf
	echo "# The lesson-controller configuration file" > $(CONFLOC)/config.conf
	echo " " >> $(CONFLOC)/config.conf
	echo "# The location of the lessondata file" >> $(CONFLOC)/config.conf
	echo "LESSON_FILE_LOC = ~/.config/lesson-controller/lessondata.dat" >> $(CONFLOC)/config.conf
	echo " " >> $(CONFLOC)/config.conf
	echo "# The location of the timeframes file" >> $(CONFLOC)/config.conf
	echo "TIME_FILE_LOC = ~/.config/lesson-controller/timeframes.dat" >> $(CONFLOC)/config.conf
	 
	lesson --create

# For resetting all files
reset: 
	# Resetting the configuration files
	- rm -r $(CONFLOC)
	- rm -r $(DATALOC)
	mkdir $(CONFLOC)
	# Replace with lesson --create-config
	touch $(CONFLOC)/config.conf
	echo "# The lesson-controller configuration file" > $(CONFLOC)/config.conf
	echo " " >> $(CONFLOC)/config.conf
	echo "# The location of the lessondata file" >> $(CONFLOC)/config.conf
	echo "LESSON_FILE_LOC = ~/.config/lesson-controller/lessondata.dat" >> $(CONFLOC)/config.conf
	echo " " >> $(CONFLOC)/config.conf
	echo "# The location of the timeframes file" >> $(CONFLOC)/config.conf
	echo "TIME_FILE_LOC = ~/.config/lesson-controller/timeframes.dat" >> $(CONFLOC)/config.conf
	 
	if [ ! -d $(DATALOC) ]; then \
		mkdir $(DATALOC); \
	fi
	 
	lesson --create
