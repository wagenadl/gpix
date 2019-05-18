// Viewer.cpp

#include "Viewer.h"
#include <cmath>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include "Navigator.h"
#include <QDebug>

Viewer::Viewer(QWidget *parent): QFrame(parent) {
  vault = 0;
  poweroftwo = 0;
  nav = 0;
  naven = false;
  // setAutoFillBackground(true);
  setTitle();
}

static constexpr int MUSTRECALC = 10000;

void Viewer::setSource(ImageVault const *v) {
  vault = v;
  QSize s = vault->size();
  if (width()==0 || height()==0) {
    poweroftwo = MUSTRECALC;
    return;
  }
  double xfac = s.width() / width();
  double yfac = s.height() / height();
  double fac = xfac > yfac ? xfac : yfac;
  poweroftwo = std::ceil(log(fac) / log(2));
  topleft = QPoint(0,0);
  if (naven) {
    nav->setSource(vault);
    nav->setMainPower(poweroftwo);
    nav->setMainRect(QRect(topleft, size()));
  }
}

void Viewer::resizeEvent(QResizeEvent *) {
  if (vault && poweroftwo == MUSTRECALC) {
    QSize s = vault->size();
    double xfac = s.width() / width();
    double yfac = s.height() / height();
    double fac = xfac > yfac ? xfac : yfac;
    poweroftwo = std::ceil(log(fac) / log(2));
    setTitle();
    topleft = QPoint(0,0);
  }
  if (naven) {
    qDebug() << "resize";
    nav->setMainRect(QRect(topleft, size()));
    nav->autoPower(size());
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
      setTitle();
      if (naven)
	nav->setMainPower(poweroftwo);
      QPoint newcenter(oldcenter.x() / 2, oldcenter.y() / 2);
      topleft = newcenter - QPoint(width()/2, height()/2);
      newPosition();
    }
    break;
  case Qt::Key_Plus: case Qt::Key_Equal:
    if (poweroftwo>-2) {
      QPoint oldcenter = topleft + QPoint(width()/2, height()/2);
      poweroftwo--;
      setTitle();
      if (naven)
	nav->setMainPower(poweroftwo);
      QPoint newcenter(oldcenter.x() * 2, oldcenter.y() * 2);
      topleft = newcenter - QPoint(width()/2, height()/2);
      newPosition();
    }
    break;
  case Qt::Key_N:
    enableNavigator(!naven);
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
  if (!vault)
    return;
  lastpos = e->pos();
  topleft = presstopleft - 3*(lastpos - presspos);
  newPosition();
}

void Viewer::newPosition() {
  const int MARGIN = width()/50;
  QSize s = vault->size(poweroftwo);
  int r = topleft.x() + width();
  int b = topleft.y() + height();
  if (r > s.width() + MARGIN)
    topleft.setX(s.width() + MARGIN - width());
  if (b > s.height() + MARGIN)
    topleft.setY(s.height() + MARGIN - height());
  if (topleft.x() < -MARGIN)
    topleft.setX(-MARGIN);
  if (topleft.y() < -MARGIN)
    topleft.setY(-MARGIN);
  if (naven)
    nav->setMainRect(QRect(topleft, size()));
  update();
}

void Viewer::paintEvent(QPaintEvent *e) {
  QWidget::paintEvent(e);
  if (!vault)
    return;
  QPainter p(this);
  if (poweroftwo<0) {
    int x0 = topleft.x() >> (-poweroftwo);
    int y0 = topleft.y() >> (-poweroftwo);
    int w = width() >> (-poweroftwo);
    int h = height() >> (-poweroftwo);
    QImage img = vault->roi(0, QRect(QPoint(x0,y0), QSize(w,h)));
    p.scale(1<<(-poweroftwo), 1<<(-poweroftwo));
    p.drawImage(QPoint(), img);
  } else {
    p.drawImage(QPoint(),
		vault->roi(poweroftwo, QRect(topleft, size())));
  }
}

void Viewer::setFilename(QString f) {
  fn = f;
  setTitle();
}

void Viewer::setTitle() {
  QString ttl;
  if (fn.isEmpty())
    ttl = "miv";
  else if (poweroftwo==MUSTRECALC)
    ttl = fn;
  else if (poweroftwo<0)
    ttl = QString("%1 - %2:1").arg(fn).arg(1<<(-poweroftwo));
  else
    ttl = QString("%1 - 1:%2").arg(fn).arg(1<<(poweroftwo));
  setWindowTitle(ttl);
}

void Viewer::enableNavigator(bool x) {
  naven = x;
  if (x) {
    if (!nav) {
      nav = new Navigator(this);
      nav->setSource(vault);
    } 
    nav->move(5,5);
    nav->setMainPower(poweroftwo);
    nav->setMainRect(QRect(topleft, size()));
    nav->autoPower(size());
    nav->show();
  } else {
    nav->hide();
  }
}
