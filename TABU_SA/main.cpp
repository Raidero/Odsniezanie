#include <iostream>
#include <Neighborhood.h>
#define TABU_SIZE 100
#define MAX_ITERATION_WITHOUT_EFFECT 10000
#define MINIMUM_EFFECT 100

int main(int argc, char** argv)
{
    if(argc > 1)
    {
        graph::Path problem_instance = graph::Path(argv[1]);
        std::cout << "Poczatkowa funkcja celu: " << problem_instance.cost_function() << '\n';

        graph::Path best = problem_instance;
        Neighborhood nei;

        mets::best_ever_criteria aspiration_criteria; // how we go up
        mets::noimprove_termination_criteria crit(MAX_ITERATION_WITHOUT_EFFECT, MINIMUM_EFFECT); //our criteria when to finish
        mets::best_ever_solution best_recorded(best); // best solution

        //tabu
        mets::simple_tabu_list tabu_list(TABU_SIZE);
        mets::tabu_search<Neighborhood> tabuAlgorithm(problem_instance, best_recorded, nei, tabu_list, aspiration_criteria, crit);
        try {
            tabuAlgorithm.search();
        }catch(mets::no_moves_error ex)
        {
            std::cout<<ex.what()<<'\n';
        }

        //wyniki
        std::cout << "Best solution: " << best_recorded.best_seen().cost_function()  << '\n';
        graph::Path path = dynamic_cast<const graph::Path&>(best_recorded.best_seen());
        std::cout << path.getPath() << '\n';

        //symulowane costam
        /*mets::exponential_cooling cooler(0.99);
        //mets::ex
        mets::simulated_annealing<Neighborhood> saAlgorithm(problem_instance, best_recorded, nei, crit, cooler, 10, 0.001);
        saAlgorithm.search();
        //wyniki
        std::cout << "Best solution: " << problem_instance.cost_function()  << '\n';
        std::cout << problem_instance.getPath() << '\n';*/
    }
    else
    {
        return -1;
    }
    return 0;
}
