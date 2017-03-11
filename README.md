# DLX
C++ 14 & STL Compliant - Exact Cover Solver - Quick usage example

```c++
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
```