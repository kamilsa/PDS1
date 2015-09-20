#include <iostream>
#include "temp_graphs/tgr.h"
#include <vector>
#include <map>
#include <stack>

using namespace std;

void mst_a1(vector<Edge *> *G, vector<Vertex *> &vert_list, Vertex *root, long low_bound, long up_bound);

void sort_edges(vector<Edge *> &edge_list); // sorts edges according to their start time
void swap(vector<Edge *> &v, int i, int j);

map<string, vector<Edge *> *> *create_sal(vector<Vertex *> &vert_list,
                                          vector<Edge *> &edge_list); // creates sorted adjacency list
void mst_a2(map<string, vector<Edge *> *> *sal, vector<Vertex *> vert_list, Vertex* root, long low_bound, long up_bound);

int main() {

    Vertex *v0 = new Vertex("0");
    Vertex *v1 = new Vertex("1");
    Vertex *v2 = new Vertex("2");
    Vertex *v3 = new Vertex("3");
    Vertex *v4 = new Vertex("4");
    Vertex *v5 = new Vertex("5");

    vector<Vertex *> *vert_list = new vector<Vertex *>();
    vert_list->insert(vert_list->end(), v0);
    vert_list->insert(vert_list->end(), v1);
    vert_list->insert(vert_list->end(), v2);
    vert_list->insert(vert_list->end(), v3);
    vert_list->insert(vert_list->end(), v4);
    vert_list->insert(vert_list->end(), v5);

    vector<Edge *> *edge_list = new vector<Edge *>();
//    edge_list->insert(edge_list->end(), new Edge(v0, v1, 1, 3));
//    edge_list->insert(edge_list->end(), new Edge(v0, v2, 1, 5));
//    edge_list->insert(edge_list->end(), new Edge(v0, v2, 3, 6));
//    edge_list->insert(edge_list->end(), new Edge(v0, v1, 4, 5));
//    edge_list->insert(edge_list->end(), new Edge(v1, v3, 4, 6));
//    edge_list->insert(edge_list->end(), new Edge(v1, v4, 5, 8));
//    edge_list->insert(edge_list->end(), new Edge(v2, v5, 6, 8));
//    edge_list->insert(edge_list->end(), new Edge(v2, v4, 7, 9));
//    edge_list->insert(edge_list->end(), new Edge(v4, v0, 8, 9));

//    edge_list->insert(edge_list->end(), new Edge(v0, v1, 1, 1));
//    edge_list->insert(edge_list->end(), new Edge(v1, v4, 3, 3));
//    edge_list->insert(edge_list->end(), new Edge(v4, v3, 4, 4));
//    edge_list->insert(edge_list->end(), new Edge(v3, v1, 2, 2));
//    edge_list->insert(edge_list->end(), new Edge(v3, v2, 4, 4));
//    edge_list->insert(edge_list->end(), new Edge(v2, v0, 2, 2));

//    mst_a1(edge_list, *vert_list, v0, 0, LONG_MAX);
    map<string,vector<Edge*>*>* sal = create_sal(*vert_list, *edge_list);
    mst_a2(sal, *vert_list, v0, 0, LONG_MAX);
    for (Vertex *v : *vert_list) {
        if (v != v0) cout << "P(" << v->getName() << ") = " << v->getP()->getName() << endl;
    }

    delete edge_list;

    return 0;
}

void mst_a1(vector<Edge *> *G, vector<Vertex *> &vert_list, Vertex *root, long low_bound, long up_bound) {
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

    for (Edge *e : *G) {
        Vertex *u = e->getSource();
        Vertex *v = e->getDestination();
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
map<string, vector<Edge *> *> *create_sal(vector<Vertex *> &vert_list, vector<Edge *> &edge_list) {
    map<string, vector<Edge *> *> *res = new map<string, vector<Edge *> *>();
    for (Vertex *v : vert_list) {
        (*res)[v->getName()] = new vector<Edge *>();
    }
    sort_edges(edge_list);
    for (Edge *e : edge_list) {
        vector<Edge *> *temp = (*res)[e->getSource()->getName()];
        temp->insert(temp->begin(), e);
    }

    //display the sal
    for (Vertex *v : vert_list) {
        vector<Edge *> *temp = (*res)[v->getName()];
        cout << v->getName() << ":" << endl;
        for (Edge *edge : *temp) {
            cout << edge->toString();
        }
    }
    return res;
};

void mst_a2(map<string, vector<Edge *> *> *sal, vector<Vertex *> vert_list, Vertex* root,  long low_bound, long up_bound) {
    struct mytuple {
        Vertex* v1;
        Vertex* v2;
        long time;
    };

    map<Vertex *, int> pos;

    for (int i = 0; i < vert_list.size(); i++) {
        Vertex *u = vert_list.operator[](i);
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
        Vertex* u = tup.v1;
        Vertex* v = tup.v2;
        long tav = tup.time; // t arrival v
        if (tav < v->getA()){
            v->setA(tav);
            v->setP(u);
            vector<Edge*>* edge_list = (*sal)[v->getName()];
            if(pos[v] < edge_list->size()){
                Edge* e = (*edge_list)[pos[v]];
                v = e->getSource();
                Vertex* vp = e->getDestination();
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
void sort_edges(vector<Edge *> &edge_list) {
    for (int i = 0; i < edge_list.size() - 1; i++) {
        for (int j = 0; j < edge_list.size() - i - 1; j++) {
            if (edge_list[j]->getStartTime() > edge_list[j + 1]->getStartTime()) swap(edge_list, j, j + 1);
        }
    }
}

void swap(vector<Edge *> &v, int i, int j) {
    Edge *temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}