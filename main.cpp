#include "ui_main.h"
#include "OGLWindow.h"
#include <iostream>

int main(int argc, char **arg) {

   QApplication app(argc, arg);
   DesktopWindow dw;
   dw.resize(dw.sizeHint());
   dw.show();
   return app.exec();
}
