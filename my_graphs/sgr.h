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
#include <unordered_set>
#include <queue>
#include <memory>
//#include <algorithm>
using namespace std;

class StaticVertex;

class StaticEdge;

class StaticGraph;

class TransitiveClosure;

class Tree;

struct classcomp;

struct KeyHasher;

struct classcomp {
    bool operator()(const shared_ptr<StaticVertex> &lhs, const shared_ptr<StaticVertex> &rhs) const;
};

struct KeyHasher
{
    std::size_t operator()(const shared_ptr<StaticVertex> &k) const;
};

namespace std {
    template <> struct equal_to<shared_ptr<StaticVertex>>
    {
        bool operator()(const shared_ptr<StaticVertex> & x, const shared_ptr<StaticVertex> & y) const;
    };
}

class StaticVertex {
private:
    std::string name;
public:
    StaticVertex(std::string name);

    StaticVertex(shared_ptr<StaticVertex> v);

    ~StaticVertex();

    std::string getName() const;

    void setName(std::string name);

};

class StaticEdge {
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

class StaticGraph {
    struct myEntry {
        double density;
        shared_ptr<StaticVertex> value;
    };
protected:
    std::map<std::string, std::vector<StaticEdge *> *> *adj_list;
    std::set<shared_ptr<StaticVertex>, classcomp> *vertSet;
    shared_ptr<StaticVertex> root; //
    std::map<std::string, shared_ptr<StaticVertex>> *labelVert;

    set<shared_ptr<StaticVertex>, classcomp> *terms; // terminal vertices -- those, which we are trying to reach

    void putInOrder(std::vector<myEntry *> *vec, myEntry *entr);

    int entrBinSearchOrNext(std::vector<myEntry *> *vec, myEntry *entr);

public:
    StaticGraph();

    StaticGraph(StaticGraph *g);

    virtual ~StaticGraph();

    virtual shared_ptr<StaticVertex> getRoot();

    virtual void setRoot(shared_ptr<StaticVertex> v);

    virtual std::set<shared_ptr<StaticVertex>, classcomp> *getVertSet();

    int get_edge_number();

    virtual std::map<std::string, shared_ptr<StaticVertex>> *getLabelVertMap();

    virtual void add_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to, long weight);

    virtual void remove_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to);

    virtual StaticEdge *hasEdge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to);

    set<shared_ptr<StaticVertex>, classcomp> *get_terms();

    void add_to_terms(shared_ptr<StaticVertex> v);

    virtual std::string toString();

    //calculates shortest path between vertex u and v within given graph, using Dijkstra method

    virtual //returns shortest paths from source
    std::map<string, long> *dijkstra(shared_ptr<StaticVertex> source);

    virtual //returns transitive closure, executing dijkstra |V| times
    shared_ptr<TransitiveClosure> transitiveClosure();

    virtual Tree *alg3(shared_ptr<TransitiveClosure> tr_cl, int i, int k, shared_ptr<StaticVertex> root,
                       std::set<shared_ptr<StaticVertex>, classcomp> *X);

    /*improved alg3*/
    virtual Tree *alg4(shared_ptr<TransitiveClosure> tr_cl, int i, int k, shared_ptr<StaticVertex> root,
                       std::set<shared_ptr<StaticVertex>, classcomp> *X);

    virtual Tree *alg5(shared_ptr<TransitiveClosure> tr_cl, int i, int k, shared_ptr<StaticVertex> root,
                       std::set<shared_ptr<StaticVertex>, classcomp> *X, StaticEdge *e);

    virtual Tree *alg6(shared_ptr<TransitiveClosure> tr_cl, int i, int k, shared_ptr<StaticVertex> root,
                       std::set<shared_ptr<StaticVertex>, classcomp> *X);

    virtual Tree *alg7(shared_ptr<TransitiveClosure> tr_cl, int i, int k, shared_ptr<StaticVertex> root,
                       std::set<shared_ptr<StaticVertex>, classcomp> *X, StaticEdge *e);
};

/*
 * class that represents transitive closure of static graph
 * inherited from StaticGraph
 */
class TransitiveClosure : StaticGraph {
private:
    int bin_search_or_next_edge(vector<StaticEdge *> *vect, StaticEdge *edge);

public:
    TransitiveClosure();

//    virtual ~TransitiveClosure();

    virtual shared_ptr<StaticVertex> getRoot() override;

    virtual void setRoot(shared_ptr<StaticVertex> v) override;

    virtual std::set<shared_ptr<StaticVertex>, classcomp> *getVertSet() override;

    virtual std::map<std::string, shared_ptr<StaticVertex>> *getLabelVertMap() override;

    virtual void add_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to, long weight) override;

    virtual void remove_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) override;

    virtual StaticEdge *hasEdge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) override;

    std::string toString() override;

    virtual std::map<std::string, long> *dijkstra(shared_ptr<StaticVertex> source) override;

    virtual shared_ptr<TransitiveClosure> transitiveClosure() override;

/*
     * returns cost in edge (u,v)
     * if there is no edge(u,v) return LONG_MAX
     */
    long costEdge(shared_ptr<StaticVertex> u, shared_ptr<StaticVertex> v);

    long costEdge(std::string u_name, std::string v_name);

    //returns minimum cost between u and v, where v is in X
    StaticEdge *min_cost_edge(shared_ptr<StaticVertex> u, set<shared_ptr<StaticVertex>, classcomp> *X);

};

/*
 * Tree class, has feature to calculate density
 */
class Tree : StaticGraph {
private:
    long totalWeight;
public:
    Tree();

    Tree(Tree *tree);

//    virtual ~Tree();

    virtual shared_ptr<StaticVertex> getRoot() override;

    virtual void setRoot(shared_ptr<StaticVertex> v) override;

    virtual std::set<shared_ptr<StaticVertex>, classcomp> *getVertSet() override;

    virtual std::map<std::string, shared_ptr<StaticVertex>> *getLabelVertMap() override;

    virtual void add_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to, long weight) override;

    /*returns the created tree after adding new edge*/
    Tree *addEdgeWithCopy(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to, long weight);

    virtual void remove_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) override;

    virtual StaticEdge *hasEdge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) override;

    virtual std::string toString() override;

    long getTotalWeight();

    /*division of total weight by covered terms from X*/
    double getDensity(std::set<shared_ptr<StaticVertex>, classcomp> *X);

    static Tree *merge(Tree *t1, Tree *t2);
};

std::set<shared_ptr<StaticVertex>, classcomp> *vert_intersect(std::set<shared_ptr<StaticVertex>, classcomp> *s1,
                                                              std::set<shared_ptr<StaticVertex>, classcomp> *s2);

std::set<shared_ptr<StaticVertex>, classcomp> *vert_minus(std::set<shared_ptr<StaticVertex>, classcomp> *s1,
                                                          std::set<shared_ptr<StaticVertex>, classcomp> *s2);

#endif //PDS1_SGR_H
