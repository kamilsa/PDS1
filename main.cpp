#include <iostream>
#include "my_graphs/tgr.h"
#include "my_graphs/sgr.h"
#include <stack>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <chrono> // to measure time

using namespace std;
using namespace boost;

void mst_a1(vector<TempEdge *> *G, vector<TempVertex *> &vert_list, TempVertex *root, long low_bound, long up_bound);

void sort_edges(vector<TempEdge *> &edge_list); // sorts edges according to their start time
void swap(vector<TempEdge *> &v, int i, int j);

map<string, vector<TempEdge *> *> *create_sal(vector<TempVertex *> &vert_list,
                                              vector<TempEdge *> &edge_list); // creates sorted adjacency list
void mst_a2(map<string, vector<TempEdge *> *> *sal, vector<TempVertex *> vert_list, TempVertex *root, long low_bound,
            long up_bound);

void static_graph_test();

void temp_graph_test();

void boost_test();

void set_test();

int main() {
//    static_graph_test();
        auto start = std::chrono::high_resolution_clock::now();
        temp_graph_test();
//        set_test();
        auto finish = std::chrono::high_resolution_clock::now();
        cout << "Execution time : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
//    boost_test();
    return 0;
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
    edge_list->insert(edge_list->end(), new TempEdge(v0, v1, 1, 3, 2));
    edge_list->insert(edge_list->end(), new TempEdge(v0, v2, 1, 5, 4));
    edge_list->insert(edge_list->end(), new TempEdge(v0, v2, 3, 6, 3));
    edge_list->insert(edge_list->end(), new TempEdge(v0, v1, 4, 5, 1));
    edge_list->insert(edge_list->end(), new TempEdge(v1, v3, 4, 6, 2));
    edge_list->insert(edge_list->end(), new TempEdge(v1, v4, 5, 8, 3));
    edge_list->insert(edge_list->end(), new TempEdge(v2, v5, 6, 8, 2));
    edge_list->insert(edge_list->end(), new TempEdge(v2, v4, 7, 9, 2));
    edge_list->insert(edge_list->end(), new TempEdge(v4, v0, 8, 9, 1));

//    edge_list->insert(edge_list->end(), new Edge(v0, v1, 1, 1));
//    edge_list->insert(edge_list->end(), new Edge(v1, v4, 3, 3));
//    edge_list->insert(edge_list->end(), new Edge(v4, v3, 4, 4));
//    edge_list->insert(edge_list->end(), new Edge(v3, v1, 2, 2));
//    edge_list->insert(edge_list->end(), new Edge(v3, v2, 4, 4));
//    edge_list->insert(edge_list->end(), new Edge(v2, v0, 2, 2));

//    mst_a1(edge_list, *vert_list, v0, 0, LONG_MAX);
    map<string, vector<TempEdge *> *> *sal = create_sal(*vert_list, *edge_list);
    mst_a2(sal, *vert_list, v0, 0, LONG_MAX);
    for (TempVertex *v : *vert_list) {
        if (v != v0)
            cout << "P(" << v->getName() << ") = " << v->getP()->getName() << " A(" << v->getName() << ") = " <<
            v->getA() << endl;
    }

    delete edge_list;

    return 0;
}


void static_graph_test() {
    StaticGraph *gr = new StaticGraph();
    StaticVertex *v1 = new StaticVertex("1");
    StaticVertex *v2 = new StaticVertex("2");
    StaticVertex *v3 = new StaticVertex("3");
    gr->add_edge(v1, v2, 1);
    gr->add_edge(v1, v3, 2);
    gr->add_edge(v3, v2, 3);

    cout << gr->toString();
    delete gr;
}

void temp_graph_test() {
    TempGraph *g = new TempGraph();

    TempVertex *v0 = new TempVertex("0");
    TempVertex *v1 = new TempVertex("1");
    TempVertex *v2 = new TempVertex("2");
    TempVertex *v3 = new TempVertex("3");
    TempVertex *v4 = new TempVertex("4");
    TempVertex *v5 = new TempVertex("5");

    g->addEdge(new TempEdge(v0, v1, 1, 3, 2));
    g->addEdge(new TempEdge(v0, v2, 1, 5, 4));
    g->addEdge(new TempEdge(v0, v2, 3, 6, 3));
    g->addEdge(new TempEdge(v0, v1, 4, 5, 1));
    g->addEdge(new TempEdge(v1, v3, 4, 6, 2));
    g->addEdge(new TempEdge(v1, v4, 5, 8, 3));
    g->addEdge(new TempEdge(v2, v5, 6, 8, 2));
    g->addEdge(new TempEdge(v2, v4, 7, 9, 2));
    g->addEdge(new TempEdge(v4, v0, 8, 9, 1));

    g->mst_a2(v0, 0, LONG_MAX, true);
    for (TempVertex *v : *g->getVertSet()) {
        if (v != v0)
            cout << "P(" << v->getName() << ") = " << v->getP()->getName() << " A(" << v->getName() << ") = " <<
            v->getA() << endl;
    }
    cout << "It was from class" << endl;

    StaticGraph *staticGraph = g->getStaticGraph(v0);
    cout << staticGraph->toString();
    TransitiveClosure *transClosure = staticGraph->transitiveClosure();
    cout << "\nTransitive closure: " << endl;
    cout << transClosure->toString();
    auto map = transClosure->getLabelVertMap(); // map to match string labels with vertexes
    set<StaticVertex*>*termSet = new set<StaticVertex*>(); // set of terminals
    termSet->insert(termSet->begin(), (*map)["1"]);
    termSet->insert(termSet->begin(), (*map)["2"]);
    termSet->insert(termSet->begin(), (*map)["3"]);
    termSet->insert(termSet->begin(), (*map)["4"]);
    termSet->insert(termSet->begin(), (*map)["5"]);
    cout << "Weight minimum spanning tree:" << endl;
    cout << staticGraph->alg4(transClosure, 2, termSet->size(), staticGraph->getRoot(), termSet)->toString();
//    cout << staticGraph->alg3(transClosure, 2, termSet->size(), staticGraph->getRoot(), termSet)->toString();
}

typedef property<vertex_distance_t, float,
        property<vertex_name_t, std::string> > VertexProperty;
typedef property<edge_weight_t, float> EdgeProperty;

typedef adjacency_list<mapS, vecS, undirectedS,
        VertexProperty, EdgeProperty> Graph;

void boost_test() {
    enum family {
        Jeanie, Debbie, Rick, John, Amanda, Margaret, Benjamin, N
    };

    string name[] = {"Jeanie", "Debbie", "Rick", "John", "Amanda",
                     "Margaret", "Benjamin"
    };

    adjacency_list<> g(N);
    add_edge(Jeanie, Debbie, g);
    add_edge(Jeanie, Rick, g);
    add_edge(Jeanie, John, g);
    add_edge(Debbie, Amanda, g);
    add_edge(Rick, Margaret, g);
    add_edge(John, Benjamin, g);

    graph_traits<adjacency_list<> >::vertex_iterator i, end;
    graph_traits<adjacency_list<> >::adjacency_iterator ai, a_end;
    property_map<adjacency_list<>, vertex_index_t>::type index_map = get(vertex_index, g);

    for (tie(i, end) = vertices(g); i != end; ++i) {
        std::cout << name[get(index_map, *i)];
        tie(ai, a_end) = adjacent_vertices(*i, g);
        if (ai == a_end)
            std::cout << " has no children";
        else
            std::cout << " is the parent of ";
        for (; ai != a_end; ++ai) {
            std::cout << name[get(index_map, *ai)];
            if (boost::next(ai) != a_end)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }
}

void set_test(){
    class pair{
    public:
        StaticVertex* v1;
        StaticVertex* v2;
    };
    pair* p1, *p2;
    p1 = new pair;
    p2 = new pair;
    StaticVertex* v1 = new StaticVertex("1");
    StaticVertex* v2 = new StaticVertex("2");
    std::set<pair*> *s = new std::set<pair*>();

    p1->v1 = v1;
    p1->v2 = v2;

    p2->v1 = v1;
    p2->v2 = v2;

    s->insert(s->begin(), p1);
    cout << s->size();
}
//deprecated. One time will delete it:)
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

void mst_a2(map<string, vector<TempEdge *> *> *sal, vector<TempVertex *> vert_list, TempVertex *root, long low_bound,
            long up_bound) {
    struct mytuple {
        TempVertex *v1;
        TempVertex *v2;
        long time;
    };

    map<TempVertex *, int> pos;

    for (int i = 0; i < vert_list.size(); i++) {
        TempVertex *u = vert_list.operator[](i);
        pos[u] = 0;
        u->setA(LONG_MAX);
    }
    stack<mytuple> *st = new stack<mytuple>();
    mytuple temp;
    temp.v1 = root;
    temp.v2 = root;
    temp.time = low_bound;
    st->push(temp);
    while (!st->empty()) {
        mytuple tup = st->top();
        st->pop();
        TempVertex *u = tup.v1;
        TempVertex *v = tup.v2;
        long tav = tup.time; // t arrival v
        if (tav < v->getA()) {
            v->setA(tav);
            v->setP(u);
            vector<TempEdge *> *edge_list = (*sal)[v->getName()];
            if (pos[v] < edge_list->size()) {
                TempEdge *e = (*edge_list)[pos[v]];
                v = e->getSource();
                TempVertex *vp = e->getDestination();
                long start = e->getStartTime();
                long arr = e->getArrTime();
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