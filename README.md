# Squares - Ludum Dare 34

When bright squares start moving towards you with no sign of stopping, what else is there to do but spin around wildly and attempt to catch them? 

Left arrow key navigates through menus, Right arrow key selects the menu entries, and both spin around the central square. 

Scores are automatically uploaded to an online communal scoreboard. To set your submission name, go into the Settings menu and just type normally. This doesn't persist across program runs, unfortunately it has to be set each time. 

## Running

Downloading and building is simple enough, the only dependency is the SFML library. No Makefile or CMake I'm afraid, I didn't have time! Binaries are also available for Linux x86_64 and Windows x86, because those are what I have handy and can compile and test for. No other platforms have been tested, but anything that has a C++11 compatible compiler and SFML support should work.

For reasons of fairness scoreboard uploads are disabled when building from source; they are only enabled for the official Ludum Dare builds found under Releases. Sorry!

## Technical Info

The main game is in (very badly written) C++11, and the scoreboard server is written in Ruby and deployed to a Heroku instance. There's also a small utility program written in Ruby to convert a `pbm` file representing a font into a series of 4-byte integers, because I was having trouble with TTF font rendering. An added bonus is that it crams an entire font into less than half a kB of space!
