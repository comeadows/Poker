/***********************************************************************************************************
*                                                                                                          *
*  File Name: Program3.cpp                                                                                 *
*  Author:  Cassie Meadows                                                                                 *
*  Date:  April 4, 2012                                                                                    *
*                                                                                                          *
*  Purpose:  Allows a player to play 5-card draw poker against a computer with some modified rules.        *
*                                                                                                          *
************************************************************************************************************/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

int menu();
void playGame(int&, int&, int&, char[]);
void dealCards(char [][6], char [][6], bool [][13], char [], char[]);
void printPlayerHand(char [][6]);
void discardCards(char [][6], bool [][13], char [], char []);
void printComputerHand(char [][6]);
void generateNewCard(char [][6], bool [][13], int, char [], char []);
void calculateResults(char [][6], char [][6], int&, int&);

bool straightFlush(char [][6]);
bool fourOfAKind(char [][6]);
bool threeOfAKind(char [][6]);
bool twoOfAKind(char [][6]);
bool allSameSuit(char [][6]);
bool straight(char [][6]);
int highestCard(char [][6]);

int main()
{
	//Variables
	ifstream inputFile;
	ofstream outputFile;
	int userChoice;
	
	char highScoreName[20];   //High scorer's name
	int highScore;            //High score
	int pScore;               //Player's score
	int cScore;               //Computer's score
	
	inputFile.open("highScore.txt");

	if (inputFile.fail()) //Checks to make sure file opens successfully.
	{
		//Creates new file since one does not exist
		outputFile.open("highScore.txt");
		highScore = 0; //sets high score to 0
		
		//Writes the new score information to highScore.txt
		outputFile << "NO HIGH SCORER YET" << endl;
		outputFile << highScore << endl;
		outputFile.close();
		
		//Adds 'NO HIGH SCORER YET' to the highScoreName
		inputFile.open("highScore.txt");
		inputFile.getline(highScoreName, 20);
		inputFile.close();
	}
	else //Reads high scorer from file
	{
		//Assigns variables from file
		inputFile.getline(highScoreName, 20);
		inputFile >> highScore;
		inputFile.close();
	}
	
	do
	{
		//Calls the menu() function and utilizes the user's choice
		switch (userChoice = menu())
		{
			case 1 : //Allows player to play the game
						pScore = 0;
						cScore = 0;
						cout << "********** LET'S PLAY **********" << endl;
						cout << endl;
						playGame(cScore, pScore, highScore, highScoreName);
						break;
			case 2 : //Lets the user display the high score
						cout << endl;
						cout << "********** HIGH SCORE **********" << endl;
						cout << "MASTER:  " << highScoreName  << endl;
						cout << "SCORE :  " << highScore      << endl;
						cout << endl;
						break;
			case 3 : //Allows player to exit the program
						cout << endl;
						cout << "GOODBYE!" << endl; 
						return 0;
						break;
			default:  cout << "Please choose 1, 2, or 3.";
		}
	//Validates user input and brings up the menu again if the player chooses to view high scores and allows player to play game until they decide not to
	} while (userChoice < 1 || userChoice > 3 || userChoice != 3); 
	
	return 0;
}
/***********************************************************************************************************
* Function:  Menu                                                                                          *
* Purpose :  This function displays a menu consisting of 3 choices for the user.                           *
*                                                                                                          *
************************************************************************************************************/
int menu()
{
	int userChoice;
	cout << endl;
	cout << "********** POKER **********" << endl;
	cout << "1.  DEAL NEW GAME          " << endl;
	cout << "2.  VIEW HIGH SCORES       " << endl;
	cout << "3.  END PROGRAM            " << endl;
	cout << "Choose 1, 2, or 3          " << endl;
	cin  >> userChoice;
	
	return userChoice;
}
/***********************************************************************************************************
* Function:  playGame                                                                                      *
* Purpose :  Function that also uses several other functions to start a game of poker for the player.      *
*                                                                                                          *
************************************************************************************************************/
void playGame(int &cS, int &pS, int &hS, char hSN[])
{
	//Creates two arrays to create the standard 52 Deck
	char suit[4] = {'S', 'H', 'D', 'C'};
	char rank[13] = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
	
	//Creats an array for the player and computer's hands
	char playerHand[2][6];
	char computerHand[2][6];

	//Keeps track of the cards removed from the deck
	bool cardTaken[4][13] = {false, false, false, false,
									 false, false, false, false,
									 false, false, false, false,
									 false, false, false, false,
									 false, false, false, false,
									 false, false, false, false,
									 false, false, false, false,
									 false, false, false, false,
									 false, false, false, false,
									 false, false, false, false,
									 false, false, false, false,
									 false, false, false, false,
									 false, false, false, false
									};

	//Calls 'dealCards' to deal cards to the computer and player
	dealCards(playerHand, computerHand, cardTaken, suit, rank);
	//Calls 'discardCards' to give the option to discard cards
	discardCards(playerHand, cardTaken, suit, rank);
	//Calls 'printComputerHand' to print the computer's hand
	printComputerHand(computerHand);
	//Calls 'calculateResults' to evaluate hands and assign points
	calculateResults(playerHand, computerHand, pS, cS);
	
	//Print scores
	cout << endl;
	cout << "***** SCORES *****" << endl;
	cout << "PLAYER:    " << pS  << endl;
	cout << "COMPUTER:  " << cS  << endl;
	cout << endl;
	
	//Prints out response based on performance
	if (pS > cS)                                                              //If the player beats the computer
		cout << "CONGRATULATIONS! YOU BEAT THE COMPUTER!"  << endl;
	else if (pS < cS)                                                         //If the computer beats the player
		cout << "YOU LOST -- THE COMPUTER BEAT YOUR SCORE!" << endl;
	else                                                                      //If the player ties with the computer
		cout << "IT'S A TIE!" << endl;
	
	if (pS > hS)                                                              //Checks to see if player made the high score
	{
		ofstream outputFile;
		hS = pS;
		
		cout << "YOU HAVE THE NEW HIGH SCORE!" << endl;
		cout << "What is your name?"           << endl;
		cin.ignore();
		cin.getline(hSN, 20);
		
		outputFile.open("highScore.txt");
		
		//Writes the new score information to highScore.txt
		outputFile << hSN;
		outputFile << "\r\n";
		outputFile << hS;
		outputFile.close();
	}
	else                                                                      //Lets the player know the high score information if the player did not beat the high score
	{
		cout << "SORRY, YOU DO NOT HAVE THE HIGH SCORE." << endl;
		cout << "THE MASTER POKER PLAYER IS:  " << hSN   << endl;
		cout << "WITH A HIGH SCORE OF:  "       << hS    << endl;
	}
}
/***********************************************************************************************************
* Function:  dealCards                                                                                     *
* Purpose :  This function deals cards for the computer and player.                                        *
*                                                                                                          *
************************************************************************************************************/
void dealCards(char pH[][6], char cH[][6], bool cT[][13], char s[], char r[])
{
	srand(unsigned(time(0)));
	int randomSuit;
	int randomRank;
	int subScript = 0;
	
	//Deals Player's hand
	for (int i = 1; i < 6; i++)                       //Assigns 5 cards to the player
	{
		do
		{
			randomSuit = (rand() % 4);                  //Randomly picks a suit (H, S, C, D) for the card
			randomRank = (rand() % 13);                 //Randomly picks a rank (2, 3, 4 etc) for the card
			
			pH[0][subScript] = s[randomSuit];		     //Assigns suit for card
			pH[1][subScript] = r[randomRank];			  //Assigns rank for card
			
		} while (cT[randomSuit][randomRank] == true);  //Re-randomizes the card if it's been used before
		
		cT[randomSuit][randomRank] = true;             //Adds the card to cardsTaken so it cannot be used again
		subScript++;
	}
	
	//Re-initializes counter
	subScript = 0;
	
	//Deals Computer's hand
	for (int i = 1; i < 6; i++)
	{
		do
		{
			randomSuit = (rand() % 4);                 //Randomly picks a suit (H, S, C, D) for the card
			randomRank = (rand() % 13);                //Randomly picks a rank (1, 2, 3 etc) for the card
			
			cH[0][subScript] = s[randomSuit];          //Assigns suit for card
			cH[1][subScript] = r[randomRank];          //Assigns rank for card
			
		} while (cT[randomSuit][randomRank] == true); //Re-randomizes the card if it's been used before
		
		cT[randomSuit][randomRank] = true;            //Adds the card to cardsTaken so it cannot be used again
		subScript++;
	}
	
	//Calls 'printPlayerHand' to display the cards chosen for the player.  Uses other functions for options.
	printPlayerHand(pH);
}
/***********************************************************************************************************
* Function:  printPlayerHand                                                                               *
* Purpose :  This function display the user's hand and uses other functions to give the player choices in  *
*            order to play a game of poker.                                                                *
*                                                                                                          *
************************************************************************************************************/
void printPlayerHand(char pH[][6])
{
	cout << "***** PLAYER's HAND *****" << endl;
	int cardNumber = 1;
	int subScript = 0;
	
	//Prints out the 5 cards chosen for the player
	for (int i = 1; i < 6; i++)
	{
		//Prints out cards with suit first, rank second
		cout << "CARD " << cardNumber << ":  " << pH[0][subScript] << pH[1][subScript] << endl;
		cardNumber++;
		subScript++;
	} 
}
/***********************************************************************************************************
* Function:  discardCards                                                                                  *
* Purpose :  This function prompts the user to discard cards and draw new ones.                            *
*                                                                                                          *
************************************************************************************************************/
void discardCards(char pH[][6], bool cT[][13], char s[], char r[])
{
	int discardCount = 0;
	char discardChoice;
	int numChoice = 0;
	
	do
	{
		cout << endl;
		cout << "Do you want to discard any of your cards? (Y/N)" << endl;
		cin  >> discardChoice;
		cout << endl;
		
		if (discardChoice == 'y' || discardChoice == 'Y')
		{
			
			//Validates user input
			do
			{
				//Prompts user for what card they'd like to discard
				cout << "Which card do you want to discard? (Enter 1, 2, 3, 4, 5)" << endl;
				cin  >> numChoice;
				cout << endl;
				
				if (numChoice < 1 || numChoice > 5)
					cout << "Your choice is invalid.  Please choose a number 1 through 5." << endl;
				
			} while (numChoice < 1 || numChoice > 5);
			
			//Calls 'generateNewCard' to generate a new card for the user
			generateNewCard(pH, cT, numChoice, s, r);
			
			//Calls 'printPlayerHand' to display the cards chosen for the player.  Uses other functions for options.
			printPlayerHand(pH);
		}
		//Updates discardCount.  Used to keep the user from discarding more than 3 cards.
		discardCount++;
		
		//Breaks the loop if the user has discarded a card 3 times.
		if (discardCount == 3)
		{
			cout << endl;
			cout << "You cannot discard more than 3 cards.  The limit is 3." << endl;
			cout << endl;
			break;
		}
		
	} while (discardChoice == 'y' || discardChoice == 'Y');
}
/***********************************************************************************************************
* Function:  printComputerHand                                                                             *
* Purpose :  This function display the CPU's hand to the player.                                           *
*                                                                                                          *
************************************************************************************************************/
void printComputerHand(char cH[][6])
{
	cout << "***** COMPUTER's HAND *****" << endl;
	int cardNumber = 1;
	int subScript = 0;
	
	//Prints out the 5 cards chosen for the computer
	for (int i = 1; i < 6; i++)
	{
		//Prints out cards with suit first, rank second
		cout << "CARD " << cardNumber << ":  " << cH[0][subScript] << cH[1][subScript] << endl;
		cardNumber++;
		subScript++;
	} 
}
/***********************************************************************************************************
* Function:  generateNewCard                                                                               *
* Purpose :  This function generates a new card for the user's discarded card.                             *
*                                                                                                          *
************************************************************************************************************/
void generateNewCard(char pH[][6], bool cT[][13], int choice, char s[], char r[])
{
	int randomSuit;
	int randomRank;
	choice -= 1;                                  //Subtracts 1 from choice to align with the card's subscript
	
	do
	{
		randomSuit = (rand() % 4);                 //Randomly picks a suit (H, S, C, D) for the card
		randomRank = (rand() % 13);                //Randomly picks a rank (1, 2, 3 etc) for the card
		
		pH[0][choice] = s[randomSuit];             //Assigns suit to replace chosen discarded card
		pH[1][choice] = r[randomRank];             //Assigns rank to replace chosen discarded card
		
	} while (cT[randomSuit][randomRank] == true); //Re-randomizes the card if it's been used before
	
	cT[randomSuit][randomRank] = true;            //Adds the card to cardsTaken so it cannot be used again
}
/***********************************************************************************************************
* Function:  calculateResults                                                                              *
* Purpose :  This function evaluates the player and computer's hand and awards points.                     *
*                                                                                                          *
************************************************************************************************************/
void calculateResults(char pH[][6], char cH[][6], int &pS, int &cS)
{
	//Calculate player's score
	int highCardP;
	
	if (straightFlush(pH))
		pS = 35;
	else if (fourOfAKind(pH))
		pS = 30;
	else if (allSameSuit(pH))
		pS = 25;
	else if (straight(pH))
		pS = 20;
	else if (threeOfAKind(pH))
		pS = 15;
	else if (twoOfAKind(pH))
		pS = 10;
	else
		highCardP = highestCard(pH);
	
	//Calculate computer's score
	int highCardC;
	
	if (straightFlush(cH))
		cS = 35;
	else if (fourOfAKind(cH))
		cS = 30;
	else if (allSameSuit(cH))
		cS = 25;
	else if (straight(cH))
		cS = 20;
	else if (threeOfAKind(cH))
		cS = 15;
	else if (twoOfAKind(cH))
		cS = 10;
	else
		highCardC = highestCard(cH);

	if (pS == 0 && cS == 0)
	{
		if (highCardP > highCardC)
			pS = 5;
		else if (highCardP < highCardC)
			cS = 5;
	}
}
/***********************************************************************************************************
* Function:  straightFlush                                                                                 *
* Purpose :  Determines if hand is a straight flush.  Player is awarded 35 points.                         *
*                                                                                                          *
************************************************************************************************************/
bool straightFlush(char pH[][6])
{
	if (allSameSuit(pH))                          //Checks to see if hand is all of the same suit
		if (straight(pH))                          //Checks to see if the hand is a straight
			return true;

	return false;
}
/***********************************************************************************************************
* Function:  fourOfAKind                                                                                   *
* Purpose :  Determines if hand has a four of a kind.  Player is awarded 30 points.                        *
*                                                                                                          *
************************************************************************************************************/
bool fourOfAKind(char pH[][6])
{
	int same = 0;
	
	//Nested for loops to compare cards
	for (int target = 0; target < 5; target++)
	{
		same = 0;
		for (int c = 0; c < 5; c++)
		{
			if (pH[1][target] == pH[1][c])
				same++;
			
			//If four cards are the same, end the loop and return true
			if (same == 4)
			{
				return true;
				break;
			}
		}
	}
	
	return false;
}
/***********************************************************************************************************
* Function:  threeOfAKind                                                                                  *
* Purpose :  Determines if hand has a three of a kind.  Player is awarded 15 points.                       *
*                                                                                                          *
************************************************************************************************************/
bool threeOfAKind(char pH[][6])
{
	int same = 0;
	
	//Nested for loops to compare cards
	for (int target = 0; target < 5; target++)
	{
		same = 0;
		for (int c = 0; c < 5; c++)
		{
			if (pH[1][target] == pH[1][c])
				same++;
			
			//If three cards are the same, end the loop and return true
			if (same == 3)
			{
				return true;
				break;
			}
		}
	}
	
	return false;
}
/***********************************************************************************************************
* Function:  twoOfAKind                                                                                    *
* Purpose :  Determines if hand has a two of a kind.  Player is awarded 10 points.                         *
*                                                                                                          *
************************************************************************************************************/
bool twoOfAKind(char pH[][6])
{
	int same = 0;
	
	//Nested for loops to compare cards
	for (int target = 0; target < 5; target++)
	{
		same = 0;
		for (int c = 0; c < 5; c++)
		{
			if (pH[1][target] == pH[1][c])
				same++;
			
			//If two cards are the same, end the loop and return true
			if (same == 2)
			{
				return true;
				break;
			}
		}
	}
	
	return false;
}
/***********************************************************************************************************
* Function:  allSameSuit                                                                                   *
* Purpose :  Determines if hand is all of the same suit.  Player is awarded 25 points.                     *
*                                                                                                          *
************************************************************************************************************/
bool allSameSuit(char pH[][6])
{	
	//Counters
	int spades = 0, hearts = 0, clubs = 0, diamonds = 0;
	
	for (int c = 0; c < 5; c++)
	{
		if(pH[0][c] == 'D')
			diamonds++;
		else if(pH[0][c] == 'C')
			clubs++;
		else if(pH[0][c] == 'S')
			spades++;
		else if(pH[0][c] == 'H')
			hearts++;
	}
	
	if(diamonds == 5 || clubs == 5 || spades == 5 || hearts == 5)
		return true;
	else
		return false;
}
/***********************************************************************************************************
* Function:  straight                                                                                      *
* Purpose :  Determines if hand has a straight.  Player is awarded 20 points.                              *
*                                                                                                          *
************************************************************************************************************/
bool straight(char pH[][6])
{
	//Counters
	int numberHand[5];
	
	//Converts player's hand to numbers
	for (int c = 0; c < 5; c++)
	{
		if(pH[1][c] == 'A')
			numberHand[c] = 14;
		else if (pH[1][c] == 'K')
			numberHand[c] = 13;
		else if (pH[1][c] == 'Q')
			numberHand[c] = 12;
		else if (pH[1][c] == 'J')
			numberHand[c] = 11;
		else if (pH[1][c] == 'T')
			numberHand[c] = 10;
		else if (pH[1][c] == '9')
			numberHand[c] = 9;
		else if (pH[1][c] == '8')
			numberHand[c] = 8;
		else if (pH[1][c] == '7')
			numberHand[c] = 7;
		else if (pH[1][c] == '6')
			numberHand[c] = 6;
		else if (pH[1][c] == '5')
			numberHand[c] = 5;
		else if (pH[1][c] == '4')
			numberHand[c] = 4;
		else if (pH[1][c] == '3')
			numberHand[c] = 3;
		else if (pH[1][c] == '2')
			numberHand[c] = 2;
	}
	
	//Sort the integer array in increasing order
	int temp;
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (numberHand[y] > numberHand[y + 1])
			{
				temp = numberHand[y + 1];
				numberHand[y + 1] = numberHand[y];
				numberHand[y] = temp;
			}
		}
	}
	
	//Check to see if the integer array is in consecutive order
	int inARow = 0;
	for (int c = 0; c < 5; c++)
	{
		if (numberHand[c] + 1 == numberHand[c + 1])
			inARow++;
	}
	
	if (inARow == 4)
		return true;
	else
		return false;
}
/***********************************************************************************************************
* Function:  highestCard                                                                                   *
* Purpose :  Determines highest card in a hand.  Player is awarded 5 points.                               *
*                                                                                                          *
************************************************************************************************************/
int highestCard(char pH[][6])
{	
	//Counters
	int numberHand[5];
	
	//Converts player's hand to numbers
	for (int c = 0; c < 5; c++)
	{
		if(pH[1][c] == 'A')
			numberHand[c] = 14;
		else if (pH[1][c] == 'K')
			numberHand[c] = 13;
		else if (pH[1][c] == 'Q')
			numberHand[c] = 12;
		else if (pH[1][c] == 'J')
			numberHand[c] = 11;
		else if (pH[1][c] == 'T')
			numberHand[c] = 10;
		else if (pH[1][c] == '9')
			numberHand[c] = 9;
		else if (pH[1][c] == '8')
			numberHand[c] = 8;
		else if (pH[1][c] == '7')
			numberHand[c] = 7;
		else if (pH[1][c] == '6')
			numberHand[c] = 6;
		else if (pH[1][c] == '5')
			numberHand[c] = 5;
		else if (pH[1][c] == '4')
			numberHand[c] = 4;
		else if (pH[1][c] == '3')
			numberHand[c] = 3;
		else if (pH[1][c] == '2')
			numberHand[c] = 2;
	}
	
	//Sort the integer array in increasing order
	int temp;
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (numberHand[y] > numberHand[y + 1])
			{
				temp = numberHand[y + 1];
				numberHand[y + 1] = numberHand[y];
				numberHand[y] = temp;
			}
		}
	}
	
	//numberHand[4] is highest because the numbers have been put in increasing order
	return numberHand[4];
}