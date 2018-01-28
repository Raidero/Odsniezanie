#include "Graph.h"
namespace graph {
    int n_vertices;
    int max_fuel;
    int base;
    double** edges;
    double total_distance;
    int radius;
    bool show_solution;

    Path::Path(char* file_name)
    {
        srand(time(nullptr));
        std::fstream file;
        int number_of_edges;
        file.open(file_name);

        file >> n_vertices >> number_of_edges >> max_fuel >> base;

        edges = new double* [n_vertices];
        for(int i = 0; i < n_vertices; ++i)
        {
            edges[i] = new double [n_vertices];
        }
        for(int i = 0; i < n_vertices; ++i)
        {
            for(int j = 0; j < n_vertices; ++j)
            {
                edges[i][j] = -1.0;
            }
        }

        total_distance = 0;
        for(int i = 0; i < number_of_edges; ++i)
        {
            int x, y;
            file >> x >> y;
            file >> edges[x][y];
            edges[y][x] = edges[x][y];
            total_distance += edges[x][y];
        }
        std::cout<<total_distance;
        for(int i = 0; i < n_vertices; ++i)
        {
            for(int j = 0; j < n_vertices; ++j)
            {
                if(edges[i][j] < 0.0)
                {
                    edges[i][j] = total_distance;
                }
            }
        }

        //shufflePath(rand()%(n_vertices*n_vertices));

        radius = 1;
        show_solution = false;
    }

    Path::~Path() {}

    void Path::deleteEdgeMap()
    {
        for(int i = 0; i < n_vertices; ++i)
        {
            delete[] edges[i];
        }
        delete[] edges;
    }

    void Path::shufflePath(int number_of_elements)
    {
        int elements = number_of_elements + 2;
        for(int i = 0; i < elements; ++i)
        {
            if(i == 0 || i == elements - 1)
            {
                /*if(i == elements - 1)
                {
                    while(!path.empty())
                    {
                        path.pop_back();
                    }
                    i = -1;
                    continue;
                }*/
                path.push_back(base);
                continue;
            }
            int element;
            //do{
            element = rand() % n_vertices;
            //} while(edges[path[i-1]][element] >= total_distance );
            path.push_back(element);
            //if(element == base)
            //    break;
        }
    }

    double Path::cost_function() const
    {
        double fuel = 0, distance_made = 0, without_fuel = 0;
        int edges_not_visited = 0;
        for(int i = 0; i < (int)path.size() - 1; ++i)
        {
            if(path[i] == base)
            {
                if(fuel < 0.0)
                    without_fuel -= fuel;
                fuel = max_fuel;
            }
            if(path[i] > path[i+1])
            {
                if(edges[path[i]][path[i+1]] < total_distance)
                    edges[path[i]][path[i+1]] = -1.0;
                distance_made += edges[path[i+1]][path[i]];
                fuel -= edges[path[i+1]][path[i]];
            }
            else
            {
                if(edges[path[i]][path[i+1]] < total_distance)
                    edges[path[i+1]][path[i]] = -1.0;
                distance_made += edges[path[i]][path[i+1]];
                fuel -= edges[path[i]][path[i+1]];
            }
        }
        if(fuel < 0.0)
            without_fuel -= fuel;
        for(int i = 0; i < n_vertices; ++i)
        {
            for(int j = i + 1; j < n_vertices; ++j)
            {
                if(edges[i][j] != edges[j][i])
                {
                    edges[j][i] = edges[i][j];
                }
                else if(edges[i][j] < total_distance)
                {
                    ++edges_not_visited;
                }
            }
        }
        if(show_solution)
        {
            std::fstream file1, file2;
            file1.open(PATH_INFO_FILE_NAME, std::ios::ate | std::ios::in | std::ios::out);
            file2.open(OBJECTIVE_FUNCTION_FILE_NAME, std::ios::ate | std::ios::in | std::ios::out);

            if(without_fuel < EPSILON)
            {
                file1 << "Without fuel: 0, Edges not visited: " << edges_not_visited << ", Distance made: " << distance_made << '\n';
            }
            else
            {
                file1 << "Without fuel: " << without_fuel << ", Edges not visited: " << edges_not_visited << ", Distance made: " << distance_made << '\n';
            }
            //file1 << this->getPath();
            file2 << A*without_fuel + B*edges_not_visited + C*distance_made << '\n';
            file1.close();
            file2.close();

            //std::cout << edges_not_visited << ' ' << distance_made << '\n';
        }
        return A*without_fuel + B*edges_not_visited + C*distance_made;
    }

    void Path::setPath(int position, int vertex)
    {
        if( position >= (int)path.size()  ||  (vertex == -1  &&  position >= (int)(path.size() - 1)) )
            return;
        std::vector<int>::iterator it = path.begin();
        while(position--)
        {
            ++it;
        }
        if(vertex >= 0)
        {
            path.insert(it, vertex);
        }
        else
        {
            path.erase(it);
        }
    }

    double Path::findBetterNeighbour()
    {
        graph::Path p;
        int pathSize = this->getPathSize();
        double current_cost = this->cost_function();
        const int r = radius;
        int *change_positions = new int[r];
        int *change_vertices = new int[r];

        for(int i = 0; i < r; ++i)
        {
            change_positions[i] = 1;
            change_vertices[i] = -1;
        }

        while(change_positions[0] != 0)
        {
            p = *this;
            for(int i = 0; i < r; ++i)
            {
                p.setPath(change_positions[i], change_vertices[i]);
            }
            if(p.cost_function() + EPSILON < current_cost)
            {
                (*this) = p;
                return this->cost_function();
            }
            ++change_vertices[0];
            for(int i = 0; i < r; ++i)
            {
                if(change_vertices[i] == n_vertices)
                {
                    change_vertices[i] = -1;
                    ++change_positions[i];
                }
                if(change_positions[i] == (pathSize + i))
                {
                    if((i+1) == r)
                    {
                        return p.cost_function();
                    }
                    change_positions[i] = 1;
                    ++change_vertices[i+1];
                }
            }
        }

        delete [] change_positions;
        delete [] change_vertices;
        return p.cost_function();
    }

    void Path::invertShow()
    {
        show_solution = !show_solution;
    }

    void Path::setRange(int r)
    {
        radius = r;
    }

    std::vector<int> Path::getPathVector() const
    {
        return path;
    }

    std::string Path::getPath() const
    {
        std::string path_string;
        for(int i = 0; i < (int)path.size(); ++i)
        {
            path_string.append(std::to_string(path[i]));
            path_string += '<';
        }
        path_string.pop_back();
        path_string += '\n';
        return path_string;
    }

    int Path::getPathSize() const
    {
        return (int)path.size();
    }
}
