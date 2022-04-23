//
// Created by ilya on 28.12.2021.
//

#include "main_widget.hpp"

#include <memory>

namespace
{
    std::shared_ptr<QSlider> createSlider() {
        std::shared_ptr<QSlider> slider(new QSlider(Qt::Horizontal));
        slider->setRange(0, 180);
        slider->setSingleStep(1);
        slider->setPageStep(10);
        slider->setTickInterval(10);
        slider->setTickPosition(QSlider::TicksRight);
        return slider;
    }

    std::shared_ptr<QLineEdit> createEdit(const QString& text) {
        std::shared_ptr<QLineEdit> edit(new QLineEdit());
        edit->setFocus();
        edit->setPlaceholderText(text);
        edit->setEchoMode(QLineEdit::Normal);
        return edit;
    }
}

MainWidget::MainWidget(float nFrame, bool is_animated) {
    setMinimumSize(1000, 1000);
    glWidget_ = std::make_shared<MorphWidget>(nFrame, is_animated);
    mainLayout_ = std::make_shared<QVBoxLayout>();
    mainLayout_->addWidget(glWidget_.get());

    frameSlider_ = createSlider();
    connect(frameSlider_.get(), SIGNAL(valueChanged(int)), glWidget_.get(), SLOT(setFrame(int)));
    connect(glWidget_.get(), SIGNAL(frameChanged(int)), frameSlider_.get(), SLOT(setValue(int)));
    mainLayout_->addWidget(frameSlider_.get());

    rotationAxisXEdit_ = createEdit("x:");
    rotationAxisYEdit_ = createEdit("y:");
    rotationAxisZEdit_ = createEdit("z:");
    connect(rotationAxisXEdit_.get(), SIGNAL(textChanged(QString)), glWidget_.get(), SLOT(setRotationXAxis(QString)));
    connect(rotationAxisYEdit_.get(), SIGNAL(textChanged(QString)), glWidget_.get(), SLOT(setRotationYAxis(QString)));
    connect(rotationAxisZEdit_.get(), SIGNAL(textChanged(QString)), glWidget_.get(), SLOT(setRotationZAxis(QString)));

    connect(glWidget_.get(), SIGNAL(rotationAxisXChanged(QString)), rotationAxisXEdit_.get(), SLOT(setText(QString)));
    connect(glWidget_.get(), SIGNAL(rotationAxisYChanged(QString)), rotationAxisYEdit_.get(), SLOT(setText(QString)));
    connect(glWidget_.get(), SIGNAL(rotationAxisZChanged(QString)), rotationAxisZEdit_.get(), SLOT(setText(QString)));

    mainLayout_->addWidget(rotationAxisXEdit_.get());
    mainLayout_->addWidget(rotationAxisYEdit_.get());
    mainLayout_->addWidget(rotationAxisZEdit_.get());

    colorDialog_ = std::make_shared<QColorDialog>();
    colorButton_ = std::make_shared<QPushButton>(tr("Change color"));
    connect(colorButton_.get(), SIGNAL(clicked()), colorDialog_.get(), SLOT(open()));
    connect(colorDialog_.get(), SIGNAL(colorSelected(QColor)), glWidget_.get(), SLOT(setColor(QColor)));
    mainLayout_->addWidget(colorButton_.get());
    setLayout(mainLayout_.get());
}
