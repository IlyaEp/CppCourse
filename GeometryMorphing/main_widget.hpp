//
// Created by ilya on 28.12.2021.
//

#ifndef HA3_MAIN_WIDGET_HPP
#define HA3_MAIN_WIDGET_HPP

#include <QSlider>
#include <QLineEdit>
#include <QColorDialog>
#include <QPushButton>
#include <QVBoxLayout>

#include <memory>

#include "morph_window.hpp"

class MainWidget : public QWidget {
Q_OBJECT

    std::shared_ptr<MorphWidget> glWidget_;
    std::shared_ptr<QSlider> frameSlider_;
    std::shared_ptr<QLineEdit> rotationAxisXEdit_;
    std::shared_ptr<QLineEdit> rotationAxisYEdit_;
    std::shared_ptr<QLineEdit> rotationAxisZEdit_;
    std::shared_ptr<QColorDialog> colorDialog_;
    std::shared_ptr<QPushButton> colorButton_;
    std::shared_ptr<QVBoxLayout> mainLayout_;

public:
    explicit MainWidget(float nFrame, bool is_animated);
};

#endif //HA3_MAIN_WIDGET_HPP
