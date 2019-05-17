// Image.h

#ifndef IMAGE_H

#define IMAGE_H

#include <QSharedPointer>

class Image {
public:
  Image(int X, int Y);
  Image(QString filename);
  Image(Image const *); // decimates
  bool isValid() const { return X>0 && Y>0; }
  uint8_t const *line(int y) const;
  int width() const { return X; }
  int height() const { return Y; }
private:
  QSharedPointer<class ImageData> d;
  int X, Y;
};

#endif