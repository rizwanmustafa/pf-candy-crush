#include <iostream>
#include <Windows.h>
#include <ctime>
#include <conio.h>
#include <iomanip>
#include <string>
#include <chrono>
#include <fstream>
#include <cctype>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

// TODO: Add instructions and add more candies to hard mode

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Taken from the internet

const int LEADERBOARD_SIZE = 3, INITIAL_MOVES = 15;
char easyModeLeaderboardNames[LEADERBOARD_SIZE][50], hardModeLeaderboardNames[LEADERBOARD_SIZE][50];
int easyModeLeaderboardScores[LEADERBOARD_SIZE], hardModeLeaderboardScores[LEADERBOARD_SIZE];


// TODO: Add more candies for hardmode
// TODO: Later this should take num of candies as input for the hard mode and easy mode diff

double getDuration(chrono::system_clock::time_point currentTime, chrono::system_clock::time_point startTime)
{
	return chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
}

bool possiblePatternExists(char gameArray[10][10], int gameSize, int row, int col);
void saveLeaderboards();

char getCandyThroughNum(int num)
{
	switch (num)
	{
	case 0:
		return '@';
	case 1:
		return '%';
	case 2:
		return '#';
	case 3:
		return '$';
	case 4:
		return '!';
	case 5:
		return '=';
	default:
		return '&';
	}
}

bool checkCandyMatch(char gameArray[10][10], int gameSize, char candy, int matchRow, int matchCol)
{
	if (matchRow < 0 || matchCol < 0 || matchRow >= gameSize || matchCol >= gameSize)
		return false;

	if (candy == gameArray[matchRow][matchCol] && candy != ' ')
		return true;
	else
		return false;
}

void swapCandies(char gameArray[10][10], int row1, int col1, int row2, int col2)
{
	char temp = gameArray[row1][col1];
	gameArray[row1][col1] = gameArray[row2][col2];
	gameArray[row2][col2] = temp;
}

void shuffleBoard(char gameArray[10][10], int gameSize)
{
	int count = gameSize * gameSize;
	while (count-- > 0)
	{
		swapCandies(gameArray, rand() % gameSize, rand() % gameSize, rand() % gameSize, rand() % gameSize);
	}
}

// This function returns 0 if no pattern found, else returns the score of the highest pattern
int patternMatches(char gameArray[10][10], int gameSize, int row, int col, bool destroyCandies)
{
	// TODO: Take a boolean parameter that checks all possible parameters and turn that on in checking possible matches
	if (row < 0 || row >= gameSize || col < 0 || col >= gameSize)
	{
		cout << "Terminal error!";
		exit(1);
	}

	// TODO: Add the 4 L Matches that start from this candy
	char candy = gameArray[row][col];
	bool leftTwoMatch = checkCandyMatch(gameArray, gameSize, candy, row, col - 1) && checkCandyMatch(gameArray, gameSize, candy, row, col - 2);
	bool rightTwoMatch = checkCandyMatch(gameArray, gameSize, candy, row, col + 1) && checkCandyMatch(gameArray, gameSize, candy, row, col + 2);
	bool topTwoMatch = checkCandyMatch(gameArray, gameSize, candy, row - 1, col) && checkCandyMatch(gameArray, gameSize, candy, row - 2, col);
	bool bottomTwoMatch = checkCandyMatch(gameArray, gameSize, candy, row + 1, col) && checkCandyMatch(gameArray, gameSize, candy, row + 2, col);
	bool rightDiagMatch = checkCandyMatch(gameArray, gameSize, candy, row + 1, col + 1) && checkCandyMatch(gameArray, gameSize, candy, row + 2, col + 2);
	bool leftDiagMatch = checkCandyMatch(gameArray, gameSize, candy, row + 1, col - 1) && checkCandyMatch(gameArray, gameSize, candy, row + 2, col - 2);

	bool lFormed = (topTwoMatch || bottomTwoMatch) && (rightTwoMatch || leftTwoMatch);

	// TODO: optimize this code later
	if (destroyCandies)
	{
		if (lFormed)
		{
			gameArray[row][col] = ' ';
			if (topTwoMatch)
			{
				gameArray[row - 1][col] = ' ';
				gameArray[row - 2][col] = ' ';
				if (rightTwoMatch)
				{
					gameArray[row][col + 1] = ' ';
					gameArray[row][col + 2] = ' ';
				}
				else
				{
					gameArray[row][col - 1] = ' ';
					gameArray[row][col - 2] = ' ';
				}
			}
			else
			{
				gameArray[row + 1][col] = ' ';
				gameArray[row + 2][col] = ' ';
				if (rightTwoMatch)
				{
					gameArray[row][col + 1] = ' ';
					gameArray[row][col + 2] = ' ';
				}
				else
				{
					gameArray[row][col - 1] = ' ';
					gameArray[row][col - 2] = ' ';
				}
			}
		}
		else if (rightDiagMatch)
		{
			gameArray[row][col] = ' ';
			gameArray[row + 1][col + 1] = ' ';
			gameArray[row + 2][col + 2] = ' ';
		}
		else if (leftDiagMatch)
		{
			gameArray[row][col] = ' ';
			gameArray[row + 1][col - 1] = ' ';
			gameArray[row + 2][col - 2] = ' ';
		}
		else if (bottomTwoMatch)
		{
			gameArray[row][col] = ' ';
			gameArray[row + 1][col] = ' ';
			gameArray[row + 2][col] = ' ';
		}
		else if (rightTwoMatch)
		{
			gameArray[row][col] = ' ';
			gameArray[row][col + 1] = ' ';
			gameArray[row][col + 2] = ' ';
		}
		else if (leftTwoMatch)
		{
			gameArray[row][col] = ' ';
			gameArray[row][col - 1] = ' ';
			gameArray[row][col - 2] = ' ';
		}
		else if (topTwoMatch)
		{
			gameArray[row][col] = ' ';
			gameArray[row - 1][col] = ' ';
			gameArray[row - 2][col] = ' ';
		}
	}

	// An L is formed when (topTwoMatch || bottomTwoMatch) && (rightTwoMatch || leftTwoMatch)
	if (lFormed)
		return 25;
	else if (rightDiagMatch || leftDiagMatch)
		return 20;
	else if (topTwoMatch || bottomTwoMatch || rightTwoMatch || leftTwoMatch)
		return 10;
	else
		return 0;

	// The following code is for debugging purposes TODO: Delete it once the project is complete
	//
	// if (rightTwoMatch) {
	//	cout << candy << endl;
	//	cout << "row " << row << " col " << col << " right two match";
	//}

	// if (bottomTwoMatch) {
	//	cout << candy << endl;
	//	cout << "row " << row << " col " << col << " bottom two match";
	// }

	// if (leftDiagMatch) {
	//	cout << candy << endl;
	//	cout << "row " << row << " col " << col << " left diag match";
	// }

	// if (rightDiagMatch) {
	//	cout << candy << endl;
	//	cout << "row " << row << " col " << col << " right diag match";
	// }

	// return rightTwoMatch || bottomTwoMatch || rightDiagMatch || leftDiagMatch;
}

char generateRandomCandyForPosition(char gameArray[10][10], int gameSize, int row, int col, bool easyMode)
{
	char prevCandy = gameArray[row][col];

	bool candyGenerated[7] = { false, false, false, false, false, false, false }; // TODO: Add candy num later as parameter
	bool allGenerated = false;
	char lastCandy;

	do
	{
		int randNum = rand() % (easyMode ? 5 : 7);
		candyGenerated[randNum] = true;
		lastCandy = getCandyThroughNum(randNum);

		gameArray[row][col] = lastCandy;
		if (patternMatches(gameArray, gameSize, row, col, false) == 0)
			break;

		allGenerated = true;
		for (int i = 0; i < (easyMode ? 5 : 7); i++)
		{
			if (!candyGenerated[i])
			{
				allGenerated = false;
				break;
			}
		}
	} while (!allGenerated);

	gameArray[row][col] = prevCandy;
	return lastCandy;
}

int getLargestPatternScore(char gameArray[10][10], int gameSize)
{
	int largestScore = 0;

	for (int row = 0; row < gameSize; row++)
	{
		for (int col = 0; col < gameSize; col++)
		{
			int score = patternMatches(gameArray, gameSize, row, col, false);
			if (score > largestScore)
				largestScore = score;
		}
	}

	return largestScore;
}

int destroyLargestPattern(char gameArray[10][10], int gameSize)
{
	int largestScore = getLargestPatternScore(gameArray, gameSize);

	for (int row = gameSize - 1; row >= 0; row--)
	{
		for (int col = gameSize - 1; col >= 0; col--)
		{
			int score = patternMatches(gameArray, gameSize, row, col, false);
			if (score == largestScore)
			{
				patternMatches(gameArray, gameSize, row, col, true);

				return largestScore;
			}
		}
	}
	return 0;
}

// TODO: Find some way to remove this hardcoding
void printBoard(char gameArray[10][10], int gameSize, int activeRow, int activeCol)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int row = 0; row < gameSize; row++)
	{
		for (int col = 0; col < gameSize; col++)
		{

			//if (patternMatches(gameArray, gameSize, row, col, false) && highlightPattern ) {
			   //SetConsoleTextAttribute(hConsole, 16);
			//} // TODO: Remove this coloring
			//else
			switch (gameArray[row][col])
			{
			case '@':
				SetConsoleTextAttribute(hConsole, 9);
				break;
			case '%':
				SetConsoleTextAttribute(hConsole, 14);
				break;
			case '#':
				SetConsoleTextAttribute(hConsole, 12);
				break;
			case '$':
				SetConsoleTextAttribute(hConsole, 11);
				break;
			case '&':
				SetConsoleTextAttribute(hConsole, 2);
				break;
			case '!':
				SetConsoleTextAttribute(hConsole, 13);
				break;
			default:
				SetConsoleTextAttribute(hConsole, 8);
			}
			if (row == activeRow && col == activeCol)
				SetConsoleTextAttribute(hConsole, 112);
			cout << gameArray[row][col];
			SetConsoleTextAttribute(hConsole, 15);
			cout << "   ";
		}

		cout << endl
			<< endl;
	}
}

void generateRandomBoard(char gameArray[10][10], int gameSize)
{
	// for (int row = 0; row < gameSize; row++) {
	//	for (int col = 0; col < gameSize; col++) {
	//		gameArray[row][col] = generateRandomCandy();
	//	}
	// }

	for (int row = 0; row < gameSize; row++)
	{
		for (int col = 0; col < gameSize; col++)
		{
			gameArray[row][col] = generateRandomCandyForPosition(gameArray, gameSize, row, col, gameSize == 8);
		}
	}
}

void clearBoard(char gameArray[10][10], int gameSize)
{
	for (int row = 0; row < gameSize; row++)
	{
		for (int col = 0; col < gameSize; col++)
		{
			gameArray[row][col] = ' ';
		}
	}
}

bool anyPatternExists(char gameArray[10][10], int gameSize)
{
	for (int row = 0; row < gameSize; row++)
	{
		for (int col = 0; col < gameSize; col++)
		{
			if (patternMatches(gameArray, gameSize, row, col, false))
				return true;
		}
	}

	return false;
}

bool needToShuffle(char gameArray[10][10], int gameSize)
{
	for (int row = 0; row < gameSize; row++)
	{
		for (int col = 0; col < gameSize; col++)
		{
			if (possiblePatternExists(gameArray, gameSize, row, col))
				return false;
		}
	}
	return true;
}

int generateValidBoard(char gameArray[10][10], int gameSize)
{
	int numIterations = 0;
	cout << "Generating random board! Please be patient as it can take some time!";
	clearBoard(gameArray, gameSize);
	do
	{
		generateRandomBoard(gameArray, gameSize);
		numIterations++;
	} while (anyPatternExists(gameArray, gameSize) || needToShuffle(gameArray, gameSize));

	return numIterations;
}

bool canFillBoard(char gameArray[10][10], int gameSize)
{
	for (int col = 0; col < gameSize; col++)
	{
		if (gameArray[0][col] == ' ')
			return true;
	}

	return false;
}

void fillBoardTopLine(char gameArray[10][10], int gameSize)
{
	// for (int col = 0; col < gameSize; col++) {
	//	if (gameArray[0][col] == ' ')
	//		gameArray[0][col] = generateRandomCandy();
	// }

	for (int col = 0; col < gameSize; col++)
	{
		// Count number of spaces in the column
		int numSpaces = 0;
		for (int row = 0; row < gameSize; row++)
			if (gameArray[row][col] == ' ')
				numSpaces++;

		if (gameArray[0][col] == ' ')
			gameArray[0][col] = generateRandomCandyForPosition(gameArray, gameSize, numSpaces - 1, col, gameSize == 8);
	}
}

bool canCandyDrop(char gameArray[10][10], int gameSize)
{
	for (int col = 0; col < gameSize; col++)
	{
		bool spaceEncountered = false;
		for (int row = gameSize - 1; row >= 0; row--)
		{
			if (spaceEncountered && gameArray[row][col] != ' ')
				return true;
			if (gameArray[row][col] == ' ')
				spaceEncountered = true;
		}
	}

	return false;
}

void dropCandyByOneStep(char gameArray[10][10], int gameSize)
{
	for (int col = 0; col < gameSize; col++)
	{
		for (int row = gameSize - 1; row > 0; row--)
		{
			if (gameArray[row][col] == ' ' && gameArray[row - 1][col] != ' ')
			{
				gameArray[row][col] = gameArray[row - 1][col];
				gameArray[row - 1][col] = ' ';
			}
		}
	}
}

// This function checks if there is a possible pattern upon swapping the current candy
bool possiblePatternExists(char gameArray[10][10], int gameSize, int row, int col)
{
	// TODO: Create another function called check Swap pattern exists and replace these hardcodings
	bool topRowExists = row - 1 >= 0;
	bool bottomRowExists = row + 1 < gameSize;
	bool rightColExists = col + 1 < gameSize;
	bool leftColExists = col - 1 >= 0;

	char candy = gameArray[row][col];
	char temp = candy; // TODO: Remove temp and use the candy variable
	bool patternMatch = false;

	// Swap with upper candy
	if (topRowExists)
	{
		gameArray[row][col] = gameArray[row - 1][col];
		gameArray[row - 1][col] = temp;
		if (patternMatches(gameArray, gameSize, row, col, false) || patternMatches(gameArray, gameSize, row - 1, col, false))
			patternMatch = true;
		// Reverse the effects of swapping
		gameArray[row - 1][col] = gameArray[row][col];
		gameArray[row][col] = temp;

		if (patternMatch)
		{
			// cout << "row " << row << " col " << col << " top" << endl; // TODO: Del later
			return true; // The reason i return after it is because i have to reverse effects of the swapping
		}
	}

	// Swap with lower candy
	if (bottomRowExists)
	{
		gameArray[row][col] = gameArray[row + 1][col];
		gameArray[row + 1][col] = temp;
		if (patternMatches(gameArray, gameSize, row, col, false) || patternMatches(gameArray, gameSize, row + 1, col, false))
			patternMatch = true;

		// Reverse the effects of swapping
		gameArray[row + 1][col] = gameArray[row][col];
		gameArray[row][col] = temp;

		if (patternMatch)
		{
			// cout << "row " << row << " col " << col << " bottom" << endl;
			return true; // The reason i return after it is because i have to reverse effects of the swapping
		}
	}

	// Swap with right candy
	if (rightColExists)
	{
		gameArray[row][col] = gameArray[row][col + 1];
		gameArray[row][col + 1] = temp;
		if (patternMatches(gameArray, gameSize, row, col, false) || patternMatches(gameArray, gameSize, row, col + 1, false))
			patternMatch = true;

		// Reverse the effects of swapping
		gameArray[row][col + 1] = gameArray[row][col];
		gameArray[row][col] = temp;

		if (patternMatch)
		{
			// cout << "row " << row << " col " << col << " right" << endl;
			return true; // The reason i return after it is because i have to reverse effects of the swapping
		}
	}

	// Swap with left candy
	if (leftColExists)
	{
		gameArray[row][col] = gameArray[row][col - 1];
		gameArray[row][col - 1] = temp;
		if (patternMatches(gameArray, gameSize, row, col, false) || patternMatches(gameArray, gameSize, row, col - 1, false))
			patternMatch = true;

		// Reverse the effects of swapping
		gameArray[row][col - 1] = gameArray[row][col];
		gameArray[row][col] = temp;

		if (patternMatch)
		{
			// cout << "row " << row << " col " << col << " left" << endl;
			return true; // The reason i return after it is because i have to reverse effects of the swapping
		}
	}

	return false;
}

// TODO: Maybe using pointers, keep the last location saved so the user doesn't have to do all the hassle
void getCandySwapped(
	char gameArray[10][10],
	int gameSize,
	int& activeRowMain,
	int& activeColMain,
	int playerScore,
	int playerMoves,
	chrono::system_clock::time_point startTime,
	int totalTime)
{
	system("cls");
	int activeRow = activeRowMain, activeCol = activeColMain;
	printBoard(gameArray, gameSize, activeRow, activeCol);
	cout << "Score: " << playerScore << endl;
	cout << "Moves: " << playerMoves << endl;
	cout << "Time Remaining: " << totalTime - getDuration(chrono::system_clock::now(), startTime) << endl;
	SetConsoleTextAttribute(hConsole, 2);
	cout << endl
		<< ">> Choose the candy you want to move!" << endl;
	SetConsoleTextAttribute(hConsole, 15);

	while (getDuration(chrono::system_clock::now(), startTime) <= totalTime)
	{
		// Take input from user as long as he/she does not press enter

		if (_kbhit() == 0) // Taken from manual of BSAI
		{
			system("cls");
			printBoard(gameArray, gameSize, activeRow, activeCol);
			cout << "Score: " << playerScore << endl;
			cout << "Moves: " << playerMoves << endl;
			cout << "Time Remaining: " << totalTime - getDuration(chrono::system_clock::now(), startTime) << endl;
			SetConsoleTextAttribute(hConsole, 2);
			cout << endl
				<< ">> Choose the candy you want to move!" << endl;
			SetConsoleTextAttribute(hConsole, 15);
			Sleep(50);
			continue;
		}

		int inputChar = _getch();
		char c = tolower(inputChar);

		if (c == '\r')
			break;
		else if ((inputChar == KEY_UP || c == 'w') && activeRow - 1 >= 0)
			activeRow -= 1;
		else if ((inputChar == KEY_DOWN || c == 's') && activeRow + 1 < gameSize)
			activeRow += 1;
		else if ((inputChar == KEY_LEFT || c == 'a') && activeCol - 1 >= 0)
			activeCol -= 1;
		else if ((inputChar == KEY_RIGHT || c == 'd') && activeCol + 1 < gameSize)
			activeCol += 1;
	}

	// Now get the swap position
	int swapRow = activeRow, swapCol = activeCol;


	while (getDuration(chrono::system_clock::now(), startTime) <= totalTime)
	{

		if (_kbhit() == 0) {

			system("cls");
			printBoard(gameArray, gameSize, swapRow, swapCol);
			cout << "Score: " << playerScore << endl;
			cout << "Moves: " << playerMoves << endl;
			cout << "Time Remaining: " << totalTime - getDuration(chrono::system_clock::now(), startTime) << endl;
			SetConsoleTextAttribute(hConsole, 2);
			cout << endl
				<< ">> Choose the position you want to swap to!" << endl;
			Sleep(50);
			continue;
		}

		SetConsoleTextAttribute(hConsole, 15);
		// Take input from user as long as he/she does not press enter

		int inputChar = _getch();
		char c = tolower(inputChar);

		if (c == '\r' && !(swapCol == activeCol && swapRow == activeRow))
			break;
		else if ((c == 'w' || inputChar == KEY_UP) && activeRow - 1 >= 0)
		{
			swapRow = activeRow - 1;
			swapCol = activeCol;
		}
		else if ((c == 's' || inputChar == KEY_DOWN) && activeRow + 1 < gameSize)
		{
			swapRow = activeRow + 1;
			swapCol = activeCol;
		}
		else if ((c == 'a' || inputChar == KEY_LEFT) && activeCol - 1 >= 0)
		{
			swapRow = activeRow;
			swapCol = activeCol - 1;
		}
		else if ((c == 'd' || inputChar == KEY_RIGHT) && activeCol + 1 < gameSize)
		{
			swapRow = activeRow;
			swapCol = activeCol + 1;
		}
	}

	if (getDuration(chrono::system_clock::now(), startTime) >= totalTime)
		return;

	// cout << "Swap with row " << swapRow + 1 << " and column " << swapCol + 1 << endl;
	// system("pause");

	// TODO: Temp swap. Change this later.

	char temp = gameArray[activeRow][activeCol];
	gameArray[activeRow][activeCol] = gameArray[swapRow][swapCol];
	gameArray[swapRow][swapCol] = temp;
	activeRowMain = swapRow;
	activeColMain = swapCol;
	system("cls");
}

bool getMode()
{
	char c;
	do
	{
		cout << "Enter game mode (E for easy, H for Hard): ";
		c = tolower(_getch());
		if (c != 'h' && c != 'e')
		{
			cout << "Invalid choice!" << endl;
			system("pause");
			system("cls");
		}
	} while (c != 'h' && c != 'e');

	return c == 'e';
}

void getName(char name[50])
{
		bool foundPipe = false;
		foundPipe = false;
	do
	{
		cout << "Enter name: ";
		cin.getline(name, 50);
		for (int i = 0; i < 50; i++) {
			if (name[i] == '\0') break;
			if (name[i] == '|') {
				foundPipe = true;
				break;
			}
		}
		if (name[0] == '\0')
		{
			cout << "Invalid choice!" << endl;
			system("pause");
			system("cls");
		}
		else
			if (foundPipe) {
				cout << "Your name cannot contain '|'" << endl;
				system("pause");
				system("cls");
			}
	} while (name[0] == '\0' || foundPipe);
}

void updateLeaderboards(bool easyMode, char playerName[50], int playerScore)
{
	if (easyMode)
	{
		for (int i = 0; i < LEADERBOARD_SIZE; i++)
		{
			if (playerScore > easyModeLeaderboardScores[i])
			{
				for (int j = LEADERBOARD_SIZE - 1; j > i; j--)
				{
					easyModeLeaderboardScores[j] = easyModeLeaderboardScores[j - 1];

					for (int x = 0; x < 50; x++)
						easyModeLeaderboardNames[j][x] = easyModeLeaderboardNames[j - 1][x];
				}
				easyModeLeaderboardScores[i] = playerScore;
				for (int x = 0; x < 50; x++)
					easyModeLeaderboardNames[i][x] = playerName[x];
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < LEADERBOARD_SIZE; i++)
		{
			if (playerScore > hardModeLeaderboardScores[i])
			{
				for (int j = LEADERBOARD_SIZE - 1; j > i; j--)
				{
					hardModeLeaderboardScores[j] = hardModeLeaderboardScores[j - 1];

					for (int x = 0; x < 50; x++)
						hardModeLeaderboardNames[j][x] = hardModeLeaderboardNames[j - 1][x];
				}
				hardModeLeaderboardScores[i] = playerScore;
				for (int x = 0; x < 50; x++)
					hardModeLeaderboardNames[i][x] = playerName[x];
				break;
			}
		}
	}
}

string getRandomWord()
{
	string words[] = { "Sweet!", "Delicious!", "Magnificent!", "Divine!", "Fabulous!", "Fantastic!", "Marvelous!", "Yummy!" };
	return words[rand() % 8];
}

void playGame()
{
	system("cls");

	char gameArray[10][10];
	int numIterations = 0;

	char playerName[50];
	getName(playerName);
	bool easyMode = getMode();
	system("cls");

	// TODO: Display move count by passing it two swap and print board func
	int moves = INITIAL_MOVES, playerScore = 0, gameSize = easyMode ? 8 : 10, numCandy = easyMode ? 5 : 7;
	int gameSeconds = easyMode ? 60 : 40;
	// TODO: Change the playerscore using the pattern match function later
	// TODO: Create a generate random candy method that takes in the num of candy allowed
	// TODO: Create a function that returns the color number depending on the char candy input;

	cout << "Generating random board! Please be patient as it can take some time!";
	numIterations = generateValidBoard(gameArray, gameSize);
	system("cls");

	// After swapping function, we should have another loop that continues as long as there is a pattern
	// It should see if candies can drop. If yes, then make them drop.
	// Once no candies can drop, generate a random candy column wise,
	// Have them dropped
	// Once all the board is filled, loop and check if any other pattern can be found and auto add to score.
	// Also once added to the score, the main function should print sweet and wait for some time
	int activeRow = 0, activeCol = 0;
	auto startTime = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - startTime;

	do
	{
		getCandySwapped(gameArray, gameSize, activeRow, activeCol, playerScore, moves, startTime, gameSeconds);
		moves--;

		while (	anyPatternExists(gameArray, gameSize))
		{
			playerScore += destroyLargestPattern(gameArray, gameSize);
			printBoard(gameArray, gameSize, -1, -1);
			cout << "Player Score: " << playerScore << endl;
			cout << "Player Moves: " << moves << endl;
			cout << "Time Remaining: " << gameSeconds - getDuration(chrono::system_clock::now(), startTime) << endl;
			SetConsoleTextAttribute(hConsole, 2);

			// TODO: GEt random good words
			cout << endl
				<< getRandomWord() << endl;
			SetConsoleTextAttribute(hConsole, 15);
			SetConsoleTextAttribute(hConsole, 12);
			cout << endl
				<< "Please wait!" << endl;
			SetConsoleTextAttribute(hConsole, 15);
			Sleep(400);

			while (canCandyDrop(gameArray, gameSize))
			{
				system("cls");
				dropCandyByOneStep(gameArray, gameSize);
				printBoard(gameArray, gameSize, -1, -1);
				cout << "Player Score: " << playerScore << endl;
				cout << "Player Moves: " << moves << endl;
				cout << "Time Remaining: " << gameSeconds - getDuration(chrono::system_clock::now(), startTime) << endl;
				SetConsoleTextAttribute(hConsole, 12);
				cout << endl
					<< "Please wait!" << endl;
				SetConsoleTextAttribute(hConsole, 15);
				Sleep(400);
			}
			system("cls");

			while (canFillBoard(gameArray, gameSize) || canCandyDrop(gameArray, gameSize))
			{
				system("cls");
				dropCandyByOneStep(gameArray, gameSize);
				fillBoardTopLine(gameArray, gameSize);
				printBoard(gameArray, gameSize, -1, -1);
				cout << "Player Score: " << playerScore << endl;
				cout << "Player Moves: " << moves << endl;
				cout << "Time Remaining: " << gameSeconds - getDuration(chrono::system_clock::now(), startTime) << endl;
				SetConsoleTextAttribute(hConsole, 12);
				cout << endl
					<< "Please wait!" << endl;
				SetConsoleTextAttribute(hConsole, 15);
				Sleep(400);
			}
			Sleep(400);
			system("cls");
		}

		// Check if we need to shuffle the board. If yes, shuffle it
		// TODO: Implement shuffling algorithm
		if (needToShuffle(gameArray, gameSize))
		{
			cout << "This is rare! We need to shuffle the board as there are no possible moves!" << endl;
			cout << "Sit tight! This shouldn't take long!" << endl;

			int count = 1000;
			bool needToRepeat = anyPatternExists(gameArray, gameSize) || needToShuffle(gameArray, gameSize);
			while (needToRepeat && count-- > 0)
			{
				shuffleBoard(gameArray, gameSize);
				needToRepeat = anyPatternExists(gameArray, gameSize) || needToShuffle(gameArray, gameSize);
			}

			if (needToRepeat && count <= 0)
			{
				// There is a high chance that we can't shuffle the board. Let's generate a new one
				cout << "Can't shuffle the existing board!" << endl;
				cout << "We will generate you another one!" << endl;
				system("pause");
				generateValidBoard(gameArray, gameSize);
			}
			else {
				system("pause");
			}
		}
		elapsed_seconds = std::chrono::system_clock::now() - startTime;
	} while (moves > 0 && elapsed_seconds.count() <= gameSeconds);

	system("cls");
	printBoard(gameArray, gameSize, -1, -1);
	cout << "Game over!" << endl;
	if (moves != 0) cout << "Time's up!" << endl;
	cout << "Your score is " << playerScore << endl;
	cout << "Bye " << playerName << endl;
	updateLeaderboards(easyMode, playerName, playerScore);
}

// TODO: Create a function for printing colored text

void displayMiniCredits(int offset)
{
	system("cls");
	// TODO: Add colors to the menu
	for (int space = 0; space < offset; space++)
		cout << " ";
	cout << "Made with ";
	SetConsoleTextAttribute(hConsole, 5);
	cout << "love";
	SetConsoleTextAttribute(hConsole, 15);

	cout << " by ";
	SetConsoleTextAttribute(hConsole, 2);
	cout << "Muhammed Zain (23F-0557)";
	SetConsoleTextAttribute(hConsole, 15);

	cout << " and ";
	SetConsoleTextAttribute(hConsole, 2);
	cout << "Rizwan Mustafa Khan (23F-0709)";
	SetConsoleTextAttribute(hConsole, 15);
	cout << endl;
	for (int space = 0; space < offset; space++)
		cout << " ";
	cout << "Batch: 2023" << endl;
	for (int space = 0; space < offset; space++)
		cout << " ";
	cout << "Section: 1C" << endl;
	for (int space = 0; space < offset; space++)
		cout << " ";
	cout << "Lab Instructor: ";
	SetConsoleTextAttribute(hConsole, 9);
	cout << "Sir Muiz Qadir";
	SetConsoleTextAttribute(hConsole, 15);

	cout << endl;
	for (int space = 0; space < offset; space++)
		cout << " ";
	cout << "Course Instructor: ";
	SetConsoleTextAttribute(hConsole, 9);
	cout << "Sir Usman Ghous";
	SetConsoleTextAttribute(hConsole, 15);
	cout << endl;
	Sleep(50);
}

void printCredits()
{
	// Animations look cool, so why not
	int offset = 0;
	for (; offset < 10; offset++)
		displayMiniCredits(offset);
	for (; offset >= 0; offset--)
		displayMiniCredits(offset);
}

void printInstructions()
{
	system("cls");
	char checkGameArray[10][10] = {
		{'$', '%', '!', '!', '#', '$', '!', '$'},
		{'@', '$', '!', '%', '@', '#', '@', '$'},
		{'#', '#', '@', '$', '!', '#', '%', '!'},
		{'$', '$', '!', '#', '!', '$', '@', '%'},
		{'%', '@', '%', '#', '%', '$', '#', '@'},
		{'!', '@', '#', '@', '@', '@', '!', '$'},
		{'#', '$', '$', '@', '%', '%', '!', '$'},
		{'%', '%', '$', '@', '!', '@', '#', '%'} };

	cout << "Peanut Fudge Instructions" << endl << endl;

	cout << "Active Candies" << endl;
	SetConsoleTextAttribute(hConsole, 8);
	cout << "The active candy is highlighted with a white background and black color" << endl;
	SetConsoleTextAttribute(hConsole, 15);

	cout << endl << "Moves" << endl;
	SetConsoleTextAttribute(hConsole, 8);
	cout << "You will get 15 moves at the start of each round!" << endl;

	SetConsoleTextAttribute(hConsole, 15);

	cout << endl << "Time" << endl;
	SetConsoleTextAttribute(hConsole, 8);
	cout << "In easy mode, you will get 60 seconds to play the game. But in hard mode, you will get only 30 seconds!" << endl;
	cout << "Use your time wisely!" << endl;
	SetConsoleTextAttribute(hConsole, 15);

	cout << endl << "Swapping Candies" << endl;
	SetConsoleTextAttribute(hConsole, 8);
	cout << "First you need to select the candy you want to swap. Then press Enter to finalize the candy" << endl;
	cout << "Then you will be able to select the direction you want to swap the candy in." << endl;
	cout << "After you have selected the direction, press Enter to finally swap the direction" << endl;
	SetConsoleTextAttribute(hConsole, 15);



	cout << endl << "Movement" << endl;
	SetConsoleTextAttribute(hConsole, 8);
	cout << "You can move using the 'WASD' keys and the arrow keys" << endl;
	SetConsoleTextAttribute(hConsole, 15);

	cout << endl << "Example of L Pattern of '@' candies" << endl << endl;
	printBoard(checkGameArray, 8, -1, -1);
}

void printLeaderboard()
{
	// TODO: Print topper name in gold
	system("cls");

	cout << "Hard Mode Leaderboard" << endl;
	cout << setw(50) << "Name" << setw(10) << "Score" << endl;
	for (int i = 0; i < LEADERBOARD_SIZE; i++)
	{
		if (i == 0) {
			SetConsoleTextAttribute(hConsole, 14);
		}
		if (hardModeLeaderboardScores[i] == -1)
			cout << setw(50) << "-" << setw(10) << "-" << endl;
		else
			cout << setw(50) << hardModeLeaderboardNames[i] << setw(10) << hardModeLeaderboardScores[i] << endl;
		if (i == 0) {
			SetConsoleTextAttribute(hConsole, 15);
		}
	}
	cout << endl
		<< endl;

	cout << "Easy Mode Leaderboard" << endl;
	cout << setw(50) << "Name" << setw(10) << "Score" << endl;
	for (int i = 0; i < LEADERBOARD_SIZE; i++)
	{
		if (i == 0) {
			SetConsoleTextAttribute(hConsole, 14);
		}
		if (easyModeLeaderboardScores[i] == -1)
			cout << setw(50) << "-" << setw(10) << "-" << endl;
		else
			cout << setw(50) << easyModeLeaderboardNames[i] << setw(10) << easyModeLeaderboardScores[i] << endl;
		if (i == 0) {
			SetConsoleTextAttribute(hConsole, 15);
		}
	}
	cout << endl
		<< endl;
}

void exitGame()
{
	// TODO: Add red color to SURE
	system("cls");
	char c;
	cout << "Are you SURE you want to exit the game? (Press 'Y' if you are SURE): ";
	c = _getch();

	if (c == 'Y') {
		saveLeaderboards();
		exit(0);
	}
}

void printMenu()
{
	// TODO: Add colors to the menu later

	while (true)
	{

		for (int i = 0; i < 25; i++)
		{
			cout << "-";
		}
		cout << endl;
		cout << "Peanut Fudge Crush" << endl;
		cout << "1. Play a game" << endl;
		cout << "2. Display credits" << endl;
		cout << "3. Print instructions" << endl;
		cout << "4. Display leaderboard" << endl;
		cout << "5. Exit" << endl;
		for (int i = 0; i < 25; i++)
		{
			cout << "-";
		}
		cout << endl;

		// TODO: Replace cin with getch to have instant reaction
		cout << "Enter your choice: ";
		char i = _getch();

		switch (i)
		{
		case '1':
			playGame();
			break;
		case '2':
			printCredits();
			break;
		case '3':
			printInstructions();
			break;
		case '4':
			printLeaderboard();
			break;
		case '5':
			exitGame();
			break;
		default:
			cout << endl
				<< "Invalid choice!" << endl;
		}

		cout << endl;
		system("pause");
		system("cls");
	}
}

void loadLeaderboards()
{
	ifstream leaderBoardFile;
	leaderBoardFile.open("lb.dat");

	if (!leaderBoardFile.is_open() || !leaderBoardFile) {
		for (int i = 0; i < LEADERBOARD_SIZE; i++)
		{
			easyModeLeaderboardNames[i][0] = '\0';
			easyModeLeaderboardScores[i] = -1;
			hardModeLeaderboardNames[i][0] = '\0';
			hardModeLeaderboardScores[i] = -1;
		}

		return;
	}

	for (int i = 0; i < LEADERBOARD_SIZE; i++) {
		string name = "";
		string score = "0";
		char c;
		while ((c = leaderBoardFile.get()) != '|') {
			name += c;
		}

		while ((c = leaderBoardFile.get()) != '|') {
			score += c;
		}

		strcpy_s(easyModeLeaderboardNames[i], name.c_str()); // Taken from https://stackoverflow.com/questions/11821491/converting-string-to-cstring-in-c
		easyModeLeaderboardScores[i] = stoi(score);

	}

	for (int i = 0; i < LEADERBOARD_SIZE; i++) {
		string name = "";
		string score = "";
		char c;
		while ((c = leaderBoardFile.get()) != '|') {
			name += c;
		}

		while ((c = leaderBoardFile.get()) != '|') {
			score += c;
		}

		strcpy_s(hardModeLeaderboardNames[i], name.c_str()); // Taken from https://stackoverflow.com/questions/11821491/converting-string-to-cstring-in-c
		hardModeLeaderboardScores[i] = stoi(score);

	}

	leaderBoardFile.close();

}

void saveLeaderboards() {
	ofstream leaderBoardFile;
	leaderBoardFile.open("lb.dat");

	if (!leaderBoardFile.is_open()) {
		system("cls");
		cout << "There was an error while trying to save leaderboards in the file 'lb.dat'!";
		cout << endl;
		return;
	}

	for (int i = 0; i < LEADERBOARD_SIZE; i++) {
		leaderBoardFile << easyModeLeaderboardNames[i] << '|' << easyModeLeaderboardScores[i] << '|';
	}
	for (int i = 0; i < LEADERBOARD_SIZE; i++) {
		leaderBoardFile << hardModeLeaderboardNames[i] << '|' << hardModeLeaderboardScores[i] << '|';
	}

	leaderBoardFile.close();
}

int main()
{
	srand(time(0));
	loadLeaderboards();
	printMenu();
	system("pause");
	return 0;
}

// TODO: Make sure that no spaces pattern matches
// TODO: ADD timer. Take code from here https://stackoverflow.com/questions/728068/how-to-calculate-a-time-difference-in-c
