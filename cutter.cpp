#include "cutter.h"

bool isInCircle(const QVector2D& point, const QVector2D& center, float radius)
{
    return std::pow(point.x() - center.x(), 2) + std::pow(point.y() - center.y(), 2) <= std::pow(radius, 2);
}

bool isInRectangle(const QVector2D& point, const QVector2D& start, const QVector2D& end, float radius)
{
    float dx = start.x() - end.x();
    float dy = start.y() - end.y();
    float k = dy / dx;
    float bi = point.y() + point.x() / k;
    float bs = start.y() + start.x() / k;
    float be = end.y() + end.x() / k;

    return std::min(bs, be) <= bi
        && bi <= std::max(bs, be)
        && std::abs(dy * (point.x() - start.x()) - dx * (point.y() - start.y())) / std::sqrt(std::pow(dx, 2) + std::pow(dy, 2)) <= radius;
}

Cutter::Cutter()
{
}

void Cutter::moveLine(const QVector3D& destLocation, ZMap& zmap)
{
    for (unsigned int i(0); i < zmap.heights.size(); i++) {
        for (unsigned int j(0); j < zmap.heights.at(0).size(); j++) {
            if (isInCircle(QVector2D(i, j), location.toVector2D(), radius)
                || isInCircle(QVector2D(i, j), destLocation.toVector2D(), radius)
                || isInRectangle(QVector2D(i, j), location.toVector2D(), destLocation.toVector2D(), radius)) {
                zmap.heights.at(i).at(j) = std::min(zmap.heights.at(i).at(j), location.z());
            }
        }
    }
    location = destLocation;
}
