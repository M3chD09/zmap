#include "cutter.h"

Cutter::Cutter()
{
}

bool Cutter::checkInCircle(const QVector2D& point, const QVector2D& center, ZMap& zmap)
{
    float distance = radiusOut - point.distanceToPoint(center);
    if (distance < 0) {
        return false;
    }
    if (distance > radiusIn) {
        zmap.heights.at((unsigned int)point.x()).at((unsigned int)point.y())
            = std::min(zmap.heights.at((unsigned int)point.x()).at((unsigned int)point.y()), location.z());
        return true;
    }
    zmap.heights.at((unsigned int)point.x()).at((unsigned int)point.y())
        = std::min(zmap.heights.at((unsigned int)point.x()).at((unsigned int)point.y()), radiusIn + location.z() - (float)std::sqrt(std::pow(radiusIn, 2) - std::pow(radiusIn - distance, 2)));
    return true;
}

bool Cutter::checkInRectangle(const QVector2D& point, const QVector2D& start, const QVector2D& end, ZMap& zmap)
{
    float dx = start.x() - end.x();
    float dy = start.y() - end.y();
    float k = dy / dx;
    float bi = point.y() + point.x() / k;
    float bs = start.y() + start.x() / k;
    float be = end.y() + end.x() / k;

    if (std::min(bs, be) > bi || bi > std::max(bs, be))
        return false;

    float distance = radiusOut - std::abs(dy * (point.x() - start.x()) - dx * (point.y() - start.y())) / std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    if (distance < 0) {
        return false;
    }
    if (distance > radiusIn) {
        zmap.heights.at((unsigned int)point.x()).at((unsigned int)point.y())
            = std::min(zmap.heights.at((unsigned int)point.x()).at((unsigned int)point.y()), location.z());
        return true;
    }
    zmap.heights.at((unsigned int)point.x()).at((unsigned int)point.y())
        = std::min(zmap.heights.at((unsigned int)point.x()).at((unsigned int)point.y()), radiusIn + location.z() - (float)std::sqrt(std::pow(radiusIn, 2) - std::pow(radiusIn - distance, 2)));
    return true;
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
    float dx = start.x() - end.x();
    float dy = start.y() - end.y();
    float k = dy / dx;
    float bi = pointY + (float)pointX / k;
    float bs = start.y() + start.x() / k;
    float be = end.y() + end.x() / k;

    if (std::min(bs, be) > bi || bi > std::max(bs, be))
        return false;

    float distance = radiusOut - std::abs(dy * ((float)pointX - start.x()) - dx * ((float)pointY - start.y())) / std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
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
