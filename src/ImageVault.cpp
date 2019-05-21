// ImageVault.cpp

#include "ImageVault.h"
#include "Image.h"
#include <QFileInfo>
#include <QDebug>

ImageVault::ImageVault() {
  maxmem = 0;
  clpblk = clpwht = 0;
}

ImageVault::~ImageVault() {
  for (Image *img: imgs)
    delete img;
}

void ImageVault::setClipping(double blk, double wht) {
  clpblk = blk;
  clpwht = wht < 0 ? blk : wht;
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
  imgs << new Image(fn, clpblk, clpwht);
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
  if (src->channels()>1)
    return colorroi(src, roi);
  else
    return grayroi(src, roi);
}

QImage ImageVault::grayroi(Image const *src, QRect roi) const {
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
  
  for (int y=0; y<Y; y++) {
    uint8_t const *srcp = src->line(y+y0) + x0;
    uint8_t *dstp = dst.scanLine(y + yout) + xout;
    memcpy(dstp, srcp, X);
  }
  return dst;
}

QImage ImageVault::colorroi(Image const *src, QRect roi) const {
  QImage dst(roi.size(), QImage::Format_RGB32);
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
  
  for (int y=0; y<Y; y++) {
    uint8_t const *rp = src->line(y+y0) + x0;
    uint32_t *dstp = reinterpret_cast<uint32_t*>(dst.scanLine(y + yout)) + xout;
    uint8_t const *gp = rp + X;
    uint8_t const *bp = gp + X;
    for (int x=0; x<X; x++) {
      uint32_t r = *rp++;
      uint32_t g = *gp++;
      uint32_t b = *bp++;
      uint32_t rgb = 0xff000000 + r + (g<<8) + (b<<16);
      *dstp++ = rgb;
    }
  }
  return dst;
}

QSize ImageVault::size(int poweroftwo) const {
  if (imgs.isEmpty())
    return QSize();
  if (poweroftwo<0) 
    return QSize(imgs.first()->width()<<(-poweroftwo),
		 imgs.first()->height()<<(-poweroftwo));
  else if (poweroftwo<imgs.size())
    return QSize(imgs[poweroftwo]->width(),
		 imgs[poweroftwo]->height());
  else
    return QSize();
}

int ImageVault::maxPowerOfTwo() const {
  return imgs.size() - 1;
}
