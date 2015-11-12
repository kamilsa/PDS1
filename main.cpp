#include <iostream>
#include "my_graphs/tgr.h"
#include "my_graphs/sgr.h"
#include <stack>
#include <memory>

#include <chrono> // to measure time
#include "dataset/enron/enron_parser.h"

using namespace std;

void static_graph_test();

void temp_graph_test();

void boost_test();

void set_test();

void enron_test();

int main() {

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1; i++) {
            temp_graph_test();
//            cout << "iteration # " << i << endl;
        }
//        set_test();
        auto finish = std::chrono::high_resolution_clock::now();
        cout << "Execution time : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
     return 0;
}


void static_graph_test() {
    StaticGraph *gr = new StaticGraph();
    shared_ptr<StaticVertex>v1(new StaticVertex("1"));
    shared_ptr<StaticVertex>v2(new StaticVertex("2"));
    shared_ptr<StaticVertex>v3(new StaticVertex("3"));
    gr->add_edge(v1, v2, 1);
    gr->add_edge(v1, v3, 2);
    gr->add_edge(v3, v2, 3);

    cout << gr->toString();
    delete gr;
}

void temp_graph_test() {
    TempGraph *temp_graph = new TempGraph();

    shared_ptr<TempVertex> v0(new TempVertex("0"));
    shared_ptr<TempVertex> v1(new TempVertex("1"));
    shared_ptr<TempVertex> v2(new TempVertex("2"));
    shared_ptr<TempVertex> v3(new TempVertex("3"));
    shared_ptr<TempVertex> v4(new TempVertex("4"));
    shared_ptr<TempVertex> v5(new TempVertex("5"));

    temp_graph->addEdge(new TempEdge(v0, v1, 1, 3, 2));
    temp_graph->addEdge(new TempEdge(v0, v2, 1, 5, 4));
    temp_graph->addEdge(new TempEdge(v0, v2, 3, 6, 3));
    temp_graph->addEdge(new TempEdge(v0, v1, 4, 5, 1));
    temp_graph->addEdge(new TempEdge(v1, v3, 4, 6, 2));
    temp_graph->addEdge(new TempEdge(v1, v4, 5, 8, 3));
    temp_graph->addEdge(new TempEdge(v2, v5, 6, 8, 2));
    temp_graph->addEdge(new TempEdge(v2, v4, 7, 9, 2));
    temp_graph->addEdge(new TempEdge(v4, v0, 8, 9, 1));

//    temp_graph->mst_a2(v0, 0, LONG_MAX, true);
//    for (TempVertex *v : *temp_graph->getVertSet()) {
//        if (v != v0)
//            cout << "P(" << v->getName() << ") = " << v->getP()->getName() << " A(" << v->getName() << ") = " <<
//            v->getA() << endl;
//    }
//    cout << "It was from class" << endl;

    StaticGraph *static_graph = temp_graph->getStaticGraph(v0);
    cout << static_graph->toString();
    TransitiveClosure *transClosure = static_graph->transitiveClosure();
    cout << "\nTransitive closure: " << endl;
    cout << transClosure->toString();
    auto map = transClosure->getLabelVertMap(); // map to match string labels with vertexes
    set<shared_ptr<StaticVertex>>*termSet = new set<shared_ptr<StaticVertex>>(); // set of terminals
    termSet->insert(termSet->begin(), (*map)["1"]);
    termSet->insert(termSet->begin(), (*map)["2"]);
    termSet->insert(termSet->begin(), (*map)["3"]);
    termSet->insert(termSet->begin(), (*map)["4"]);
    termSet->insert(termSet->begin(), (*map)["5"]);
    cout << "Weight minimum spanning tree:" << endl;
    Tree* wmst = static_graph->alg6(transClosure, 2, termSet->size(), static_graph->getRoot(), termSet);
//    Tree* wmst = static_graph->alg4(transClosure, 2, termSet->size(), static_graph->getRoot(), termSet);
//    Tree* wmst = static_graph->alg3(transClosure, 2, termSet->size(), static_graph->getRoot(), termSet);
//    cout << staticGraph->alg4(transClosure, 2, termSet->size(), staticGraph->getRoot(), termSet)->toString();
//    cout << staticGraph->alg3(transClosure, 2, termSet->size(), staticGraph->getRoot(), termSet)->toString();
    cout << wmst->toString();
    delete temp_graph;
    delete static_graph;
    delete transClosure;
    delete wmst;
}

void set_test(){
    class pair{
    public:
        shared_ptr<StaticVertex> v1;
        shared_ptr<StaticVertex> v2;
    };
    pair* p1, *p2;
    p1 = new pair;
    p2 = new pair;
    shared_ptr<StaticVertex> v1(new StaticVertex("1"));
    shared_ptr<StaticVertex> v2(new StaticVertex("2"));
    std::set<pair*> *s = new std::set<pair*>();

    p1->v1 = v1;
    p1->v2 = v2;

    p2->v1 = v1;
    p2->v2 = v2;

    s->insert(s->begin(), p1);
    cout << s->size();
}

void enron_test(){
    string filename = "./dataset/enron/test.enron";
    shared_ptr<enron_parser> ep(new enron_parser(filename));
    TempGraph* tg = ep->getTG();
    cout << "Vert number: " << tg->getVertsNumber() << endl;
    cout << "Edges number: " << tg->getEdgeNumber() << endl;



    ep.reset();
}