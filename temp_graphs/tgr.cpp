//
// Created by Kamil on 19/09/15.
//

#include "tgr.h"

Vertex::Vertex(std::string name) {
    this->name = name;
    this->P = nullptr;
}

std::string Vertex::getName() {
    return this->name;
}

void Vertex::setName(std::string string) {
    this->name = name;
}

Edge::Edge(Vertex *source, Vertex *destination, long startTime, long arrTime) {
    this->source = source;
    this->destination = destination;
    this->startTime = startTime;
    this->arrTime = arrTime;
}

Vertex *Edge::getSource() const  {
    return source;
}

void Edge::setSource(Vertex *source) {
    Edge::source = source;
}

void Edge::setDestination(Vertex *destination){
    Edge::destination = destination;
}

long Edge::getStartTime() const {
    return startTime;
}

void Edge::setStartTime(long startTime) {
    Edge::startTime = startTime;
}

long Edge::getArrTime() const {
    return arrTime;
}

void Edge::setArrTime(long arrTime) {
    Edge::arrTime = arrTime;
}

Edge::~Edge() {
    delete source;
    delete destination;
}

Vertex *Vertex::getP() {
    return this->P;
}

void Vertex::setP(Vertex *P) {
    this->P = P;
}

long Vertex::getA() {
    return A;
}

void Vertex::setA(long A) {
    this->A = A;
}

std::string Edge::toString() {
    std::string res = "";
    res += "source = [" + this->getSource()->getName() + "] ";
    res += "destination = [" + this->getDestination()->getName() + "] ";
    res += "start time = [" + std::to_string(this->getStartTime()) + "] ";
    res += "arrival time = [" + std::to_string(this->getArrTime()) + "]\n";
    return res;
}
