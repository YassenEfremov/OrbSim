#include "notepad.hpp"

#include <QApplication>


int main(int argc, char **argv)
{
    QApplication just_app(argc, argv);
    Notepad notepad;

    notepad.show();
    return just_app.exec();
}
