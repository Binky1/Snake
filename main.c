#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#define WIDTH 100
#define HEIGHT 25
#define MAXSNAKE 10


enum Direction {Up = 0, Down = 1, Left = 2, Right = 3};



typedef struct{
    COORD xy;
}Snake;
typedef struct{
    COORD xy;
}Apple;


void CreateApple(Apple* apple){
    int xpos = rand() % WIDTH;
    int ypos = rand() % HEIGHT;

    COORD c = {xpos, ypos};
    apple->xy = c;
}

void DisplaySettings(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SMALL_RECT windowSize = {0, 0, 30, WIDTH};

    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

void drawscreen(Snake* s, Apple* a, int index){
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    
    for (int i = 0; i < index + 1; i++)
    {
        
        SetConsoleCursorPosition(hStdout, s[i].xy);
        printf("%s", "f");
        
    }

    // SetConsoleCursorPosition(hStdout, s[0].xy);
    // printf("%s", "f");
    SetConsoleCursorPosition(hStdout, a -> xy);
    printf("%s", "a");
}

COORD* CopyPositions(Snake* s, int index){
    COORD* arr = malloc((index - 1) * sizeof(COORD));
    for (int i = 0; i < index; i++)
    {
        arr[i] = s[i].xy;
    }
    return arr;
    
}

void IncreaseMovement(Snake* s, enum Direction* d, int index){
        COORD* positions = CopyPositions(s, index);
        for (int i = 1; i <= index; i++)
        {
            s[i].xy = positions[i - 1];
        }
        free(positions);
        

        if (*d == Up){
        
            if (s[0].xy.Y <= 0)
            {
                *d = Down;
            }
            else{
                s[0].xy.Y -= 1;
            }   
        
        
        }
        else if(*d == Down){
            if (s[0].xy.Y >= HEIGHT)
            {
                *d = Up;
            }
            else{
                s[0].xy.Y +=1;
            }   
        }
        else if(*d == Left){
            if (s[0].xy.X <= 0)
            {
                *d = Right;
            }
            else{
                s[0].xy.X -=1;
            }  
        }
        else if (*d == Right)
        {
            if (s[0].xy.X >= WIDTH)
            {
                *d = Left;
            }
            else{
                s[0].xy.X +=1;
            }   
        }
    }
    
    
    

void Eat(Snake* s, Apple* apple, int *index){
    if(s[0].xy.X == apple->xy.X && s[0].xy.Y == apple->xy.Y){
        CreateApple(apple);
        if ((*index) < MAXSNAKE){
            (*index)++;
            Snake temp = {s[*(index) - 1].xy};
            s[(*index) - 1] = temp;
        }
        
    }
    
}



void main(){

    srand(time(NULL));
    Apple apple;
    CreateApple(&apple);
    DisplaySettings();

    //Variables
    enum Direction d;
    d = Right;

    // Initialize the async keyboard
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD fdwMode, fdwOldMode;

    GetConsoleMode(hStdin, &fdwOldMode);
    fdwMode = ENABLE_WINDOW_INPUT;
    SetConsoleMode(hStdin, fdwMode);
    INPUT_RECORD irInBuf;
    DWORD cNumRead;



    COORD c = {5, 10};
    Snake s = {c};
    Snake *snake = malloc(MAXSNAKE * sizeof(Snake));
    // printf("%p", snake[3]);
    snake[0] = s;
    int index = 0;

    while(1){

        if(PeekConsoleInput(hStdin, &irInBuf, 1, &cNumRead) && cNumRead != 0){
            ReadConsoleInput(hStdin, &irInBuf, 1, &cNumRead);

            if (irInBuf.EventType == KEY_EVENT && irInBuf.Event.KeyEvent.bKeyDown)
            {
                switch (irInBuf.Event.KeyEvent.wVirtualKeyCode){
                    case VK_UP:
                        d = Up;
                        break;
                    case VK_RIGHT:
                        d = Right;
                        break;
                    case VK_LEFT:
                        d = Left;
                        break;
                    case VK_DOWN:
                        d = Down;
                        break;
                }
            }
            
        }
        Eat(snake, &apple, &index);
        IncreaseMovement(snake, &d, index);
        drawscreen(snake, &apple, index);
        Sleep(50);
        system("cls");
        
    }
    free(snake);
    // while(run){

    //     drawscreen(&s);
    // }
}