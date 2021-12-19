#include "camera.h"

Camera::Camera()
{
}

void Camera::keyPressEvent(QKeyEvent* event)
{
    float Speed = 0.1f;
    if (event->key() == Qt::Key_W) {
        pos += Speed * front;
    }
    if (event->key() == Qt::Key_A) {
        pos -= Speed * QVector3D::crossProduct(front, up).normalized();
    }
    if (event->key() == Qt::Key_S) {
        pos -= Speed * front;
    }
    if (event->key() == Qt::Key_D) {
        pos += Speed * QVector3D::crossProduct(front, up).normalized();
    }
}

void Camera::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        if (isFirstMousePress) {
            mousePressPosition = QVector2D(event->position());
            isFirstMousePress = false;
        }

        qreal angularSpeed = 2;
        QVector2D diff = QVector2D(event->position()) - mousePressPosition;
        mousePressPosition = QVector2D(event->position());
        QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
        rotationAxis = (rotationAxis * angularSpeed + n).normalized();
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
    }
}

void Camera::wheelEvent(QWheelEvent* event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        fov -= (float)numSteps.y();
    }

    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;

    event->accept();
}
