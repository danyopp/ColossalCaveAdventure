This is my personal version of Colossal Cave Adventure, a game developed by Will Crowther in the 1970s. It consist of 2 c files, both which need to be compiled. They have been tested for functionality using GNU Compiler Collection (GCC). To compile and run, use the following code:

    gcc -o yoppd.buildrooms yoppd.buildrooms.c
    ./yoppd.buildrooms
    gcc -o yoppd.adventure yoppd.adventure.c -lpthread
    ./yoppd.adventure

**buildrooms.c:**
This program creates a set of 7 random rooms for use in the adventure game. It must be run before the adventure game can be run. Each room connects to at least three other rooms and a random start and end location are selected. All of this data is written to files that will be read by adventure.c

**adventure.c:**
adventure.c is a multithreaded program that allows you to play the game on one thread and query the current time on another thread. The program will use the most recent files created by buildrooms, so to create a new layout in the game, just rerun *buildrooms*. 

To Navigate the game, you will be given a choice of which room you can enter based upon which room you are in. There are seven rooms total. Each room has a minimum of three connections and a maximum of six connections. The user is prompted to enter the room they would like to go to next. The goal is to find the exit room as fast as possible. Room names are case sensitive. 

It at any time during game play, you enter "time" instead of a room name, the program will output the current time. 

When you find the final room, the game will alert you that you have found the exit. It will give you the path you took and the number of rooms you went through. 