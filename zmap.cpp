#include "zmap.h"

#include <cmath>

ZMap::ZMap()
{
    init();
}

void ZMap::init()
{
    heights.clear();
    heights.resize(1000, std::vector<float>(1000, height));
}

unsigned int ZMap::tranglesCount(void)
{
    return 6 * (((unsigned int)heights.size() + 1) * ((unsigned int)(heights.at(0).size()) + 1) - 3);
}

QVector3D ZMap::unify(QVector3D& value)
{
    float row = (float)heights.size(), col = (float)(heights.at(0).size());
    float scale = (row + col) / 2.0f;
    return (value - QVector3D(col / 2.0f, height / 2.0f, row / 2.0f)) / scale;
}

void ZMap::calTopTrangles(void)
{
    unsigned int row = (unsigned int)heights.size(), col = (unsigned int)(heights.at(0).size());
    unsigned int verticesCount = 2 * (row - 1) * (col - 1);
    QVector3D a, b, c, d, n1, n2;

    topVertices.clear();
    topNormals.clear();
    topVertices.reserve(verticesCount);
    topNormals.reserve(verticesCount);

    for (unsigned int i(1); i < row; i++) {
        for (unsigned int j(1); j < col; j++) {
            a = QVector3D(j - 1, heights.at(i - 1).at(j - 1), (i - 1));
            b = QVector3D(j - 1, heights.at(i).at(j - 1), (i));
            c = QVector3D(j, heights.at(i - 1).at(j), (i - 1));
            d = QVector3D(j, heights.at(i).at(j), (i));
            topVertices.append({ unify(a), unify(b), unify(c), unify(c), unify(b), unify(d) });

            n1 = QVector3D::normal(b - a, c - a);
            n2 = QVector3D::normal(c - d, b - d);
            topNormals.append({ n1, n1, n1, n2, n2, n2 });
        }
    }
}

void ZMap::calBottomTrangles(void)
{
    unsigned int row = (unsigned int)heights.size(), col = (unsigned int)(heights.at(0).size());
    QVector3D a = QVector3D(0, 0, 0);
    QVector3D b = QVector3D(0, 0, row - 1);
    QVector3D c = QVector3D(col - 1, 0, 0);
    QVector3D d = QVector3D(col - 1, 0, row - 1);
    QVector3D n = QVector3D(0, -1.0f, 0);

    bottomVertices.clear();
    bottomNormals.clear();

    bottomVertices.append({ unify(a), unify(b), unify(c), unify(c), unify(b), unify(d) });
    bottomNormals.append({ n, n, n, n, n, n });
}

void ZMap::calBackTrangles(void)
{
    unsigned int col = (unsigned int)(heights.at(0).size());
    unsigned int verticesCount = 2 * (col - 1);
    QVector3D a, b, c, d, n = QVector3D(0, 0, -1.0f);

    backVertices.clear();
    backNormals.clear();
    backVertices.reserve(verticesCount);
    backNormals.reserve(verticesCount);

    for (unsigned int j(1); j < col; j++) {
        a = QVector3D(j - 1, 0, 0);
        b = QVector3D(j - 1, heights.front().at(j - 1), 0);
        c = QVector3D(j, 0, 0);
        d = QVector3D(j, heights.front().at(j), 0);

        backVertices.append({ unify(a), unify(b), unify(c), unify(c), unify(b), unify(d) });
        backNormals.append({ n, n, n, n, n, n });
    }
}

void ZMap::calFrontTrangles(void)
{
    unsigned int row = (unsigned int)heights.size(), col = (unsigned int)(heights.at(0).size());
    unsigned int verticesCount = 2 * (col - 1);
    QVector3D a, b, c, d, n = QVector3D(0, 0, 1.0f);

    frontVertices.clear();
    frontNormals.clear();
    frontVertices.reserve(verticesCount);
    frontNormals.reserve(verticesCount);

    for (unsigned int j(1); j < col; j++) {
        a = QVector3D(j - 1, 0, row - 1);
        b = QVector3D(j - 1, heights.back().at(j - 1), row - 1);
        c = QVector3D(j, 0, row - 1);
        d = QVector3D(j, heights.back().at(j), row - 1);

        frontVertices.append({ unify(a), unify(b), unify(c), unify(c), unify(b), unify(d) });
        frontNormals.append({ n, n, n, n, n, n });
    }
}

void ZMap::calLeftTrangles(void)
{
    unsigned int row = (unsigned int)heights.size();
    unsigned int verticesCount = 2 * (row - 1);
    QVector3D a, b, c, d, n = QVector3D(-1.0f, 0, 0);

    leftVertices.clear();
    leftNormals.clear();
    leftVertices.reserve(verticesCount);
    leftNormals.reserve(verticesCount);

    for (unsigned int i(1); i < row; i++) {
        a = QVector3D(0, heights.at(i - 1).front(), i - 1);
        b = QVector3D(0, 0, i - 1);
        c = QVector3D(0, heights.at(i).front(), i);
        d = QVector3D(0, 0, i);

        leftVertices.append({ unify(a), unify(b), unify(c), unify(c), unify(b), unify(d) });
        leftNormals.append({ n, n, n, n, n, n });
    }
}

void ZMap::calRightTrangles(void)
{
    unsigned int row = (unsigned int)heights.size(), col = (unsigned int)(heights.at(0).size());
    unsigned int verticesCount = 2 * (row - 1);
    QVector3D a, b, c, d, n = QVector3D(1.0f, 0, 0);

    rightVertices.clear();
    rightNormals.clear();
    rightVertices.reserve(verticesCount);
    rightNormals.reserve(verticesCount);

    for (unsigned int i(1); i < row; i++) {
        a = QVector3D(col - 1, heights.at(i - 1).back(), i - 1);
        b = QVector3D(col - 1, 0, i - 1);
        c = QVector3D(col - 1, heights.at(i).back(), i);
        d = QVector3D(col - 1, 0, i);

        rightVertices.append({ unify(a), unify(b), unify(c), unify(c), unify(b), unify(d) });
        rightNormals.append({ n, n, n, n, n, n });
    }
}

void ZMap::calAllTrangles(void)
{
    allVertices.clear();
    allNormals.clear();
    allVertices.reserve(tranglesCount());
    allNormals.reserve(tranglesCount());

    calTopTrangles();
    appendAndClear(topVertices, topNormals);
    calBottomTrangles();
    appendAndClear(bottomVertices, bottomNormals);
    calBackTrangles();
    appendAndClear(backVertices, backNormals);
    calFrontTrangles();
    appendAndClear(frontVertices, frontNormals);
    calLeftTrangles();
    appendAndClear(leftVertices, leftNormals);
    calRightTrangles();
    appendAndClear(rightVertices, rightNormals);
}

void ZMap::appendAndClear(QList<QVector3D>& vertices, QList<QVector3D>& normals)
{
    allVertices.append(vertices);
    vertices.clear();
    allNormals.append(normals);
    normals.clear();
}
