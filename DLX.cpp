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

#include "DLX.hpp"

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// BUILD MATRIX /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

auto DLXSolver::seal_matrix(std::vector<std::shared_ptr<DataObject>> &last_obj_linked)
{
	for (auto i = 0; i<last_obj_linked.size(); ++i)
	{
		MATRIX[0][i]->U = last_obj_linked[i];
		last_obj_linked[i]->D = MATRIX[0][i];
	}
}

auto DLXSolver::link_to_column(std::shared_ptr<DataObject> &obj, int col,
	std::vector<std::shared_ptr<DataObject>> &last_obj_linked)
{
	obj->C = MATRIX[0][col];
	last_obj_linked[col]->D = obj;
	obj->U = last_obj_linked[col];

	last_obj_linked[col] = obj;
}

auto DLXSolver::link_row(std::vector<std::shared_ptr<DataObject>> &row)
{
	for (auto i = 0; i < row.size(); ++i)
	{
		std::size_t offset = ((i + 1) % (row.size()));

		row[i]->R = row[offset];
		row[offset]->L = row[i];
	}
}

DLXSolver::DLXSolver(std::vector<std::vector<int>>  &set)
{
	std::vector<std::shared_ptr<DataObject>> colObjects;
	std::vector<std::shared_ptr<DataObject>> lastObjLinked;

	for (auto i = 0; i < set[0].size(); ++i)
	{
		auto nvCol = std::make_shared<ColumnObject>();
		nvCol->set_id(i);
		colObjects.push_back(nvCol);
		lastObjLinked.push_back(nvCol);
	}
	link_row(colObjects);
	MATRIX.push_back(colObjects);

	for (const auto &set_row : set)
	{
		std::vector<std::shared_ptr<DataObject>> rowObjects;

		for (auto i = 0; i < set[0].size(); ++i)
		{
			if (set_row[i] == 1)
			{
				auto nvObj = std::make_shared<DataObject>();
				link_to_column(nvObj, i, lastObjLinked);
				std::dynamic_pointer_cast<ColumnObject>(colObjects[i])->increment();
				rowObjects.push_back(nvObj);
			}
		}

		link_row(rowObjects);
		MATRIX.push_back(rowObjects);
	}

	seal_matrix(lastObjLinked);

	H = std::make_shared<DataObject>();
	H->R = MATRIX[0][0];
	MATRIX[0][0]->L = H;
	H->L = MATRIX[0][colObjects.size() - 1];
	MATRIX[0][colObjects.size() - 1]->R = H;


}

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// SOLVE ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

auto DLXSolver::cover_column(std::shared_ptr<DataObject> &C)
{
	C->R->L = C->L;
	C->L->R = C->R;

	for (auto i = C->D; i != C; i = i->D)
	{
		for (auto j = i->R; j != i; j = j->R)
		{
			j->D->U = j->U;
			j->U->D = j->D;
			std::dynamic_pointer_cast<ColumnObject>(j->C)->decrement();
		}
	}
}

auto DLXSolver::uncover_column(std::shared_ptr<DataObject> &C)
{
	for (auto i = C->U; i != C; i = i->U)
	{
		for (auto j = i->L; j != i; j = j->L)
		{
			std::dynamic_pointer_cast<ColumnObject>(j->C)->increment();
			j->D->U = j;
			j->U->D = j;
		}
	}

	C->R->L = C;
	C->L->R = C;
}

auto DLXSolver::next_column()
{
	std::vector<std::shared_ptr<DataObject>> column_set;

	for (auto j = H->R; j != H; j = j->R)
	{
		if (!column_set.empty() &&
			(std::dynamic_pointer_cast<ColumnObject>(j)->get_size() <
				std::dynamic_pointer_cast<ColumnObject>(column_set[0])->get_size()))
		{
			column_set.clear();
		}
		if (column_set.empty() ||
			(std::dynamic_pointer_cast<ColumnObject>(j)->get_size() ==
				std::dynamic_pointer_cast<ColumnObject>(column_set[0])->get_size()))
		{
			column_set.push_back(j);
		}
	}

	return column_set[0];
}


void DLXSolver::search(DLXResult &result, DLXState &state)
{
	if (state.stopped) return;

	if (H->R == H)
	{
		result.number_of_solutions++;
		result.solutions.push_back(state.stack);

		if (result.number_of_solutions >= 1)
		{
			state.stopped = true;
		}

		return;
	}

	auto c = next_column();
	/*if (std::dynamic_pointer_cast<ColumnObject>(c)->get_size() < 1) 
	{
		std::cout << "ZERO" << std::endl;
		return;
	}*/
	cover_column(c);
	for (auto r = c->D; r != c; r = r->D)
	{
		state.stack.push_back(r);
		for (auto j = r->R; j != r; j = j->R)
			cover_column(j->C);

		search(result, state);
		
		state.stack.pop_back();
		
		for (auto j = r->L; j != r; j = j->L)
			uncover_column(j->C);
		
	}
	uncover_column(c);
}

void DLXSolver::solve(DLXResult &result)
{
	auto state = DLXState();
	search(result, state);
	int ii = 0;
}


