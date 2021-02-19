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
bool CheckConstraints();

bool While_Solving_Check_Coords(const int &, const int&  ,const int & );
void SolvePuzzle();

bool CheckConstraintsToBuild(const vector<vector<int> >& ,const int&, const int&, const int&);


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
		// build constraints matrix (using static arrays)

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
	if (CheckConstraints())
	{
		cout << "Constraints do not match board" << endl;
		return 6;
	}



	//NOW ALL CHECKS ARE DONE FOR PUZZLE TO BE SOLVED, AFTER SOLUTION, X AND CONSTRAINTS ARE GONNA BE CHECKED
	//cout << "SOLVING------ " << endl;
	SolvePuzzle();



	if (solutions > 1)
	{
		cout << "X's can have more than 1 value" << endl;
		return 7;
	}
	if (solutions == 1)
	{
		cout << "Files are in correct format"<< endl;
		PrintBoard(solved_board_matrix);
	}

	return 0;
}


void PrintBoard(const vector <vector<int>> &board)
{
	for (int i = 0; i < board.size();i++)
	{
		for (int j = 0; j < board[0].size();j++)
		{
			cout << board[i][j] << " ";
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
bool CheckConstraints()
{
	int x1, x2, y1, y2;
	for (int i = 0; i < constraint_matrix.size();i++)
	{
		x1 = constraint_matrix[i][0] - 1;
		y1 = constraint_matrix[i][1] - 1;

		x2 = constraint_matrix[i][2] - 1;
		y2 = constraint_matrix[i][3] - 1;

		if (board_matrix[x1][y1] != 0 && board_matrix[x2][y2] != 0)
		{
			if (board_matrix[x1][y1] < board_matrix[x2][y2])
			{
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
			if (board_matrix[i][j] == 0)							
			{
				row = i;
				column = j;
				for (int value = 1; value <= board_matrix.size(); value++)		
				{
					if (While_Solving_Check_Coords(value, row, column) )			//check if that row and column is suitable for value			
					{	
						if ( CheckConstraintsToBuild(board_matrix, value, row, column))
						{
							board_matrix[i][j] = value;				// place "value" in that location
							SolvePuzzle();							//backtrack
							board_matrix[i][j] = 0;					//to try again, replace initial X with 0 again
						}
					}
				}
				return;
			}
		}
	}
	solutions++;
	if (solutions == 1)
	{
		solved_board_matrix = board_matrix;				//the board previous than last one is the correct one (last one contains 0s again)
	}													// we are gonna print solved_matrix_board at the end
}
bool CheckConstraintsToBuild(const vector<vector<int> >&board, const int &value, const int& value_row, const int& value_column)  //)
{
	bool flag = true;

	int x1, x2, y1, y2;
	for (int i = 0; i < constraint_matrix.size();i++)
	{
		 x1 = constraint_matrix[i][0]-1;
		 y1 = constraint_matrix[i][1]-1;

		 x2 = constraint_matrix[i][2]-1;
		 y2 = constraint_matrix[i][3]-1;
		if(x1 == value_row && y1 == value_column )
		{ 
			if (value > board_matrix[x2][y2])				//if the "value" which is a possible X value, is greater than x2y2, then put it in board
			{
				flag = true;
			}
			else
				flag = false;
		}
		else if (x2 == value_row && y2 == value_column)
		{
			if (value < board_matrix[x1][y1])				//if the "value" which is a possible X value, is less than x1y1, then put it in board
			{			
				flag = true;
			}
			else
				flag = false;
		}
	}
	return flag;
}
