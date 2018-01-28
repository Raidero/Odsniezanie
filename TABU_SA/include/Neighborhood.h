#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H
#include <Comparator.h>

class Neighborhood
{
public:
    typedef std::vector<Vertex*>::iterator iterator;

    Neighborhood() {}
    virtual ~Neighborhood()
    {
        for(iterator it = begin(); it != end(); ++it)
	      delete (*it);
    }

    iterator begin() { return moves.begin(); }
    iterator end() { return moves.end(); }

    void refresh(const mets::feasible_solution& sol)
    {
        for(iterator it = begin(); it != end(); ++it)
	      delete (*it);
        while(!moves.empty())
        {
            moves.pop_back();
        }
        const graph::Path& p = dynamic_cast<const graph::Path&>(sol);
        const int vertices = p.getNumberOfVertices();
        const int pathSize = p.getPathSize();
        for(int i = 1; i < pathSize; ++i)
        {
            for(int j = 0; j < vertices; ++j)
            {
                moves.push_back(new Vertex(i, j, p.getPathVector()));
            }
            if(i < pathSize - 1)
            {
                moves.push_back(new Vertex(i, p.getPathVector()));
            }
        }
    }

private:
    std::vector<Vertex*> moves;
};

#endif // NEIGHBORHOOD_H
