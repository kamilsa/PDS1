#include <iostream>
#include "my_graphs/tgr.h"
#include "my_graphs/sgr.h"
#include <vector>
#include <map>
#include <stack>

using namespace std;

void mst_a1(vector<TempEdge *> *G, vector<TempVertex *> &vert_list, TempVertex *root, long low_bound, long up_bound);

void sort_edges(vector<TempEdge *> &edge_list); // sorts edges according to their start time
void swap(vector<TempEdge *> &v, int i, int j);

map<string, vector<TempEdge *> *> *create_sal(vector<TempVertex *> &vert_list,
                                          vector<TempEdge *> &edge_list); // creates sorted adjacency list
void mst_a2(map<string, vector<TempEdge *> *> *sal, vector<TempVertex *> vert_list, TempVertex* root, long low_bound, long up_bound);

void static_graph_test();

int main() {
    static_graph_test();
    TempVertex *v0 = new TempVertex("0");
    TempVertex *v1 = new TempVertex("1");
    TempVertex *v2 = new TempVertex("2");
    TempVertex *v3 = new TempVertex("3");
    TempVertex *v4 = new TempVertex("4");
    TempVertex *v5 = new TempVertex("5");

    vector<TempVertex *> *vert_list = new vector<TempVertex *>();
    vert_list->insert(vert_list->end(), v0);
    vert_list->insert(vert_list->end(), v1);
    vert_list->insert(vert_list->end(), v2);
    vert_list->insert(vert_list->end(), v3);
    vert_list->insert(vert_list->end(), v4);
    vert_list->insert(vert_list->end(), v5);

    vector<TempEdge *> *edge_list = new vector<TempEdge *>();
    edge_list->insert(edge_list->end(), new TempEdge(v0, v1, 1, 3));
    edge_list->insert(edge_list->end(), new TempEdge(v0, v2, 1, 5));
    edge_list->insert(edge_list->end(), new TempEdge(v0, v2, 3, 6));
    edge_list->insert(edge_list->end(), new TempEdge(v0, v1, 4, 5));
    edge_list->insert(edge_list->end(), new TempEdge(v1, v3, 4, 6));
    edge_list->insert(edge_list->end(), new TempEdge(v1, v4, 5, 8));
    edge_list->insert(edge_list->end(), new TempEdge(v2, v5, 6, 8));
    edge_list->insert(edge_list->end(), new TempEdge(v2, v4, 7, 9));
    edge_list->insert(edge_list->end(), new TempEdge(v4, v0, 8, 9));

//    edge_list->insert(edge_list->end(), new Edge(v0, v1, 1, 1));
//    edge_list->insert(edge_list->end(), new Edge(v1, v4, 3, 3));
//    edge_list->insert(edge_list->end(), new Edge(v4, v3, 4, 4));
//    edge_list->insert(edge_list->end(), new Edge(v3, v1, 2, 2));
//    edge_list->insert(edge_list->end(), new Edge(v3, v2, 4, 4));
//    edge_list->insert(edge_list->end(), new Edge(v2, v0, 2, 2));

//    mst_a1(edge_list, *vert_list, v0, 0, LONG_MAX);
    map<string,vector<TempEdge*>*>* sal = create_sal(*vert_list, *edge_list);
    mst_a2(sal, *vert_list, v0, 0, LONG_MAX);
    for (TempVertex *v : *vert_list) {
        if (v != v0) cout << "P(" << v->getName() << ") = " << v->getP()->getName()  << " A(" << v->getName() << ") = " << v->getA() << endl;
    }

    delete edge_list;

    return 0;
}


void static_graph_test(){
    StaticGraph* gr = new StaticGraph();
    StaticVertex* v1 = new StaticVertex("1");
    StaticVertex* v2 = new StaticVertex("2");
    StaticVertex* v3 = new StaticVertex("3");
    gr->add_edge(v1, v2, 1);
    gr->add_edge(v1, v3, 2);
    gr->add_edge(v3, v2, 3);

    gr->toString();
    delete gr;
}

void mst_a1(vector<TempEdge *> *G, vector<TempVertex *> &vert_list, TempVertex *root, long low_bound, long up_bound) {
    for (int i = 0; i < vert_list.size(); i++) {
        if (vert_list[i] != root) {
            vert_list[i]->setA(LONG_MAX);
            vert_list[i]->setP(root);
        }
        else {
            root->setA(low_bound);
            root->setP(nullptr);
        }

    }

    for (TempEdge *e : *G) {
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

/*
 * creates sorted adjacency list
 */
map<string, vector<TempEdge *> *> *create_sal(vector<TempVertex *> &vert_list, vector<TempEdge *> &edge_list) {
    map<string, vector<TempEdge *> *> *res = new map<string, vector<TempEdge *> *>();
    for (TempVertex *v : vert_list) {
        (*res)[v->getName()] = new vector<TempEdge *>();
    }
    sort_edges(edge_list);
    for (TempEdge *e : edge_list) {
        vector<TempEdge *> *temp = (*res)[e->getSource()->getName()];
        temp->insert(temp->begin(), e);
    }

    //display the sal
    for (TempVertex *v : vert_list) {
        vector<TempEdge *> *temp = (*res)[v->getName()];
        cout << v->getName() << ": ";
        for (TempEdge *edge : *temp) {
            cout << edge->toString() << ", ";
        }
        cout << endl;
    }
    return res;
};

void mst_a2(map<string, vector<TempEdge *> *> *sal, vector<TempVertex *> vert_list, TempVertex* root,  long low_bound, long up_bound) {
    struct mytuple {
        TempVertex* v1;
        TempVertex* v2;
        long time;
    };

    map<TempVertex *, int> pos;

    for (int i = 0; i < vert_list.size(); i++) {
        TempVertex *u = vert_list.operator[](i);
        pos[u] = 0;
        u->setA(LONG_MAX);
    }
    stack<mytuple>* st = new stack<mytuple>();
    mytuple temp;
    temp.v1 = root; temp.v2 = root; temp.time = low_bound;
    st->push(temp);
    while(!st->empty()){
        mytuple tup = st->top();
        st->pop();
        TempVertex* u = tup.v1;
        TempVertex* v = tup.v2;
        long tav = tup.time; // t arrival v
        if (tav < v->getA()){
            v->setA(tav);
            v->setP(u);
            vector<TempEdge*>* edge_list = (*sal)[v->getName()];
            if(pos[v] < edge_list->size()){
                TempEdge* e = (*edge_list)[pos[v]];
                v = e->getSource();
                TempVertex* vp = e->getDestination();
                long start = e->getStartTime();
                long arr = e->getArrTime();
                while (pos[v] < edge_list->size() && v->getA() <= start){
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

/*
 * sorts edges according to their start time
 */
void sort_edges(vector<TempEdge *> &edge_list) {
    for (int i = 0; i < edge_list.size() - 1; i++) {
        for (int j = 0; j < edge_list.size() - i - 1; j++) {
            if (edge_list[j]->getStartTime() > edge_list[j + 1]->getStartTime()) swap(edge_list, j, j + 1);
        }
    }
}

void swap(vector<TempEdge *> &v, int i, int j) {
    TempEdge *temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}