// ALPER ARI 27017

#include <iostream>		
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include "strutils.h"
using namespace std;

//our board and constraints matrices
vector <vector<int>> board_matrix;
vector <array <int, 4>> constraint_matrix; //before adding new array, create new array
vector <vector<int>> solved_board_matrix;

unsigned short int boardSize;
unsigned short int constraintSize;

void PrintBoard(const vector <vector<int>>&);
void PrintConst();


// ------------------------CHECKS THAT WILL BE DONE------------------------------
bool CheckBoardSize();
bool CheckConstSize();
bool CheckRepeatedInRow();
bool CheckRepeatedInColumn();
bool CheckBoardValues();

bool While_Solving_Check_Coords(const int &, const int&  ,const int & );
void SolvePuzzle();

bool CheckConstraints(const vector<vector<int> >&);


int solutions = 0;

int main()
{

	string board_name, const_name, line;
	cout << "Enter the board file: "; cin >> board_name;



	//open board file
	ifstream file;
	file.open(board_name.c_str());
	if (file.fail())
	{
		cout << "Could not open board file." << endl;
		return 1;
	}
	else
	{

		//1st line is board size, getline gonna be used only once
		getline(file, line);								 //getline, reads as string, always
		boardSize = atoi(line);								//update boardSize

		//now enter while loop to build board matrix itself
		string currentElement;
		vector<int> row;
		while (getline(file, line))						//while getting lines
		{
			istringstream myStream(line);
			while (myStream >> currentElement)				 //while stream reads until end of line
			{
				if (currentElement != "X")
				{
					row.push_back(atoi(currentElement));				 //add current int to row vector, later add row to board
				}
				else
				{
					row.push_back(0);
				}
			}
			board_matrix.push_back(row);row.clear();			//add vector<string> row to main board matrix
		}
		//PrintBoard();
		file.close();
	}


	//open constraints file
	cout << "Enter the constraint file: "; cin >> const_name;

	file.open(const_name.c_str());
	if (file.fail())
	{
		cout << "Could not open const file." << endl;
		return 2;
	}
	else
	{
		// build constraints matrix (using static vectors)

		getline(file, line);							//get the first line and update constraintSize
		constraintSize = atoi(line);

		int current;
		array<int, 4> row;
		while (getline(file, line))
		{
			if (line != "")
			{
				istringstream myStream(line);
				for (int i = 0; i < 4; i++)
				{
					myStream >> current;
					row[i] = current;
				}
				constraint_matrix.push_back(row);
			}
		}
		cout << endl;
		//PrintConst();
		file.close();
	}
	// ------------------- BOARD AND CONSTRAINT MATRICES ARE BUILT UNTIL NOW-----------------------------------
	//PrintBoard(board_matrix);


	// TIME FOR COUPLE OF CHECKS------------------------------

	if (!CheckBoardSize())
	{
		cout << "File contains more lines than size" << endl;
		return 1;
	}
	if (!CheckConstSize())
	{
		cout << "There are more constraints than previously stated" << endl;
		return 2;
	}
	if (CheckRepeatedInRow())
	{
		cout << "There is a value repeating in same row" << endl;
		return 3;
	}
	if (CheckRepeatedInColumn())
	{
		cout << "There is a value repeating in same column" << endl;
		return 4;
	}
	if (CheckBoardValues())
	{
		return 5;
	}



	//NOW ALL CHECKS ARE DONE FOR PUZZLE TO BE SOLVED, AFTER SOLUTION, X AND CONSTRAINTS ARE GONNA BE CHECKED
	//cout << "SOLVING------ " << endl;
	SolvePuzzle();


	//cout << endl << "x: " << x << endl;

	if (solutions == 1)
	{
		if (CheckConstraints(solved_board_matrix))
		{
			cout << "Files are in correct format" << endl;
			PrintBoard(solved_board_matrix);

		}
		else
		{
			cout << "Constraints do not match board" << endl;
			return 1;
		}
	}
	else
	{
		cout << "X's can have more than 1 value" << endl;
		return 1;
	}


	return 0;
}


void PrintBoard(const vector <vector<int>> &board)
{
	for (int i = 0; i < board.size();i++)
	{
		for (int j = 0; j < board[0].size();j++)
		{
			cout << board[i][j] << "\t";
		}
		cout << endl ;
	}
	cout << endl;
}
void PrintConst()
{
	for (int i = 0; i < constraint_matrix.size();i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << constraint_matrix[i][j] << "\t";
		}
		cout << endl;
	}
}

bool CheckBoardSize()
{
	return (board_matrix.size() == boardSize && board_matrix[0].size() == boardSize);
}
bool CheckConstSize()
{
	return constraint_matrix.size() == constraintSize;
}
bool CheckRepeatedInRow()
{
	int current;
	for (int i = 0; i < board_matrix.size();i++)
	{
		current = board_matrix[i][0];
		for (int j = 1; j < board_matrix[0].size();j++)
		{
			if (board_matrix[i][j] == current && current != 0)
				return true;
		}
	}
	return false;
}
bool CheckRepeatedInColumn()
{
	int current;
	for (int i = 0; i < board_matrix[0].size();i++)
	{
		current = board_matrix[0][i];
		for (int j = 1; j < board_matrix.size();j++)
		{
			if (board_matrix[j][i] == current && current != 0)
				return true;
		}
	}
	return false;
}
bool CheckBoardValues()
{
	
	for (int i = 0; i < board_matrix.size();i++)
	{
		for (int j = 0; j < board_matrix[0].size();j++)
		{
			if (board_matrix[i][j] > boardSize || board_matrix[i][j] <0)
			{
				cout << "Value " << board_matrix[i][j] << " is out of matrix size" << endl;
				return true;
			}
		}
	}
	return false;
}

bool While_Solving_Check_Coords(const int &value, const int &row,const int &column)
{
	bool possible = true;
	for (int i = 0; i < board_matrix[row].size();i++)
	{
		if (board_matrix[row][i] == value)
			possible = false;
	}
	for (int j = 0; j < board_matrix.size();j++)
	{
		if (board_matrix[j][column] == value)
		{
			possible = false;
		}
	}
	return possible;
}
void SolvePuzzle()
{
	int row, column;

	for (int i = 0; i < board_matrix.size();i++)
	{
		for (int j = 0; j < board_matrix[0].size(); j++)
		{
			if (board_matrix[i][j] == 0)										//if indexed value is X, then put values of [1,n] orderly
			{
				row = i;
				column = j;
				for (int value = 1; value <= board_matrix.size(); value++)		//replace X with "value"
				{
					if (While_Solving_Check_Coords(value, row, column))					// NOW CHECK IF THAT VALUE EXISTS IN THE SAME ROW
					{
						board_matrix[i][j] = value;								// place "value" in that location

						SolvePuzzle();
						
						board_matrix[i][j] = 0;
					}
				}
				return;
			}
		}
	}
	solutions++;
	if (solutions == 1)
	{
		solved_board_matrix = board_matrix;
	}
	//boardx.txt
	//const1.txt
}
bool CheckConstraints(const vector<vector<int> >&board)
{
	for (int i = 0; i < constraint_matrix.size();i++)
	{
		int x1 = constraint_matrix[i][0]-1;
		int y1 = constraint_matrix[i][1]-1;

		int x2 = constraint_matrix[i][2]-1;
		int y2 = constraint_matrix[i][3]-1;
		
		if (board[x1][y1] < board[x2][y2])
			return false;
	}
	return true;
}
