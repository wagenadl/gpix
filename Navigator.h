// Navigator.h

#ifndef NAVIGATOR_H

#define NAVIGATOR_H

#include <QFrame>
#include "ImageVault.h"

class Navigator: public QFrame {
public:
  Navigator(QWidget *parent=0);
  virtual ~Navigator();
  void setSource(ImageVault *iv);
  void setMainScale(int poweroftwo);
  void setOurScale(int poweroftwo);
  void setMainRect(QRect);
  void paintEvent(QPaintEvent *) override;
public slots:
  void autosize();
private:
  int mainpwr;
  int ourpwr;
  QRect mainrect;
  ImageVault *vault;
};

#endif
