// Posthoc.cpp

#include "Posthoc.h"
#include <QDebug>

QImage Posthoc::apply(QImage img) const {
  int X = img.width();
  int Y = img.height();
  if (img.format() != QImage::Format_Grayscale8) {
    qDebug() << "Posthoc applied to non-grayscale image";
    return img;
  }

  // first, blur
  if (blurfrac>0) {
    // we'll go back and forth twice to do a 2K order LP filter in each direction
    uint8_t *dat = img.bits();
    const int L = img.bytesPerLine();
    const float a = blurfrac;
    const float b = 1 - blurfrac;
    
    // first, horizontal
    for (int y=0; y<Y; y++) {
      uint8_t *line = dat + L;
      float v0 = 128;
      for (int k=0; k<K; k++) {
	for (int x=0; x<X; x++) {
	  v0 = a*v0 + b * *line;
	  *line = v0;
	  line++;
	}
	for (int x=0; x<X; x++) {
	  --line;
	  v0 = a*v0 + b * *line;
	  *line = v0;
	}
      }
    }

    // then, vertical
    for (int x=0; x<X; x++) {
      uint8_t *col = dat + x;
      float v0 = 128;
      for (int k=0; k<K; k++) {
	for (int y=0; y<Y; y++) {
	  v0 = a * v0 + b * *col;
	  *col = v0;
	  col += L;
	}
	for (int y=0; y<Y; y++) {
	  col -= L
	  v0 = a * v0 + b * *col;
	  *col = v0;
	}
      }
    }
  }

  // then correct intensity
  if (blacklevel>0 || whitelevel<255 || log2gamma!=0) {
    // create lookup table and have at it
  }
  
  return img;
}
    
