/*
Software Systems Homework 1
Noam Rubin
1/28/14
*/
#include <stdio.h>
#include <stdlib.h>

/*
Prompts the user for a card name, truncated to two characters. returns the card name.
*/
char card_name_reader(void)
{
    char card_name[3];
    puts("Enter the card name: ");
    scanf("%2s", card_name);
    return *card_name;
}
/*
Takes a card name and returns the card value as an int
A return value of 0 indicates that an error occured in parsing the card number.

*card_name: pointer to the card name
*/
int get_card_val(char *card_name)
{
    int val = 0;
    switch (card_name[0])
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
count: the current card count in the game
*/
void update_card_count(int val, int count)
{
    if ((val > 2) && (val < 7))
    {
        count++;
    }
    else if (val == 10)
    {
        count--;
    }
}
/*
Runs the main game loop
*/
void main_loop(void)
{
    int val, count, STOP;
    val = count = STOP = 0;
    char[3] card_name;
    while (!STOP)
    {
        card_name = card_name_reader();
        if (card_name[0] == 'X')
        {
            printf("Game Over! The final count is: %i\n", count);
            break;
        }
        val = get_card_val(card_name);
        if (val != 0)
        {
            update_card_count(val, count)
            printf("The current count is: %i\n", count);
        }
    }
}

int main()
{
    main_loop();
    return 0;
}


// int main()
// {
//     char card_name[3];
//     int count = 0;
//     while ( card_name[0] != 'X')
//     {
//         puts("Enter the card_name: ");
//         scanf("%2s", card_name);
//         int val = 0;
//         switch (card_name[0])
//         {
//         case 'K':
//         case 'Q':
//         case 'J':
//             val = 10;
//             break;
//         case 'A':
//             val = 11;
//             break;
//         case 'X':
//             continue;
//         default:
//             val = atoi(card_name);
//             if ((val < 1) || (val > 10))
//             {
//                 puts("I don't understand that value!");
//                 continue;
//             }
//         }
//         if ((val > 2) && (val < 7))
//         {
//             count++;
//         }
//         else if (val == 10)
//         {
//             count--;
//         }
//         printf("Current count: %i\n", count);
//     }
//     printf("final count: %i\n", count);
//     return 0;
// }
