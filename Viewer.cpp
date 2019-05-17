// Viewer.cpp

#include "Viewer.h"
#include <cmath>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>

Viewer::Viewer(QWidget *parent): QFrame(parent) {
  vault = 0;
  poweroftwo = 0;
  // setAutoFillBackground(true);
}

void Viewer::setSource(ImageVault const *v) {
  vault = v;
  QSize s = vault->size();
  if (width()==0 || height()==0) {
    poweroftwo = -1;
    return;
  }
  double xfac = s.width() / width();
  double yfac = s.height() / height();
  double fac = xfac > yfac ? xfac : yfac;
  poweroftwo = std::ceil(log(fac) / log(2));
  topleft = QPoint(0,0);
}

void Viewer::resizeEvent(QResizeEvent *) {
  if (vault && poweroftwo < 0) {
    QSize s = vault->size();
    double xfac = s.width() / width();
    double yfac = s.height() / height();
    double fac = xfac > yfac ? xfac : yfac;
    poweroftwo = std::ceil(log(fac) / log(2));
    topleft = QPoint(0,0);
  }
  update();
}

void Viewer::keyPressEvent(QKeyEvent *e) {
  if (!vault)
    return;
  switch (e->key()) {
  case Qt::Key_Minus: 
    if (poweroftwo<vault->maxPowerOfTwo()) {
      QPoint oldcenter = topleft + QPoint(width()/2, height()/2);
      poweroftwo++;
      QPoint newcenter(oldcenter.x() / 2, oldcenter.y() / 2);
      topleft = newcenter - QPoint(width()/2, height()/2);
      update();
    }
    break;
  case Qt::Key_Plus: case Qt::Key_Equal:
    if (poweroftwo>0) {
      QPoint oldcenter = topleft + QPoint(width()/2, height()/2);
      poweroftwo--;
      QPoint newcenter(oldcenter.x() * 2, oldcenter.y() * 2);
      topleft = newcenter - QPoint(width()/2, height()/2);
      update();
    }
    break;
  default:
    break;
  }
}

void Viewer::mousePressEvent(QMouseEvent *e) {
  presspos = e->pos();
  presstopleft = topleft;
}

void Viewer::mouseMoveEvent(QMouseEvent *e) {
  lastpos = e->pos();
  topleft = presstopleft - lastpos + presspos;
  update();
}

void Viewer::paintEvent(QPaintEvent *e) {
  QWidget::paintEvent(e);
  QPainter p(this);
  if (vault)
    p.drawImage(QPoint(),
		vault->roi(poweroftwo, QRect(topleft, size())));
}
