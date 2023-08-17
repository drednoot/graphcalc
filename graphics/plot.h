#ifndef PLOT_H
#define PLOT_H

#include <QBoxLayout>
#include <QWidget>

#include "../QCustomPlot/qcustomplot.h"

#define MAX_PIXELS 1000
#define LOW_X_DEFAULT -10.0
#define HIGH_X_DEFAULT 10.0
#define PRECISION_DEFAULT 4

class Graph : public QWidget {
  Q_OBJECT
 public:
  Graph(QWidget *parent = nullptr);
  void closeEvent(QCloseEvent *event);
  QCustomPlot *plot;
  void show();

 signals:
  void set_graph_not_shown();

 public slots:
  void redraw(const QString &text);
  void low_x_changed(const QString &text);
  void high_x_changed(const QString &text);
  void precision_changed(const QString &text);

 private:
  void draw();
  double low_x, high_x;
  int precision;
  QString graph_text;
};

#endif
