#include <iostream>
#include <qapplication.h>
#include "DisplayWidget.h"

using namespace std;

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    DisplayWidget DisplayWidget(768, 512);
    DisplayWidget.show();
    return app.exec();
}