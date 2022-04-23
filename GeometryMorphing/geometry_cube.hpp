//
// Created by ilya on 28.12.2021.
//

#ifndef HA3_GEOMETRY_CUBE_HPP
#define HA3_GEOMETRY_CUBE_HPP

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class GeometryCube : protected QOpenGLFunctions {

    const uint8_t NUMBER_OF_FACES_ = 6;
    const uint number_of_vertices_ = 2048;
    QOpenGLBuffer vertices_;
    QOpenGLBuffer indices_;

public:
    GeometryCube();
    ~GeometryCube();
    void drawGeometry(QOpenGLShaderProgram *program);
};

#endif //HA3_GEOMETRY_CUBE_HPP
