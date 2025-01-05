#include<iostream>
#include<conio.h>
#include<windows.h>
#include<time.h>
using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;
bool gameover;
const int width = 20;
const int height = 20;
int headX, headY, fruitX, fruitY, score;
int tailX[100], tailY[100];
int tail_len;
int gameSpeed;  // Added for speed control

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setup() {
    srand(time(NULL));
    gameover = false;
    dir = STOP;
    headX = width / 2;
    headY = height / 2;
    fruitX = rand() % (width - 2) + 1;
    fruitY = rand() % (height - 2) + 1;
    score = 0;
    tail_len = 0;
}

void draw() {
    gotoxy(0,0);  // Instead of system("cls")
    
    // Draw top border
    for(int i = 0; i < width + 2; i++)
        cout << "|";
    cout << endl;

    // Draw sides and game area
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(j == 0)
                cout << "|";
                
            if(i == headY && j == headX)
                cout << "O";
            else if(i == fruitY && j == fruitX)
                cout << "*";
            else {
                bool print = false;
                for(int k = 0; k < tail_len; k++) {
                    if(tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        print = true;
                    }
                }
                if(!print)
                    cout << " ";
            }
            
            if(j == width - 1)
                cout << "|";
        }
        cout << endl;
    }

    // Draw bottom border
    for(int i = 0; i < width + 2; i++)
        cout << "|";
    cout << endl;

    // Display score
    cout << "\nScore: " << score << endl;
}

void input() {
    if(_kbhit()) {
        switch(_getch()) {
            case 'a':
            case 'A':
                if(dir != RIGHT) dir = LEFT;
                break;
            case 'd':
            case 'D':
                if(dir != LEFT) dir = RIGHT;
                break;
            case 'w':
            case 'W':
                if(dir != DOWN) dir = UP;
                break;
            case 's':
            case 'S':
                if(dir != UP) dir = DOWN;
                break;
            case 'x':
            case 'X':
                gameover = true;
                break;
        }
    }
}

void logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = headX;
    tailY[0] = headY;
    
    for(int i = 1; i < tail_len; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch(dir) {
        case LEFT:
            headX--;
            break;
        case RIGHT:
            headX++;
            break;
        case UP:
            headY--;
            break;
        case DOWN:
            headY++;
            break;
    }

    // Wall collision with wrap-around
    if(headX >= width) headX = 0;
    else if(headX < 0) headX = width - 1;
    if(headY >= height) headY = 0;
    else if(headY < 0) headY = height - 1;

    // Tail collision
    for(int i = 0; i < tail_len; i++) {
        if(tailX[i] == headX && tailY[i] == headY)
            gameover = true;
    }

    // Fruit collision
    if(headX == fruitX && headY == fruitY) {
        score += 10;
        fruitX = rand() % (width - 2) + 1;
        fruitY = rand() % (height - 2) + 1;
        tail_len++;
        
        // Increase speed with score
        gameSpeed = max(30, gameSpeed - 2);
    }
}

int main() {
    SetConsoleTitle("Snake Game");
    
    // Hide cursor
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
    
    // Start screen
    system("cls");
    cout << "\n\n\t=========="  << endl;
    cout << "\tSnake Game" << endl;
    cout << "\t=========="  << endl;
    cout << "\n\tControls:\n";
    cout << "\tW - Up\n";
    cout << "\tS - Down\n";
    cout << "\tA - Left\n";
    cout << "\tD - Right\n";
    cout << "\tX - Exit\n\n";
    
    // Difficulty selection
    cout << "\tSelect Difficulty:\n";
    cout << "\t1. Easy (Slower)\n";
    cout << "\t2. Medium\n";
    cout << "\t3. Hard (Faster)\n";
    cout << "\tEnter choice (1-3): ";
    
    int difficulty;
    cin >> difficulty;
    
    switch(difficulty) {
        case 1: gameSpeed = 70; break;  // Easy
        case 2: gameSpeed = 45; break;  // Medium
        case 3: gameSpeed = 20; break;  // Hard
        default: gameSpeed = 45;        // Default to Medium
    }
    
    system("cls");
    cout << "\n\tPress any key to start...";
    _getch();
    system("cls");
    setup();
    
    while(!gameover) {
        draw();
        input();
        logic();
        Sleep(gameSpeed);
    }
    
    system("cls");
    cout << "\n\n\tGame Over!\n";
    cout << "\tFinal Score: " << score << endl;
    cout << "\tPress any key to exit...\n";
    _getch();
    return 0;
}