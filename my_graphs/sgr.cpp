//
// Created by Kamil on 21/09/15.
//

#include "sgr.h"

StaticVertex::StaticVertex(std::string name) {
    this->name = name;
}

std::string StaticVertex::getName(){
    return name;
}

void StaticVertex::setName(std::string name) {
    StaticVertex::name = name;
}

int StaticEdge::getWeight() {
    return weight;
}

void StaticEdge::setWeight(int weight) {
    StaticEdge::weight = weight;
}

StaticVertex *StaticEdge::getFrom() {
    return from;
}

void StaticEdge::setFrom(StaticVertex *from) {
    StaticEdge::from = from;
}

StaticVertex *StaticEdge::getTo() {
    return to;
}

void StaticEdge::setTo(StaticVertex *to) {
    StaticEdge::to = to;
}
