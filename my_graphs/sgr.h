//
// Created by Kamil on 21/09/15.
//

#ifndef PDS1_SGR_H
#define PDS1_SGR_H


#include <iosfwd>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <queue>
//#include <algorithm>

class StaticVertex;
class StaticEdge;
class StaticGraph;
class TransitiveClosure;
class Tree;


class StaticVertex{
private:
    std::string name;
public:
    StaticVertex(std::string name);

    std::string getName();

    void setName(std::string name);
};

class StaticEdge{
private:
    StaticVertex* from;
    StaticVertex* to;
    long weight;
public:
    StaticEdge(StaticVertex* from, StaticVertex* to, long weight);
    long getWeight();

    void setWeight(long weight);

    StaticVertex *getFrom();

    void setFrom(StaticVertex *from);

    StaticVertex *getTo();

    void setTo(StaticVertex *to);

    std::string toString();
};

class StaticGraph{
protected:
    std::map<std::string,std::vector<StaticEdge*>*>* adj_list;
    std::set<StaticVertex*>* vertSet;
    StaticVertex* root; //
    std::map<std::string, StaticVertex*>* labelVert;
public:
    StaticGraph();

    StaticGraph(StaticGraph* g);

    virtual ~StaticGraph();

    virtual StaticVertex* getRoot();

    virtual void setRoot(StaticVertex* v);

    virtual std::set<StaticVertex*>* getVertSet();

    virtual std::map<std::string, StaticVertex*>* getLabelVertMap();

    virtual void add_edge(StaticVertex* from, StaticVertex* to, long weight);

    virtual void remove_edge(StaticVertex* from, StaticVertex* to);

    virtual StaticEdge* hasEdge(StaticVertex* from, StaticVertex* to);

    virtual std::string toString();

    //calculates shortest path between vertex u and v within given graph, using Dijkstra method

    virtual //returns shortest paths from source
    std::map<StaticVertex*, long>* dijkstra(StaticVertex* source);

    virtual //returns transitive closure, executing dijkstra |V| times
    TransitiveClosure* transitiveClosure();

    virtual Tree* alg3(TransitiveClosure* tr_cl, int i, int k, StaticVertex* root, std::set<StaticVertex*>* X);

    /*improved alg3*/
    virtual Tree* alg4(TransitiveClosure* tr_cl, int i, int k, StaticVertex* root, std::set<StaticVertex*>* X);

    virtual Tree* alg5(TransitiveClosure* tr_cl, int i, int k, StaticVertex* root, std::set<StaticVertex*>* X, StaticEdge* e);
};

/*
 * class that represents transitive closure of static graph
 * inherited from StaticGraph
 */
class TransitiveClosure : StaticGraph{
public:
    TransitiveClosure();

    virtual ~TransitiveClosure();

    virtual StaticVertex *getRoot() override;

    virtual void setRoot(StaticVertex *v) override;

    virtual std::set<StaticVertex*>* getVertSet() override;

    virtual std::map<std::string, StaticVertex*>* getLabelVertMap() override;

    virtual void add_edge(StaticVertex *from, StaticVertex *to, long weight) override;

    virtual void remove_edge(StaticVertex *from, StaticVertex *to) override;

    virtual StaticEdge* hasEdge(StaticVertex* from, StaticVertex* to) override;

    std::string toString() override;

    virtual std::map<StaticVertex *, long> *dijkstra(StaticVertex *source) override;

    virtual TransitiveClosure *transitiveClosure() override;

/*
     * returns cost in edge (u,v)
     * if there is no edge(u,v) return LONG_MAX
     */
    long costEdge(StaticVertex *u, StaticVertex* v);
    long costEdge(std::string u_name, std::string v_name);

};

/*
 * Tree class, has feature to calculate density
 */
class Tree : StaticGraph{
private:
    long totalWeight;
public:
    Tree();

    Tree(Tree* tree);

    virtual ~Tree();

    virtual StaticVertex *getRoot() override;

    virtual void setRoot(StaticVertex *v) override;

    virtual std::set<StaticVertex*>* getVertSet() override;

    virtual std::map<std::string, StaticVertex*>* getLabelVertMap() override;

    virtual void add_edge(StaticVertex *from, StaticVertex *to, long weight) override;

    virtual void remove_edge(StaticVertex *from, StaticVertex *to) override;

    virtual StaticEdge* hasEdge(StaticVertex* from, StaticVertex* to) override;

    virtual std::string toString() override;

    long getTotalWeight();

    static Tree* merge(Tree* t1, Tree* t2);
};

std::set<StaticVertex*>* vert_intersect(std::set<StaticVertex*>* s1, std::set<StaticVertex*>* s2);
std::set<StaticVertex*>* vert_minus(std::set<StaticVertex*>* s1, std::set<StaticVertex*>* s2);
#endif //PDS1_SGR_H
