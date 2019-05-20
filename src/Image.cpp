// Image.cpp

#include "Image.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <QDebug>

class ImageData {
public:
  ImageData() { }
  ImageData(int X, int Y): image(Y, X, CV_8UC1) { }
public:
  cv::Mat image;
};

Image::Image(int X, int Y): X(X), Y(Y) {
  d = QSharedPointer<ImageData>(new ImageData(X, Y));
  qDebug() << "constructed image" << X << Y;
}

Image::Image(QString fn, double clpblk, double clpwht) {
  d = QSharedPointer<ImageData>(new ImageData());
  cv::Mat img = cv::imread(fn.toUtf8().data(),
			   cv::IMREAD_GRAYSCALE
			   | cv::IMREAD_ANYDEPTH); // Read the file
  if (img.empty()) {
    X = Y = 0;
    return;
  }

  cv::Size s = img.size();
  X = s.width;
  Y = s.height;
   
  if (img.type() == CV_16UC1) {
    // determine actually used range
    QVector<uint64_t> histo(65536);
    uint64_t *h = histo.data();
    for (int y=0; y<Y; y++) {
      uint16_t const *line = &img.at<uint16_t>(y,0);
      for (int x=0; x<X; x++) 
	h[line[x]]++;
    }
    uint64_t N = X*Y;
    uint64_t lowthr = int(.01*clpblk * N);
    uint64_t highthr = int(.01*clpwht * N);
    uint16_t mn = 0;
    uint64_t lowacc = 0;
    while (lowacc < lowthr) 
      lowacc += h[mn++];
    uint16_t mx = 65535;
    uint64_t highacc = 0;
    while (highacc < highthr)
      highacc += h[mx--];
    int32_t dm = mx - mn;
    if (dm<=0)
      dm = 1;
    // stretch contrast
    for (int y=0; y<Y; y++) {
      uint16_t *line = &img.at<uint16_t>(y,0);
      for (int x=0; x<X; x++) {
	int32_t v = line[x];
	v -= mn;
	v *= 256;
	v /= dm;
	line[x] = v<0 ? 0 : v>255 ? 255 : v;
      }
    }
    qDebug() << "range" << mn << mx;
  }

  if (img.type() == CV_8UC1) {
    d->image = img;
  } else {
    img.assignTo(d->image, CV_8UC1);
  }

  qDebug() << "loaded image" << X << Y << "of type" << img.type();
  qDebug() << "cf. 8UC1" << CV_8UC1 << "or 16" << CV_16UC1;
}

uint8_t const *Image::line(int y) const {
  return &d->image.at<uint8_t>(y,0);
}

Image::Image(Image const *src) {
  X = src->width() / 2;
  Y = src->height() / 2;
  d = QSharedPointer<ImageData>(new ImageData(X, Y));
  for (int y=0; y<Y; y++) {
    uint8_t const *srcp1 = src->line(2*y);
    uint8_t const *srcp2 = src->line(2*y+1);
    uint8_t *dstp = &d->image.at<uint8_t>(y,0);
    for (int x=0; x<X; x++) {
      uint32_t v = *srcp1++;
      v += *srcp1++;
      v += *srcp2++;
      v += *srcp2++;
      *dstp++ = (v+2)/4;
    }
  }
}
