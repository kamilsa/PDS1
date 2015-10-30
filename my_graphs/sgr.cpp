//
// Created by Kamil on 21/09/15.
//

#include <iostream>
#include "sgr.h"

/*********************************************/
/*         StaticVertex implementation       */
/*********************************************/
StaticVertex::StaticVertex(std::string name) {
    this->name = name;
}

StaticVertex::StaticVertex(StaticVertex *v) {
    this->name = v->name;
}

StaticVertex::~StaticVertex(){
}

std::string StaticVertex::getName() {
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

/*********************************************/
/*         StaticEdge implementation       */
/*********************************************/
StaticEdge::StaticEdge(StaticVertex *from, StaticVertex *to, long weight) {
    this->from = from;
    this->to = to;
    this->weight = weight;
}

StaticEdge::~StaticEdge() {
//    delete(from);
//    delete(to);
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

std::string StaticEdge::toString() {
    std::string res;
    res = "<" + from->getName() + ", " + to->getName() + ", " + std::to_string(weight) + ">";
    return res;
}

/*********************************************/
/*         StaticGraph implementation       */
/*********************************************/

StaticGraph::StaticGraph() {
    adj_list = new std::map<std::string, std::vector<StaticEdge *> *>();
    vertSet = new std::set<StaticVertex *>();
    labelVert = new std::map<std::string, StaticVertex *>();
}

StaticGraph::StaticGraph(StaticGraph *g) {
    adj_list = new std::map<std::string, std::vector<StaticEdge *> *>();
    vertSet = new std::set<StaticVertex *>();
    labelVert = new std::map<std::string, StaticVertex *>();
    for (auto it1 = g->adj_list->begin(); it1 != g->adj_list->end(); it1++) {
        for (StaticEdge *edge : *it1->second) {
            this->add_edge(edge->getFrom(), edge->getTo(), edge->getWeight());
        }
    }
}

StaticGraph::~StaticGraph() {
    for (auto it1 = adj_list->begin(); it1 != adj_list->end(); it1++){
        auto edge_list = it1->second;
        for (auto edge : *edge_list){
            delete(edge);
        }
        delete(edge_list);
    }
    delete this->adj_list;
    for (auto vert : *vertSet)
        delete(vert);
    delete this->vertSet;
    delete this->labelVert;
}

StaticVertex *StaticGraph::getRoot() {
    return root;
}

void StaticGraph::setRoot(StaticVertex *v) {
    this->root = v;
}

std::set<StaticVertex *> *StaticGraph::getVertSet() {
    return vertSet;
}

std::map<std::string, StaticVertex *> *StaticGraph::getLabelVertMap() {
    return labelVert;
}

void StaticGraph::add_edge(StaticVertex *from, StaticVertex *to, long weight) {
    vertSet->insert(vertSet->end(), from);
    vertSet->insert(vertSet->end(), to);
    if ((*adj_list)[from->getName()] == 0x00) {
        (*adj_list)[from->getName()] = new std::vector<StaticEdge *>();
        (*labelVert)[from->getName()] = from;
    }
    if ((*adj_list)[to->getName()] == 0x00) {
        (*adj_list)[to->getName()] = new std::vector<StaticEdge *>();
        (*labelVert)[to->getName()] = to;
    }
    StaticEdge *edge = new StaticEdge(from, to, weight);
    std::vector<StaticEdge *> *temp = (*adj_list)[from->getName()];
    temp->insert(temp->end(), edge);
}

void StaticGraph::remove_edge(StaticVertex *from, StaticVertex *to) {
    //TODO
}

StaticEdge *StaticGraph::hasEdge(StaticVertex *from, StaticVertex *to) {
    auto list = (*adj_list)[from->getName()];
    if (list == nullptr) return nullptr;
    for (int i = 0; i < list->size(); i++) {
        StaticEdge *e = (*list)[i];
        if (e->getTo()->getName() == to->getName()) {
            return e;
        }
    }
    return nullptr;
}

std::string StaticGraph::toString() {
    std::string res = "";
    for (auto it1 = adj_list->begin(); it1 != adj_list->end(); it1++) {
        auto temp_list = it1->second;
        if (!temp_list->empty()) {
            res += (*temp_list)[0]->getFrom()->getName() + ": ";
            for (auto el : *temp_list) {
                res += el->toString() + ", ";
            }
            res += "\n";
        }
    }
    return res;
}

StaticGraph *dist = new StaticGraph();// transitive closure

std::map<StaticVertex *, long> *StaticGraph::dijkstra(StaticVertex *source) {
    std::map<StaticVertex *, long> *dist = new std::map<StaticVertex *, long>();

    // if we need keep previous vertexes in path
    std::map<StaticVertex *, StaticVertex *> *prev = new std::map<StaticVertex *, StaticVertex *>();
    std::queue<StaticVertex *> *q = new std::queue<StaticVertex *>();
    q->push(source);
    (*dist)[source] = 0;
    for (StaticVertex *v : *vertSet) {
        if (v != source) {
            (*dist)[v] = LONG_MAX;
            (*prev)[v] = nullptr;
            q->push(v);
        }
    }
    while (!q->empty()) {
        StaticVertex *u = q->front();
        q->pop();

        auto adj = (*adj_list)[u->getName()];
        for (int i = 0; i < adj->size(); i++) {
            StaticEdge *e = (*adj)[i];
            StaticVertex *v = e->getTo();
            long alt;

            //LONG_MAX represents infinity. Thus, to avoid bits overloading we make this comparison
            if ((*dist)[u] != LONG_MAX)
                alt = (*dist)[u] + e->getWeight();
            else
                alt = LONG_MAX;
            if (alt < (*dist)[v]) {
                (*dist)[v] = alt;
                (*prev)[v] = u;
            }
        }
    }

    //display the shortest paths:
    std::cout << "Shortest paths from " << source->getName() << ": " << std::endl;
    for (StaticVertex *v : *vertSet) {
        std::cout << v->getName() << ": " << (*dist)[v] << std::endl;
    }

    delete q;
    delete prev;
    return dist;
}

TransitiveClosure *StaticGraph::transitiveClosure() {
    TransitiveClosure *trcl = new TransitiveClosure();
    for (StaticVertex *u : *vertSet) {
        auto shortDists = dijkstra(u); // shortest distance from u to other nodes, infinity defines unreachable nodes
        for (StaticVertex *v : *vertSet) {
            if ((*shortDists)[v] != LONG_MAX) {
                StaticVertex *new_u = new StaticVertex(u);
                StaticVertex *new_v = new StaticVertex(v);
                trcl->add_edge(new_u, new_v, (*shortDists)[v]);
            }
        }
    }
    return trcl;
}

Tree *StaticGraph::alg3(TransitiveClosure *tr_cl, int i, int k, StaticVertex *root, std::set<StaticVertex *> *X) {
    Tree *tree = new Tree();
    if (i == 1) {
        if (k > X->size()) return tree;
        while (k > 0) {
            long minCost = LONG_MAX;
            StaticVertex *minVert;// vert with lighter-weight total path from root
            for (StaticVertex *v : *X) { // by now finding minVert too costly, could be improved
                long cost = tr_cl->costEdge(root, v); // O(N) method -- too costly as well
                if (cost < minCost) {
                    minCost = cost;
                    minVert = v;
                }
            }
            if (minCost != LONG_MAX)
                tree->add_edge(root, minVert, minCost);
            k--;
            X->erase(minVert);
        }
    }
    else {
        std::map<Tree *, double> *den = new std::map<Tree *, double>();
        while (k > 0) {
            Tree *treeBest = new Tree();
            (*den)[treeBest] = LONG_MAX;
            for (StaticVertex *v : *tr_cl->getVertSet()) {
                for (int kp = 1; kp <= k; ++kp) {
                    Tree *treeP = alg3(tr_cl, i - 1, kp, v, new std::set<StaticVertex *>(X->begin(), X->end()));
                    int size = X->size();
                    treeP->add_edge(root, v, tr_cl->costEdge(root, v));
                    int covered = 0; // how many terminals are covered
                    for (StaticVertex *v : *X) {
                        if (treeP->getVertSet()->find(v) != treeP->getVertSet()->end())
                            covered++;
                    }
                    double treePWeight = (double) treeP->getTotalWeight() / covered;
                    if ((*den)[treeBest] > treePWeight) {
                        treeBest = treeP;
                        (*den)[treeBest] = treePWeight;
                    }
                }
            }
            tree = Tree::merge(tree, treeBest); // TODO costly
            std::set<StaticVertex *> *intersect = vert_intersect(X, treeBest->getVertSet());
            k -= intersect->size();
            X = vert_minus(X, treeBest->getVertSet());
        }
    }
    return tree;
}

Tree *StaticGraph::alg4(TransitiveClosure *tr_cl, int i, int k, StaticVertex *root, std::set<StaticVertex *> *X) {
    Tree *tree = new Tree();
    if (i == 1) {
        if (k > X->size()) return tree;
        while (k > 0) {
            long minCost = LONG_MAX;
            StaticVertex *minVert;// vert with lighter-weight total path from root
            for (StaticVertex *v : *X) { // by now finding minVert too costly, could be improved
                long cost = tr_cl->costEdge(root, v); // O(N) method -- too costly as well
                if (cost < minCost) {
                    minCost = cost;
                    minVert = v;
                }
            }
            if (minCost != LONG_MAX)
                tree->add_edge(root, minVert, minCost);
            k--;
            X->erase(minVert);
        }
    }
    else {
        std::map<Tree *, double> *den = new std::map<Tree *, double>();
        while (k > 0) {
            Tree *treeBest = new Tree();
            (*den)[treeBest] = LONG_MAX;
            for (StaticVertex *v : *tr_cl->getVertSet()) {
                Tree *treeP = alg5(tr_cl, i - 1, k, v, new std::set<StaticVertex *>(X->begin(), X->end()),
                                   tr_cl->hasEdge(root, v));
                int size = X->size();
                treeP->add_edge(root, v, tr_cl->costEdge(root, v));
                int covered = 0; // how many terminals are covered
                for (StaticVertex *v : *X) {
                    if (treeP->getVertSet()->find(v) != treeP->getVertSet()->end())
                        covered++;
                }
                double treePWeight = (double) treeP->getTotalWeight() / covered;
                if ((*den)[treeBest] > treePWeight) {
                    treeBest = treeP;
                    (*den)[treeBest] = treePWeight;
                }
            }
            tree = Tree::merge(tree, treeBest);
            std::set<StaticVertex *> *intersect = vert_intersect(X, treeBest->getVertSet());
            k -= intersect->size();
            X = vert_minus(X, treeBest->getVertSet());
        }
    }
    return tree;
}

Tree *StaticGraph::alg5(TransitiveClosure *tr_cl, int i, int k, StaticVertex *root, std::set<StaticVertex *> *X,
                        StaticEdge *e) {
    Tree *tree = new Tree();
    Tree *treeC = new Tree();
    bool first = true;
    if (i == 1) {
        while (k > 0) {
            long minCost = LONG_MAX;
            StaticVertex *minVert;// vert with lighter-weight total path from root
            for (StaticVertex *v : *X) { // by now finding minVert too costly, could be improved
                long cost = tr_cl->costEdge(root, v); // O(N) method -- too costly as well
                if (cost < minCost) {
                    minCost = cost;
                    minVert = v;
                }
            }
            if (minCost != LONG_MAX)
                treeC->add_edge(root, minVert, minCost);
            k--;
            std::set<StaticVertex *> *oldX = new std::set<StaticVertex *>(X->begin(), X->end());
            X->erase(minVert);
            if (first) {
                tree = treeC;
                first = false;
            }
            else {
                if (tree->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX) >
                    treeC->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX)) {
                    tree = treeC;
                }
            }
            delete(oldX);
        }
    }
    else {
        std::map<Tree *, double> *den = new std::map<Tree *, double>();
        while (k > 0) {
            Tree *treeBest = new Tree();
            (*den)[treeBest] = LONG_MAX;
            for (StaticVertex *v : *tr_cl->getVertSet()) {
                Tree *treeP = alg5(tr_cl, i - 1, k, v, new std::set<StaticVertex *>(X->begin(), X->end()),
                                   tr_cl->hasEdge(root, v));
                treeP->add_edge(root, v, tr_cl->costEdge(root, v));
                double treePWeight = treeP->getDensity(X);
                if ((*den)[treeBest] > treePWeight) {
                    treeBest = treeP;
                    (*den)[treeBest] = treePWeight;
                }
            }
            treeC = Tree::merge(treeC, treeBest);
            std::set<StaticVertex *> *intersect = vert_intersect(X, treeBest->getVertSet());
            k -= intersect->size();
            X = vert_minus(X, treeBest->getVertSet());

            std::set<StaticVertex *> *oldX = new std::set<StaticVertex *>(X->begin(), X->end());
            if (first) {
                tree = treeC;
                first = false;
            }
            else {
                if (tree->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX) >
                    treeC->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX)) {
                    tree = treeC;
                }
            }
            delete(oldX);
        }
    }
    return tree;
}

void StaticGraph::putInOrder(std::vector<StaticGraph::myEntry*> *vec, StaticGraph::myEntry* entr) {
    if (vec->size() == 0) {
        vec->insert(vec->begin(), entr);
    }
    else {
        int iToPut = entrBinSearchOrNext(vec, entr);
        vec->insert(vec->begin() + iToPut, entr);
    }
}

int StaticGraph::entrBinSearchOrNext(std::vector<StaticGraph::myEntry*> *vec, StaticGraph::myEntry* entr) {
    int low = 0;
    int high = vec->size() - 1;

    while (low <= high) {
        // To convert to Javascript:
        // var mid = low + ((high - low) / 2) | 0;
        int mid = low + ((high - low) / 2);

        /**/ if ((*vec)[mid]->density < entr->density) low = mid + 1;
        else if ((*vec)[mid]->density > entr->density) high = mid - 1;
        else return mid + 1;
    }

    if (high < 0)
        return 0;   // key < data[0]
    else if (low > (vec->size() - 1))
        return vec->size(); // key >= data[len-1]
    else
        return (low < high)
               ? low + 1
               : high + 1;
}

Tree *StaticGraph::alg6(TransitiveClosure *tr_cl, int i, int k, StaticVertex *root, std::set<StaticVertex *> *X) {
    Tree *tree = new Tree();
    if (i == 1) {
        if (k > X->size()) return tree;
        while (k > 0) {
            long minCost = LONG_MAX;
            StaticVertex *minVert;// vert with lighter-weight total path from root
            for (StaticVertex *v : *X) { // by now finding minVert too costly, could be improved
                long cost = tr_cl->costEdge(root, v); // O(N) method -- too costly as well
                if (cost < minCost) {
                    minCost = cost;
                    minVert = v;
                }
            }
            if (minCost != LONG_MAX)
                tree->add_edge(root, minVert, minCost);
            k--;
            X->erase(minVert);
        }
    }
    else {
        std::map<Tree *, double> *den = new std::map<Tree *, double>();
        bool first = true;
        std::vector<myEntry*> *sorted = new std::vector<myEntry*>();
        while (k > 0) {
            Tree *treeBest = new Tree();
            (*den)[treeBest] = LONG_MAX;

            //-----------------------------------------------------
            if (first) {
                for (StaticVertex *v : *vertSet) {
                    StaticEdge *e = tr_cl->hasEdge(root, v);
                    Tree *treeP = alg7(tr_cl, i - 1, k, v, new std::set<StaticVertex *>(X->begin(), X->end()),
                                       e);
                    treeP->add_edge(root, v, e->getWeight());
                    double treePWeight = treeP->getDensity(X);
                    if ((*den)[treeBest] > treePWeight) {
                        treeBest = treeP;
                        (*den)[treeBest] = treePWeight;
                    }
                    //TODO:put with sorting
                    myEntry* entry = new myEntry;
                    entry->value = v;
                    entry->density = treePWeight;
                    putInOrder(sorted, entry);
                }
                first = false;
            }
            else {
                double t_best = LONG_MAX;
                for (myEntry* entry : *sorted) {
                    StaticVertex *v = entry->value;
                    if (entry->density < t_best) {
                        StaticEdge *e = tr_cl->hasEdge(root, v);
                        Tree *treeP = alg7(tr_cl, i - 1, k, v, new std::set<StaticVertex *>(X->begin(), X->end()),
                                           e);
                        treeP->add_edge(root, v, e->getWeight());
                        double treePWeight = treeP->getDensity(X);
                        if ((*den)[treeBest] > treePWeight){
                            treeBest = treeP;
                            (*den)[treeBest] = treePWeight;
                        }
                        entry->density = treePWeight;
                        if (treePWeight < t_best)
                            t_best = treePWeight;
                    }
                    else{
                        std::vector<myEntry*>* temp = new std::vector<myEntry*>();
                        for (int j = 0; j < sorted->size(); j++){
                            myEntry* e = (*sorted)[j];
                            putInOrder(temp, e);
                        }
                        delete(sorted);
                        sorted = temp;
                        break;
                    }
                }
            }
            tree = Tree::merge(tree, treeBest);
            std::set<StaticVertex *> *intersect = vert_intersect(X, treeBest->getVertSet());
            k -= intersect->size();
            X = vert_minus(X, treeBest->getVertSet());
        }
    }
    return tree;
}

Tree *StaticGraph::alg7(TransitiveClosure *tr_cl, int i, int k, StaticVertex *root, std::set<StaticVertex *> *X,
                        StaticEdge *e) {
    Tree *tree = new Tree();
    Tree *treeC = new Tree();
    bool first = true;
    if (i == 1) {
        while (k > 0) {
            long minCost = LONG_MAX;
            StaticVertex *minVert;// vert with lighter-weight total path from root
            for (StaticVertex *v : *X) { // by now finding minVert too costly, could be improved
                long cost = tr_cl->costEdge(root, v); // O(N) method -- too costly as well
                if (cost < minCost) {
                    minCost = cost;
                    minVert = v;
                }
            }
            if (minCost != LONG_MAX)
                treeC->add_edge(root, minVert, minCost);
            k--;
            std::set<StaticVertex *> *oldX = new std::set<StaticVertex *>(X->begin(), X->end());
            X->erase(minVert);
            if (first) {
                tree = treeC;
                first = false;
            }
            else {
                if (tree->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX) >
                    treeC->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX)) {
                    tree = treeC;
                }
            }
        }
    }
    else {
        std::map<Tree *, double> *den = new std::map<Tree *, double>();
        std::vector<myEntry*>* sorted = new std::vector<myEntry*>();
        while (k > 0) {
            Tree *treeBest = new Tree();
            (*den)[treeBest] = LONG_MAX;
            if (first) {
                for (StaticVertex *v : *tr_cl->getVertSet()) {
                    Tree *treeP = alg7(tr_cl, i - 1, k, v, new std::set<StaticVertex *>(X->begin(), X->end()),
                                       tr_cl->hasEdge(root, v));
                    treeP->add_edge(root, v, tr_cl->costEdge(root, v));
                    double treePWeight = treeP->getDensity(X);
                    if ((*den)[treeBest] > treePWeight) {
                        treeBest = treeP;
                        (*den)[treeBest] = treePWeight;
                    }
                    myEntry *entry = new myEntry;
                    entry->value = v;
                    entry->density = treePWeight;
                    putInOrder(sorted, entry);
                }
                first = false;
            }
            else{
                double t_best = LONG_MAX;
                for (myEntry* entry : *sorted) {
                    StaticVertex *v = entry->value;
                    if (entry->density < t_best) {
                        StaticEdge *e = tr_cl->hasEdge(root, v);
                        Tree *treeP = alg7(tr_cl, i - 1, k, v, new std::set<StaticVertex *>(X->begin(), X->end()),
                                           e);
                        treeP->add_edge(root, v, e->getWeight());
                        double treePWeight = treeP->getDensity(X);
                        if ((*den)[treeBest] > treePWeight){
                            treeBest = treeP;
                            (*den)[treeBest] = treePWeight;
                        }
                        entry->density = treePWeight;
                        if (treePWeight < t_best)
                            t_best = treePWeight;
                    }
                    else{
                        std::vector<myEntry*>* temp = new std::vector<myEntry*>();
                        for (int j = 0; j < sorted->size(); j++){
                            myEntry* e = (*sorted)[j];
                            putInOrder(temp, e);
                        }
                        delete(sorted);
                        sorted = temp;
                        break;
                    }
                }
            }
            treeC = Tree::merge(treeC, treeBest);
            std::set<StaticVertex *> *intersect = vert_intersect(X, treeBest->getVertSet());
            k -= intersect->size();
            X = vert_minus(X, treeBest->getVertSet());

            std::set<StaticVertex *> *oldX = new std::set<StaticVertex *>(X->begin(), X->end());
            if (first) {
                tree = treeC;
                first = false;
            }
            else {
                if (tree->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX) >
                    treeC->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX)) {
                    tree = treeC;
                }
            }
            delete(oldX);
        }
    }
    return tree;
}
/*********************************************/
/*      Transitive Closure implementation    */
/*********************************************/

long TransitiveClosure::costEdge(StaticVertex *u, StaticVertex *v) {
    auto vertList = (*adj_list)[u->getName()];
    for (int i = 0; i < vertList->size(); i++) {
        StaticEdge *edge = (*vertList)[i];
        if (edge->getTo() == v) {
            return edge->getWeight();
        }
    }
    return LONG_MAX;
}

long TransitiveClosure::costEdge(std::string u_name, std::string v_name) {
    auto vertList = (*adj_list)[u_name];
    for (int i = 0; i < vertList->size(); i++) {
        StaticEdge *edge = (*vertList)[i];
        if (edge->getTo()->getName() == v_name) {
            return edge->getWeight();
        }
    }
    return LONG_MAX;
}

TransitiveClosure::TransitiveClosure() : StaticGraph() {
    StaticGraph::StaticGraph();
}

TransitiveClosure::~TransitiveClosure() {
    StaticGraph::~StaticGraph();
}

StaticVertex *TransitiveClosure::getRoot() {
    return StaticGraph::getRoot();
}

void TransitiveClosure::setRoot(StaticVertex *v) {
    StaticGraph::setRoot(v);
}

std::set<StaticVertex *> *TransitiveClosure::getVertSet() {
    return StaticGraph::getVertSet();
}

std::map<std::string, StaticVertex *> *TransitiveClosure::getLabelVertMap() {
    return StaticGraph::getLabelVertMap();
}

void TransitiveClosure::add_edge(StaticVertex *from, StaticVertex *to, long weight) {
    StaticGraph::add_edge(from, to, weight);
}

void TransitiveClosure::remove_edge(StaticVertex *from, StaticVertex *to) {
    StaticGraph::remove_edge(from, to);
}

StaticEdge *TransitiveClosure::hasEdge(StaticVertex *from, StaticVertex *to) {
    return StaticGraph::hasEdge(from, to);
}

std::string TransitiveClosure::toString() {
    return StaticGraph::toString();
}

std::map<StaticVertex *, long> *TransitiveClosure::dijkstra(StaticVertex *source) {
    return StaticGraph::dijkstra(source);
}

TransitiveClosure *TransitiveClosure::transitiveClosure() {
    return StaticGraph::transitiveClosure();
}

/*********************************************/
/*          Tree class implementation        */
/*********************************************/

Tree::Tree() : StaticGraph() {
    StaticGraph::StaticGraph();
    totalWeight = 0;
}

Tree::Tree(Tree *tree) {
    this->adj_list = new std::map<std::string, std::vector<StaticEdge *> *>(tree->adj_list->begin(),
                                                                            tree->adj_list->end());
    for (auto it1 = this->adj_list->begin(); it1 != this->adj_list->end(); it1++) {
        std::vector<StaticEdge *> *vect = it1->second;
        vect = new std::vector<StaticEdge *>(vect->begin(), vect->end());
        it1->second = vect;
    }
    this->vertSet = new std::set<StaticVertex *>(tree->vertSet->begin(), tree->vertSet->end());
    this->labelVert = new std::map<std::string, StaticVertex *>(tree->labelVert->begin(), tree->labelVert->end());
    totalWeight = 0;
}

//Tree::~Tree() {
//    StaticGraph::~StaticGraph();
//}

StaticVertex *Tree::getRoot() {
    return StaticGraph::getRoot();
}

void Tree::setRoot(StaticVertex *v) {
    StaticGraph::setRoot(v);
}

std::set<StaticVertex *> *Tree::getVertSet() {
    return StaticGraph::getVertSet();
}

std::map<std::string, StaticVertex *> *Tree::getLabelVertMap() {
    return StaticGraph::getLabelVertMap();
}

void Tree::add_edge(StaticVertex *from, StaticVertex *to, long weight) {
    StaticGraph::add_edge(from, to, weight);
    totalWeight += weight;
}

Tree *Tree::addEdgeWithCopy(StaticVertex *from, StaticVertex *to, long weight) {
    Tree *tree = new Tree(this);
    tree->add_edge(from, to, weight);
    return tree;
}

void Tree::remove_edge(StaticVertex *from, StaticVertex *to) {
    StaticGraph::remove_edge(from, to);
}

StaticEdge *Tree::hasEdge(StaticVertex *from, StaticVertex *to) {
    return StaticGraph::hasEdge(from, to);
}

std::string Tree::toString() {
    return StaticGraph::toString();
}

long Tree::getTotalWeight() {
    return totalWeight;
}

double Tree::getDensity(std::set<StaticVertex *> *X) {
    int covered = 0;
    for (StaticVertex *v: *X) {
        if (vertSet->find(v) != vertSet->end())
            covered++;
    }
    return (double) getTotalWeight() / covered;
}

Tree *Tree::merge(Tree *t1, Tree *t2) {
    Tree *res = new Tree();
    for (auto it1 = t1->adj_list->begin(); it1 != t1->adj_list->end(); it1++) {
        for (StaticEdge *edge : *it1->second) {
            res->add_edge(edge->getFrom(), edge->getTo(), edge->getWeight());
        }
    }

    for (auto it1 = t2->adj_list->begin(); it1 != t2->adj_list->end(); it1++) {
        for (StaticEdge *edge : *it1->second) {
            if (!res->hasEdge(edge->getFrom(), edge->getTo())) {
                res->add_edge(edge->getFrom(), edge->getTo(), edge->getWeight());
            }
        }
    }
    return res;
}

std::set<StaticVertex *> *vert_intersect(std::set<StaticVertex *> *s1, std::set<StaticVertex *> *s2) {
    std::set<StaticVertex *> *res = new std::set<StaticVertex *>();
    for (StaticVertex *v: *s1) {
        if (s2->find(v) != s2->end()) {
            res->insert(res->begin(), v);
        }
    }
    return res;
}

std::set<StaticVertex *> *vert_minus(std::set<StaticVertex *> *s1, std::set<StaticVertex *> *s2) {
    std::set<StaticVertex *> *res = new std::set<StaticVertex *>();
    for (StaticVertex *v: *s1) {
        if (s2->find(v) == s2->end()) {
            res->insert(res->begin(), v);
        }
    }
    return res;
}
