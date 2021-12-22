#include "cutter.h"

Cutter::Cutter()
{
}

bool Cutter::checkInCircle(unsigned int pointX, unsigned int pointY, const QVector2D& center, ZMap& zmap)
{
    float distance = radiusOut - QVector2D(pointX, pointY).distanceToPoint(center);
    if (distance < 0) {
        return false;
    }
    if (distance > radiusIn) {
        zmap.heights.at(pointX).at(pointY)
            = std::min(zmap.heights.at(pointX).at(pointY), location.z());
        return true;
    }
    zmap.heights.at(pointX).at(pointY)
        = std::min(zmap.heights.at(pointX).at(pointY), radiusIn + location.z() - (float)std::sqrt(std::pow(radiusIn, 2) - std::pow(radiusIn - distance, 2)));
    return true;
}

bool Cutter::checkInRectangle(unsigned int pointX, unsigned int pointY, const QVector2D& start, const QVector2D& end, ZMap& zmap)
{
    float distance;

    if (std::abs(start.x() - end.x()) <= std::numeric_limits<float>::epsilon()) {
        if ((float)pointY < std::min(start.y(), end.y())
            || (float)pointY > std::max(start.y(), end.y())) {
            return false;
        }
        distance = radiusOut - std::abs(pointX - start.x());
    } else if (std::abs(start.y() - end.y()) <= std::numeric_limits<float>::epsilon()) {
        if ((float)pointX < std::min(start.x(), end.x())
            || (float)pointX > std::max(start.x(), end.x())) {
            return false;
        }
        distance = radiusOut - std::abs(pointY - start.y());
    } else {
        float dx = start.x() - end.x();
        float dy = start.y() - end.y();
        float k = dy / dx;
        float bi = (float)pointY + (float)pointX / k;
        float bs = start.y() + start.x() / k;
        float be = end.y() + end.x() / k;

        if (std::min(bs, be) > bi || bi > std::max(bs, be)) {
            return false;
        }

        distance = radiusOut - std::abs(dy * ((float)pointX - start.x()) - dx * ((float)pointY - start.y())) / std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    }

    if (distance < 0) {
        return false;
    }
    if (distance > radiusIn) {
        zmap.heights.at(pointX).at(pointY)
            = std::min(zmap.heights.at(pointX).at(pointY), location.z());
        return true;
    }
    zmap.heights.at(pointX).at(pointY)
        = std::min(zmap.heights.at(pointX).at(pointY), radiusIn + location.z() - (float)std::sqrt(std::pow(radiusIn, 2) - std::pow(radiusIn - distance, 2)));
    return true;
}

void Cutter::moveLine(const QVector3D& destLocation, ZMap& zmap)
{
    for (unsigned int i(0); i < zmap.heights.size(); i++) {
        for (unsigned int j(0); j < zmap.heights.at(0).size(); j++) {
            checkInRectangle(i, j, location.toVector2D(), destLocation.toVector2D(), zmap)
                || checkInCircle(i, j, location.toVector2D(), zmap)
                || checkInCircle(i, j, destLocation.toVector2D(), zmap);
        }
    }
    location = destLocation;
}

bool Cutter::start(ZMap& zmap)
{
    if (locations.empty()) {
        return false;
    }

    QVector3D destLocation = locations.front();
    if (location.distanceToPoint(destLocation) <= stepDistance) {
        locations.pop();
        moveLine(destLocation, zmap);
        return true;
    }

    destLocation = location + stepDistance * (destLocation - location).normalized();
    moveLine(destLocation, zmap);
    return true;
}
