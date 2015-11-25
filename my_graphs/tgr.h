//
// Created by Kamil on 19/09/15.
//

#ifndef PDS1_TGR_H
#define PDS1_TGR_H


#include <iosfwd>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "sgr.h"
#include <memory>

using namespace std;

class TempVertex;
class TempEdge;
class TempGraph;

/**
 * Class vertex -- represents vertex in temporal graph
 * Consist of:
 * TempVertex* P -- stores source vertex for given vertex
 * float A -- stores earlies arrival time in UNIX format
 */
class TempVertex {
private:
    std::string name;
    shared_ptr<TempVertex> P;
    float A;

    /* list of tuples with start and arrival times sorted in non-decreasing order of their arrival time
     * Is needed to transform temporal graph to static
     */
    std::vector<TempEdge*>* sas;
    int binarySearchOrNext(float target, int low, int high);
public:
    TempVertex(std::string name);
    ~TempVertex();

    std::string getName();

    void setName(std::string);

    shared_ptr<TempVertex> getP();

    void setP(shared_ptr<TempVertex> P);

    float getA();

    void setA(float A);

    /*
     * adds arrival edge to this node, with corresponding start time to list if SAs. List become automatically sorted
     * by arrival times
     */
    void add_arr_edge(TempEdge *edge);

    std::vector<TempEdge*>* getSas();
};

/*
 * Class that represents the temporal edge between two temporal vertices
 * Consist of:
 * TempVertex *source -- source temporal vertex
 * TempVertex *destination -- destination temporal vertex
 * float startTime -- starting time for this edge, supposed to keep UNIX time
 * float arrTime -- arrival time for edge, supposed to keep UNIX time
 */
class TempEdge {
private:
    shared_ptr<TempVertex> source;
//    TempVertex *source;
    shared_ptr<TempVertex> destination;
//    TempVertex *destination;
    float startTime;
    float arrTime;
    float weight;
public:
    TempEdge(shared_ptr<TempVertex> source, shared_ptr<TempVertex> destination, float startTime, float arrTime, float weight);

    ~TempEdge();

    shared_ptr<TempVertex> getSource() const;

    void setSource(shared_ptr<TempVertex> source);

    shared_ptr<TempVertex> getDestination() const {
        return destination;
    }

    void setDestination(shared_ptr<TempVertex> destination);

    float getStartTime() const;

    void setStartTime(float startTime);

    float getArrTime() const;

    void setArrTime(float arrTime);

    float getWeight();

    void setWeight(float weight);

    std::string toString();
};

/*
 * Class that represents temporal graph
 * Consist of:
 * std::vector<TempVertex*>* vertList -- vector of temporal vertices
 * std::vector<TempEdge*>* edgeList -- vector of temporal edges
 * std::map<std::string, std::vector<TempEdge *> *> *sal -- sorted adjacency list. Maps the name of vertex with
 * corresponding list of adjacent vertexes
 */
class TempGraph{
private:
//    std::vector<TempVertex*>* vertList; // FIX: does not work properly
    enum sort_type{ASC, DESC};
    std::set<shared_ptr<TempVertex> >* vertSet;
    std::vector<TempEdge*>* edgeList;
    std::map<std::string, std::vector<TempEdge *> *> *sal;
    map<string, shared_ptr<TempVertex> > *label2Vert;
    void quicksort(std::vector<TempEdge*>* arr, int low, int high, sort_type type);
    int partition(std::vector<TempEdge*>* arr, int low, int high, sort_type type);
    void swap(std::vector<TempEdge*>* arr, int i1, int i2);
public:
    TempGraph();
    ~TempGraph();
    void addEdge(shared_ptr<TempVertex> from, shared_ptr<TempVertex> to, float startTime, float arrTime, float weight);
    void addEdge(TempEdge* edge);

    int getVertsNumber();

    int getEdgeNumber();

    std::map<std::string, std::vector<TempEdge *> *> *getSal();

    bool hasEdge(shared_ptr<TempVertex> from, shared_ptr<TempVertex> to);

    std::string toString();
    std::vector<TempEdge*>* deriveSortedEdgeList(); // derive list of edges sorted by started time

    /* sort edge lists in adjacency list in either ascending or descending order of edges
     *
     */
    void sortAdjacencyList(sort_type type);
    std::set<shared_ptr<TempVertex> >* getVertSet();
    /* construct time minimum spanning tree via assigning to each vertex its previous vertex and earliest arrival time
     * low_bound -- is the low time bound of observatin period
     * up_bound -- is the upper time bound of observation period
     * needToSort -- defines whether we want to sort adjacency list. If false it means that
     * adjacency list is already sorted and all we need is we just to go through each
     * list and get edges in order of their occurency
    */
    void mst_a1(shared_ptr<TempVertex> root, float low_bound, float up_bound, bool needToSort); // fill out A and P fields for each vertex.

    /* constructs time minimum spanning tree using sorted adjacency list and assigning to each vertex corresponding
     * previous one and earliest arrival time
     * low_bound -- is the low time bound of observatin period
     * up_bound -- is the upper time bound of observation period
     * sort -- defines if adjacency list is supposed to sort (it could be already sorted)
     */
    void mst_a2(shared_ptr<TempVertex> root,  float low_bound, float up_bound, bool sort);

    StaticGraph* getStaticGraph(shared_ptr<TempVertex> root);
};
#endif //PDS1_TGR_H
