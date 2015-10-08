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
private:
    std::map<std::string,std::vector<StaticEdge*>*>* adj_list;
    std::set<StaticVertex*>* vertSet;
public:
    StaticGraph();
    ~StaticGraph();
    void add_edge(StaticVertex* from, StaticVertex* to, long weight);
    void remove_edge(StaticVertex* from, StaticVertex* to);
    std::string toString();

    //calculates shortest path between vertex u and v within given graph, using Dijkstra method

    //returns transitive closure of graph
    StaticGraph* FloydWarshall();
};
#endif //PDS1_SGR_H
