//
// Created by Kamil on 21/09/15.
//

#ifndef PDS1_SGR_H
#define PDS1_SGR_H

#include <iosfwd>
#include <string>
#include <altivec.h>
#include <map>
#include <vector>

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
    int weight;
public:

    int getWeight();

    void setWeight(int weight);

    StaticVertex *getFrom();

    void setFrom(StaticVertex *from);

    StaticVertex *getTo();

    void setTo(StaticVertex *to);
};

class StaticGraph{
private:
    std::map<std::string,std::vector<StaticVertex>> adj_list;
public:
    StaticGraph();
    void add_edge(StaticVertex* from, StaticVertex* to, int weight);
    void remove_edge(StaticVertex* from, StaticVertex* to);
};
#endif //PDS1_SGR_H
