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
  if (argc!=2)
    return usage();
  if (!iv.load(argv[1])) {
    std::cerr << "File not found: " << argv[1] << "\n";
    return 2;
  }


  Viewer vwr;
  vwr.resize(vwr.width(), vwr.width() * iv.size().height() / iv.size().width());
  vwr.setSource(&iv);
  vwr.setFilename(argv[1]);
  vwr.show();
  return app.exec();
}
