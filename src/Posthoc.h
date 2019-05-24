// Posthoc.h

#ifndef POSTHOC_H

#define POSTHOC_H

#include <QImage>

class Posthoc {
public:
  uint8_t blacklevel;
  uint8_t whitelevel;
  float log2gamma;
  float blurfrac;
  int K;
public:
  Posthoc(): blacklevel(0), whitelevel(255), log2gamma(0),
    blurfrac(0), K(1) { }
  void apply(QImage &) const;
};

#endif
