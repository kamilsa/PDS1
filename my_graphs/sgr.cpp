//
// Created by Kamil on 21/09/15.
//

#include <iostream>
#include <unordered_map>
#include "sgr.h"


bool classcomp::operator()(const shared_ptr<StaticVertex> &lhs, const shared_ptr<StaticVertex> &rhs) const {
    return lhs->getName() < rhs->getName();
}

std::size_t KeyHasher::operator()(const shared_ptr<StaticVertex> &k) const {
    using std::size_t;
    using std::hash;
    using std::string;

    return ((hash<string>()(k->getName())));
}

bool equal_to<shared_ptr<StaticVertex>>::operator()(const shared_ptr<StaticVertex> &x,
                                                    const shared_ptr<StaticVertex> &y) const {
    return x->getName() == y->getName();
}
/*********************************************/
/*         StaticVertex implementation       */
/*********************************************/
StaticVertex::StaticVertex(std::string name) {
    this->name = name;
    incidents = new unordered_map<string, StaticEdge*>();
}

StaticVertex::StaticVertex(shared_ptr<StaticVertex> v) {
    this->name = v->name;
    incidents = new unordered_map<string, StaticEdge*>();
}

StaticVertex::~StaticVertex() {
    delete incidents;
}

std::string StaticVertex::getName() const {
    return name;
}

void StaticVertex::setName(std::string name) {
    StaticVertex::name = name;
}

void StaticVertex::add_to_incidents(StaticEdge *e) {
    (*incidents)[e->getTo()->getName()] = e;
}

StaticEdge *StaticVertex::get_from_incidents(string str) {
    return (*incidents)[str];
}

shared_ptr<StaticVertex>StaticEdge::getFrom() {
    return from;
}

shared_ptr<StaticVertex>StaticEdge::getTo() {
    return to;
}

/*********************************************/
/*         StaticEdge implementation       */
/*********************************************/
StaticEdge::StaticEdge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to, float weight) {
    this->from = from;
    this->to = to;
    this->weight = weight;
}

StaticEdge::~StaticEdge() {
//    delete(from);
//    delete(to);
}

float StaticEdge::getWeight() {
    return weight;
}

void StaticEdge::setWeight(float weight) {
    StaticEdge::weight = weight;
}

void StaticEdge::setFrom(shared_ptr<StaticVertex> from) {
    StaticEdge::from = from;
}

void StaticEdge::setTo(shared_ptr<StaticVertex> to) {
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
    vertSet = new std::set<shared_ptr<StaticVertex>, classcomp>();
    labelVert = new std::map<std::string, shared_ptr<StaticVertex>>();
    terms = new set<shared_ptr<StaticVertex>, classcomp>();
}

StaticGraph::StaticGraph(StaticGraph *g) {
    adj_list = new std::map<std::string, std::vector<StaticEdge *> *>();
    vertSet = new std::set<shared_ptr<StaticVertex>, classcomp>();
    labelVert = new std::map<std::string, shared_ptr<StaticVertex>>();
    for (auto it1 = g->adj_list->begin(); it1 != g->adj_list->end(); it1++) {
        for (StaticEdge *edge : *it1->second) {
            this->add_edge(edge->getFrom(), edge->getTo(), edge->getWeight());
        }
    }
    terms = g->get_terms();
}

StaticGraph::~StaticGraph() {
    for (auto it1 = adj_list->begin(); it1 != adj_list->end(); it1++) {
        auto edge_list = it1->second;
        for (auto edge : *edge_list) {
            delete (edge);
        }
        delete (edge_list);
    }
    delete this->adj_list;
    for (auto vert : *vertSet)
        vert.reset();
    delete this->vertSet;
    delete this->labelVert;
    delete this->terms;
}

shared_ptr<StaticVertex>StaticGraph::getRoot() {
    return root;
}

void StaticGraph::setRoot(shared_ptr<StaticVertex> v) {
    this->root = v;
}

std::set<shared_ptr<StaticVertex>, classcomp> *StaticGraph::getVertSet() {
    return vertSet;
}

int StaticGraph::get_edge_number() {
    int res = 0;
    for (auto it1 = this->adj_list->begin(); it1 != adj_list->end(); it1++) {
        res += it1->second->size();
    }
    return res;
}

std::map<std::string, shared_ptr<StaticVertex>> *StaticGraph::getLabelVertMap() {
    return labelVert;
}

void StaticGraph::add_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to, float weight) {
    vertSet->insert(vertSet->end(), from);
    vertSet->insert(vertSet->end(), to);
    if ((*adj_list)[from->getName()] == 0x00) {
        (*adj_list)[from->getName()] = new std::vector<StaticEdge *>();
        (*labelVert)[from->getName()] = from;
        from->add_to_incidents(new StaticEdge(from, from, 0));
    }
    if ((*adj_list)[to->getName()] == 0x00) {
        (*adj_list)[to->getName()] = new std::vector<StaticEdge *>();
        (*labelVert)[to->getName()] = to;
    }
    StaticEdge *edge = new StaticEdge(from, to, weight);
    from->add_to_incidents(edge); // add to collection of incident vertices of from
    std::vector<StaticEdge *> *temp = (*adj_list)[from->getName()];
    temp->insert(temp->end(), edge);
}

void StaticGraph::remove_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) {
    //TODO
}

StaticEdge *StaticGraph::hasEdge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) {
//    auto list = (*adj_list)[from->getName()];
//    if (list == nullptr) return nullptr;
//    for (int i = 0; i < list->size(); i++) {
//        StaticEdge *e = (*list)[i];
//        if (e->getTo()->getName() == to->getName()) {
//            return e;
//        }
//    }
//    return nullptr;
    return from->get_from_incidents(to->getName());
}

set<shared_ptr<StaticVertex>, classcomp> *StaticGraph::get_terms() {
    return terms;
}

void StaticGraph::add_to_terms(shared_ptr<StaticVertex> v) {
    terms->insert(terms->begin(), v);
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

std::unordered_map<string, float> *StaticGraph::dijkstra(shared_ptr<StaticVertex> source) {
    std::unordered_map<string, float> *dist = new std::unordered_map<string, float>();

    // if we need keep previous vertexes in path
//    std::unordered_map<shared_ptr<StaticVertex>, shared_ptr<StaticVertex>> *prev = new std::unordered_map<shared_ptr<StaticVertex>, shared_ptr<StaticVertex>>();
    std::queue<shared_ptr<StaticVertex>> *q = new std::queue<shared_ptr<StaticVertex>>();
    q->push(source);
    (*dist)[source->getName()] = 0;
    for (shared_ptr<StaticVertex> v : *vertSet) {
        if (v->getName() != source->getName()) {
            (*dist)[v->getName()] = numeric_limits<float>::infinity();
//            (*prev)[v] = nullptr;
            q->push(v);
        }
    }
    while (!q->empty()) {
        shared_ptr<StaticVertex> u = q->front();
        q->pop();

        auto adj = (*adj_list)[u->getName()];
        for (int i = 0; i < adj->size(); i++) {
            StaticEdge *e = (*adj)[i];
            shared_ptr<StaticVertex> v = e->getTo();
            float alt;

            alt = (*dist)[u->getName()] + e->getWeight();
            if (alt < (*dist)[v->getName()]) {
                (*dist)[v->getName()] = alt;
            }
        }
    }

    //display the shortest paths:
//    std::cout << "Shortest paths from " << source->getName() << ": " << std::endl;
//    for (shared_ptr<StaticVertex> v : *vertSet) {
//        std::cout << v->getName() << ": " << (*dist)[v->getName()] << std::endl;
//    }

    delete q;
//    delete prev;
    return dist;
}

shared_ptr<TransitiveClosure> StaticGraph::transitiveClosure() {
    shared_ptr<TransitiveClosure> trcl(new TransitiveClosure());
    unordered_set<string> *considered = new unordered_set<string>();
    for (shared_ptr<StaticVertex> u : *vertSet) {
        if (considered->find(u->getName()) == considered->end())
            considered->insert(considered->begin(), u->getName());
        else
            continue;

        auto shortDists = dijkstra(u); // shortest distance from u to other nodes, infinity defines unreachable nodes
//        cout << endl;
//        cout << u->getName() << ":" << endl;
//        for (auto it1 = shortDists->begin(); it1 != shortDists->end(); it1++){
//            cout << it1->first << " : " << it1->second << endl;
//        }

        for (shared_ptr<StaticVertex> v : *vertSet) {
//            if(u->getName() != v->getName()) {
            if ((*shortDists)[v->getName()] != numeric_limits<float>::infinity()) {
//                    shared_ptr<StaticVertex> new_u(new StaticVertex(u));
//                    shared_ptr<StaticVertex> new_v(new StaticVertex(v));
                trcl->add_edge(u, v, (*shortDists)[v->getName()]);
            }
//            }
        }
    }
    delete considered;
    return trcl;
}

Tree *StaticGraph::alg3(shared_ptr<TransitiveClosure> tr_cl, int i, int k, shared_ptr<StaticVertex> root,
                        std::set<shared_ptr<StaticVertex>, classcomp> *X) {
    Tree *tree = new Tree();
    if (i == 1) {
        if (k > X->size()) return tree;
        while (k > 0) {
            StaticEdge *min_edge = tr_cl->min_cost_edge(root, X);// edge with lighter-weight total path from root
            if (min_edge != nullptr)
                tree->add_edge(root, min_edge->getTo(), min_edge->getWeight());
            k--;
            std::set<shared_ptr<StaticVertex>, classcomp> *oldX = new std::set<shared_ptr<StaticVertex>, classcomp>(
                    X->begin(), X->end());
            if (min_edge != nullptr)
                X->erase(min_edge->getTo());
        }
    }
    else {
        float bestDen;
        while (k > 0) {
            Tree *treeBest = new Tree();
            bestDen = numeric_limits<float>::infinity();
            for (shared_ptr<StaticVertex> v : *tr_cl->getVertSet()) {
                for (int kp = 1; kp <= k; ++kp) {
                    Tree *treeP = alg3(tr_cl, i - 1, kp, v,
                                       new std::set<shared_ptr<StaticVertex>, classcomp>(X->begin(), X->end()));
                    treeP->add_edge(root, v, tr_cl->costEdge(root, v));
                    float treePDen = treeP->getDensity(X);
                    if (bestDen > treePDen) {
                        treeBest = treeP;
                        bestDen = treePDen;
                    }
                }
            }
            tree = Tree::merge(tree, treeBest); // TODO costly
            std::set<shared_ptr<StaticVertex>, classcomp> *intersect = vert_intersect(X, treeBest->getVertSet());
            k -= intersect->size();
            delete intersect;
            cout << "k = " << k << endl;
            auto tmp = vert_minus(X, treeBest->getVertSet());
            delete X;
            tmp = X;
        }
    }
    return tree;
}

Tree *StaticGraph::alg4(shared_ptr<TransitiveClosure> tr_cl, int i, int k, shared_ptr<StaticVertex> root,
                        std::set<shared_ptr<StaticVertex>, classcomp> *X) {
    Tree *tree = new Tree();
    if (i == 1) {
        if (k > X->size()) return tree;
        while (k > 0) {
            StaticEdge *min_edge = tr_cl->min_cost_edge(root, X);// edge with lighter-weight total path from root
            if (min_edge != nullptr)
                tree->add_edge(root, min_edge->getTo(), min_edge->getWeight());
            k--;
            std::set<shared_ptr<StaticVertex>, classcomp> *oldX = new std::set<shared_ptr<StaticVertex>, classcomp>(
                    X->begin(), X->end());
            if (min_edge != nullptr)
                X->erase(min_edge->getTo());
        }
    }
    else {
        float bestDen;
        while (k > 0) {
            Tree *treeBest = new Tree();
            bestDen = numeric_limits<float>::infinity();
            for (shared_ptr<StaticVertex> v : *tr_cl->getVertSet()) {
                StaticEdge *e = tr_cl->hasEdge(root, v); ///!! If there is no such e could be problems
                if (e == nullptr) continue;
                Tree *treeP = alg5(tr_cl, i - 1, k, v,
                                   new std::set<shared_ptr<StaticVertex>, classcomp>(X->begin(), X->end()), e);
                treeP->add_edge(root, v, tr_cl->costEdge(root, v));
                float treePDen = treeP->getDensity(X);
                if (bestDen > treePDen) {
                    treeBest = treeP;
                    bestDen = treePDen;
                }
            }
            tree = Tree::merge(tree, treeBest);
            std::set<shared_ptr<StaticVertex>, classcomp> *intersect = vert_intersect(X, treeBest->getVertSet());
            k -= intersect->size();
            delete intersect;
            cout << "k = " << k << endl;
            auto tmp = vert_minus(X, treeBest->getVertSet());
            delete X;
            X = tmp;
        }
    }
    return tree;
}

Tree *StaticGraph::alg5(shared_ptr<TransitiveClosure> tr_cl, int i, int k, shared_ptr<StaticVertex> root,
                        std::set<shared_ptr<StaticVertex>, classcomp> *X,
                        StaticEdge *e) {
    Tree *tree = new Tree();
    Tree *treeC = new Tree();
    bool first = true;
    if (i == 1) {
        while (k > 0) {
            StaticEdge *min_edge = tr_cl->min_cost_edge(root, X);// edge with lighter-weight total path from root
            if (min_edge != nullptr)
                treeC->add_edge(root, min_edge->getTo(), min_edge->getWeight());
            k--;
            std::set<shared_ptr<StaticVertex>, classcomp> *oldX = new std::set<shared_ptr<StaticVertex>, classcomp>(
                    X->begin(), X->end());
            if (min_edge != nullptr)
                X->erase(min_edge->getTo());

            if (first) {
                tree = treeC;
                first = false;
            }
            else {
//                if (tree->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX) >
//                    treeC->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX)) {
//                    tree = treeC;
//                } mistaken!
                if (tree->getDensityWithEdge(oldX, e) > treeC->getDensityWithEdge(oldX, e)) {
                    tree = treeC;
                }
            }
            delete (oldX);
        }
    }
    else {
        float bestDen;
        while (k > 0) {
            Tree *treeBest = new Tree();
            bestDen = numeric_limits<float>::infinity();
            for (shared_ptr<StaticVertex> v : *tr_cl->getVertSet()) {
                Tree *treeP = alg5(tr_cl, i - 1, k, v,
                                   new std::set<shared_ptr<StaticVertex>, classcomp>(X->begin(), X->end()),
                                   tr_cl->hasEdge(root, v));
                treeP->add_edge(root, v, tr_cl->costEdge(root, v));
                float treePDen = treeP->getDensity(X);
                if (bestDen > treePDen) {
                    treeBest = treeP;
                    bestDen = treePDen;
                }
            }
            treeC = Tree::merge(treeC, treeBest);
            std::set<shared_ptr<StaticVertex>, classcomp> *intersect = vert_intersect(X, treeBest->getVertSet());
            k -= intersect->size();
            delete intersect;
            X = vert_minus(X, treeBest->getVertSet());

            std::set<shared_ptr<StaticVertex>, classcomp> *oldX = new std::set<shared_ptr<StaticVertex>, classcomp>(
                    X->begin(), X->end());
            if (first) {
                tree = treeC;
                first = false;
            }
            else {
                //                if (tree->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX) >
//                    treeC->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX)) {
//                    tree = treeC;
//                } mistaken!
                if (tree->getDensityWithEdge(oldX, e) > treeC->getDensityWithEdge(oldX, e)) {
                    tree = treeC;
                }
            }
            delete (oldX);
        }
    }
    return tree;
}

void StaticGraph::putInOrder(std::vector<StaticGraph::myEntry *> *vec, StaticGraph::myEntry *entr) {
    if (vec->size() == 0) {
        vec->insert(vec->begin(), entr);
    }
    else {
        int iToPut = entrBinSearchOrNext(vec, entr);
        vec->insert(vec->begin() + iToPut, entr);
    }
}

int StaticGraph::entrBinSearchOrNext(std::vector<StaticGraph::myEntry *> *vec, StaticGraph::myEntry *entr) {
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

Tree *StaticGraph::alg6(shared_ptr<TransitiveClosure> tr_cl, int i, int k, shared_ptr<StaticVertex> root,
                        std::set<shared_ptr<StaticVertex>, classcomp> *X) {
    Tree *tree = new Tree();
    if (i == 1) {
        if (k > X->size()) return tree;
        while (k > 0) {
            StaticEdge *min_edge = tr_cl->min_cost_edge(root, X);// edge with lighter-weight total path from root
            if (min_edge != nullptr)
                tree->add_edge(root, min_edge->getTo(), min_edge->getWeight());
            k--;
//            std::set<shared_ptr<StaticVertex>, classcomp> *oldX = new std::set<shared_ptr<StaticVertex>, classcomp>(
//                    X->begin(), X->end());
            if (min_edge != nullptr)
                X->erase(min_edge->getTo());
        }
    }
    else {
        float bestDen;
        bool first = true;
        std::vector<myEntry *> *sorted = new std::vector<myEntry *>();
        float prev = -1;
        while (k > 0) {
            Tree *treeBest = new Tree();
            bestDen = numeric_limits<float>::infinity();

            //-----------------------------------------------------
            if (first) {
                for (shared_ptr<StaticVertex> v : *vertSet) {
                    StaticEdge *e = tr_cl->hasEdge(root, v); ///!! If there is no such e could be problems
//                    if (e == nullptr) continue;
                    if (e == nullptr) e = new StaticEdge(root, v, numeric_limits<float>::infinity());
                    Tree *treeP = alg7(tr_cl, i - 1, k, v,
                                       new std::set<shared_ptr<StaticVertex>, classcomp>(X->begin(), X->end()),
                                       e);
                    treeP->add_edge(root, v, e->getWeight());
                    float treePDen = treeP->getDensity(X);
                    if (bestDen > treePDen) {
                        treeBest = treeP;
                        bestDen = treePDen;
                    }
                    //putting with sorting:
                    myEntry *entry = new myEntry;
                    entry->value = v;
                    entry->density = treePDen;
                    if (!isnan(entry->density))
                        putInOrder(sorted, entry);
                }
                first = false;
            }
            else {
                float t_best = numeric_limits<float>::infinity();
                for (myEntry *entry : *sorted) {
                    shared_ptr<StaticVertex> v = entry->value;
                    if (entry->density < t_best) {
                        StaticEdge *e = tr_cl->hasEdge(root, v);
                        if (e == nullptr) e = new StaticEdge(root, v, numeric_limits<float>::infinity());
                        Tree *treeP = alg7(tr_cl, i - 1, k, v,
                                           new std::set<shared_ptr<StaticVertex>, classcomp>(X->begin(), X->end()),
                                           e);
                        treeP->add_edge(root, v, e->getWeight());
                        float treePDen = treeP->getDensity(X);
                        if (bestDen > treePDen) {
                            treeBest = treeP;
                            bestDen = treePDen;
                        }
                        entry->density = treePDen;
                        if (treePDen < t_best)
                            t_best = treePDen;
                    }
                    else {
                        std::vector<myEntry *> *temp = new std::vector<myEntry *>();
                        for (int j = 0; j < sorted->size(); j++) {
                            myEntry *e = (*sorted)[j];
                            if (!isnan(e->density))
                                putInOrder(temp, e);
                        }
                        delete (sorted);
                        sorted = temp;
                        break;
                    }
                }
            }
            tree = Tree::merge(tree, treeBest);
            std::set<shared_ptr<StaticVertex>, classcomp> *intersect = vert_intersect(X, treeBest->getVertSet());
            k -= intersect->size();
            delete intersect;
            cout << "k = " << k << endl;
            auto tmp = vert_minus(X, treeBest->getVertSet());
            delete X;
            X = tmp;
            if (prev == k)
                break;
            else
                prev = k;
        }
    }
    return tree;
}

Tree *StaticGraph::alg7(shared_ptr<TransitiveClosure> tr_cl, int i, int k, shared_ptr<StaticVertex> root,
                        std::set<shared_ptr<StaticVertex>, classcomp> *X,
                        StaticEdge *e) {
    Tree *tree = new Tree();
    Tree *treeC = new Tree();
    bool first1 = true;
    bool first2 = true;
    if (i == 1) {
        while (k > 0) {
            StaticEdge *min_edge = tr_cl->min_cost_edge(root, X);// edge with lighter-weight total path from root
            if (min_edge != nullptr)
                treeC->add_edge(root, min_edge->getTo(), min_edge->getWeight());
            k--;
//            std::set<shared_ptr<StaticVertex>, classcomp> *oldX = new std::set<shared_ptr<StaticVertex>, classcomp>(
//                    X->begin(), X->end());

            if (first1) {
                tree = treeC;
                first1 = false;
            }
            else {
                if (tree->getDensityWithEdge(X, e) > treeC->getDensityWithEdge(X, e)) {
                    delete tree;
                    tree = treeC;
                }
            }
            if (min_edge != nullptr)
                X->erase(min_edge->getTo());
        }
    }
    else {
        float bestDen;
        std::vector<myEntry *> *sorted = new std::vector<myEntry *>();
        float prev = -1;
        while (k > 0) {
            Tree *treeBest = new Tree();
            bestDen = numeric_limits<float>::infinity();
            if (first1) {
                for (shared_ptr<StaticVertex> v : *tr_cl->getVertSet()) {
                    StaticEdge *edge = tr_cl->hasEdge(root, v);
                    Tree *treeP = alg7(tr_cl, i - 1, k, v,
                                       new std::set<shared_ptr<StaticVertex>, classcomp>(X->begin(), X->end()), edge);
                    treeP->add_edge(root, v, tr_cl->costEdge(root, v));
                    float treePDen = treeP->getDensity(X);
                    if (bestDen > treePDen) {
                        treeBest = treeP;
                        bestDen = treePDen;
                    }
                    myEntry *entry = new myEntry;
                    entry->value = v;
                    entry->density = treePDen;
                    putInOrder(sorted, entry);
                }
                first1 = false;
            }
            else {
                float t_best = numeric_limits<float>::infinity();
                for (myEntry *entry : *sorted) {
                    shared_ptr<StaticVertex> v = entry->value;
                    if (entry->density < t_best) {
                        StaticEdge *e = tr_cl->hasEdge(root, v);
                        Tree *treeP = alg7(tr_cl, i - 1, k, v,
                                           new std::set<shared_ptr<StaticVertex>, classcomp>(X->begin(), X->end()),
                                           e);
                        treeP->add_edge(root, v, e->getWeight());
                        float treePDen = treeP->getDensity(X);
                        if (bestDen > treePDen) {
                            treeBest = treeP;
                            bestDen = treePDen;
                        }
                        entry->density = treePDen;
                        if (treePDen < t_best)
                            t_best = treePDen;
                    }
                    else {
                        std::vector<myEntry *> *temp = new std::vector<myEntry *>();
                        for (int j = 0; j < sorted->size(); j++) {
                            myEntry *e = (*sorted)[j];
                            putInOrder(temp, e);
                        }
                        delete (sorted);
                        sorted = temp;
                        break;
                    }
                }
            }
            treeC = Tree::merge(treeC, treeBest);
            std::set<shared_ptr<StaticVertex>, classcomp> *intersect = vert_intersect(X, treeBest->getVertSet());
            k -= intersect->size();
            delete intersect;
            X = vert_minus(X, treeBest->getVertSet());

//            std::set<shared_ptr<StaticVertex>, classcomp> *oldX = new std::set<shared_ptr<StaticVertex>, classcomp>(
//                    X->begin(), X->end());
            if (first2) {
                tree = treeC;
                first2 = false;
            }
            else {
                //                if (tree->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX) >
//                    treeC->addEdgeWithCopy(e->getFrom(), e->getTo(), e->getWeight())->getDensity(oldX)) {
//                    tree = treeC;
//                } mistaken!
                if (tree->getDensityWithEdge(X, e) > treeC->getDensityWithEdge(X, e)) {
                    tree = treeC;
                }
            }
        }
    }
    return tree;
}
/*********************************************/
/*      Transitive Closure implementation    */
/*********************************************/

float TransitiveClosure::costEdge(shared_ptr<StaticVertex> u, shared_ptr<StaticVertex> v) {
//    auto vertList = (*adj_list)[u->getName()];
//    for (int i = 0; i < vertList->size(); i++) {
//        StaticEdge *edge = (*vertList)[i];
//        if (edge->getTo()->getName() == v->getName()) {
//            return edge->getWeight();
//        }
//    }
    auto e = this->hasEdge(u, v);
    if (e != nullptr)
        return e->getWeight();
    else
        return numeric_limits<float>::infinity();
}

//float TransitiveClosure::costEdge(std::string u_name, std::string v_name) {
//    auto vertList = (*adj_list)[u_name];
//    for (int i = 0; i < vertList->size(); i++) {
//        StaticEdge *edge = (*vertList)[i];
//        if (edge->getTo()->getName() == v_name) {
//            return edge->getWeight();
//        }
//    }
//    return numeric_limits<float>::infinity();
//}

StaticEdge *TransitiveClosure::min_cost_edge(shared_ptr<StaticVertex> u, set<shared_ptr<StaticVertex>, classcomp> *X) {
    auto adj = (*adj_list)[u->getName()];
    for (auto el : *adj) {
        if (X->find(el->getTo()) != X->end()) {
            return el;
        }
    }
    return nullptr;
}

int TransitiveClosure::bin_search_or_next_edge(vector<StaticEdge *> *vect, StaticEdge *edge) {
    int low = 0;
    int high = vect->size() - 1;

    while (low <= high) {
        // To convert to Javascript:
        // var mid = low + ((high - low) / 2) | 0;
        int mid = low + ((high - low) / 2);

        /**/ if ((*vect)[mid]->getWeight() < edge->getWeight()) low = mid + 1;
        else if ((*vect)[mid]->getWeight() > edge->getWeight()) high = mid - 1;
        else return mid + 1;
    }

    if (high < 0)
        return 0;   // key < data[0]
    else if (low > (vect->size() - 1))
        return vect->size(); // key >= data[len-1]
    else
        return (low < high)
               ? low + 1
               : high + 1;
}

TransitiveClosure::TransitiveClosure() : StaticGraph() {
    StaticGraph::StaticGraph();
}

//TransitiveClosure::~TransitiveClosure() {
//    StaticGraph::~StaticGraph();
//}

shared_ptr<StaticVertex>TransitiveClosure::getRoot() {
    return StaticGraph::getRoot();
}

void TransitiveClosure::setRoot(shared_ptr<StaticVertex> v) {
    StaticGraph::setRoot(v);
}

std::set<shared_ptr<StaticVertex>, classcomp> *TransitiveClosure::getVertSet() {
    return StaticGraph::getVertSet();
}

std::map<std::string, shared_ptr<StaticVertex>> *TransitiveClosure::getLabelVertMap() {
    return StaticGraph::getLabelVertMap();
}

void TransitiveClosure::add_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to, float weight) {
//    StaticGraph::add_edge(from, to, weight);
    vertSet->insert(vertSet->end(), from);
    vertSet->insert(vertSet->end(), to);
    if ((*adj_list)[from->getName()] == 0x00) {
        (*adj_list)[from->getName()] = new std::vector<StaticEdge *>();
        (*labelVert)[from->getName()] = from;
        from->add_to_incidents(new StaticEdge(from, from, 0));
    }
    if ((*adj_list)[to->getName()] == 0x00) {
        (*adj_list)[to->getName()] = new std::vector<StaticEdge *>();
        (*labelVert)[to->getName()] = to;
    }
    StaticEdge *edge = new StaticEdge(from, to, weight);
    from->add_to_incidents(edge); // add to collection of incident vertices of from
    std::vector<StaticEdge *> *temp = (*adj_list)[from->getName()];
    if (temp->empty())
        temp->insert(temp->end(), edge);
    else {
        int pos = bin_search_or_next_edge(temp, edge);
        temp->insert(temp->begin() + pos, edge);
    }
}

void TransitiveClosure::remove_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) {
    StaticGraph::remove_edge(from, to);
}

StaticEdge *TransitiveClosure::hasEdge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) {
    return StaticGraph::hasEdge(from, to);
}

std::string TransitiveClosure::toString() {
    return StaticGraph::toString();
}

std::unordered_map<std::string, float> *TransitiveClosure::dijkstra(shared_ptr<StaticVertex> source) {
    return StaticGraph::dijkstra(source);
}

shared_ptr<TransitiveClosure> TransitiveClosure::transitiveClosure() {
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
    this->vertSet = new std::set<shared_ptr<StaticVertex>, classcomp>(tree->vertSet->begin(), tree->vertSet->end());
    this->labelVert = new std::map<std::string, shared_ptr<StaticVertex>>(tree->labelVert->begin(),
                                                                          tree->labelVert->end());
    totalWeight = 0;
}

//Tree::~Tree() {
//    StaticGraph::~StaticGraph();
//}

shared_ptr<StaticVertex>Tree::getRoot() {
    return StaticGraph::getRoot();
}

void Tree::setRoot(shared_ptr<StaticVertex> v) {
    StaticGraph::setRoot(v);
}

std::set<shared_ptr<StaticVertex>, classcomp> *Tree::getVertSet() {
    return StaticGraph::getVertSet();
}

std::map<std::string, shared_ptr<StaticVertex>> *Tree::getLabelVertMap() {
    return StaticGraph::getLabelVertMap();
}

void Tree::add_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to, float weight) {
    StaticGraph::add_edge(from, to, weight);
    totalWeight += weight;
}

Tree *Tree::addEdgeWithCopy(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to, float weight) {
    Tree *tree = new Tree(this);
    tree->add_edge(from, to, weight);
    return tree;
}

void Tree::remove_edge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) {
    StaticGraph::remove_edge(from, to);
}

StaticEdge *Tree::hasEdge(shared_ptr<StaticVertex> from, shared_ptr<StaticVertex> to) {
    return StaticGraph::hasEdge(from, to);
}

std::string Tree::toString() {
    return StaticGraph::toString();
}

float Tree::getTotalWeight() {
    return totalWeight;
}

float Tree::getDensity(std::set<shared_ptr<StaticVertex>, classcomp> *X) {
    int covered = 0;
    covered = (int) vert_intersect(X, vertSet)->size();
    return getTotalWeight() / covered;
}

float Tree::getDensityWithEdge(set<shared_ptr<StaticVertex>, classcomp> *X, StaticEdge *e) {
    float totalWeight = getTotalWeight() + e->getWeight();
    int covered = (int) vert_intersect(X, vertSet)->size();
    if (vertSet->find(e->getTo()) == vertSet->end() && X->find(e->getTo()) != X->end()) {
        covered++;
    }
    if (vertSet->find(e->getFrom()) == vertSet->end() && X->find(e->getFrom()) != X->end()) {
        covered++;
    }
    return totalWeight / covered;
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

std::set<shared_ptr<StaticVertex>, classcomp> *vert_intersect(std::set<shared_ptr<StaticVertex>, classcomp> *s1,
                                                              std::set<shared_ptr<StaticVertex>, classcomp> *s2) {
    std::set<shared_ptr<StaticVertex>, classcomp> *res = new std::set<shared_ptr<StaticVertex>, classcomp>();
    for (shared_ptr<StaticVertex> v: *s1) {
        if (s2->find(v) != s2->end()) {
            res->insert(res->begin(), v);
        }
    }
    return res;
}

std::set<shared_ptr<StaticVertex>, classcomp> *vert_minus(std::set<shared_ptr<StaticVertex>, classcomp> *s1,
                                                          std::set<shared_ptr<StaticVertex>, classcomp> *s2) {
    std::set<shared_ptr<StaticVertex>, classcomp> *res = new std::set<shared_ptr<StaticVertex>, classcomp>();
    for (shared_ptr<StaticVertex> v: *s1) {
        if (s2->find(v) == s2->end()) {
            res->insert(res->begin(), v);
        }
    }
    return res;
}
