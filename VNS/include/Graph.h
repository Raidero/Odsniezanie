#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>

#define A 64.0  //fuel
#define B 32.0  //edges not used
#define C 1.0   //distance made

#define EPSILON 0.001
#define OBJECTIVE_FUNCTION_FILE_NAME "objective function.txt"
#define PATH_INFO_FILE_NAME "paths.txt"

namespace graph{
    class Path{
    public:
        Path() {}
        Path(char* file_name);
        ~Path();
        void deleteEdgeMap();
        Path& operator=(const Path &p)
        {
            if(this == &p)
               return *this;
            while(!this->path.empty())
            {
                this->path.pop_back();
            }
            const int n = p.path.size();
            for(int i = 0; i < n; ++i)
            {
                this->path.push_back(p.path[i]);
            }
            return *this;
        }
        void shufflePath(int number_of_elements);
        double cost_function() const;
        std::string getPath() const;
        void setRange(int r);
        double findBetterNeighbour();
        void invertShow();
    private:
        bool getConnection(int i, int j) const;
        std::vector<int> getPathVector() const;
        int getPathSize() const;
        void setPath(int position, int vertex);

        std::vector<int> path;
    };
}


#endif // GRAPH_H
