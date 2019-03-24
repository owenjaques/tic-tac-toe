/*
A pretty sweet tic tac toe game made 2018/12/02 by Owen Jaques.

Writing the game to support two players wasn't terribly difficult
but what took forever was writing it so that you could play 
against the computer, and you never won. 

Note: The printscr function uses the extended asii library for
printing the borders
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define X ('x')
#define O ('o')
#define ERR ("Hey someones already put their thing there\n")
#define EMPTY (' ')

void printscr(char screen[3][3]);
void p1(char screen[3][3]);
void p2(char screen[3][3]);
int winCheckP1(char screen[3][3]);
int winCheckP2(char screen[3][3]);
void ai(char screen[3][3], int move);
void ai2(char screen[3][3]);
int blockChk(char screen[3][3]);
int winOp(char screen[3][3]);
void game1(char screen[3][3]);
void game2(char screen[3][3]);
void game3(char screen[3][3]);
void initScr(char screen[3][3]);

void printscr(char screen[3][3]){
    //prints the screen
    printf("%c%c%c%c%c\n%c%c%c%c%c\n%c%c%c%c%c\n%c%c%c%c%c\n%c%c%c%c%c\n", 
    screen[0][0], 186, screen[0][1], 186, screen[0][2], 205, 206, 205, 206, 205, 
    screen[1][0], 186, screen[1][1], 186, screen[1][2], 205, 206, 205, 206, 205, 
    screen[2][0], 186, screen[2][1], 186, screen[2][2], 205, 206, 205, 206, 205);
}

void p1(char screen[3][3]){
    int space, y, x;
    printf("Player 1's Turn\n");
    fflush(stdin);
    scanf("%d", &space);
    space--;//puts space down one number to make easier to compute for arrays
    x = space%3;
    y = (space-x)/3;
    if(screen[y][x] == EMPTY)
        screen[y][x] = O;
    else if(space > 8){
        printf("Please enter a number from 1 to 9\n");//goes off if you enter a number other then 1-9
        printscr(screen);
        p1(screen);
    }
    else{
        printf("%s", ERR);//gies off if you enter a space that is already occupied
        printscr(screen);
        p1(screen);
    } 
}

void p2(char screen[3][3]){
    int space, y, x;
    printf("Player 2's Turn\n");
    fflush(stdin);
    scanf("%d", &space);
    space--;//puts space down one number to make easier to compute for arrays
    x = space%3;
    y = (space-x)/3;
    if(screen[y][x] == EMPTY)
        screen[y][x] = X;
    else if(space > 8){
        printf("Please enter a number from 1 to 9\n");//goes off if you enter a number other then 1-9
        printscr(screen);
        p2(screen);
    }
    else{
        printf("%s", ERR);//gies off if you enter a space that is already occupied
        printscr(screen);
        p2(screen);
    }
}

void ai(char screen[3][3], int move){
    //static int move = 1;//start a move counter. critical when it is determining what to do
    printf("Computer's Turn\n");
    if(move == 1){
        int corner = rand()%4+1;//picks a semi random starting corner
        if(corner == 1){
            screen[0][0] = X;
        }
        else if(corner == 2){
            screen[0][2] = X;
        }
        else if(corner == 3){
            screen[2][0] = X;
        }
        else if(corner == 4){
            screen[2][2] = X;
        }
    }
    else if(move == 2){
        //places another x in an adjacent corner
        if(screen[0][0] == X){
            if(screen[0][2] != O && screen[0][1] != O){
                screen[0][2] = X;
            }
            else{
                screen[2][0] = X;
            }
        }
        else if(screen[0][2] == X){
            if(screen[0][0] != O && screen[0][1] != O){
                screen[0][0] = X;
            }
            else{
                screen[2][2] = X;
            }
        }
        else if(screen[2][0] == X){
            if(screen[0][0] != O && screen[1][0] != O){
                screen[0][0] = X;
            }
            else{
                screen[2][2] = X;
            }
        }
        else if(screen[2][2] == X){
            if(screen[2][0] != O && screen[2][1] != O){
                screen[2][0] = X;
            }
            else{
                screen[0][2] = X;
            }
        }
    }
    else if(move == 3){
        //this is where it starts getting complicated
        //checks for oppurtunities to block the other player and tries to win
        if(winOp(screen) != 1 && blockChk(screen) != 1){
            //places in another corner
            if(screen[0][0] == X && screen[0][2] == X){
                if(screen[2][2] != O && screen[1][2] != O)
                    screen[2][2] = X;
                else 
                    screen[2][0] = X;
            }
            else if(screen[2][2] == X && screen[2][0] == X){
                if(screen[0][0] != O && screen[1][0] != O)
                    screen[0][0] = X;
                else 
                    screen[0][2] = X;
            }
            else if(screen[0][0] == X && screen[2][0] == X){
                if(screen[0][2] != O && screen[0][1] != O)
                    screen[0][2] = X;
                else 
                    screen[2][2] = X;
            }
            else if(screen[0][2] == X && screen[2][2] == X){
                if(screen[0][0] != O && screen[0][1] != O)
                    screen[0][0] = X;
                else 
                    screen[2][0] = X;
            }
        }
    }
    else if(move == 4){
        //hopefully wins
        if(winOp(screen) != 1){
            blockChk(screen);
        }
    }
    else if(move == 5){
        //comes to a draw
        for(int k = 0; k < 3; k++){
            for(int i = 0; i < 3; i++){
                if(screen[k][i] == EMPTY){
                    screen[k][i] = X;
                    break;
                }
            }
        }
    }
    move++;
}

//function is still pretty broken needs a lot of work
void ai2(char screen[3][3]){
    static int move = 1;//start a move counter. critical when it is determining what to do
    static int filmid = 0;//used in turn one to see if the middle was filled by an X or O
    printf("Computer's Turn\n");
    if(move == 1){
        int corner = rand()%4+1;//picks a semi random starting corner
        if(screen[1][1] != O){//tries to fill middle
            screen[1][1] = X;
            filmid = 1;
        }
        else if(corner == 1){
            screen[0][0] = X;
        }
        else if(corner == 2){
            screen[0][2] = X;
        }
        else if(corner == 3){
            screen[2][0] = X;
        }
        else if(corner == 4){
            screen[2][2] = X;
        }
    }
    else if(move == 2){
        if(winOp(screen) != 1 && blockChk(screen) != 1){
            //places another x in an adjacent corner
            if(filmid){
                int corner = rand()%2+1;//picks a semi random place to put an X
                if(corner == 1){
                    if(screen[0][0] != O)
                        screen[0][0] = X;
                    else if(screen[2][0] != O)
                        screen[2][0] = X;
                    else
                        screen[2][2] = X;
                }
                else if(corner == 2){
                    if(screen[0][2] != O)
                        screen[0][2] = X;
                    else if(screen[2][2] != O)
                        screen[2][2] = X;
                    else
                        screen[2][0] = X;
                }
            }
            else if(screen[0][0] == X){
                if(screen[0][2] != O && screen[0][1] != O){
                    screen[0][2] = X;
                }
                else{
                    screen[2][0] = X;
                }
            }
            else if(screen[0][2] == X){
                if(screen[0][0] != O && screen[0][1] != O){
                    screen[0][0] = X;
                }
                else{
                    screen[2][2] = X;
                }
            }
            else if(screen[2][0] == X){
                if(screen[0][0] != O && screen[1][0] != O){
                    screen[0][0] = X;
                }
                else{
                    screen[2][2] = X;
                }
            }
            else if(screen[2][2] == X){
                if(screen[2][0] != O && screen[2][1] != O){
                    screen[2][0] = X;
                }
                else{
                    screen[0][2] = X;
                }
            }
        }
    }
    else if(move == 3){
        if(winOp(screen) != 1 && blockChk(screen) != 1){
            for(int k = 0; k < 3; k++){
                for(int i = 0; i < 3; i++){
                    if(screen[k][i] == EMPTY){
                        screen[k][i] = X;
                        break;
                    }
                }   
            }
        }
    }
    else if(move == 4){
        if(winOp(screen) != 1 && blockChk(screen) != 1){
            for(int k = 0; k < 3; k++){
                for(int i = 0; i < 3; i++){
                    if(screen[k][i] == EMPTY){
                        screen[k][i] = X;
                        break;
                    }
                }   
            }
        }
    }
    else if(move == 5){
        //comes to a draw
        for(int k = 0; k < 3; k++){
            for(int i = 0; i < 3; i++){
                if(screen[k][i] == EMPTY){
                    screen[k][i] = X;
                    break;
                }
            }
        }
    }
    move++;
}

int blockChk(char screen[3][3]){
    //checks for opurtinities to block the other players attempts to win
    //horizontal checks
    for(int k = 0; k < 3; k++){
            if(screen[k][0] == O && screen[k][1] == O){
                screen[k][2] = X;
                return 1;
            }
            else if(screen[k][1] == O && screen[k][2] == O){
                screen[k][0] = X;
                return 1;
            }
            else if(screen[k][0] == O && screen[k][2] == O){
                screen[k][1] = X;
                return 1;
            }
        }
    //vertical checks
    for(int k = 0; k < 3; k++){
        if(screen[0][k] == O && screen[1][k] == O){
            screen[2][k] = X;
            return 1;
        }
        else if(screen[0][k] == O && screen[2][k] == O){
            screen[1][k] = X;
            return 1;
        }
        else if(screen[1][k] == O && screen[2][k] == O){
            screen[0][k] = X;
            return 1;
        }
    }
    //diagonal checks
    if(screen[0][0] == O && screen[1][1] == O){
        screen[2][2] = X;
        return 1;
    }
    else if(screen[0][0] == O && screen[2][2] == O){
        screen[1][1] = X;
        return 1;
    }
    else if(screen[2][2] == O && screen[1][1] == O){
        screen[0][0] = X;
        return 1;
    }
    else if(screen[0][2] == O && screen[1][1] == O){
        screen[2][0] = X;
        return 1;
    }
    else if(screen[0][2] == O && screen[2][0] == O){
        screen[1][1] = X;
        return 1;
    }
    else if(screen[2][0] == O && screen[1][1] == O){
        screen[0][2] = X;
        return 1;
    }
    return 0;
}

int winOp(char screen[3][3]){
    //checks for opurtunities to win
    //horizontal checks
    for(int k = 0; k < 3; k++){
            if(screen[k][0] == X && screen[k][1] == X && screen[k][2] != O){
                screen[k][2] = X;
                return 1;
            }
            else if(screen[k][1] == X && screen[k][2] == X && screen[k][0] != O){
                screen[k][0] = X;
                return 1;
            }
            else if(screen[k][0] == X && screen[k][2] == X && screen[k][1] != O){
                screen[k][1] = X;
                return 1;
            }
        }
    //vertical checks
    for(int k = 0; k < 3; k++){
        if(screen[0][k] == X && screen[1][k] == X && screen[2][k] != O){
            screen[2][k] = X;
            return 1;
        }
        else if(screen[0][k] == X && screen[2][k] == X && screen[1][k] != O){
            screen[1][k] = X;
            return 1;
        }
        else if(screen[1][k] == X && screen[2][k] == X && screen[0][k] != O){
            screen[0][k] = X;
            return 1;
        }
    }
    //diagonal checks
    if(screen[0][0] == X && screen[1][1] == X && screen[2][2] != O){
        screen[2][2] = X;
        return 1;
    }
    else if(screen[0][0] == X && screen[2][2] == X && screen[1][1] != O){
        screen[1][1] = X;
        return 1;
    }
    else if(screen[2][2] == X && screen[1][1] == X && screen[0][0] != O){
        screen[0][0] = X;
        return 1;
    }
    else if(screen[0][2] == X && screen[1][1] == X && screen[2][0] != O){
        screen[2][0] = X;
        return 1;
    }
    else if(screen[0][2] == X && screen[2][0] == X && screen[1][1] != O){
        screen[1][1] = X;
        return 1;
    }
    else if(screen[2][0] == X && screen[1][1] == X && screen[0][2] != O){
        screen[0][2] = X;
        return 1;
    }
    return 0;
}

int winCheckP1(char screen[3][3]){
    //checks if player 1 has won
    if(screen[0][0] == O && screen[0][1] == O && screen[0][2] == O) return 1;//horizontal checks
    else if(screen[1][0] == O && screen[1][1] == O && screen[1][2] == O) return 1;
    else if(screen[2][0] == O && screen[2][1] == O && screen[2][2] == O) return 1;
    else if(screen[0][0] == O && screen[1][0] == O && screen[2][0] == O) return 1;//vertical checks
    else if(screen[0][1] == O && screen[1][1] == O && screen[2][1] == O) return 1;
    else if(screen[0][2] == O && screen[1][2] == O && screen[2][2] == O) return 1;
    else if(screen[0][0] == O && screen[1][1] == O && screen[2][2] == O) return 1;//diagonal checks
    else if(screen[0][2] == O && screen[1][1] == O && screen[2][0] == O) return 1;
    else return 0;
}

int winCheckP2(char screen[3][3]){
    //checks if player 2 has won
    if(screen[0][0] == X && screen[0][1] == X && screen[0][2] == X) return 1;//horizontal checks
    else if(screen[1][0] == X && screen[1][1] == X && screen[1][2] == X) return 1;
    else if(screen[2][0] == X && screen[2][1] == X && screen[2][2] == X) return 1;
    else if(screen[0][0] == X && screen[1][0] == X && screen[2][0] == X) return 1;//vertical checks
    else if(screen[0][1] == X && screen[1][1] == X && screen[2][1] == X) return 1;
    else if(screen[0][2] == X && screen[1][2] == X && screen[2][2] == X) return 1;
    else if(screen[0][0] == X && screen[1][1] == X && screen[2][2] == X) return 1;//diagonal checks
    else if(screen[0][2] == X && screen[1][1] == X && screen[2][0] == X) return 1;
    else return 0;
}

void game1(char screen[3][3]){
    //plays game against the computer
    int winchk;
    int turn = 1;
    while(1){
        ai(screen, turn);
        printscr(screen);
        winchk = winCheckP2(screen);
        if(winchk){
            printf("Beaten by a computer.\n");
            break;
        }
        turn++;
        if(turn > 5){
            printf("Looks like a draw.\n");
            break;
        }
        p1(screen);
        printscr(screen);
        winchk = winCheckP1(screen);
        if(winchk){
            printf("Player wins!\n");
            break;
        }
    }
}

void game2(char screen[3][3]){
    //plays game with two players
    int winchk;
    int turn = 1;
    while(1){
        p1(screen);
        printscr(screen);
        winchk = winCheckP1(screen);
        if(winchk){
            printf("Player 1 wins!\n");
            break;
        }
        turn++;
        if(turn > 5){
            printf("Looks like a draw.\n");
            break;
        }
        p2(screen);
        printscr(screen);
        winchk = winCheckP2(screen);
        if(winchk){
            printf("Player 2 wins!\n");
            break;
        }
    }
}

void game3(char screen[3][3]){
    //an experimental game mode where the user goes first against the ai
    int winchk;
    int turn = 1;
    while(1){
        p1(screen);
        printscr(screen);
        winchk = winCheckP1(screen);
        if(winchk){
            printf("Player 1 wins!");
            break;
        }
        turn++;
        if(turn > 5){
            printf("Looks like a draw.");
            break;
        }
        ai2(screen);
        printscr(screen);
        winchk = winCheckP2(screen);
        if(winchk){
            printf("The computer has won.");
            break;
        }
    }
}

void initScr(char screen[3][3]){
    printf("Enter the number accociated with the spot to place your letter\n");
    char s = '1';
    for(int k = 0; k < 3; k++){
        for(int i = 0; i < 3; i++){
            screen[k][i] = s;
            s++;
        }
    }
    printscr(screen);
    //fills screen with blank spaces
    for(int k = 0; k < 3; k++){
        for(int i = 0; i < 3; i++){
            screen[k][i] = EMPTY;
        }
    }
}

int playAgain(){
    printf("Want to play again?\n");
    char response;
    while(response != 'y' || 'Y' || 'n' || 'N'){
        fflush(stdin);
        response = tolower(getchar());
        switch(response) {
            case 'y':
                return 1;
            case 'n':
                return 0;
            default:
                printf("Enter yes or no\n");
        }
    }
}

int main(){
    srand(time(NULL));
    char screen[3][3];
    //menu
    int input = 0;
    int plays = 1;//related to playing again
    while(plays){
        printf("To play against the computer press 1. For two players press 2.\n");
        fflush(stdin);
        scanf("%d", &input);
        switch(input) {
            case 1: 
                initScr(screen);
                printf("The computer will go first.\n");
                sleep(2);
                game1(screen);
                plays = playAgain();
                break;
            
            case 2:
                initScr(screen);
                game2(screen);
                plays = playAgain();
                break;

            case 3:
                initScr(screen);
                game3(screen);
                break;
        }
    }
    fflush(stdin);
    getchar();
    return 0;
}