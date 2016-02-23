#include "stdafx.h"
#include <iostream>
#include <fstream> 
#include <string>
#include <queue>

const int c_matrixSize = 100;
typedef std::queue<std::pair<int, int>> intPairQueue;

struct FillingMatrix
{
	char matrix[c_matrixSize + 2][c_matrixSize + 2];
	intPairQueue queue;
	int lineCount;

	FillingMatrix()
		:queue()
		,matrix()
		,lineCount(0)
	{
	}

	bool InitMatrixFromFile(const std::string& fileName)
	{
		std::ifstream inputFile(fileName);
		if (inputFile.is_open())
		{
			std::string readLine;

			for (std::size_t i = 1; i <= c_matrixSize; i++)
			{
				if (!std::getline(inputFile, readLine))
					readLine = "";
				else
					lineCount++;
				for (std::size_t j = 1; j <= c_matrixSize;)
				{
					if (j < readLine.length() && readLine != "")
						for (char chr : readLine)
						{
							if (j <= c_matrixSize)
							{
								if (chr == '#' || chr == 'O')
									matrix[i][j] = chr;
								else
									matrix[i][j] = ' ';
								j++;
							}
							else
								break;
						}
					else
					{
						matrix[i][j] = '\0';
						j++;
					}
				}
			}
			CreateBorder();
			return true;
		}
		return false;
	}

	void CreateBorder()
	{
		for (std::size_t i = 0; i < c_matrixSize + 2; i++)
		{
			matrix[i][0] = '#';
			matrix[i][101] = '#';
		}

		for (std::size_t j = 0; j < c_matrixSize + 2; j++)
		{
			matrix[0][j] = '#';
			matrix[101][j] = '#';
		}
	}

	void StartWave(int i, int j) 
	{
		if (matrix[i][j] != '#' && matrix[i][j] != '.')
		{
			if (matrix[i][j] == ' ' || matrix[i][j] == '\0')
				matrix[i][j] = '.';
			queue.push(std::make_pair(i - 1, j));
			queue.push(std::make_pair(i + 1, j));
			queue.push(std::make_pair(i, j - 1));
			queue.push(std::make_pair(i, j + 1));
		}

		return;
	}

	void FillMatrix() 
	{
		for (std::size_t i = 1; i <= c_matrixSize; i++)
		{
			for (std::size_t j = 1; j <= c_matrixSize; j++)
			{
				if (matrix[i][j] == 'O')
				{
					queue.push(std::make_pair(i, j));
				}
			}
		}

		while (!queue.empty())
		{
			StartWave(queue.front().first, queue.front().second);
			queue.pop();
		}
	}

	bool WriteInFile(const std::string& fileName)
	{
		std::ofstream outputFile(fileName);
		std::size_t j;

		for (std::size_t i = 1; i <= c_matrixSize; i++)
		{
			j = 1;
			for (j;j <= c_matrixSize; j ++)
			{
				if (matrix[i][j] == '\0')
					break;

				outputFile << matrix[i][j];
			}

			if (lineCount != 0 || j != 1)
			{
				outputFile << std::endl;
				lineCount--;
			}
		}

		if (!outputFile.flush())
			return false;

		return true;
	}
};

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Error: inputted incorrect number of parametrs!" << std::endl << "Usage: fill.exe <input file> <output file>";
		return 1;
	}

	FillingMatrix fillingMatrix;
	if (fillingMatrix.InitMatrixFromFile(argv[1]))
	{
		fillingMatrix.FillMatrix();
		if (!fillingMatrix.WriteInFile(argv[2]))
		{
			std::cout << "Error: cant' write filling matrix in file!";
			return 1;
		}

		return 0;
	}
	else
	{
		std::cout << "Incorrect input file name!";
		return 1;
	}
}

