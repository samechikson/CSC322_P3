# This is the Makefile I use to compile my program from the command line.
# It's fine to use XCode or Eclipse, but if you want to do a command-line
# compile then you can modify this file.
#
# The first line in each "paragraph" is a make target: after the colon are
# all the files this target depends one.  The second line in each paragraph
# specifies how the target is created.  Issuing "make" at the command line
# makes the "all" target, which makes "play", which compiles everthing that
# needs to be compiled (by looking at when the corresponding source code
# files were last changed).  Saying "make clean" removes all the compiled
# files.  The three different OPTIONS at the start control what sort of
# executable is created.

CC=g++
OPTIONS=-O2 -c             # For verbose mode
#OPTIONS=-O2 -c -Dquiet    # For quiet mode
#OPTIONS=-g -c             # For debug mode

all: play

play: play.o player.o mjmplayer.o shedgame.o crazy8.o switch.o blackjack.o card.o
	$(CC) -o play play.o shedgame.o crazy8.o switch.o blackjack.o player.o mjmplayer.o card.o

play.o: play.cpp mjmplayer.h crazy8.h switch.h blackjack.h player.h shedgame.h card.h
	$(CC) $(OPTIONS) play.cpp

mjmplayer.o: mjmplayer.cpp mjmplayer.h player.h card.h shedgame.h
	$(CC) $(OPTIONS) mjmplayer.cpp

player.o: player.cpp player.h
	$(CC) $(OPTIONS) player.cpp

card.o: card.cpp card.h
	$(CC) $(OPTIONS) card.cpp

shedgame.o: shedgame.cpp shedgame.h player.h card.h
	$(CC) $(OPTIONS) shedgame.cpp

crazy8.o: crazy8.cpp crazy8.h card.h shedgame.h
	$(CC) $(OPTIONS) crazy8.cpp

switch.o: switch.cpp switch.h card.h shedgame.h
	$(CC) $(OPTIONS) switch.cpp

blackjack.o: blackjack.cpp blackjack.h card.h shedgame.h
	$(CC) $(OPTIONS) blackjack.cpp

clean:
	rm -f card.o shedgame.o crazy8.o switch.o blackjack.o mjmplayer.o player.o play.o play
