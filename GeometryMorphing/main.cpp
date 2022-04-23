#include <iostream>
#include <QApplication>
#include <QCommandLineParser>

#include "main_widget.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Geometry morphing");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption t_parameter("t",
                                   QCoreApplication::translate("main", "Morph parameter initial value"),
                                   QCoreApplication::translate("main", "Float value in range 0-1"));
    parser.addOption(t_parameter);
    QCommandLineOption a_parameter("a",QCoreApplication::translate("main", "Flag to disable animation"));
    parser.addOption(a_parameter);
    parser.process(app);

    auto t = 0.0f;
    if (parser.isSet(t_parameter)) {
        bool check = false;
        auto new_t = parser.values(t_parameter).first().toFloat(&check);
        if (not check) {
            std::cout << "Value for -t should be float!" << std::endl;
            return 0;
        }

        t = std::clamp(new_t, 0.0f, 1.0f);
    }

    auto is_animated = true;
    if (parser.isSet(a_parameter))
    {is_animated = false;}

    MainWidget widget(t, is_animated);
    widget.show();
    return QApplication::exec();
}
