/*
Software Systems Homework 1
Noam Rubin
1/30/14
*/
#include <stdio.h>
#include <stdlib.h>

/*
Modifer function (for the sake of scope)
Takes a pointer to a three-character array and populates the array with values from STDIN
*/
void card_name_reader(char *card_name)
{
    puts("Enter the card name: ");
    scanf("%2s", card_name);
}
/*
Takes a card name and returns the card value as an int
A return value of 0 indicates that an error occured in parsing the card number.

*card_name: pointer to the card name
*/
int get_card_val(char *card_name)
{
    int val = 0;
    switch (*card_name)
    {
    case 'K':
    case 'Q':
    case 'J':
        val = 10;
        break;
    case 'A':
        val = 11;
        break;
    default:
        val = atoi(card_name);
        if ((val < 1) || (val > 10))
        {
            puts("I don't understand that value!");
            val = 0;
            break;
        }
    }
    return val;
}

/*
Adjusts the current card count

val: the value of the card
*count: pointer to the current card count in the game
*/
void update_card_count(int val, int* count)
{
    if ((val > 2) && (val < 7))
    {
        *count = *count + 1;
    }
    else if (val == 10)
    {
        *count = *count - 1;
    }
}
/*
Runs the main game loop
*/
void loop(void)
{
    int val = 0;
    int count = 0;
    int STOP = 0;
    char card_name[3];
    while (!STOP)
    {
        card_name_reader(card_name);
        if (card_name[0] == 'X')
        {
            printf("Game Over! The final count is: %i\n", count);
            break;
        }
        val = get_card_val(card_name);
        if (val != 0)
        {
            update_card_count(val, &count);
            printf("The current count is: %i\n", count);
        }
    }
}

int main()
{
    loop();
    return 0;
}