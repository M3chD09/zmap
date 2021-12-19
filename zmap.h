#ifndef ZMAP_H
#define ZMAP_H

#include <vector>

#include <QList>
#include <QVector3D>

class ZMap {
public:
    ZMap();

    std::vector<std::vector<float>> heights;

    QList<QVector3D> allVertices;
    QList<QVector3D> allNormals;
    QList<QVector3D> topVertices;
    QList<QVector3D> topNormals;
    QList<QVector3D> bottomVertices;
    QList<QVector3D> bottomNormals;
    QList<QVector3D> backVertices;
    QList<QVector3D> backNormals;
    QList<QVector3D> frontVertices;
    QList<QVector3D> frontNormals;
    QList<QVector3D> leftVertices;
    QList<QVector3D> leftNormals;
    QList<QVector3D> rightVertices;
    QList<QVector3D> rightNormals;

    void init(void);

    unsigned int tranglesCount(void);

    void calAllTrangles(void);
    void calTopTrangles(void);
    void calBottomTrangles(void);
    void calBackTrangles(void);
    void calFrontTrangles(void);
    void calLeftTrangles(void);
    void calRightTrangles(void);

private:
    QVector3D unify(QVector3D& value);
    void appendAndClear(QList<QVector3D>& vertices, QList<QVector3D>& normals);
};

#endif // ZMAP_H
