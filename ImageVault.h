// ImageVault.h

#ifndef IMAGEVAULT_H

#define IMAGEVAULT_H

#include <stdint.h>
#include <QString>
#include <QImage>

class ImageVault {
public:
  ImageVault();
  ~ImageVault();
  void setMaxMemory(int64_t);
  bool load(QString fn); // true if OK
  QImage roi(int poweroftwo, QRect roi) const;
  QSize size(int poweroftwo=0) const;
  int maxPowerOfTwo() const;
private:
  QList<class Image *> imgs;
  int64_t maxmem;
};

#endif
