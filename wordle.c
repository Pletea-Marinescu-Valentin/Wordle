#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define WORD_LENGTH 5
#define MAX_GUESSES 6

void initializeNcurses();
void enableColor();
void print_previous_guesses(char guess[MAX_GUESSES][WORD_LENGTH + 1], char word[WORD_LENGTH], int num_guesses);
void print_current_guess(char guess[MAX_GUESSES][WORD_LENGTH + 1], char word[WORD_LENGTH], int num_guesses, int num_remaining_guesses);
void get_next_guess(char guess[MAX_GUESSES][WORD_LENGTH + 1], int num_guesses);
int check_guess(char guess[MAX_GUESSES][WORD_LENGTH + 1], char word[WORD_LENGTH], int num_guesses);

int main()
{
    initializeNcurses();
    enableColor();

    // Initialize random seed
    srand(time(NULL));

    // Initialize word list
    const char *word_list[] = {"apple", "house", "actor", "stone", "lemon", "grape", "melon", "peach", "pearl", "horse"};
    int num_words = sizeof(word_list) / sizeof(char *);

    // Choose a random word
    char word[WORD_LENGTH + 1];
    int word_index = rand() % num_words;
    strncpy(word, word_list[word_index], WORD_LENGTH);
    word[WORD_LENGTH] = '\0';

    // Initialize guess array
    char guess[MAX_GUESSES][WORD_LENGTH + 1];
    for (int i = 0; i < MAX_GUESSES; i++)
    {
        for (int j = 0; j < WORD_LENGTH + 1; j++)
        {
            guess[i][j] = '\0';
        }
    }

    // Initialize game state
    int num_remaining_guesses = MAX_GUESSES;
    int num_guesses = 0;
    int num_correct = 0;

    // Main game loop
    while (num_remaining_guesses > 0 && num_correct < WORD_LENGTH)
    {
        clear();
        print_previous_guesses(guess, word, num_guesses);
        print_current_guess(guess, word, num_guesses, num_remaining_guesses);
        get_next_guess(guess, num_guesses);
        num_correct = check_guess(guess, word, num_guesses);
    
        num_remaining_guesses--;
        num_guesses++;
        if (num_correct == WORD_LENGTH)
        {
            printw("\nCongratulations, you guessed the word '%s' in %d attempts!\n", word, num_guesses);
            break;
        }
    }
    if (num_remaining_guesses == 0)
    {
        printw("\nSorry, you ran out of guesses. The word was \"%s\".\n", word);
    }
    refresh();
    // Wait for user input before exiting
    getch();
    endwin();

    return 0;
}

// initialize ncurses
void initializeNcurses()
{
    initscr();
    cbreak();
    noecho();
}

// enable colors
void enableColor()
{
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);  // for correct letters in correct position
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // for correct letters in wrong position
}

// Function to print previous guesses with colored letters
void print_previous_guesses(char guess[MAX_GUESSES][WORD_LENGTH + 1], char word[WORD_LENGTH], int num_guesses)
{
    printw("PREVIOUS GUESSES:\n");
    for (int i = 0; i <= num_guesses; i++)
    {
        for (int j = 0; j < WORD_LENGTH; j++)
        {
            if (guess[i][j] == word[j])
            {
                attron(COLOR_PAIR(1));
                printw("%c ", guess[i][j]);
                attroff(COLOR_PAIR(1));
            }
            else if (strchr(word, guess[i][j]))
            {
                attron(COLOR_PAIR(2));
                printw("%c ", guess[i][j]);
                attroff(COLOR_PAIR(2));
            }
            else
            {
                printw("%c ", guess[i][j]);
            }
        }
        printw("\n");
    }
    printw("\n");
}

// Function to print the current guess and word
void print_current_guess(char guess[MAX_GUESSES][WORD_LENGTH + 1], char word[WORD_LENGTH], int num_guesses, int num_remaining_guesses)
{
    printw("Guess the word (you have %d attempts):\n", num_remaining_guesses);
    for (int i = 0; i < WORD_LENGTH; i++)
    {
        if (guess[num_guesses][i] == word[i])
        {
            attron(COLOR_PAIR(1));
            printw("%c ", guess[num_guesses][i]);
            attroff(COLOR_PAIR(1));
        }
        else if (strchr(word, guess[num_guesses][i]))
        {
            attron(COLOR_PAIR(2));
            printw("%c ", guess[num_guesses][i]);
            attroff(COLOR_PAIR(2));
        }
        else
        {
            printw("%c ", guess[num_guesses][i]);
        }
    }
    printw("\n");
}

// Function to get the next guess from the user
void get_next_guess(char guess[MAX_GUESSES][WORD_LENGTH + 1], int num_guesses)
{
    int ch;
    int i = 0;
    for (i = 0; i < WORD_LENGTH; i++)
    {
        guess[num_guesses][i] = '\0';
    }
    i = 0;
    while ((ch = getch()) != '\n' && i < WORD_LENGTH)
    {
        if (ch == KEY_BACKSPACE || ch == 127)
        { // handle backspace key
            if (i > 0)
            {
                i--;
                guess[num_guesses][i] = '\0';
                printw("\b \b"); // erase the character on the screen
            }
        }
        else if (isalpha(ch))
        { // handle alphabetic characters
            guess[num_guesses][i] = (char)ch;
            printw("%c", ch); // print the input character
            i++;
        }
    }
}

// Function to check the guess
int check_guess(char guess[MAX_GUESSES][WORD_LENGTH + 1], char word[WORD_LENGTH], int num_guesses)
{
    int num_correct = 0;
    for (int i = 0; i < WORD_LENGTH; i++)
    {
        if (guess[num_guesses][i] == word[i])
        {
            num_correct++;
        }
    }
    return num_correct;
}