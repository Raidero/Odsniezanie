#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <fstream>
#include <mets.hh>
#include <stdlib.h>
#include <time.h>
#include <string>

#define A 64.0  //fuel
#define B 16.0  //edges not used
#define C 1.0   //distance made
namespace graph{

    class Path: public mets::evaluable_solution {
    public:
        Path(char* file_name);
        virtual ~Path();
        void shufflePath();
        mets::gol_type cost_function() const;
        mets::gol_type cost_function(int position, int vertex) const;
        void copy_from(const copyable& copy);
        void setPath(int position, int vertex);
        int getNumberOfVertices() const;
        int getPathSize() const { return (int)path.size();}
        bool getConnection(int i, int j) const;
        int getVertex(int i) const { return path[i]; }
        std::string getPath() const
        {
            std::string path_string;
            for(int i = 0; i < (int)path.size(); ++i)
            {
                path_string.append(std::to_string(path[i]));
                path_string += ' ';
            }
            return path_string;
        }
        std::vector<int> getPathVector() const { return path; }
    private:
        std::vector<int> path;
    };
}


#endif // GRAPH_H
