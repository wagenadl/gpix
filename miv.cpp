// miv.cpp

#include "ImageVault.h"
#include <QApplication>
#include "Viewer.h"
#include <iostream>

int usage() {
  std::cerr << "Usage: miv IMAGEFILE\n";
  return 1;
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  ImageVault iv;
  if (argc==2)
    iv.load(argv[1]);
  else
    return usage();

  Viewer vwr;
  vwr.resize(vwr.width(), vwr.width() * iv.size().height() / iv.size().width());
  vwr.setSource(&iv);
  vwr.show();
  return app.exec();
}
