//
// Created by Kamil on 19/09/15.
//

#ifndef PDS1_TGR_H
#define PDS1_TGR_H


#include <iosfwd>
#include <string>
#include <vector>
#include <map>

/**
 * Class vertex -- represents vertex in temporal graph
 * Consist of:
 * TempVertex* P -- stores source vertex for given vertex
 * long A -- stores earlies arrival time in UNIX format
 */
class TempVertex {
private:
    std::string name;
    TempVertex *P;
    long A;
public:
    TempVertex(std::string name);

    std::string getName();

    void setName(std::string);

    TempVertex *getP();

    void setP(TempVertex *P);

    long getA();

    void setA(long A);
};

/*
 * Class that represents the temporal edge between two temporal vertices
 * Consist of:
 * TempVertex *source -- source temporal vertex
 * TempVertex *destination -- destination temporal vertex
 * long startTime -- starting time for this edge, supposed to keep UNIX time
 * long arrTime -- arrival time for edge, supposed to keep UNIX time
 */
class TempEdge {
private:
    TempVertex *source;
    TempVertex *destination;
    long startTime;
    long arrTime;
public:
    TempEdge(TempVertex *source, TempVertex *destination, long startTime, long arrTime);

    ~TempEdge();

    TempVertex *getSource() const;

    void setSource(TempVertex *source);

    TempVertex *getDestination() const {
        return destination;
    }

    void setDestination(TempVertex *destination);

    long getStartTime() const;

    void setStartTime(long startTime);

    long getArrTime() const;

    void setArrTime(long arrTime);

    std::string toString();
};

/*
 * Class that represents temporal graph
 * Consist of:
 * std::vector<TempVertex*>* vertList -- vector of temporal vertices
 * std::vector<TempEdge*>* edgeList -- vector of temporal edges
 * std::map<std::string, std::vector<TempEdge *> *> *sal -- sorted adjacency list. Maps the name of vertex with corresponding list of adjacent vertexes
 */
class TempGraph{
private:
    std::vector<TempVertex*>* vertList;
    std::vector<TempEdge*>* edgeList;
    std::map<std::string, std::vector<TempEdge *> *> *sal;
public:
    TempGraph();
    void addEdge(TempVertex* from, TempVertex* to, int startTime, int arrTime);
    static TempGraph createFromEdges(std::vector<TempEdge*> edgeList);
};
#endif //PDS1_TGR_H
