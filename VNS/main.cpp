#include <iostream>
#include <Graph.h>

void vns(graph::Path& problem_instance, int k_max);

int main(int argc, char** argv)
{
    if(argc == 4)
    {
        graph::Path problem_instance = graph::Path(argv[1]);
        problem_instance.shufflePath(std::stoi(argv[2], nullptr, 10));

        std::vector<int> path;

        std::cout << "Poczatkowa funkcja celu: " << problem_instance.cost_function() << '\n';
        std::cout << problem_instance.getPath() << '\n';

        vns(problem_instance, std::stoi(argv[3], nullptr, 10));

        std::cout << "Best solution: " << problem_instance.cost_function()  << '\n';
        std::cout << problem_instance.getPath() << '\n';
        problem_instance.deleteEdgeMap();
    }
    else
    {
        std::cout << "Wrong number of arguments\n";
        return -1;
    }
    return 0;
}


void vns(graph::Path& problem_instance, int k_max)
{
    int k = 1;
    double current_solution = problem_instance.cost_function();
    while(k <= k_max)
    {
        double new_solution = problem_instance.findBetterNeighbour();
        if(new_solution < current_solution)
        {
            problem_instance.invertShow();
            problem_instance.cost_function();
            problem_instance.invertShow();

            current_solution = new_solution;
            k = 1;
            problem_instance.setRange(k);
        }
        else
        {
            ++k;
            problem_instance.setRange(k);
        }
    }
}
