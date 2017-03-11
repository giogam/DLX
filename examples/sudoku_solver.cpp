/******************************************************************************
MIT License

Copyright (c) 2017 Giorgio Gambino

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#include "..\DLX.hpp"

namespace SUDOKUConstants
{

	static const int SET_SIZE = 324;
	static const int COLUMN_SET_OFFSET = 81;
	static const int ROW_SET_OFFSET = 162;
	static const int SQUARE_SET_OFFSET = 243;

	static constexpr std::array<std::array<int, 9>, 9> REGION_MAP =
	{
		{
			{ 0,  1,  2,  9,  10, 11, 18, 19, 20 },
			{ 3,  4,  5,  12, 13, 14, 21, 22, 23 },
			{ 6,  7,  8,  15, 16, 17, 24, 25, 26 },
			{ 27, 28, 29, 36, 37, 38, 45, 46, 47 },
			{ 30, 31, 32, 39, 40, 41, 48, 49, 50 },
			{ 33, 34, 35, 42, 43, 44, 51, 52, 53 },
			{ 54, 55, 56, 63, 64, 65, 72, 73, 74 },
			{ 57, 58, 59, 66, 67, 68, 75, 76, 77 },
			{ 60, 61, 62, 69, 70, 71, 78, 79, 80 }
		}
	};

};

class DLXSudokuSolver : public DLXSolver
{
public:
	DLXSudokuSolver(const std::string &sudoku_problem);
	void solve_board(DLXResult &result);

private:
	auto loadBoard(const std::string &sudoku_problem);
	auto makeSet(const int i, const int j, const int value);
	auto canDoThisMove(const std::vector<int> &board, int row, int col, int value);
	inline auto getRegion(const int row, const int column) { return (3 * (row / 3) + (column / 3)); };
};




void DLXSudokuSolver::solve_board(DLXResult &result)
{
	solve(result);
}


auto DLXSudokuSolver::makeSet(const int row, const int column, const int value)
{
	std::vector<int> set(SUDOKUConstants::SET_SIZE);

	set[(row * 9) + column] = 1;
	set[SUDOKUConstants::ROW_SET_OFFSET + ((row * 9) + (value - 1))] = 1;
	set[SUDOKUConstants::COLUMN_SET_OFFSET + ((column * 9) + (value - 1))] = 1;
	set[SUDOKUConstants::SQUARE_SET_OFFSET + ((getRegion(row, column) * 9) + (value - 1))] = 1;

	return set;
}

auto DLXSudokuSolver::canDoThisMove(const std::vector<int> &board, int row, int col, int value)
{
	int region = getRegion(row, col);
	for (int i = 0; i < 9; ++i)
	{
		if ((board[(row * 9) + i] == value) || (board[(i * 9) + col] == value) || board[SUDOKUConstants::REGION_MAP[region][i]] == value) return false;
	}

	return true;
}

auto DLXSudokuSolver::loadBoard(const std::string &sudoku_problem)
{
	std::vector<std::vector<int>> set;
	std::regex words_regex("(\\d)");
	auto board_str = std::sregex_iterator(sudoku_problem.begin(), sudoku_problem.end(), words_regex);
	auto words_end = std::sregex_iterator();
	std::vector<int> board;

	for (std::sregex_iterator i = board_str; i != words_end; ++i)
	{
		board.push_back(std::stoi(i->str()));
		
	}

	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			int current = board[i* 9 + j];
			if (current !=0 )
			{
				set.push_back(makeSet(i, j, current));
			}
			else
			{
				for (int val = 0; val<9; ++val)
				{
					//if (canDoThisMove(board, i, j, val))
						set.push_back(makeSet(i, j, val));
				}
			}

		}
	}

	return set;

}

DLXSudokuSolver::DLXSudokuSolver(const std::string &sudoku_problem)
	:DLXSolver(loadBoard(sudoku_problem))
{

}


template <class T, std::size_t N>
std::ostream& operator<<(std::ostream& o, const std::array<T, N>& arr)
{
	std::copy(arr.cbegin(), arr.cend(), std::ostream_iterator<T>(o, " "));
	return o;
}


int main(int argc, char **argv)
{

	auto print_sol = [](DLXResult &result)
	{
		std::array<int, 7> set_row = { 0, 0, 0, 0, 0, 0, 0 };

		for (const auto sol : result.solutions)
		{
			for (const auto el : sol)
			{
				auto e = el;
				do
				{
					set_row[std::dynamic_pointer_cast<ColumnObject>(e->C)->get_id()] = 1;
					e = e->R;
				} while (e != el);
				std::cout << set_row << std::endl;
				set_row.fill(0);
			}
		}
	};

	std::string strBoard = "003020600900305001001806400008102900700000008006708200002609500800203009005010300";
	DLXSudokuSolver Solver(strBoard);

	DLXResult result;
	Solver.solve_board(result);

	print_sol(result);

	return 0;
}
