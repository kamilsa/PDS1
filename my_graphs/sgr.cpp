//
// Created by Kamil on 21/09/15.
//

#include <iostream>
#include "sgr.h"
//-----------------------------------------------------------------
StaticVertex::StaticVertex(std::string name) {
    this->name = name;
}

std::string StaticVertex::getName(){
    return name;
}

void StaticVertex::setName(std::string name) {
    StaticVertex::name = name;
}
StaticVertex *StaticEdge::getFrom() {
    return from;
}

StaticVertex *StaticEdge::getTo() {
    return to;
}
//-----------------------------------------------------------------
StaticEdge::StaticEdge(StaticVertex *from, StaticVertex *to, long weight) {
    this->from = from;
    this->to = to;
    this->weight = weight;
}

long StaticEdge::getWeight() {
    return weight;
}

void StaticEdge::setWeight(long weight) {
    StaticEdge::weight = weight;
}

void StaticEdge::setFrom(StaticVertex *from) {
    StaticEdge::from = from;
}

void StaticEdge::setTo(StaticVertex *to) {
    StaticEdge::to = to;
}

std::string StaticEdge::toString(){
    std::string res;
    res = "<" + from->getName() + ", " + to->getName() + ", " + std::to_string(weight) + ">";
    return res;
}
//-----------------------------------------------------------------

StaticGraph::StaticGraph() {
    adj_list = new std::map<std::string, std::vector<StaticEdge*>*>();
    vertSet = new std::set<StaticVertex*>();
}

void StaticGraph::add_edge(StaticVertex *from, StaticVertex *to, long weight) {
    vertSet->insert(vertSet->end(), from);
    vertSet->insert(vertSet->end(), to);
    if((*adj_list)[from->getName()] == 0x00){
        (*adj_list)[from->getName()] = new std::vector<StaticEdge*>();
    }
    StaticEdge* edge = new StaticEdge(from, to, weight);
    std::vector<StaticEdge*>* temp = (*adj_list)[from->getName()];
    temp->insert(temp->end(), edge);
}

StaticGraph::~StaticGraph() {
    delete this->adj_list;
    delete this->vertSet;
}
std::string StaticGraph::toString() {
    std::string res = "";
    for(auto it1 = adj_list->begin(); it1 != adj_list->end(); it1++){
        auto temp_list = it1->second;
        if(!temp_list->empty()) {
            res += (*temp_list)[0]->getFrom()->getName() + ": ";
            for (auto el : *temp_list) {
                res += el->toString() + ", ";
            }
            res += "\n";
        }
    }
    return res;
}

StaticGraph *StaticGraph::FloydWarshall() {
    StaticGraph* dist = new StaticGraph();// transitive closure
    for (auto it1 = adj_list->begin(); it1 != adj_list->end(); it1++){
        std::vector<StaticEdge*>* currVect = it1->second;
        for(int i = 0; i < currVect->size(); i++){
            StaticEdge* edge = (*currVect)[i];
            dist->add_edge(edge->getFrom(), edge->getTo(), edge->getWeight());
        }
    }
    return nullptr;
}
