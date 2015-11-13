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

    enron_test();
//    set_test();
    return 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1; i++) {
        temp_graph_test();
//            cout << "iteration # " << i << endl;
    }
//        set_test();
    auto finish = std::chrono::high_resolution_clock::now();
    cout << "Execution time : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    return 0;
}


void static_graph_test() {
    StaticGraph *gr = new StaticGraph();
    shared_ptr<StaticVertex> v1(new StaticVertex("1"));
    shared_ptr<StaticVertex> v2(new StaticVertex("2"));
    shared_ptr<StaticVertex> v3(new StaticVertex("3"));
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
    shared_ptr<TransitiveClosure> transClosure(static_graph->transitiveClosure());
    cout << "\nTransitive closure: " << endl;
    cout << transClosure->toString();
    auto map = transClosure->getLabelVertMap(); // map to match string labels with vertexes
    set<shared_ptr<StaticVertex>, classcomp> *termSet = new set<shared_ptr<StaticVertex>, classcomp>(); // set of terminals
    termSet->insert(termSet->begin(), (*map)["1"]);
    termSet->insert(termSet->begin(), (*map)["2"]);
    termSet->insert(termSet->begin(), (*map)["3"]);
    termSet->insert(termSet->begin(), (*map)["4"]);
    termSet->insert(termSet->begin(), (*map)["5"]);
    cout << "Weight minimum spanning tree:" << endl;
    Tree *wmst = static_graph->alg6(transClosure, 2, termSet->size(), static_graph->getRoot(), termSet);
//    Tree* wmst = static_graph->alg4(transClosure, 2, termSet->size(), static_graph->getRoot(), termSet);
//    Tree* wmst = static_graph->alg3(transClosure, 2, termSet->size(), static_graph->getRoot(), termSet);
//    cout << staticGraph->alg4(transClosure, 2, termSet->size(), staticGraph->getRoot(), termSet)->toString();
//    cout << staticGraph->alg3(transClosure, 2, termSet->size(), staticGraph->getRoot(), termSet)->toString();
    cout << wmst->toString();
    delete temp_graph;
    delete static_graph;
    transClosure.reset();
    delete wmst;
}

void set_test() {
    StaticVertex* st1 = new StaticVertex("1");
    StaticVertex* st2 = new StaticVertex("1");
    shared_ptr<StaticVertex> sh1(st1);
    shared_ptr<StaticVertex> sh2(st2);
    unordered_set<shared_ptr<StaticVertex>, KeyHasher>* s = new unordered_set<shared_ptr<StaticVertex>, KeyHasher>();
    s->insert(s->begin(), sh1);
    s->insert(s->begin(), sh2);
    cout << s->size();
//    delete s;
}

void enron_test() {

    cout << "Reading dataset started.." << endl;
    auto start = std::chrono::high_resolution_clock::now();
    string filename = "./dataset/enron/test.enron";
    shared_ptr<enron_parser> ep(new enron_parser(filename));
    shared_ptr<TempGraph> tg(ep->getTG());
    auto finish = std::chrono::high_resolution_clock::now();
    cout << "Reading dataset is done within(ms) : " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << endl << endl;
    cout << "Temporal Verts number: " << tg->getVertsNumber() << endl;
    cout << "Temporal Edges number: " << tg->getEdgeNumber() << endl << endl;


//    shared_ptr<TempGraph> tg_small(ep->get_small_graph(5));
//    cout << "Vert number: " << tg_small->getVertsNumber() << endl;
//    cout << "Edges number: " << tg_small->getEdgeNumber() << endl;
//    ep->save_graph("./dataset/enron/test.enron", tg_small);
//    tg_small.reset();
//
//    return;
//    cout << ep->get_terms(ep->getRoot(), 0)->size() << endl;

    cout << "Getting static graph process is started.." << endl;
    start = std::chrono::high_resolution_clock::now();
    shared_ptr<StaticGraph> sg(tg->getStaticGraph(ep->getRoot()));
    finish = std::chrono::high_resolution_clock::now();
    cout << "Getting static graph is done within(ms) : " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << endl << endl;

    cout << "Static Verts number: " << sg->getVertSet()->size() << endl;
    cout << "Static Edges number: " << sg->get_edge_number() << endl << endl;

    auto terms(sg->get_terms());
    terms->erase(sg->getRoot());

    cout << "Calculating transitive closure is started.." << endl;
    start = std::chrono::high_resolution_clock::now();
    shared_ptr<TransitiveClosure> tr_cl(sg->transitiveClosure());
    finish = std::chrono::high_resolution_clock::now();
    cout << "Calculating transitive closure is done within(ms) : " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << endl << endl;

//    cout << "Transitive Closure: " << endl;
//    cout << tr_cl->toString();

    int i = 2;
    cout << "Calculating wMST(alg6) with i = " << i << " is started.." << endl;
    start = std::chrono::high_resolution_clock::now();
//    shared_ptr<Tree> wmst(sg->alg3(tr_cl, 2, terms->size(), sg->getRoot(), terms));
//    shared_ptr<Tree> wmst(sg->alg4(tr_cl, 2, terms->size(), sg->getRoot(), terms));
    shared_ptr<Tree> wmst(sg->alg6(tr_cl, i, terms->size(), sg->getRoot(), terms));
    finish = std::chrono::high_resolution_clock::now();
    cout << "Calculating wMST is done within(ms) : " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << endl << endl;

//    cout << "wMST: " << endl;
//    cout << wmst->toString();

    wmst.reset();
    tr_cl.reset();
    sg.reset();
    tg.reset();
    ep.reset();
    delete(terms); // FIX it
}
