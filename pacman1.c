#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For sleep (on Linux/Unix)

#ifdef _WIN32
#include <conio.h>   // For _getch(), only for Windows
#include <windows.h> // For Sleep on Windows
#else
#include <fcntl.h>   // For non-blocking input (_kbhit)
#include <termios.h> // For _getch() on Linux/Unix
#endif

#define WIDTH 20
#define HEIGHT 10

// Define the map using an array
char map[HEIGHT][WIDTH] = {"###################", "#.......#.........#",
                           "#.#####.#.#######.#", "#.#.....#.......#.#",
                           "#.#.#######.#####.#", "#.#.......#.......#",
                           "#.#########.# ## ##", "#...........#.....#",
                           "#############..P..#", "###################"};

// Global variables for the game state
int pacmanX = 8, pacmanY = 17; // Starting position of Pac-Man
int ghostX = 1, ghostY = 1;    // Starting position of the ghost
int score = 0;

// Platform-independent _getch() for non-Windows systems
#ifndef _WIN32
int _getch() {
  struct termios oldt, newt;
  int ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}

// Platform-independent _kbhit() for non-Windows systems
int _kbhit() {
  struct termios oldt, newt;
  int ch;
  int oldf;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}
#endif

// Print the game map
void printMap() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (i == pacmanX && j == pacmanY)
        printf("P"); // Pac-Man
      else if (i == ghostX && j == ghostY)
        printf("G"); // Ghost
      else
        printf("%c", map[i][j]);
    }
    printf("\n");
  }
  printf("Score: %d\n", score);
}

// Pac-Man movement function
void movePacman(char direction) {
  int newX = pacmanX;
  int newY = pacmanY;

  // Update Pac-Man's position based on input
  if (direction == 'w')
    newX--;
  else if (direction == 's')
    newX++;
  else if (direction == 'a')
    newY--;
  else if (direction == 'd')
    newY++;

  // Check if new position is a wall
  if (map[newX][newY] != '#') {
    pacmanX = newX;
    pacmanY = newY;

    // Check for dots to collect
    if (map[pacmanX][pacmanY] == '.') {
      score++;
      map[pacmanX][pacmanY] = ' '; // Clear the dot
    }
  }
}

// Ghost movement (simple logic, moving towards Pac-Man)
void moveGhost() {
  // Move the ghost one step toward Pac-Man
  if (ghostX < pacmanX && map[ghostX + 1][ghostY] != '#')
    ghostX++;
  else if (ghostX > pacmanX && map[ghostX - 1][ghostY] != '#')
    ghostX--;
  else if (ghostY < pacmanY && map[ghostX][ghostY + 1] != '#')
    ghostY++;
  else if (ghostY > pacmanY && map[ghostX][ghostY - 1] != '#')
    ghostY--;
}

// Main game loop
int main() {
  char input;
  int gameOver = 0;

  printf("Welcome to Pac-Man!\n");
  printf("Use WASD to move. Press 'q' to quit.\n");

  // Main game loop
  while (!gameOver) {
    // Clear screen (Windows and Linux)
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Linux/Unix
#endif

    // Print the map
    printMap();

    // Get user input for Pac-Man movement
    if (_kbhit()) {
      input = _getch();
      if (input == 'q')
        break;
      movePacman(input);
    }

    // Move the ghost
    moveGhost();

    // Check if ghost catches Pac-Man
    if (pacmanX == ghostX && pacmanY == ghostY) {
      gameOver = 1;
      printf("Game Over! The ghost caught you.\n");
    }

    // Add a small delay to control game speed
#ifdef _WIN32
    Sleep(200); // Sleep for 200 ms on Windows
#else
    usleep(200000);  // Sleep for 200 ms on Unix/Linux
#endif
  }

  return 0;
}
