#include "stdafx.h"
#include <iostream>
#include <fstream> 
#include <string>
#include <queue>

const int C_MATRIX_SIZE = 100;
typedef std::queue<std::pair<int, int>> IntPairQueue;

struct SFillingMatrix
{
	char m_matrix[C_MATRIX_SIZE + 2][C_MATRIX_SIZE + 2];
	IntPairQueue m_queue;
	int m_lineCount;

	SFillingMatrix()
		:m_queue()
		,m_matrix()
		,m_lineCount(0)
	{
	}

	void InitRowFromString(std::size_t& i, std::size_t& j, const std::string &readLine)
	{
		if (j < readLine.length() && !readLine.empty())
		{
			for (char chr : readLine)
			{
				if (j > C_MATRIX_SIZE)
				{
					break;
				}

				if (chr == '#' || chr == 'O')
				{
					m_matrix[i][j] = chr;
				}
				else
				{
					m_matrix[i][j] = ' ';
				}

				j++;
			}
		}
		else
		{
			m_matrix[i][j] = '\0';
			j++;
		}
	}

	bool InitMatrixFromFile(const std::string& fileName)
	{
		std::ifstream inputFile(fileName);
		if (inputFile.is_open())
		{
			std::string readLine;

			for (std::size_t i = 1; i <= C_MATRIX_SIZE; i++)
			{
				if (!std::getline(inputFile, readLine))
				{
					readLine.clear();
				}
				else
				{
					m_lineCount++;
				}
				for (std::size_t j = 1; j <= C_MATRIX_SIZE;)
				{
					InitRowFromString(i, j, readLine);
				}
			}

			CreateBorder();

			return true;
		}

		return false;
	}

	void CreateBorder()
	{
		for (std::size_t i = 0; i < C_MATRIX_SIZE + 2; i++)
		{
			m_matrix[i][0] = '#';
			m_matrix[i][101] = '#';
		}

		for (std::size_t j = 0; j < C_MATRIX_SIZE + 2; j++)
		{
			m_matrix[0][j] = '#';
			m_matrix[101][j] = '#';
		}
	}

	void StartWave(int i, int j) 
	{
		if (m_matrix[i][j] != '#' && m_matrix[i][j] != '.')
		{
			if (m_matrix[i][j] == ' ' || m_matrix[i][j] == '\0')
			{
				m_matrix[i][j] = '.';
			}

			m_queue.push(std::make_pair(i - 1, j));
			m_queue.push(std::make_pair(i + 1, j));
			m_queue.push(std::make_pair(i, j - 1));
			m_queue.push(std::make_pair(i, j + 1));
		}

		return;
	}

	void FillMatrix() 
	{
		for (std::size_t i = 1; i <= C_MATRIX_SIZE; i++)
		{
			for (std::size_t j = 1; j <= C_MATRIX_SIZE; j++)
			{
				if (m_matrix[i][j] == 'O')
				{
					m_queue.push(std::make_pair(i, j));
				}
			}
		}

		while (!m_queue.empty())
		{
			StartWave(m_queue.front().first, m_queue.front().second);
			m_queue.pop();
		}
	}

	bool WriteInFile(const std::string& fileName)
	{
		std::ofstream outputFile(fileName);
		std::size_t j;

		for (std::size_t i = 1; i <= C_MATRIX_SIZE; i++)
		{
			j = 1;
			for (j; j <= C_MATRIX_SIZE; j++)
			{
				if (m_matrix[i][j] == '\0')
				{
					break;
				}

				outputFile << m_matrix[i][j];
			}

			if (m_lineCount != 0 || j != 1)
			{
				outputFile << std::endl;
				m_lineCount--;
			}
		}

		if (!outputFile.flush())
		{
			return false;
		}

		return true;
	}
};

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Error: inputted incorrect number of parametrs!" << std::endl << "Usage: fill.exe <input file> <output file>" << std::endl;
		return 1;
	}

	SFillingMatrix fillingMatrix;
	if (fillingMatrix.InitMatrixFromFile(argv[1]))
	{
		fillingMatrix.FillMatrix();
		if (!fillingMatrix.WriteInFile(argv[2]))
		{
			std::cout << "Error: cant' write filling matrix in file!" << std::endl;
			return 1;
		}

		return 0;
	}
	else
	{
		std::cout << "Incorrect input file name!" << std::endl;
		return 1;
	}
}

