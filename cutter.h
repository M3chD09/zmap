#ifndef CUTTER_H
#define CUTTER_H

#include "zmap.h"
#include <QVector2D>
#include <QVector3D>

class Cutter {
public:
    Cutter();
    Cutter(float radius, const QVector3D& location)
        : radius(radius)
        , location(location) {};

    float radius;
    QVector3D location;

    void moveLine(const QVector3D& destLocation, ZMap& zmap);
};

#endif // CUTTER_H
