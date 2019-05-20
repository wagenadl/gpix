// miv.cpp

#include "ImageVault.h"
#include <QApplication>
#include "Viewer.h"
#include <QCommandLineParser>
#include <QFileDialog>
#include <QDebug>

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  app.setApplicationName("miv");
  QCommandLineParser p;
  p.addHelpOption();
  p.addPositionalArgument("file", "image file name");
  p.addOption(QCommandLineOption("b",
                                 "Percentage of black to clip in 16-bit images",
                                 "black%"));
  p.addOption(QCommandLineOption("w",
                                 "Percentage of white to clip in 16-bit images",
                                 "white%"));
  p.process(app);
  QStringList args(p.positionalArguments());
  QString fn;
  if (args.isEmpty())
    fn = QFileDialog::getOpenFileName(0, "Please select image file",
                                      "",
                                      "Common image types (*.jpg *.png *.tif)"
                                      ";;All files (*)");
  else
    fn = args[0];
  if (fn.isEmpty()) 
    return 2;

  QString blk = p.value("b");
  QString wht = p.value("w");
  double whtprc = -1;
  double blkprc = -1;
  if (!blk.isEmpty())
    blkprc = blk.toDouble();
  if (!wht.isEmpty())
    whtprc = wht.toDouble();
  if (whtprc<0)
    whtprc = blkprc;
  else if (blkprc<0)
    blkprc = whtprc;
  if (whtprc<0)
    whtprc = 0;
  if (blkprc<0)
    blkprc = 0;
  
  ImageVault iv;
  iv.setClipping(blkprc, whtprc);
  if (!iv.load(fn)) {
    qCritical() << "File not found:" << fn;
    return 2;
  }

  Viewer vwr;
  vwr.resize(vwr.width(),
             vwr.width() * iv.size().height() / iv.size().width());
  vwr.setSource(&iv);
  vwr.setFilename(fn);
  vwr.show();
  return app.exec();
}
