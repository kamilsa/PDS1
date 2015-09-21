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
StaticEdge::StaticEdge(StaticVertex *from, StaticVertex *to, int weight) {
    this->from = from;
    this->to = to;
    this->weight = weight;
}

int StaticEdge::getWeight() {
    return weight;
}

void StaticEdge::setWeight(int weight) {
    StaticEdge::weight = weight;
}

void StaticEdge::setFrom(StaticVertex *from) {
    StaticEdge::from = from;
}

void StaticEdge::setTo(StaticVertex *to) {
    StaticEdge::to = to;
}
//-----------------------------------------------------------------

StaticGraph::StaticGraph() {
    adj_list = new std::map<std::string, std::vector<StaticEdge*>*>();
}

void StaticGraph::add_edge(StaticVertex *from, StaticVertex *to, int weight) {
    if((*adj_list)[from->getName()] == 0x00){
        (*adj_list)[from->getName()] = new std::vector<StaticEdge*>();
    }
    StaticEdge* edge = new StaticEdge(from, to, weight);
    std::vector<StaticEdge*>* temp = (*adj_list)[from->getName()];
    temp->insert(temp->end(), edge);
}

StaticGraph::~StaticGraph() {
    delete this->adj_list;
}
void StaticGraph::toString() {
    for(auto it1 = adj_list->begin(); it1 != adj_list->end(); it1++){
        auto temp_list = it1->second;
        if(!temp_list->empty()) {
            std::cout << (*temp_list)[0]->getFrom()->getName() << ": ";
            for (auto el : *temp_list) {
                std::cout << el->getTo()->getName() << ", ";
            }
            std::cout << std::endl;
        }
    }
}
