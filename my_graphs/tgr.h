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
class TempVertex {
private:
    std::string name;
    TempVertex *P;
    long A;
public:
    TempVertex(std::string name);

    std::string getName();

    void setName(std::string);

    TempVertex *getP();

    void setP(TempVertex *P);

    long getA();

    void setA(long A);
};


class TempEdge {
private:
    TempVertex *source;
    TempVertex *destination;
    long startTime;
    long arrTime;
public:
    TempEdge(TempVertex *source, TempVertex *destination, long startTime, long arrTime);

    ~TempEdge();

    TempVertex *getSource() const;

    void setSource(TempVertex *source);

    TempVertex *getDestination() const {
        return destination;
    }

    void setDestination(TempVertex *destination);

    long getStartTime() const;

    void setStartTime(long startTime);

    long getArrTime() const;

    void setArrTime(long arrTime);

    std::string toString();
};

#endif //PDS1_TGR_H
