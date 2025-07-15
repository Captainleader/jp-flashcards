# jp-flashcards
Flashcard app to learn Japanese using SDL2 and C++

Japanese Flashcard App (SDL2 + C++)
Hi, I'm very new to coding in c++ so any feedback is appreciated.

A lightweight flashcard application built using C++, SDL2, and SDL_ttf, designed to help users learn Japanese through romaji-to-hiragana conversion and spaced repetition.

Features:
- Type answers in romaji and convert to hiragana
- Spaced repetition logic with levels and timed reviews
- Shows stats for new, learning, and mastered cards
- Loads card data from a .txt file
- SDL2-based UI with keyboard input and rendering
- TTF font rendering and optional image background

Requirements:
- SDL2
- SDL2_ttf
- SDL2_image
- SDL2_gfx
- C++17 or newer

All required DLL files are located in the "dlls/" folder.

How to Run:

1. Windows:
   Run the program using the provided batch file:
   run.bat

   This sets up the DLL path and launches the program.

2. Manual Execution:
   If you want to run the .exe directly, make sure to:
   - Use SetDllDirectory("dlls") in your code before initializing SDL
   - Or place the DLLs next to the .exe (less clean)


Build Instructions (MSYS2 / MinGW):

g++ main.cpp -o my_program.exe -I/mingw64/include/SDL2 -L/mingw64/lib ^
-lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_gfx

Make sure to link all the libraries you use.

Card Format:

Each line in card_data.tsv should follow this format:
  id tab question tab answer tab description tab level tab next_review_time

Example:
(30	人	にん,ひと	person; someone; somebody	2	2025:07:15:11:27:22)

Notes:
- Customize appearance using SDL rectangles and fonts
- Input is typed in romaji and converted to hiragana

License:
MIT – You are free to use, modify, and distribute this code.
