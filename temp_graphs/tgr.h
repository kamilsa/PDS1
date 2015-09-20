//
// Created by Kamil on 19/09/15.
//

#ifndef PDS1_TGR_H
#define PDS1_TGR_H


#include <iosfwd>
#include <string>

/**
 * Class vertex -- represents vertex in temporal graph
 */
class Vertex {
private:
    std::string name;
    Vertex *P;
    long A;
public:
    Vertex(std::string name);

    std::string getName();

    void setName(std::string);

    Vertex *getP();

    void setP(Vertex *P);

    long getA();

    void setA(long A);
};


class Edge {
private:
    Vertex *source;
    Vertex *destination;
    long startTime;
    long arrTime;
public:
    Edge(Vertex *source, Vertex *destination, long startTime, long arrTime);

    ~Edge();

    Vertex *getSource() const;

    void setSource(Vertex *source);

    Vertex *getDestination() const {
        return destination;
    }

    void setDestination(Vertex *destination);

    long getStartTime() const;

    void setStartTime(long startTime);

    long getArrTime() const;

    void setArrTime(long arrTime);

    std::string toString();
};

#endif //PDS1_TGR_H
