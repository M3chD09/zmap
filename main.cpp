#include <QOpenGLWidget>
#include <QtWidgets>

#include "glwidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    GLWidget window;
    window.show();
    return app.exec();
}
