#ifndef VERTEX_H
#define VERTEX_H
#include <Graph.h>

class Vertex: public mets::mana_move
{
public:
    Vertex(int p, int v,const std::vector<int> npath): position(p), vertex(v) {
        for(int i = 0; i < (int)npath.size(); ++i)
        {
             path.push_back(npath[i]);
        }
    }
    Vertex(int p, std::vector<int> npath): position(p), vertex(-1) {
        for(int i = 0; i < (int)npath.size(); ++i)
        {
            path.push_back(npath[i]);
        }

    }
    virtual ~Vertex() {}

    mets::gol_type evaluate(const mets::feasible_solution& sol) const
    {
        const graph::Path& p = dynamic_cast<const graph::Path&>(sol);
        return p.cost_function(position, vertex);
    }
    void apply(mets::feasible_solution& sol) const
    {
        graph::Path& p = dynamic_cast<graph::Path&>(sol);
        p.setPath(position, vertex);
    }

    mana_move* clone() const { return new Vertex(position, vertex, path); }

    size_t hash() const { return position; }

    bool operator==(const mana_move& other) const
    {
        const Vertex& other_path = dynamic_cast<const Vertex&>(other);
        if( other_path.path.size() != path.size() )
            return false;
        for(int i = 0; i < (int)other_path.path.size(); ++i)
        {
            if(path[i] != other_path.path[i])
                return false;
        }
        return (this->position == other_path.position && this->vertex == other_path.vertex);
    }
private:
    int position;
    int vertex;
    std::vector<int> path;
};

#endif // COMPARATOR_H
