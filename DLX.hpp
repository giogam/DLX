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


#include <iostream>
#include <string>
#include <regex>
#include <array>
#include <memory>
#include <list>
#include <bitset>
#include <vector>

class DataObject
{

public:
	virtual ~DataObject() = default;

	std::shared_ptr<DataObject> L;
	std::shared_ptr<DataObject> R;
	std::shared_ptr<DataObject> U;
	std::shared_ptr<DataObject> D;
	std::shared_ptr<DataObject> C;
};

class ColumnObject : public DataObject
{
public:

	inline auto set_id(int id) { m_id = id; }
	inline auto increment() { m_size++; }
	inline auto decrement() { m_size--; }
	inline auto  get_size() { return m_size; }
	inline auto  get_id() { return m_id; }

private:
	std::size_t m_size = 0;
	int m_id = -1;
};

struct DLXResult
{
	int number_of_solutions = 0;
	std::vector<std::vector<std::shared_ptr<DataObject>>> solutions;
};

struct DLXState
{
	bool stopped = false;
	std::vector<std::shared_ptr<DataObject>> stack;
};


class DLXSolver
{
public:
	explicit DLXSolver(std::vector<std::vector<int>>  &set);
	
	
	void DLXSolver::solve(DLXResult &result);
	
	

private:
	void DLXSolver::search(DLXResult &result, DLXState &state);
	auto link_to_column(std::shared_ptr<DataObject> &obj, int col, std::vector<std::shared_ptr<DataObject>> &last_obj_linked);
	auto link_row(std::vector<std::shared_ptr<DataObject>> &row);
	auto seal_matrix(std::vector<std::shared_ptr<DataObject>> &last_obj_linked);
	auto next_column();
	auto cover_column(std::shared_ptr<DataObject> &C);
	auto uncover_column(std::shared_ptr<DataObject> &C);

	std::vector<std::vector<std::shared_ptr<DataObject>>> MATRIX;
	std::shared_ptr<DataObject> H;
};
