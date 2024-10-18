#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // For _kbhit() and _getch() on Windows

#define WIDTH 20
#define HEIGHT 10

// Define the map using an array
char map[HEIGHT][WIDTH] = {
    "###################",
    "#.......#.........#",
    "#.#####.#.#######.#",
    "#.#.....#.......#.#",
    "#.#.#######.#####.#",
    "#.#.......#.......#",
    "#.#########.#######",
    "#...........#.....#",
    "#############..P..#",
    "###################"
};

// Define the directions for ghost movement (simple queue approach)
typedef struct Queue {
    int x;
    int y;
    struct Queue *next;
} Queue;

// Insert into queue
Queue* enqueue(Queue *tail, int x, int y) {
    Queue *newNode = (Queue*)malloc(sizeof(Queue));
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;

    if (tail != NULL)
        tail->next = newNode;

    return newNode;
}

// Remove from queue
Queue* dequeue(Queue *head) {
    Queue *temp = head;
    if (head != NULL) {
        head = head->next;
        free(temp);
    }
    return head;
}

// Global variables for the game state
int pacmanX = 8, pacmanY = 17; // Starting position of Pac-Man
int ghostX = 1, ghostY = 1;     // Starting position of the ghost
int score = 0;

// Print the game map
void printMap() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == pacmanX && j == pacmanY)
                printf("P");  // Pac-Man
            else if (i == ghostX && j == ghostY)
                printf("G");  // Ghost
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
    if (direction == 'w') newX--;
    else if (direction == 's') newX++;
    else if (direction == 'a') newY--;
    else if (direction == 'd') newY++;

    // Check if new position is a wall
    if (map[newX][newY] != '#') {
        pacmanX = newX;
        pacmanY = newY;

        // Check for dots to collect
        if (map[pacmanX][pacmanY] == '.') {
            score++;
            map[pacmanX][pacmanY] = ' ';  // Clear the dot
        }
    }
}

// Ghost movement (simple queue-based pathfinding)
void moveGhost() {
    Queue *head = NULL, *tail = NULL;

    // Enqueue ghost's current position
    tail = enqueue(tail, ghostX, ghostY);
    if (head == NULL)
        head = tail;

    // Move the ghost one step towards Pac-Man (BFS or simple logic)
    if (ghostX < pacmanX && map[ghostX + 1][ghostY] != '#')
        ghostX++;
    else if (ghostX > pacmanX && map[ghostX - 1][ghostY] != '#')
        ghostX--;
    else if (ghostY < pacmanY && map[ghostX][ghostY + 1] != '#')
        ghostY++;
    else if (ghostY > pacmanY && map[ghostX][ghostY - 1] != '#')
        ghostY--;

    // Dequeue (freeing memory of ghost's previous position)
    head = dequeue(head);
}

// Main game loop
int main() {
    char input;
    int gameOver = 0;

    printf("Welcome to Pac-Man!\n");
    printf("Use WASD to move. Press 'q' to quit.\n");

    // Main game loop
    while (!gameOver) {
        // Print the map
        system("cls");  // Clear screen (Windows). For Linux use "clear".
        printMap();

        // Get user input for Pac-Man movement
        if (_kbhit()) {
            input = _getch();
            if (input == 'q') break;
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
        _sleep(200);
    }

    return 0;
}
