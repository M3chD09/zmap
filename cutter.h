#ifndef CUTTER_H
#define CUTTER_H

#include "zmap.h"

#include <queue>

#include <QVector2D>
#include <QVector3D>

class Cutter {
public:
    Cutter();
    Cutter(float radiusOut, float radiusIn, const QVector3D& location)
        : radiusOut(radiusOut)
        , radiusIn(radiusIn)
        , location(location) {};

    float radiusOut;
    float radiusIn;

    QVector3D location;
    std::queue<QVector3D> locations;

    void moveLine(const QVector3D& destLocation, ZMap& zmap);
    bool start(ZMap& zmap);

private:
    const float stepDistance = 5.0f;

    bool checkInCircle(unsigned int pointX, unsigned int pointY, const QVector2D& center, ZMap& zmap);
    bool checkInRectangle(unsigned int pointX, unsigned int pointY, const QVector2D& start, const QVector2D& end, ZMap& zmap);
};

#endif // CUTTER_H
