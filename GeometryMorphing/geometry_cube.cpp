//
// Created by ilya on 28.12.2021.
//

#include <QVector3D>
#include <cmath>

#include "geometry_cube.hpp"

GeometryCube::GeometryCube(): indices_(QOpenGLBuffer::IndexBuffer) {
    initializeOpenGLFunctions();
    const auto number_of_vertices_on_face = static_cast<uint>(sqrt(number_of_vertices_));

    /* draw cube
     *
     *  ----------
     * /        / |
     * ---------- |
     * |        | |
     * |        | /
     * ----------
     *
     * */

    const auto delta = 1. / (number_of_vertices_on_face - 1);
    std::vector<std::pair<float, float>> coordinates_for_vertices(number_of_vertices_);
    for (uint i = 0; i < number_of_vertices_on_face; i++) {
        for (uint j = 0; j < number_of_vertices_on_face; j++) {
            coordinates_for_vertices[j * number_of_vertices_on_face + i] = {-0.5 + i * delta,
                                                                            -0.5 + j * delta};
        }
    }

    std::vector<QVector3D> vertices(number_of_vertices_ * NUMBER_OF_FACES_);
    for (uint v = 0; v < number_of_vertices_; v++) {
        for (uint polyhedron = 0; polyhedron < NUMBER_OF_FACES_; polyhedron++) {
            auto sign = polyhedron < 3? 1.f : -1.f;

            if (polyhedron % 3 == 0) {
                const auto y = coordinates_for_vertices[v].first;
                const auto z = coordinates_for_vertices[v].second;
                vertices[polyhedron * number_of_vertices_ + v] = QVector3D(sign * 0.5f, y, z);
            }

            if (polyhedron % 3 == 1) {
                const auto x = coordinates_for_vertices[v].first;
                auto z = coordinates_for_vertices[v].second;
                vertices[polyhedron * number_of_vertices_ + v] = QVector3D(x, sign * 0.5f, z);
            }

            if (polyhedron % 3 == 2) {
                const auto x = coordinates_for_vertices[v].first;
                const auto y = coordinates_for_vertices[v].second;
                vertices[polyhedron * number_of_vertices_ + v] = QVector3D(x, y, sign * 0.5f);
            }
        }
    }

    vertices_.create();
    vertices_.bind();
    vertices_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(QVector3D)));

    auto indices_in_right_order = std::vector<GLushort>();
    const auto offsets = {1u, number_of_vertices_on_face, number_of_vertices_on_face, 1u,
                                            number_of_vertices_on_face + 1};
    for (uint i = 0; i < number_of_vertices_on_face - 1; i++) {
        for (uint j = 0; j < number_of_vertices_on_face - 1; j++) {
            const auto start = j * number_of_vertices_on_face + i;
            indices_in_right_order.push_back(start);
            for (auto offset : offsets) {
                indices_in_right_order.push_back(start + offset);
            }
        }
    }

    std::vector<uint> indices(indices_in_right_order.size() * NUMBER_OF_FACES_);

    for (uint i = 0; i < indices_in_right_order.size(); i++) {
        for (uint j = 0; j < NUMBER_OF_FACES_; j++) {
            indices[j * indices_in_right_order.size() + i] = indices_in_right_order[i] + j * number_of_vertices_;
        }
    }

    indices_.create();
    indices_.bind();
    indices_.allocate(indices.data(), static_cast<int>(indices.size() * sizeof(uint)));
}

GeometryCube::~GeometryCube() {
    vertices_.destroy();
    indices_.destroy();
}

void GeometryCube::drawGeometry(QOpenGLShaderProgram *program) {
    vertices_.bind();
    indices_.bind();

    const int position = program->attributeLocation("position");
    program->enableAttributeArray(position);
    program->setAttributeBuffer(position, GL_FLOAT, 0, 3, sizeof(QVector3D));

    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, nullptr);
}
