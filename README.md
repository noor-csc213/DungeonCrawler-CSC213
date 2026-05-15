================================================================
 
  DUNGEON CRAWLER — OOP Final Project
  
  Course    : object oriented progaming
  
  Instructor: Huda Sarfraz
  
  Student   : [Noor fatima(0712)]
  
================================================================

PURPOSE OF FILES
----------------

DungeonCrawler.cpp
  This is the main and only source file for the project.
  It contains the complete implementation of the Dungeon Crawler
  text-based game written in C++.

  OOP Concepts Demonstrated:
  - Inheritance   : DungeonCrawler class inherits from the Game
                    base class and overrides the pure virtual
                    function play().
    
  - Polymorphism  : In main(), a Game* base pointer is used to
                    call DungeonCrawler::play() at runtime.
    
  - Composition   : Room and Player classes are composed inside
                    DungeonCrawler, demonstrating has-a relation.
    
  - Encapsulation : All data members are private. Public methods
                    provide controlled access.
    
  - File I/O      : Leaderboard class reads and writes scores
                    to dungeon_scores.txt using fstream.

HOW TO COMPILE AND RUN
-----------------------
  Requirements: Any C++ compiler (g++, MinGW, MSVC)

  Command (Windows / Linux / Mac):
    g++ -o DungeonCrawler DungeonCrawler.cpp -std=c++11
    ./DungeonCrawler        (Linux/Mac)
    DungeonCrawler.exe      (Windows)

THE STORY
  -----------
  You are a hero trapped inside a deadly dungeon.
  Treasure glitters in hidden rooms, but deadly traps lurk at every turn.
  Your only goal: reach the EXIT and escape with your life!

 OBJECTIVE
  -----------
  > Start at [S] in the top-left corner.

  > Navigate the 6 x 6 dungeon grid room by room.

  > Collect gold from Treasure rooms [$].

  > Avoid Trap rooms [X]  they drain your HP badly.

  > Reach EXIT [E] at the bottom-right corner to WIN.

HOW TO PLAY
-----------
  W = Move North (Up)
  
  S = Move South (Down)
  
  A = Move West  (Left)
  
  D = Move East  (Right)
  
  L = View Leaderboard
  
  Q = Quit and save score
  

  MAP LEGEND
  ------------
  [@]   your current position.
  
  [?]  Unknown room, enter to reveal it.
  
  [S]  Starting room.
  
  [E]  EXIT, reach this to WIN! Always bottom-right.
  
  [$]  Treasure room, collect gold coins.
  
  [X]  Trap room, lose HP on entry.
  
  [ ]  Empty room, safe.
 
TIPS
  ------
  > All rooms are hidden until you physically step inside.

  > The EXIT is always at the very last row and column.

  > Collect as much gold as possible before reaching the exit.

  > Red HP bar means danger, one more trap could kill you!

  > Press [L] any time to view the leaderboard.


================================================================
