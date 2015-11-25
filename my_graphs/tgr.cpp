//
// Created by Kamil on 19/09/15.
//

#include <ostream>
#include <iostream>
#include <stack>
#include "tgr.h"

//-----------------------------------------------------------------
TempVertex::TempVertex(std::string name) {
    this->name = name;
    this->P = nullptr;
    this->sas = new std::vector<TempEdge *>();
}

std::string TempVertex::getName() {
    return this->name;
}

TempVertex::~TempVertex() {
    delete sas;
}

void TempVertex::setName(std::string string) {
    this->name = name;
}

shared_ptr<TempVertex> TempVertex::getP() {
    return this->P;
}

void TempVertex::setP(shared_ptr<TempVertex> P) {
    this->P = P;
}

float TempVertex::getA() {
    return A;
}

void TempVertex::setA(float A) {
    this->A = A;
}

std::vector<TempEdge *> *TempVertex::getSas() {
    return sas;
}

void TempVertex::add_arr_edge(TempEdge *edge) {

    if (sas->empty()) {
        sas->insert(sas->end(), edge);
    }
    else {
        int pos = binarySearchOrNext(edge->getArrTime(), 0, sas->size() - 1);
        sas->insert(sas->begin() + pos, edge);
//        std::cout << "pos = " << pos << std::endl;
    }
//    for (int i = 0; i < sas->size(); i++){
//        std::cout << (*sas)[i].arrival << " ";
//    }
//    std::cout << std::endl;
}

int TempVertex::binarySearchOrNext(float target, int low, int high) {
    if (high < low)
        return low;
    else {
        int mid = (low + high) / 2;
        if (target == (*sas)[mid]->getArrTime())
            return mid;
        else if (target < (*sas)[mid]->getArrTime())
            return binarySearchOrNext(target, low, mid - 1);
        else
            return binarySearchOrNext(target, mid + 1, high);
    }
}

//-----------------------------------------------------------------
TempEdge::TempEdge(shared_ptr<TempVertex> source, shared_ptr<TempVertex> destination, float startTime, float arrTime, float weight) {
    this->source = source;
    this->destination = destination;
    this->startTime = startTime;
    this->arrTime = arrTime;
    this->weight = weight;
}

shared_ptr<TempVertex> TempEdge::getSource() const {
    return source;
}

void TempEdge::setSource(shared_ptr<TempVertex> source) {
    TempEdge::source = source;
}

void TempEdge::setDestination(shared_ptr<TempVertex> destination) {
    TempEdge::destination = destination;
}

float TempEdge::getStartTime() const {
    return startTime;
}

void TempEdge::setStartTime(float startTime) {
    TempEdge::startTime = startTime;
}

float TempEdge::getArrTime() const {
    return arrTime;
}

void TempEdge::setArrTime(float arrTime) {
    TempEdge::arrTime = arrTime;
}

float TempEdge::getWeight(){
    return this->weight;
}

void TempEdge::setWeight(float weight) {
    this->weight = weight;
}

TempEdge::~TempEdge() {
    source.reset();
    destination.reset();
}

std::string TempEdge::toString() {
    std::string res = "<";
    res += this->getSource()->getName() + ", ";
    res += this->getDestination()->getName() + ", ";
    res += std::to_string(this->getStartTime()) + ", ";
    res += std::to_string(this->getArrTime()) + ", ";
    res += std::to_string(this->weight) + ">";
    return res;
}

//-----------------------------------------------------------------
TempGraph::TempGraph() {
    this->edgeList = new std::vector<TempEdge *>();
    this->vertSet = new std::set<shared_ptr<TempVertex> >();
    this->sal = new std::map<std::string, std::vector<TempEdge *> *>();
    this->label2Vert = new map<string, shared_ptr<TempVertex>>();
}

TempGraph::~TempGraph(){
    delete edgeList;
    delete vertSet;
    for (auto it1 = sal->begin(); it1 != sal->end(); it1++){
        for (auto edge : *it1->second){
            delete edge;
        }
        delete(it1->second);
    }
    delete sal;
    delete label2Vert;
}

void TempGraph::addEdge(shared_ptr<TempVertex> from, shared_ptr<TempVertex> to, float startTime, float arrTime, float weight) {
    if ((*sal)[from->getName()] == 0x00) {
        vertSet->insert(from);
        (*sal)[from->getName()] = new std::vector<TempEdge *>();
    }
    if ((*sal)[to->getName()] == 0x00) {
        vertSet->insert(to);
        (*sal)[to->getName()] = new std::vector<TempEdge *>();
    }
    
    if ((*label2Vert)[from->getName()] == 0x00){
        (*label2Vert)[from->getName()] = from;
    }
    else
        from = (*label2Vert)[from->getName()];

    if ((*label2Vert)[to->getName()] == 0x00){
        (*label2Vert)[to->getName()] = to;
    }
    else
        to = (*label2Vert)[to->getName()];


    auto temp_vector = (*sal)[from->getName()];
//    float weight = arrTime - startTime; // !!!! YOU CAN DEFINE WEIGHT DIFFERENTLY
    TempEdge *tempEdge = new TempEdge(from, to, startTime, arrTime, weight);
    temp_vector->insert(temp_vector->end(), tempEdge);

    to->add_arr_edge(tempEdge);
}

void TempGraph::addEdge(TempEdge *edge) {
    shared_ptr<TempVertex> from = edge->getSource();
    shared_ptr<TempVertex> to = edge->getDestination();
    if ((*sal)[from->getName()] == 0x00) {
        vertSet->insert(from);
        (*sal)[from->getName()] = new std::vector<TempEdge *>();
    }
    if ((*sal)[to->getName()] == 0x00) {
        vertSet->insert(to);
        (*sal)[to->getName()] = new std::vector<TempEdge *>();
    }
    to->add_arr_edge(edge);

    auto temp_vector = (*sal)[from->getName()];
    temp_vector->insert(temp_vector->end(), edge);
}

int TempGraph::getVertsNumber() {
    return vertSet->size();
}


int TempGraph::getEdgeNumber() {
    int res = 0;
    for(auto it1 = sal->begin(); it1 != sal->end(); it1++){
        res += it1->second->size();
    }
    return res;
}

std::map<std::string, std::vector<TempEdge *> *> *TempGraph::getSal() {
    return sal;
}

bool TempGraph::hasEdge(shared_ptr<TempVertex>from, shared_ptr<TempVertex>to) {
    auto list = (*sal)[from->getName()];
    for (auto el : (*list)){
        if (el->getDestination()->getName() == to->getName()){
            return true;
        }
    }
    return false;
}

std::string TempGraph::toString() {
    std::string res = "";
    for (auto it1 = sal->begin(); it1 != sal->end(); it1++) {
        auto temp_list = it1->second;
        if (!temp_list->empty()) {
            res += (*temp_list)[0]->getSource()->getName() + ": ";
            for (auto el : *temp_list) {
                res += el->toString() + ", ";
            }
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
    quicksort(res, 0, res->size() - 1, ASC);
    //display sorted list
//    for(TempEdge* edge : *res){
//        std::cout << "<" << edge->getSource()->getName() << ", " << edge->getDestination()->getName() << ", "
//        << edge->getStartTime() << ", " << edge->getArrTime() << ">" << std::endl;
//    }
    return res;
}

void TempGraph::quicksort(std::vector<TempEdge *> *arr, int low, int high, sort_type type) {
    if (low < high) {
        int p = partition(arr, low, high, type);
        quicksort(arr, low, p, type);
        quicksort(arr, p + 1, high, type);
    }
}

int TempGraph::partition(std::vector<TempEdge *> *arr, int low, int high, sort_type type) {
    if (low >= high) return -1;
    auto pivot = (*arr)[low];
    int i = low - 1;
    int j = high + 1;
    while (true) {
        if (type == ASC) {
            do i++; while ((*arr)[i]->getStartTime() < pivot->getStartTime());
            do j--; while ((*arr)[j]->getStartTime() > pivot->getStartTime());
        } else if (type == DESC) {
            do i++; while ((*arr)[i]->getStartTime() > pivot->getStartTime());
            do j--; while ((*arr)[j]->getStartTime() < pivot->getStartTime());
        }
        if (i >= j) return j;
        else swap(arr, i, j);
    }
}

void TempGraph::swap(std::vector<TempEdge *> *arr, int i1, int i2) {
    TempEdge *temp = (*arr)[i1];
    (*arr)[i1] = (*arr)[i2];
    (*arr)[i2] = temp;
}

void TempGraph::mst_a1(shared_ptr<TempVertex> root, float low_bound, float up_bound, bool need) {
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

    for (shared_ptr<TempVertex> v : (*vertSet)) {
        if (v != root) {
            v->setA(numeric_limits<float>::infinity());
            v->setP(root);
        }
        else {
            //in this case v is root:)
            v->setA(low_bound);
            v->setP(nullptr);
        }
    }

    for (TempEdge *e : *edges) {
        shared_ptr<TempVertex> u = e->getSource();
        shared_ptr<TempVertex> v = e->getDestination();
        float start_time = e->getStartTime();
        float arr_time = e->getArrTime();

        if (start_time >= u->getA() && arr_time < v->getA() && arr_time <= up_bound) {
            v->setA(arr_time);
            v->setP(u);
        }
    }
}

std::set<shared_ptr<TempVertex> > *TempGraph::getVertSet() {
    return vertSet;
}

void TempGraph::mst_a2(shared_ptr<TempVertex> root, float low_bound, float up_bound, bool sort) {
    if (sort) {
        sortAdjacencyList(DESC);
    }
    std::cout << toString() << std::endl;
    struct mytuple {
        shared_ptr<TempVertex> v1;
        shared_ptr<TempVertex> v2;
        float time;
    };

    std::map<shared_ptr<TempVertex> , int> pos;


    //initial step
    for (shared_ptr<TempVertex> u : (*vertSet)) {
        pos[u] = 0;
        u->setA(numeric_limits<float>::infinity());
    }
    std::stack<mytuple> *st = new std::stack<mytuple>();
    mytuple temp;
    temp.v1 = root;
    temp.v2 = root;
    temp.time = low_bound;
    st->push(temp);
    while (!st->empty()) {
        mytuple tup = st->top();
        st->pop();
        shared_ptr<TempVertex> u = tup.v1;
        shared_ptr<TempVertex> v = tup.v2;
        float tav = tup.time; // t arrival v
        if (tav < v->getA()) {
            v->setA(tav);
            v->setP(u);
            std::vector<TempEdge *> *edge_list = (*sal)[v->getName()];
            if (pos[v] < edge_list->size()) {
                TempEdge *e = (*edge_list)[pos[v]];
                v = e->getSource();
                shared_ptr<TempVertex> vp = e->getDestination();
                float start = e->getStartTime();
                float arr = e->getArrTime();
                while (pos[v] < edge_list->size() && v->getA() <= start) {
                    mytuple temp_tup;
                    temp_tup.v1 = v;
                    temp_tup.v2 = vp;
                    temp_tup.time = arr;
                    st->push(temp_tup);
                    pos[v]++; // in paper it was pos[u]++
                    if (pos[v] >= edge_list->size()) break;
                    if (v->getA() > start) break;
                    e = (*edge_list).at(pos[v]);
                    v = e->getSource();
                    vp = e->getDestination();
                    start = e->getStartTime();
                    arr = e->getArrTime();
                }
            }
        }
    }
    delete st;
}

void TempGraph::sortAdjacencyList(TempGraph::sort_type type) {
    for (auto it1 = sal->begin(); it1 != sal->end(); it1++) {
        quicksort(it1->second, 0, it1->second->size() - 1, DESC);
    }
};

StaticGraph *TempGraph::getStaticGraph(shared_ptr<TempVertex> root) {
    //triple of vertex and its corresponding arrival time, and weight of in-edge
    struct Triple {
        float corArrTime;
        float weight;
        shared_ptr<StaticVertex>staticVertex;
    };

    StaticGraph *st = new StaticGraph();

    std::map<shared_ptr<TempVertex> , std::vector<Triple *> *> *vert_times = new std::map<shared_ptr<TempVertex> , std::vector<Triple *> *>();

    //first part -- creating vertexes
    for (shared_ptr<TempVertex> v : *vertSet) {
        (*vert_times)[v] = new std::vector<Triple *>();
        std::vector<Triple *> *vect = (*vert_times)[v];
        if (v != root) {
            //TODO: create lists of static vertexes and their corresponding times
            std::vector<TempEdge *> *t_edges = v->getSas();
            for (int i = 0; i < t_edges->size(); i++) {
                TempEdge *edge = (*t_edges)[i];
                Triple *pair = new Triple;
                pair->corArrTime = edge->getArrTime();
                pair->weight = edge->getWeight();
                shared_ptr<StaticVertex> temp(new StaticVertex(edge->getDestination()->getName() + "_" + std::to_string(i+1)));
                pair->staticVertex = temp;
                vect->insert(vect->end(), pair);
            }
            Triple *pair1 = new Triple;
            pair1->corArrTime = numeric_limits<float>::infinity();
            pair1->weight = 0;
            shared_ptr<StaticVertex> temp(new StaticVertex((*t_edges)[0]->getDestination()->getName()));
            pair1->staticVertex = temp;
            st->add_to_terms(temp);
            vect->insert(vect->end(), pair1);

            for (int i = 0; i < vect->size() - 1; i++) {
                shared_ptr<StaticVertex>from(new StaticVertex((*vect)[i]->staticVertex));
                shared_ptr<StaticVertex>to(new StaticVertex((*vect)[i + 1]->staticVertex));
                st->add_edge(from, to, 0);
            }
        }
        else{
            (*vert_times)[v] = new std::vector<Triple *>();
            std::vector<Triple *> *vect = (*vert_times)[v];
            Triple * pair = new Triple;
            pair->corArrTime = 0;
            pair->weight = 0;
            shared_ptr<StaticVertex> temp(new StaticVertex(v->getName()));
            pair->staticVertex = temp;
            st->add_to_terms(temp);
            vect->insert(vect->end(), pair);

            st->setRoot(pair->staticVertex); // if want to define root
        }
    }

    //second part -- creating edges
    for (auto it1 = sal->begin(); it1 != sal->end(); it1++) {
        std::vector<TempEdge *> *t_edges = it1->second;
        for (int i = 0; i < t_edges->size(); i++) {
            TempEdge *t_edge = (*t_edges)[i];
            shared_ptr<TempVertex> u = t_edge->getSource(); // source vertex
            shared_ptr<TempVertex> v = t_edge->getDestination(); // destination vertex
            std::vector<Triple *> *uPairs = (*vert_times)[u];
            std::vector<Triple *> *vPairs = (*vert_times)[v];

            Triple * from = nullptr;
            Triple * to = nullptr;
            float weight;
            if(u != root) {
                for (int j = 0; j < uPairs->size() - 1; j++) {
                    if ((*uPairs)[j]->corArrTime <= t_edge->getStartTime() &&
                        (*uPairs)[j + 1]->corArrTime > t_edge->getStartTime()) {
                        from = (*uPairs)[j];
                        break;
                    }
                }
            }
            else{
                from = (*uPairs)[0];
            }
            for(int j = 0; j < vPairs->size(); j++){
                if ((*vPairs)[j]->corArrTime == t_edge->getArrTime()){
                    to = (*vPairs)[j];
                    break;
                }
            }
            if (to == nullptr || from == nullptr)
                continue;
            weight = t_edge->getWeight();
            st->add_edge(from->staticVertex, to->staticVertex, weight);
        }
    }

    for (auto it1 = vert_times->begin(); it1 != vert_times->end(); it1++){
        auto tr_list = it1->second;
        for (auto triple : *tr_list){
            delete(triple);
        }
        delete(tr_list);
    }
    delete vert_times;
    return st;
}
