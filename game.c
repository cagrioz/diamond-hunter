// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constants
#define SIZE 7
#define ROUND_COUNT 10

// Function Prototypes
void initializeBoards(char [][SIZE], char [][SIZE], int);
void displayUserBoard(char [][SIZE]);
int randomDiamond(void);
void init(void);
int checkDiamonds(int, int, char [][SIZE]);
int provideHints(char [][SIZE], char [][SIZE], int, int);
int getGuessedCoordinates(int *, int *);

// Main Function
int main() {
    int diamondsToHunt, round, score, highestScore = 0;
    char userBoard[SIZE][SIZE], diamondBoard[SIZE][SIZE], neighbourDiamonds, retry = 'Y';

    // Get different random number for each function call
    srand(time(NULL));

    // If user wants to play again restart the game
    while (retry == 'Y' || retry == 'y') {
        // Reset the values to be ready to new game
        round = 1;
        score = 0;

        // Starting text
        printf("*Diamond-Hunter*\nLets get started!\nEnter the number of diamonds to hunt: ");
        scanf("%d", &diamondsToHunt);
        fflush(stdin);

        // Init the boards
        initializeBoards(userBoard, diamondBoard, diamondsToHunt);

        int guessedRow, guessedCol, roundScore = 0, isFound = 0, guessResult;

        // Allow user to play if there is a diamond left
        while (round <= ROUND_COUNT && diamondsToHunt > 0) {
            // Start the round and ask user to enter estimated coordinates
            printf("Round %d:\nEnter the coordinates of the diamonds: ", round);
            guessResult = getGuessedCoordinates(&guessedRow, &guessedCol);

            // Keep ask to user until he enters coordinates that not already guessed
            // and check if it's in our range by guessResult
            while (userBoard[guessedRow - 1][guessedCol - 1] != '?' || !guessResult) {
                // Start the round and ask user to enter estimated coordinates
                if (guessResult) {
                    printf("Still Round %d:\nEnter the coordinates of the diamonds that hasn't been entered yet: ", round);
                } else {
                    printf("Still Round %d:\nEnter the coordinates that in range (between 1-7): ", round);
                }

                guessResult = getGuessedCoordinates(&guessedRow, &guessedCol);
            }

            // Check if user has found the diamond
            isFound = checkDiamonds(guessedRow, guessedCol, diamondBoard);

            // If diamond is found
            if (isFound) {
                // Set asterisk to guessed place
                userBoard[guessedRow - 1][guessedCol - 1] = '*';

                // Set the round score as 100
                roundScore = 100;

                // Decrement the number of diamonds left
                diamondsToHunt--;
            } else {
                // Check the neighbours if they have a diamond and store the total count of diamonds
                provideHints(userBoard, diamondBoard, guessedRow, guessedCol);

                roundScore = 0;
            }

            // Update the total score
            score += roundScore;

            // Display the current and total score
            printf("You got %d points!\n", roundScore);
            printf("Your total points is %d!\n\n", score);

            // Display the user board
            displayUserBoard(userBoard);

            // Check if player did find all diamonds in last round
            if (round == ROUND_COUNT && diamondsToHunt > 0) {
                printf("You run out of rounds! Game over!");
            }

            // Save the total score in last round if score is greater than highestScore
            if (round == ROUND_COUNT && highestScore < score) {
                highestScore = score;
            }

            // Go the the next round
            round++;
        }

        // If player has found all the diamonds display a win message
        if (diamondsToHunt == 0) {
            printf("You found all the diamonds, congrats!");
        }

        // Ask player if he wants to play again
        printf("\nWould you like to play again (Y/N)? ");
        scanf("%c", &retry);
        fflush(stdin);
    }

    // Once player decides to stop playing, display the highest score he had
    if (retry == 'N' || retry == 'n') {
        printf("----------");
        printf("\nYour highest score is %d\n", highestScore);
        printf("----------");
    }

    return 0;
}

/**
 * Ask user to enter his guess
 * 
 * @param int row 
 *   To edit row vairable in main.
 * @param int col 
 *   To edit row vairable in main.
 * 
 * @return int
 *   If row and column is in range of 1-7 return 1 else return 0.
*/
int getGuessedCoordinates(int *row, int *col) {
    // Start the round and ask user to enter estimated coordinates
    scanf("(%d,%d)", row, col);
    fflush(stdin);

    printf("\n");

    // If row or col is not in our range return 0
    if ( (*row > SIZE || *col > SIZE) || (*col < 1 || *row < 1) ) {
        return 0;
    }

    return 1;
}

/**
 * Provide hints to player if there is a diamond near.
 * 
 * @param char userBoard[] 
 *   2D array (canvas) user board that displayed.
 * @param char diamondBoard[] 
 *   2D array (canvas) diamond board that holds location of diamonds (hidden from the user).
 * @param int row 
 *   The guessed row
 * @param int col 
 *   The guessed col.
 * 
 * @return int
 *   ASCII value of count of neighbours that have diamonds.
*/
int provideHints(char userBoard[][SIZE], char diamondBoard[][SIZE], int row, int col) {
    int neighbourDiamonds = 0;
    char diamondCount;
    
    // Change row & column to actually indexes
    row -= 1;
    col -= 1;

    // Check the neighbours
    for (int i = -1; i <= 1; i += 2) {

        // Check if neighbour that 1 column after and before has a diamond
        if (col + i <= 6 && col + i >= 0) {
            if (diamondBoard[row][col + i] == '*') {
                neighbourDiamonds++;
            }

        }
        
        // Check if neighbour that 1 row after and before has a diamond
        if (row + i <= 6 && row + i >= 0) {
            if (diamondBoard[row + i][col] == '*') {
                neighbourDiamonds++;
            }
        }

        // Check the cross neighbours
        for (int j = -1; j <=1; j += 2) {
            if ((row + i <= 6 && row + i >= 0) && (col + j <= 6 && col + j >= 0)) {
                // diaBoard[row + 1][col - 1] diaBoard[row - 1][col + 1] diaBoard[row - 1][col - 1] diaBoard[row + 1][col + 1]
                if (diamondBoard[row + i][col + j] == '*') {
                    neighbourDiamonds++;
                }
            }
        }

    }

    // Convert actual decimals to ASCII code of numbers
    // Ex. 1 + 48 = '1'  --  2 + 48 = '2'
    diamondCount = (neighbourDiamonds + 48);

    // Insert the count of diamonds found at neighbours to the user board
    userBoard[row][col] = diamondCount;

    // If diamonds found are greater than 0 then return 1 otherwise return 0
    return (neighbourDiamonds) ? 1 : 0;
}

/**
 * To check If there is a diamond in guessed location.
 * 
 * @param int row 
 *   The guessed row.
 * @param int col 
 *   The guessed column.
 * @param char diamondBoard[] 
 *   Check if there is a diamond on guessed row and column (diamondBoard[row][col]).
 * 
 * @return int
 *   If there is a diamond return 1 else return 0.
*/
int checkDiamonds(int row, int col, char diamondBoard[][SIZE]) {
    return (diamondBoard[row - 1][col - 1] == '*') ? 1 : 0;
}

/**
 * Display the user board in CLI
 * 
 * @param char userBoard[] 
 *   2D array that holds updated data of user board.
*/
void displayUserBoard(char userBoard[][SIZE]) {
    printf("   UserBoard\n");
    
    // Horizontal enumeration
    for (int i = 0; i < SIZE; i++) {
        if (i == 0) {
            printf("  ");
        }
        printf("%d ", (i + 1));
    }
    printf("\n");

    // Display the user board
    for (int i = 0; i < SIZE; i++) {
        // Verical enumeration
        printf("%d ", (i + 1));
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", userBoard[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Initialize the diamond board and user board
 * 
 * @see displayUserBoard();
 * @see randomDiamond();
 * 
 * @param char userBoard[] 
 *   Init the user board array with default value that placed in the main function.
 * @param char diamondBoard[] 
 *   Place the diamonds randomly.
 * @param int diamondsToHunt
 *   The value of diamonds that going to be hunted during the game. This variable has been specified in the main by user. It's used in this function to place correct amount of diamonds.
*/
void initializeBoards(char userBoard[][SIZE], char diamondBoard[][SIZE], int diamondsToHunt) {

    // Set all elements inside the both board as '?'
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            userBoard[i][j] = diamondBoard[i][j] = '?';
        }
    }

    // Place the diamonds randomly
    int col, row;
    while (diamondsToHunt != 0) {
        // Get random col and row number
        row = randomDiamond();
        col = randomDiamond();

        // Check if diamond is already exist in that random coordinate
        while (diamondBoard[row][col] == '*') {
            // Get random col and row number
            row = randomDiamond();
            col = randomDiamond();
        }
        
        // Place the diamond
        diamondBoard[row][col] = '*';

        diamondsToHunt--;
    }
    printf("\n");
    
    // Display the user board
    displayUserBoard(userBoard);

    // Display the user board
    for (int i = 0; i < SIZE; i++) {
        // Verical enumeration
        printf("%d ", (i + 1));
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", diamondBoard[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Get a random value between 0-6
 * 
 * @see displayUserBoard()
 * 
 * @return int
 *   Random value between 0-6 to place the diamonds randomly
*/
int randomDiamond() {
    return rand() % 7;
}