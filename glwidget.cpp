#include "glwidget.h"

#include <cmath>

GLWidget::GLWidget()
{
    timerUpdate = new QTimer(this);
    timerCal = new QTimer(this);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    delete program;
    delete vshader;
    delete fshader;
    delete timerUpdate;
    delete timerCal;
    vbo.destroy();
    vao.destroy();
    doneCurrent();
}

void GLWidget::updateZMap()
{
    if (!cutter.start(zmap)) {
        timerUpdate->stop();
        timerCal->stop();
        return;
    }
}

void GLWidget::calZMap()
{
    zmap.calAllTrangles();
    update();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader.vert");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader.frag");
    program->link();

    locModel = program->uniformLocation("model");
    locView = program->uniformLocation("view");
    locProjection = program->uniformLocation("projection");

    program->bind();
    program->setUniformValue("objectColor", objectColor);
    program->setUniformValue("lightColor", lightColor);
    program->setUniformValue("lightPos", lightPos);

    cutter.location = QVector3D(1100, 600, 400);
    cutter.locations.push(QVector3D(100, 600, 400));
    cutter.locations.push(QVector3D(800, 100, 400));
    cutter.locations.push(QVector3D(500, 900, 400));
    cutter.locations.push(QVector3D(200, 100, 400));
    cutter.locations.push(QVector3D(900, 600, 400));

    zmap.calAllTrangles();
    unsigned int verticesSize = (unsigned int)zmap.allVertices.count() * 3 * sizeof(float);

    if (!vao.isCreated()) {
        vao.create();
    }
    vao.bind();
    if (!vbo.isCreated()) {
        vbo.create();
        vbo.bind();
        vbo.allocate(verticesSize * 2);
    }

    program->enableAttributeArray(0);
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    program->enableAttributeArray(1);
    program->setAttributeBuffer(1, GL_FLOAT, verticesSize, 3);
    vao.release();
}

void GLWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->bind();

    matModel.setToIdentity();
    matModel.rotate(camera.rotation);
    program->setUniformValue(locModel, matModel);

    matView.setToIdentity();
    matView.lookAt(camera.pos, camera.pos + camera.front, camera.up);
    program->setUniformValue(locView, matView);

    matProjection.setToIdentity();
    matProjection.perspective(camera.fov, qreal(width()) / qreal(height()), 0.1f, 100.f);
    program->setUniformValue(locProjection, matProjection);

    unsigned int verticesSize = (unsigned int)zmap.allVertices.count() * 3 * sizeof(float);
    vao.bind();
    vbo.write(0, zmap.allVertices.constData(), verticesSize);
    vbo.write(verticesSize, zmap.allNormals.constData(), verticesSize);
    glDrawArrays(GL_TRIANGLES, 0, zmap.tranglesCount());
    vao.release();

    program->disableAttributeArray(0);
}

void GLWidget::resizeGL(int w, int h)
{
    matProjection.setToIdentity();
    matProjection.perspective(camera.fov, qreal(w) / qreal(h), 0.1f, 100.f);
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{
    camera.keyPressEvent(event);

    if (event->key() == Qt::Key_Z) {
        connect(timerUpdate, &QTimer::timeout, this, QOverload<>::of(&GLWidget::updateZMap));
        timerUpdate->start(10);
        connect(timerCal, &QTimer::timeout, this, QOverload<>::of(&GLWidget::calZMap));
        timerCal->start(10);
    }
    if (event->key() == Qt::Key_P) {
        timerUpdate->stop();
        timerCal->stop();
    }
    if (event->key() == Qt::Key_R) {
        timerUpdate->stop();
        timerCal->stop();
        zmap.init();
        zmap.calAllTrangles();
    }

    update();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    camera.mouseMoveEvent(event);
    update();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    camera.wheelEvent(event);
    update();
}
