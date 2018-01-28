#include "Graph.h"
namespace graph {

    int n_vertices;
    int max_fuel;
    int base;
    double** edges;
    double total_distance;

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
        shufflePath();
    }

    Path::~Path() {}

    void Path::shufflePath()
    {
        int elements = (rand() % 2*n_vertices) + 4;
        int element;
        for(int i = 0; i < elements; ++i)
        {
            if(i == 0 || i == elements - 1)
            {
                if(i == elements - 1)
                {
                    while(!path.empty())
                    {
                        path.pop_back();
                    }
                    i = -1;
                    continue;
                }
                path.push_back(base);
                continue;
            }

            do{
                element = rand() % n_vertices;
            } while(edges[path[i-1]][element] >= total_distance );
            path.push_back(element);
            if(element == base)
                break;
        }
    }
    mets::gol_type Path::cost_function() const
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
        //std::cout << getPath() << '\n';
        //std::cout << "Without fuel: " << without_fuel << "\nEdges not visited: " << edges_not_visited << "\nDistance made: " << distance_made<< '\n';
        return A*without_fuel + B*edges_not_visited + C*distance_made;
    }

    void Path::copy_from(const copyable& copy)
    {
        const Path& g = dynamic_cast<const Path&>(copy);
        while(!path.empty())
        {
            path.pop_back();
        }
        for(int i = 0; i < (int)g.path.size(); ++i)
        {
            path.push_back(g.path[i]);
        }
    }

    mets::gol_type Path::cost_function(int position, int vertex) const
    {
        double fuel = 0, distance_made = 0, without_fuel = 0;
        int edges_not_visited = 0;
        for(int i = 0; i < (int)path.size() - 1; ++i)
        {
            if(i + 1 == position && vertex >= 0)
            {
                if(path[i] == base)
                {
                    if(fuel < 0.0)
                        without_fuel -= fuel;
                    fuel = max_fuel;
                }
                if(path[i] > vertex)
                {
                    if(edges[path[i]][vertex] < total_distance)
                        edges[path[i]][vertex] = -1.0;
                    distance_made += edges[vertex][path[i]];
                    fuel -= edges[vertex][path[i]];
                }
                else
                {
                    if(edges[path[i]][vertex] < total_distance)
                        edges[vertex][path[i]] = -1.0;
                    distance_made += edges[path[i]][vertex];
                    fuel -= edges[path[i]][vertex];
                }

                if(vertex == base)
                {
                    if(fuel < 0.0)
                        without_fuel -= fuel;
                    fuel = max_fuel;
                }
                if(vertex > path[i+1])
                {
                    if(edges[vertex][path[i+1]] < total_distance)
                        edges[vertex][path[i+1]] = -1.0;
                    distance_made += edges[path[i+1]][vertex];
                    fuel -= edges[path[i+1]][vertex];
                }
                else
                {
                    if(edges[vertex][path[i+1]] < total_distance)
                        edges[path[i+1]][vertex] = -1.0;
                    distance_made += edges[vertex][path[i+1]];
                    fuel -= edges[vertex][path[i+1]];
                }
                continue;
            }
            else if( i + 1 == position && vertex < 0)
            {
                if(path[i] == base)
                {
                    if(fuel < 0.0)
                        without_fuel -= fuel;
                    fuel = max_fuel;
                }
                if(path[i] > path[i+2])
                {
                    if(edges[path[i]][path[i+2]] < total_distance)
                        edges[path[i]][path[i+2]] = -1.0;
                    distance_made += edges[path[i+2]][path[i]];
                    fuel -= edges[path[i+2]][path[i]];
                }
                else
                {
                    if(edges[path[i]][path[i+2]] < total_distance)
                        edges[path[i+2]][path[i]] = -1.0;
                    distance_made += edges[path[i]][path[i+2]];
                    fuel -= edges[path[i]][path[i+2]];
                }
                ++i;
                continue;
            }
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
        //std::cout<<without_fuel<<' '<<edges_not_visited<<' '<<distance_made<<'\n';
        //std::cout << getPath() << '\n';
        return A*without_fuel + B*edges_not_visited + C*distance_made;
    }

    void Path::setPath(int position,int vertex)
    {
        typedef std::vector<int>::iterator iterator;
        iterator it = path.begin();
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

    int Path::getNumberOfVertices() const
    {
        return n_vertices;
    }

    bool Path::getConnection(int i, int j) const
    {
        return (edges[i][j] < total_distance);
    }
}
