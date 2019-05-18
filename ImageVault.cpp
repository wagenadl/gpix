// ImageVault.cpp

#include "ImageVault.h"
#include "Image.h"
#include <QFileInfo>
#include <QDebug>

ImageVault::ImageVault() {
  maxmem = 0;
}

ImageVault::~ImageVault() {
  for (Image *img: imgs)
    delete img;
}

void ImageVault::setMaxMemory(int64_t mm) {
  maxmem = mm;
}

bool ImageVault::load(QString fn) {
  QFileInfo fi(fn);
  if (!fi.exists())
    return false;
  if (maxmem>0 && fi.size()>maxmem)
    return false;
  imgs.clear();
  qDebug() << "Loading" << fn << "(" << fi.size() << "bytes)";
  imgs << new Image(fn);
  int W = imgs[0]->width();
  int H = imgs[0]->height();
  while (W>2 && H>2) {
    qDebug() << "Decimating from" << W << "x" << H;
    Image const *src(imgs.last());
    imgs << new Image(src);
    W = imgs.last()->width();
    H = imgs.last()->height();
  }
  return true;
}

QImage ImageVault::roi(int poweroftwo, QRect roi) const {
  if (poweroftwo>=imgs.size())
    return QImage();
  Image const *src = imgs[poweroftwo];
  QImage dst(roi.size(), QImage::Format_Grayscale8);
  int Y = roi.height();
  int X = roi.width();
  int x0 = roi.left();
  int y0 = roi.top();
  int xout = 0;
  int yout = 0;
  bool mustfill = false;
  if (x0<0) {
    X += x0;
    xout = -x0;
    x0 = 0;
    mustfill = true;
  }
  if (y0<0) {
    Y += y0;
    yout = -y0;
    y0 = 0;
    mustfill = true;
  }
  if (x0 + X > src->width()) {
    X = src->width() - x0;
    mustfill = true;
  }
  if (y0 + Y > src->height()) {
    Y = src->height() - y0;
    mustfill = true;
  }
  if (mustfill)
    dst.fill(QColor(128,128,128));
  if (X<=0 || Y<=0)
    return dst;
  
  qDebug() << poweroftwo << roi << xout << x0 << X << "x" << yout << y0 << Y;
  for (int y=0; y<Y; y++) {
    uint8_t const *srcp = src->line(y+y0) + x0;
    uint8_t *dstp = dst.scanLine(y + yout) + xout;
    memcpy(dstp, srcp, X);
  }
  return dst;
}

QSize ImageVault::size(int poweroftwo) const {
  if (imgs.isEmpty())
    return QSize();
  if (poweroftwo<0) 
    return QSize(imgs.first()->width()<<poweroftwo,
		 imgs.first()->height()<<poweroftwo);
  else if (poweroftwo<imgs.size())
    return QSize(imgs[poweroftwo]->width(),
		 imgs[poweroftwo]->height());
  else
    return QSize();
}

int ImageVault::maxPowerOfTwo() const {
  return imgs.size() - 1;
}
