// Posthoc.cpp

#include "Posthoc.h"
#include <QDebug>
#include <cmath>

void Posthoc::apply(QImage &img) const {
  int X = img.width();
  int Y = img.height();
  const int L = img.bytesPerLine();
  if (img.format() != QImage::Format_Grayscale8) {
    qDebug() << "Posthoc applied to non-grayscale image";
    return;
  }
  
  
  // first, blur
  if (blurfrac>0) {
    // we'll go back and forth twice to do a 2K order LP filter in each direction
    const float a = blurfrac;
    const float b = 1 - blurfrac;
    uint8_t *dat = img.bits();
    
    // first, horizontal
    for (int y=0; y<Y; y++) {
      uint8_t *line = dat + L*y;
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
	  col -= L;
	  v0 = a * v0 + b * *col;
	  *col = v0;
	}
      }
    }
  }

  // then correct intensity
  if (blacklevel>0 || whitelevel<255 || log2gamma!=0) {
    // create lookup table and have at it
    uint8_t lookup[256];
    if (whitelevel <= blacklevel) {
      for (int v=0; v<256; v++)
        lookup[v] = v<128 ? 0 : 255;
    } else {
      for (int v=0; v<256; v++) {
        float x = (v-blacklevel) *1.0 / (whitelevel - blacklevel);
        if (x<0)
          x = 0;
        else if (x>1)
          x = 1;
        x = std::pow(x, std::pow(2, log2gamma));
        lookup[v] = int(255.9999 * x);
      }
    }
    uint8_t *dat = img.bits();
    for (int y=0; y<Y; y++) {
      uint8_t *line = dat + L*y;
      for (int x=0; x<X; x++) {
        *line = lookup[*line];
        ++line;
      }
    }
  }
}
    
