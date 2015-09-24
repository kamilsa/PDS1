//
// Created by Kamil on 19/09/15.
//

#include <ostream>
#include <iostream>
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

TempVertex *TempEdge::getSource() const {
    return source;
}

void TempEdge::setSource(TempVertex *source) {
    TempEdge::source = source;
}

void TempEdge::setDestination(TempVertex *destination) {
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

//-----------------------------------------------------------------
TempGraph::TempGraph() {
    this->edgeList = new std::vector<TempEdge *>();
    this->vertList = new std::vector<TempVertex *>();
    this->sal = new std::map<std::string, std::vector<TempEdge *> *>();
}

void TempGraph::addEdge(TempVertex *from, TempVertex *to, int startTime, int arrTime) {
    if ((*sal)[from->getName()] == 0x00) {
        (*sal)[from->getName()] = new std::vector<TempEdge *>();
    }
    auto temp_vector = (*sal)[from->getName()];
    TempEdge *tempEdge = new TempEdge(from, to, startTime, arrTime);
    temp_vector->insert(temp_vector->end(), tempEdge);
}

void TempGraph::addEdge(TempEdge *edge) {
    TempVertex *from = edge->getSource();
    if ((*sal)[from->getName()] == 0x00) {
        (*sal)[from->getName()] = new std::vector<TempEdge *>();
    }
    auto temp_vector = (*sal)[from->getName()];
    temp_vector->insert(temp_vector->end(), edge);
}

std::string TempGraph::toString() {
    std::string res = "";
    for (auto it1 = sal->begin(); it1 != sal->end(); it1++) {
        auto temp_list = it1->second;
        if (!temp_list->empty()) {
//            std::cout << (*temp_list)[0]->getSource()->getName() << ": ";
            res += (*temp_list)[0]->getSource()->getName() + ": ";
            for (auto el : *temp_list) {
//                std::cout << el->getDestination()->getName() << ", ";
                res += el->getDestination()->getName() + ", ";
            }
//            std::cout << std::endl;
            res += "\n";
        }
    }
    return res;
}

std::vector<TempEdge *> *TempGraph::deriveSortedEdgeList() {
    std::vector<TempEdge *> *res = new std::vector<TempEdge *>();
    for (auto it1 = sal->begin(); it1 != sal->end(); it1++) {
        std::vector<TempEdge *> *temp_list = it1->second;
        for (int i = 0; i < temp_list->size(); i++) {
            res->insert(res->end(), (*temp_list)[i]);
        }
    }
    quicksort(res, 0, res->size() - 1);
    //display sorted list
//    for(TempEdge* edge : *res){
//        std::cout << "<" << edge->getSource()->getName() << ", " << edge->getDestination()->getName() << ", "
//        << edge->getStartTime() << ", " << edge->getArrTime() << ">" << std::endl;
//    }
    return res;
}

void TempGraph::quicksort(std::vector<TempEdge *> *arr, int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);
        quicksort(arr, low, p);
        quicksort(arr, p + 1, high);
    }
}

int TempGraph::partition(std::vector<TempEdge *> *arr, int low, int high) {
    if (low >= high) return -1;
    auto pivot = (*arr)[low];
    int i = low - 1;
    int j = high + 1;
    while (true) {
        do i++; while ((*arr)[i]->getStartTime() < pivot->getStartTime());
        do j--; while ((*arr)[j]->getStartTime() > pivot->getStartTime());
        if (i >= j) return j;
        else swap(arr, i, j);
    }
}

void TempGraph::swap(std::vector<TempEdge *> *arr, int i1, int i2) {
    TempEdge *temp = (*arr)[i1];
    (*arr)[i1] = (*arr)[i2];
    (*arr)[i2] = temp;
}

void TempGraph::mst_a1(TempVertex *root, long low_bound, long up_bound, bool need) {
    std::vector<TempEdge *> *edges;
    if (need) {
        edges = deriveSortedEdgeList();
    } else {
        edges = new std::vector<TempEdge *>();
        for (auto it1 = sal->begin(); it1 != sal->end(); it1++) {
            std::vector<TempEdge *> *temp_list = it1->second;
            for (int i = 0; i < temp_list->size(); i++) {
                edges->insert(edges->end(), (*temp_list)[i]);
            }
        }
    }

    for (TempEdge *e : *edges) {
        TempVertex *u = e->getSource();
        TempVertex *v = e->getDestination();
        long start_time = e->getStartTime();
        long arr_time = e->getArrTime();

        if (start_time >= u->getA() && arr_time < v->getA() && arr_time <= up_bound) {
            v->setA(arr_time);
            v->setP(u);
        }
    }
}
