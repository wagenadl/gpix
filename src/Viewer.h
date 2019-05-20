// Viewer.h

#ifndef VIEWER_H

#define VIEWER_H

#include "ImageVault.h"
#include <QFrame>

class Viewer: public QFrame {
public:
  Viewer(QWidget *parent=0);
  virtual ~Viewer() {}
  void setFilename(QString);
  void setSource(ImageVault const *vault);
  void keyPressEvent(QKeyEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void wheelEvent(QWheelEvent *) override;
  void resizeEvent(QResizeEvent *) override;
  void paintEvent(QPaintEvent *) override;
  void setTitle();
  void enableNavigator(bool x=true);
  void newPosition();
  void zoomIn();
  void zoomOut();
private:
  ImageVault const *vault;
  int poweroftwo;
  QPoint topleft;
  QPoint presspos;
  QPoint presstopleft;
  QPoint lastpos;
  QString fn;
  class Navigator *nav;
  bool naven;
  int pixaccum;
};

#endif
