//
// Created by ilya on 28.12.2021.
//

#include "morph_window.hpp"

#include <cmath>

MorphWidget::MorphWidget(float nFrame, bool is_animated, QWidget *parent):
    QOpenGLWidget(parent), is_animated_(is_animated), nFrame_(nFrame) {
    rotationAxis_ = QVector3D(1, 1, 1);
    color_ = QColor::fromRgb(0, 0, 0);
}

MorphWidget::~MorphWidget() {
    makeCurrent();
    doneCurrent();
}

void MorphWidget::timerEvent(QTimerEvent *) {
    nFrame_++;
    update();
}

void MorphWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1, 1, 1, 1);
    geometry_ = std::make_shared<GeometryCube>();
    if (is_animated_)
    {timer_.start(20, this);}

    if (!program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
    {close();}

    if (!program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
    {close();}

    if (!program_.link())
    {close();}

    if (!program_.bind())
    {close();}

    glEnable(GL_DEPTH_TEST);
}

void MorphWidget::resizeGL(const int w, const int h) {
    float aspect = static_cast<float>(w) / static_cast<float>(h ? h : 1);
    const float zNear = 3.0, zFar = 7.0, fov = 45.0;
    projection_.setToIdentity();
    projection_.perspective(fov, aspect, zNear, zFar);
}

void MorphWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    color_.setRedF(1.0 - (sin(nFrame_ / 30.0) + 1.) / 2.);

    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);

    matrix.rotate(5 * nFrame_, rotationAxis_.x(), rotationAxis_.y(), rotationAxis_.z());

    program_.setUniformValue("mvp", projection_ * matrix);
    program_.setUniformValue("time", static_cast<float>(nFrame_ / 30));
    program_.setUniformValue("color", color_);

    geometry_->drawGeometry(&program_);
}

void MorphWidget::setRotationXAxis(const QString &new_x) {
    bool is_float;
    new_x.toFloat(&is_float);

    if (is_float) {
        rotationAxis_ = QVector3D(new_x.toFloat(), rotationAxis_.y(), rotationAxis_.z());
        update();
        emit rotationAxisXChanged(new_x);
    }
}

void MorphWidget::setRotationYAxis(const QString &new_y) {
    bool is_float;
    new_y.toFloat(&is_float);

    if (is_float) {
        rotationAxis_ = QVector3D(rotationAxis_.x(), new_y.toFloat(), rotationAxis_.z());
        update();
        emit rotationAxisYChanged(new_y);
    }
}

void MorphWidget::setRotationZAxis(const QString &new_z) {
    bool is_float;
    new_z.toFloat(&is_float);

    if (is_float)
    {
        rotationAxis_ = QVector3D(rotationAxis_.x(), rotationAxis_.y(), new_z.toFloat());
        update();
        emit rotationAxisZChanged(new_z);
    }
}

void MorphWidget::setColor(const QColor &c) {
    color_ = c;
    update();
}

void MorphWidget::setFrame(const int frame) {
    nFrame_ = static_cast<float>(frame);
    update();
    emit frameChanged(frame);
}
