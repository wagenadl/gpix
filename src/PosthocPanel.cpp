// PosthocPanel.cpp

#include "PosthocPanel.h"
#include "ui_PosthocPanel.h"
#include <cmath>

PosthocPanel::PosthocPanel(QWidget *parent): QDialog(parent) {
  ui = new Ui_PosthocPanel;
  ui->setupUi(this);
  connect(ui->blacklevel, &QSlider::valueChanged,
          [this](int x) {
            ph.blacklevel = x;
            settingsChanged();
          });
  connect(ui->whitelevel, &QSlider::valueChanged,
          [this](int x) {
            ph.whitelevel = x;
            settingsChanged();
          });
  connect(ui->gammaadjust, &QSlider::valueChanged,
          [this](int x) {
            ph.log2gamma = x/50.;
            settingsChanged();
          });
  connect(ui->lowpass, &QSlider::valueChanged,
          [this](int x) {
            ph.blurfrac = x/100.;
            settingsChanged();
          });
}

PosthocPanel::~PosthocPanel() {
}

Posthoc const &PosthocPanel::posthoc() const {
  return ph;
}

void PosthocPanel::reset() {
  ph.blacklevel = 0;
  ph.whitelevel = 255;
  ph.log2gamma = 0;
  ph.blurfrac = 0;
  ph.K = 1;
  settingsChanged();
}

