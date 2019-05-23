// Posthoc.h

#ifndef POSTHOC_H

#define POSTHOC_H

#include <QImage>

public Posthoc {
public:
  uint8_t blacklevel;
  uint8_t whitelevel;
  float log2gamma;
  float blurfrac;
  int K;
public:
  Posthoc(): blacklevel(0), whitelevel(255), log2gamma(0),
    blurfrac(0), K(1) { }
  }
  QImage apply(QImage) const;
};

#endif
