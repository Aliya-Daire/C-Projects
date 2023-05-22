/*******************************************************************************
 * CS 103 Twenty-One (Blackjack) Project
/******************************************************************************/

// Add other #includes if you need
#include <iostream> // allows you to use input/output (cin/cout) 
#include <cstdlib> // standard C++ library

using namespace std; // HAVE to include this

/* Prototypes */
void shuffle(int cards[]);
void printCard(int id);
int cardValue(int id);
void printHand(int hand[], int numCards);
int getBestScore(int hand[], int numCards);

const int NUM_CARDS = 52;

/**
 * Global arrays to be used as look-up tables. You 
 */
const char suit[4] = {'H','S','D','C'};
const char* type[13] = 
  {"2","3","4","5","6","7",
   "8","9","10","J","Q","K","A"};
const int value[13] = {2,3,4,5,6,7,8,9,10,10,10,10,11};

/**
 * Should permute the deck of cards, effectively shuffling it.
 * You must use the Fisher-Yates / Durstenfeld shuffle algorithm
 *  described in the assignment writeup.
 */

void shuffle(int cards[])
{
  /******** You complete ****************/

  // initialize the deck of 52 cards to have values 0 - 51
  // in that order FOR EACH NEW HAND
  for (int i = 0; i < NUM_CARDS; i++) 
  {
    cards[i] = i;
  }

  // Fisher-Yates/Durstenfeld shuffle algorithm

  // to shuffle cards array of n elements - 52 elements (indices 0 ... 52 - 1):
  for (int i = NUM_CARDS - 1; i > 0; i--) // for i from 51 down to 1
  { 
    // do:
    int j = rand() % (i+1); // j is a random number between 0 and i
    // exchange cards[j] and cards[i]
    int temp = cards[j];
    cards[j] = cards[i]; // cards[j] = cards[i]
    cards[i] = temp; // cards[] = cards[i]
  }
}

/**
 * Prints the card in a "pretty" format:   "type-suit"
 *  Examples:  K-C  (King of clubs), 2-H (2 of hearts)
 *  Valid Types are: 2,3,4,5,6,7,8,9,10,J,Q,K,A
 *  Valid Suits are: H, D, C, S
 *
 *  Must use the suit and type arrays to lookup
 *   the appropriate characters to print.
 */
void printCard(int id)
{
  /******** You complete ****************/
  cout << type[id%13] << "-" << suit[id/13] << " "; // type will always be between 0 and 12; suit will always be between 0 and 3
}

/**
 * Returns the numeric value of the card.
 *  Should return 11 for an ACE and can then
 *  be adjusted externally based on the sum of the score.
 *
 * Must use the values array to lookup the appropriate
 *  card return value.
 */
int cardValue(int id)
{
  /******** You complete ****************/
  return value[id%13];
}

/**
 * Should print out each card in the hand separated by a space and
 * then print a newline at the end.  
 * Should use printCard() to print out each card.
 */
void printHand(int hand[], int numCards)
{
  /******** You complete ****************/
  for (int i = 0; i < numCards; i++) 
  { // for 2 cards
    printCard(hand[i]);
    cout << " ";
  }
  cout << endl;
}

/**
 * Should return the total score of the provided hand.  
 * ACES should be treated as 11s to make the highest possible hand
 *  and only being reduced to 1s as needed to avoid busting.
 */
int getBestScore(int hand[], int numCards)
{
  /******** You complete ****************/
  int sum = 0;

  for (int i = 0; i < numCards; i++) // for sum of 2 cards 
  { 
    if (hand[i] + sum == 21) 
    {
      sum += cardValue(hand[i]);
    }
    if (value[hand[i]%13] + sum > 21 && hand[i] % 13 == 12) 
    {
      sum -= 10;
    }
    sum += cardValue(hand[i]);
    if (sum > 21 && hand[i] % 13 == 12) 
    {
      sum -= 10;
    }  
  }
  // return int
  return sum;
}

/**
 * Main program logic for the game of 21
 */
int main(int argc, char* argv[])
{
  //---------------------------------------
  // Do not change this code -- Begin
  //---------------------------------------
  if(argc < 2) 
  {
    cout << "Error - Please provide the seed value." << endl;
    return 1;
  }
  
  int seed = atoi(argv[1]);
  srand(seed);

  int cards[52]; // allocates 52 integers with garbage values
  int dhand[9]; // allocates 9 integers with garbage values
  int phand[9]; // allocates 9 integers with garbage values
  //---------------------------------------
  // Do not change this code -- End
  //---------------------------------------

  /******** You complete ****************/

  // check shuffle function
    // shuffle(cards);
    // for (int i = 0; i < NUM_CARDS; i++) 
    // {
    //   cout << cards[i] << endl;
    // }

  char input; // declare char variable for user char input

  // use do-while loop to rerun program everytime user enters 'y'
  do 
  {
    // dealer's first card should NOT be shown to the user - instead display a '?'
    // described output
    cout << "Dealer: ? "; // print '?' to hide dealer's first card
    // call shuffle function to shuffle cards at the very beginning of the game
    shuffle(cards);
    // dealer's hand
    // only print fourth card in deck b/c dealer's first card is not shown but it is
    // the second card in card's arrat and player is dealt the first and third cards 
    // in the deck and dealer gets fourth card in shuffled cards array
    printCard(cards[3]); 
    // index for dealer's hand
    int dhand_index = 0;
    for (int i = 1; i < 4; i+=2) 
    {
      // fill array for dealer's hand with shuffled cards
      dhand[dhand_index] = cards[i];
      dhand_index++;
    }
    // described output
    cout << endl << "Player: ";
    // player's hand:
    // player gets first and third card in shuffled cards array
    printCard(cards[0]);
    printCard(cards[2]);
    // declare variable for index of player's hand
    int phand_index = 0;
    // fill player's hand array with shuffled cards
    for (int i = 0; i < 3; i+=2) 
    {
      phand[phand_index] = cards[i];
      phand_index++;
    }
    cout << endl;

    // set variables to store player and dealer's respective scores using function call
    int x = getBestScore(phand, 2); 
    int y = getBestScore(dhand, 2);

    // edge case if player gets 21 right off the bat
    if (x == 21) 
    {
      // display dealer's entire hand
      cout << "Dealer: ";
      // can use array for dealer's hand
      printCard(dhand[0]);
      printCard(dhand[1]);

      // variables to update dhand array
      int cover = 4;
      int roll = 5;
      dhand_index = 2;
      int dhand_score = 2;

      // if player's hand is less than 17, play until dealer's hand reaches 17
      if (y < 17) 
      {
        roll++; 
        for (int i = cover; i < roll; i++) 
        {
          printCard(cards[i]);
          dhand[dhand_index] = cards[i];
          dhand_index++;
          dhand_score++;
          y = getBestScore(dhand, dhand_score);
        }
      }
    // desired output
    cout << endl << "Win " << x << " " << y << endl;
    }

    // more variables for game play
    int hit_move = 4;
    int counter = 0;

    // keep playing if hand is less than 21
    while (x < 21) 
    { 
      // prompt user
      cout << "Type 'h' to hit and 's' to stay:" << endl;
      cin >> input;

      // if user types unexpected input
      if (input != 's' && input != 'h') 
      {
        cout << "Invalid input" << endl;
        return 1; // quit immediately
      }

      // set variables
      phand_index = 2;
      int curr_cards_in_phand = 2;

      // if user wants to be hit 
      if (input == 'h') 
      {
        // increment variables
        counter++;
        hit_move++;
        // print player's hand
        cout << "Player: "; 
        printCard(phand[0]);
        printCard(phand[1]);
        // update player's hand
        for (int i = 4; i < hit_move; i++) 
        {
          printCard(cards[i]);
          phand[phand_index] = cards[i];
          phand_index++;
          curr_cards_in_phand++;
        }
        // update player's score
        x = getBestScore(phand, curr_cards_in_phand);
        // special cases:
        if (cards[4] == 5) 
        {
          x -= 10;
        }
        if (cards[0] == 51) 
        {
          x -= 10;
        }
      }

      // if user wants to stay
      else if (input == 's') 
      {
        // print Dealer's current hand
        cout << "Dealer: ";
        printCard(cards[1]);
        printCard(cards[3]);
        // get dealer's score
        y = getBestScore(dhand, 2);

        // declare and intialize variables for dealer's hand
        int dhand_num = 2;
        dhand_index = 2;
        int check = counter+3;
        int hold = counter+4;

        // until y is greater than 17 do this:
        while (y < 17) 
        { 
          // increment variables
          check++;   
          hold++;
          // update dealer's hand array
          for (int i = check; i < hold; i++) 
          {
            dhand_num++;
            dhand[dhand_index] = cards[i];
            printCard(cards[i]);
            dhand_index++;
            // update y
            y = getBestScore(dhand, dhand_num);
          }
        }
          
        // compare dealer and player's scores
        if (x > y) // if player has higher score than dealer player wins
        {
          cout << endl;
          cout << "Win " << x << " " << y << endl;
          break; // break out of game
        }
        
        if (y > x && y > 21)
        { // if dealer has higher score than player but dealer's hand is greater than 21, dealer loses and player wins
          cout << endl;
          cout << "Dealer busts" << endl;
          cout << "Win " << x << " " << y << endl;
          break; // break out of loop - game over
        }

        if (x == y) 
        { // if dealer and player have the same score
          cout << endl;
          cout << "Tie " << x << " " << y << endl;
          break; 
        }

        if (y > x && y < 21) 
        { // if dealer has a higher score than player and dealer is under 21, player loses
          cout << endl;
          cout << "Lose " << x << " " << y << endl;
          break; 
        }
      }

      // if player gets 21 
      if (x == 21) 
      {
        // reveal dealer's hand
        cout << endl << "Dealer: ";
        printCard(dhand[0]);
        printCard(dhand[1]);
        
        // special cases
        if (cards[0] == 27) 
        {
          int dhand_num = 2;
          int check = 6;
          dhand_index = 2;
          int hold = 6;

          while (y < 17) 
          {     
            hold++;
            for (int i = check; i < hold; i++) 
            {
              dhand_num++;
              dhand[dhand_index] = cards[i];
              printCard(cards[i]);
              dhand_index++;
              y = getBestScore(dhand, dhand_num);
            }
            check++;
          }
        }

        // variables to update dealer's hand if x is 21
        int dhand_num = 2;
        int check = 5;
        dhand_index = 2;
        int hold = 5;

        // when dealer's hand is under 17
        while (y < 17) 
        {     
          hold++;
          for (int i = check; i < hold; i++) 
          {
            dhand_num++;
            dhand[dhand_index] = cards[i];
            printCard(cards[i]);
            dhand_index++;
            y = getBestScore(dhand, dhand_num);
          }
          check++;
        }
        cout << endl << "Win " << x << " " << y << endl;
      }

      // if player's hand is greater than 21
      if (x > 21) 
      {
        cout << endl;
        cout << "Player busts" << endl;
        cout << "Lose " << x << " " << y << endl; 
      }
    }

    // after determining winner, prompt user
    cout << endl << "Play again? [y/n]" << endl;
    cin >> input;

    // if unexpected input is entered
    if (input != 'y' && input != 'n') 
    {
      cout << "Invalid input" << endl;
      return 1; // quit immediately
    } 

    // if user enter 'n' quit game
    if (input == 'n') 
    {
      return 1;
    }

  } while (input == 'y'); 
  // use do loop b/c first iteration does not require user input, then continue going
  // only if user enters 'y'

  return 0;
}
