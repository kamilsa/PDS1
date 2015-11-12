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
#include <memory>
//#include <algorithm>
using namespace std;

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

    StaticVertex(shared_ptr<StaticVertex> v);

    ~StaticVertex();

    std::string getName();

    void setName(std::string name);

    bool operator < (StaticVertex);

    bool operator > (StaticVertex);
};

class StaticEdge{
private:
    shared_ptr<StaticVertex> from;
    shared_ptr<StaticVertex> to;
    long weight;
public:
    StaticEdge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to, long weight);

    ~StaticEdge();

    long getWeight();

    void setWeight(long weight);

    shared_ptr<StaticVertex> getFrom();

    void setFrom(shared_ptr<StaticVertex> from);

    shared_ptr<StaticVertex> getTo();

    void setTo(shared_ptr<StaticVertex> to);

    std::string toString();
};

class StaticGraph{
    struct myEntry{
        double density;
        shared_ptr<StaticVertex> value;
    };
protected:
    std::map<std::string,std::vector<StaticEdge*>*>* adj_list;
    std::set<shared_ptr<StaticVertex>>* vertSet;
    shared_ptr<StaticVertex> root; //
    std::map<std::string, shared_ptr<StaticVertex>>* labelVert;

    void putInOrder(std::vector<myEntry*>* vec, myEntry* entr);
    int entrBinSearchOrNext(std::vector<myEntry*>* vec, myEntry* entr);
public:
    StaticGraph();

    StaticGraph(StaticGraph* g);

    virtual ~StaticGraph();

    virtual shared_ptr<StaticVertex> getRoot();

    virtual void setRoot(shared_ptr<StaticVertex> v);

    virtual std::set<shared_ptr<StaticVertex>>* getVertSet();

    virtual std::map<std::string, shared_ptr<StaticVertex>>* getLabelVertMap();

    virtual void add_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to, long weight);

    virtual void remove_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to);

    virtual StaticEdge* hasEdge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to);

    virtual std::string toString();

    //calculates shortest path between vertex u and v within given graph, using Dijkstra method

    virtual //returns shortest paths from source
    std::map<string, long>* dijkstra(shared_ptr<StaticVertex> source);

    virtual //returns transitive closure, executing dijkstra |V| times
    TransitiveClosure* transitiveClosure();

    virtual Tree* alg3(TransitiveClosure* tr_cl, int i, int k, shared_ptr<StaticVertex> root, std::set<shared_ptr<StaticVertex>>* X);

    /*improved alg3*/
    virtual Tree* alg4(TransitiveClosure* tr_cl, int i, int k, shared_ptr<StaticVertex> root, std::set<shared_ptr<StaticVertex>>* X);

    virtual Tree* alg5(TransitiveClosure* tr_cl, int i, int k, shared_ptr<StaticVertex> root, std::set<shared_ptr<StaticVertex>>* X, StaticEdge* e);

    virtual Tree* alg6(TransitiveClosure* tr_cl, int i, int k, shared_ptr<StaticVertex> root, std::set<shared_ptr<StaticVertex>>* X);

    virtual Tree* alg7(TransitiveClosure* tr_cl, int i, int k, shared_ptr<StaticVertex> root, std::set<shared_ptr<StaticVertex>>* X, StaticEdge* e);
};

/*
 * class that represents transitive closure of static graph
 * inherited from StaticGraph
 */
class TransitiveClosure : StaticGraph{
public:
    TransitiveClosure();

//    virtual ~TransitiveClosure();

    virtual shared_ptr<StaticVertex> getRoot() override;

    virtual void setRoot(shared_ptr<StaticVertex> v) override;

    virtual std::set<shared_ptr<StaticVertex>>* getVertSet() override;

    virtual std::map<std::string, shared_ptr<StaticVertex>>* getLabelVertMap() override;

    virtual void add_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to, long weight) override;

    virtual void remove_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) override;

    virtual StaticEdge* hasEdge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) override;

    std::string toString() override;

    virtual std::map<std::string, long> *dijkstra(shared_ptr<StaticVertex> source) override;

    virtual TransitiveClosure *transitiveClosure() override;

/*
     * returns cost in edge (u,v)
     * if there is no edge(u,v) return LONG_MAX
     */
    long costEdge(shared_ptr<StaticVertex> u, shared_ptr<StaticVertex> v);
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

//    virtual ~Tree();

    virtual shared_ptr<StaticVertex> getRoot() override;

    virtual void setRoot(shared_ptr<StaticVertex> v) override;

    virtual std::set<shared_ptr<StaticVertex> >* getVertSet() override;

    virtual std::map<std::string, shared_ptr<StaticVertex>>* getLabelVertMap() override;

    virtual void add_edge(shared_ptr<StaticVertex>from, shared_ptr<StaticVertex>to, long weight) override;

    /*returns the created tree after adding new edge*/
    Tree* addEdgeWithCopy(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex>to, long weight);

    virtual void remove_edge(shared_ptr<StaticVertex>from, shared_ptr<StaticVertex>to) override;

    virtual StaticEdge* hasEdge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) override;

    virtual std::string toString() override;

    long getTotalWeight();

    /*division of total weight by covered terms from X*/
    double getDensity(std::set<shared_ptr<StaticVertex>>* X);

    static Tree* merge(Tree* t1, Tree* t2);
};

std::set<shared_ptr<StaticVertex>>* vert_intersect(std::set<shared_ptr<StaticVertex>>* s1, std::set<shared_ptr<StaticVertex>>* s2);
std::set<shared_ptr<StaticVertex>>* vert_minus(std::set<shared_ptr<StaticVertex>>* s1, std::set<shared_ptr<StaticVertex>>* s2);
#endif //PDS1_SGR_H
