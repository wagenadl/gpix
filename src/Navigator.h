// Navigator.h

#ifndef NAVIGATOR_H

#define NAVIGATOR_H

#include <QFrame>
#include "ImageVault.h"

class Navigator: public QFrame {
public:
  Navigator(QWidget *parent=0);
  virtual ~Navigator();
  void setSource(ImageVault const *iv);
  void setMainPower(int poweroftwo);
  void setOurPower(int poweroftwo);
  void setMainRect(QRect);
  void paintEvent(QPaintEvent *) override;
public slots:
  void autoSize();
  void autoPower(QSize s); // S is size of parent window
private:
  int mainpwr;
  int ourpwr;
  QRect mainrect;
  ImageVault const *vault;
};

#endif
