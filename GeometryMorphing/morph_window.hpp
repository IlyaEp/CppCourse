//
// Created by ilya on 28.12.2021.
//

#ifndef HA3_MORPH_WINDOW_HPP
#define HA3_MORPH_WINDOW_HPP

#include <QOpenGLWidget>
#include <QColor>
#include <QBasicTimer>

#include <memory>

#include "geometry_cube.hpp"


class MorphWidget : public QOpenGLWidget, protected QOpenGLFunctions {
Q_OBJECT

    QOpenGLShaderProgram program_;
    std::shared_ptr<GeometryCube> geometry_;
    QVector3D rotationAxis_;
    QMatrix4x4 projection_;
    QBasicTimer timer_;
    bool is_animated_;
    QColor color_;
    float nFrame_;

public:
    explicit MorphWidget(float nFrame, bool is_animated, QWidget *parent = nullptr);
    ~MorphWidget() override;

public slots:
    void setRotationXAxis(const QString &new_x);
    void setRotationYAxis(const QString &new_y);
    void setRotationZAxis(const QString &new_z);
    void setColor(const QColor &);
    void setFrame(int frame);

signals:
    void rotationAxisXChanged(const QString &);
    void rotationAxisYChanged(const QString &);
    void rotationAxisZChanged(const QString &);
    void frameChanged(int frame);

protected:
    void timerEvent(QTimerEvent *e) override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif //HA3_MORPH_WINDOW_HPP
