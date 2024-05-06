#include "main_window.hpp"

#include <QApplication>


int main(int argc, char *argv[]) {

    QApplication orbsim(argc, argv);
    MainWindow main_window;

    main_window.show();
    return orbsim.exec();
}
