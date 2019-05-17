// Viewer.h

#ifndef VIEWER_H

#define VIEWER_H

#include "ImageVault.h"
#include <QFrame>

class Viewer: public QFrame {
public:
  Viewer(QWidget *parent=0);
  virtual ~Viewer() {}
  void setSource(ImageVault const *vault);
  void keyPressEvent(QKeyEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void resizeEvent(QResizeEvent *) override;
  void paintEvent(QPaintEvent *) override;
private:
  ImageVault const *vault;
  int poweroftwo;
  QPoint topleft;
  QPoint presspos;
  QPoint presstopleft;
  QPoint lastpos;
};

#endif
