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

template <class T, std::size_t N>
std::ostream& operator<<(std::ostream& o, const std::array<T, N>& arr)
{
	std::copy(arr.cbegin(), arr.cend(),std::ostream_iterator<T>(o, " "));
	return o;
}


int main( int argc, char **argv)
{

	auto print_sol = [](DLXResult &result)
	{
		std::array<int,7> set_row = { 0, 0, 0, 0, 0, 0, 0 };

		for (const auto sol : result.solutions)
		{
			for (const auto el : sol)
			{
				auto e = el;
				do
				{
					set_row[std::dynamic_pointer_cast<ColumnObject>(e->C)->get_id()] =  1;
					e = e->R;
				} while (e != el);
				std::cout << set_row << std::endl;
				set_row.fill(0);
			}
		}
	};

	std::vector<std::vector <int>> set = 
	{ 
		{0, 0, 1, 0, 1, 1, 0},
		{1, 0, 0, 1, 0, 0, 1},
		{0, 1, 1, 0, 0, 1, 0},
		{1, 0, 0, 1, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 1},
		{0, 0, 0, 1, 1, 0, 1} 
	};
	
	DLXSolver Solver(set);

	DLXResult result;
	Solver.solve(result);

	print_sol(result);

	return 0;
}
