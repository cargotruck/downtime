EX_GUI_NAME=./src/Gui_main.cpp
EX_CMD_NAME=./src/Cmd_main.cpp
CC=g++
CFLAGS= -w -Wall -std=c++11 
INC1=./src/Main_win.cpp ./src/Address_book_win.cpp ./src/Log_win.cpp ./src/Preferences_win.cpp
INC2=./src/mailer.cpp ./src/Ping.cpp ./src/Node.cpp ./src/Utilities.cpp ./src/Log.cpp
INC3=-I /usr/local/include/Poco
LIB1=$(shell wx-config --libs)
LIB2=-L /usr/local/lib
LIBS=-lPocoNet -lPocoFoundation -lPocoUtil
LINKWX_IMG=$(shell wx-config --cxxflags)

all : downtime downtime_gui

downtime_gui: $(EX_GUI_NAME)	
	$(CC) $(CFLAGS) $(INC1) $(INC2) $(LINKWX_IMG) $(EX_GUI_NAME) -o $@ $(LIB1) $(LIB2) $(LIBS)

downtime: $(EX_CMD_NAME)
	$(CC) $(CFLAGS) $(INC2) $(LINKWX_IMG) $(EX_CMD_NAME) -o $@ $(LIB1) $(LIB2) $(LIBS)

.PHONY: clean
clean:
	rm -f downtime_gui
	rm -f downtime
