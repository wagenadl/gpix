// Navigator.cpp

#include "Navigator.h"
#include <QPainter>

static constexpr int TOOMUCH = 10000;

Navigator::Navigator(QWidget *parent): QFrame(parent) {
  vault = 0;
  mainpwr = 0;
  ourpwr = TOOMUCH;
}

Navigator::~Navigator() {
}

void Navigator::setSource(ImageVault *iv) {
  vault = iv;
  update();
}

void Navigator::setMainScale(int poweroftwo) {
  mainpwr = poweroftwo;
  update();
}

void Navigator::setOurScale(int poweroftwo) {
  ourpwr = poweroftwo;
  update();
}

void Navigator::setMainRect(QRect r) {
  mainrect = r;
  update();
}

void Navigator::autopower(QSize mainsize) {
  if (!vault)
    return;
  int h = mainsize.height() / 7; // reasonable max?
  int m = 0;
  while (vault->size(m).height() > h)
    m++;
  ourpwr = m;
  autosize();
}
 
void Navigator::autosize() {
  if (!vault)
    return;
  if (mainrect.isEmpty())
    return;
  if (ourpwr == TOOMUCH)
    return;
  resize(vault->size(ourpwr));
  update();
}

void Navigator::paintEvent(QPaintEvent *e) {
  QWidget::paintEvent(e);
  if (!vault)
    return;
  QPainter p;
  p.drawImage(QPoint(0,0), vault->roi(ourpwr, QRect(QPoint(0,0), size())));
  p.setPen(QPen(QColor(255, 192, 0)));
  int l = mainrect.left() >> (ourpwr - mainpwr);
  int t = mainrect.top() >> (ourpwr - mainpwr);
  int r = mainrect.right() >> (ourpwr - mainpwr);
  int b = mainrect.bottom() >> (ourpwr - mainpwr);
  p.drawRect(QRect(QPoint(l,t), QPoint(r,b)));
}
