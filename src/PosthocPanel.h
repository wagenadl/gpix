// PosthocPanel.h

#ifndef POSTHOCPANEL_H

#define POSTHOCPANEL_H

#include <QDialog>
#include "Posthoc.h"

class PosthocPanel: public QDialog {
  Q_OBJECT;
public:
  PosthocPanel(QWidget *parent=0);
  virtual ~PosthocPanel();
  Posthoc const &posthoc() const;
  void reset();
signals:
  void settingsChanged();
private:
  Posthoc ph;
  class Ui_PosthocPanel *ui;
};

#endif
