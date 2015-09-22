//
// Created by Kamil on 19/09/15.
//

#include "tgr.h"
//-----------------------------------------------------------------
TempVertex::TempVertex(std::string name) {
    this->name = name;
    this->P = nullptr;
}

std::string TempVertex::getName() {
    return this->name;
}

void TempVertex::setName(std::string string) {
    this->name = name;
}

TempVertex *TempVertex::getP() {
    return this->P;
}

void TempVertex::setP(TempVertex *P) {
    this->P = P;
}

long TempVertex::getA() {
    return A;
}

void TempVertex::setA(long A) {
    this->A = A;
}

//-----------------------------------------------------------------
TempEdge::TempEdge(TempVertex *source, TempVertex *destination, long startTime, long arrTime) {
    this->source = source;
    this->destination = destination;
    this->startTime = startTime;
    this->arrTime = arrTime;
}

TempVertex *TempEdge::getSource() const  {
    return source;
}

void TempEdge::setSource(TempVertex *source) {
    TempEdge::source = source;
}

void TempEdge::setDestination(TempVertex *destination){
    TempEdge::destination = destination;
}

long TempEdge::getStartTime() const {
    return startTime;
}

void TempEdge::setStartTime(long startTime) {
    TempEdge::startTime = startTime;
}

long TempEdge::getArrTime() const {
    return arrTime;
}

void TempEdge::setArrTime(long arrTime) {
    TempEdge::arrTime = arrTime;
}

TempEdge::~TempEdge() {
    delete source;
    delete destination;
}

std::string TempEdge::toString() {
    std::string res = "<";
    res += this->getSource()->getName() + ", ";
    res += this->getDestination()->getName() + ", ";
    res += std::to_string(this->getStartTime()) + ", ";
    res += std::to_string(this->getArrTime()) + ">";
    return res;
}
