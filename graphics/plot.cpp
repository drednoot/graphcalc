#include "plot.h"

#include <iostream>

#include "../backend.h"

Graph::Graph(QWidget *parent) : QWidget(parent) {
  plot = new QCustomPlot(this);

  QBoxLayout *main = new QBoxLayout(QBoxLayout::TopToBottom, this);
  QLineEdit *low_x_le = new QLineEdit(this);
  QLineEdit *high_x_le = new QLineEdit(this);
  QLineEdit *precision_le = new QLineEdit(this);
  QBoxLayout *le_lay = new QBoxLayout(QBoxLayout::LeftToRight, this);

  low_x = LOW_X_DEFAULT;
  high_x = HIGH_X_DEFAULT;
  precision = PRECISION_DEFAULT;

  plot->addGraph();

  plot->graph(0)->setPen(QPen(Qt::blue));

  connect(plot->xAxis, SIGNAL(rangeChanged(QCPRange)), plot->xAxis2,
          SLOT(setRange(QCPRange)));
  connect(plot->yAxis, SIGNAL(rangeChanged(QCPRange)), plot->yAxis2,
          SLOT(setRange(QCPRange)));
  plot->xAxis->setRange(low_x, high_x);
  plot->yAxis->setRange(-6, 6);

  connect(low_x_le, &QLineEdit::textChanged, this, &Graph::low_x_changed);
  connect(high_x_le, &QLineEdit::textChanged, this, &Graph::high_x_changed);
  connect(precision_le, &QLineEdit::textChanged, this,
          &Graph::precision_changed);

  plot->xAxis2->setVisible(true);
  plot->xAxis2->setTickLabels(false);
  plot->yAxis2->setVisible(true);
  plot->yAxis2->setTickLabels(false);

  plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                        QCP::iSelectPlottables);

  setMaximumSize(800, 600);
  setMinimumSize(100, 100);
  plot->setMinimumSize(600, 400);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  low_x_le->setPlaceholderText("Lowest x value");
  high_x_le->setPlaceholderText("Highest x value");
  precision_le->setPlaceholderText("Precision");

  le_lay->addWidget(low_x_le);
  le_lay->addWidget(high_x_le);
  le_lay->addWidget(precision_le);
  main->addWidget(plot);
  main->addLayout(le_lay);
  setLayout(main);
}

void Graph::closeEvent(QCloseEvent *event) {
  emit set_graph_not_shown();
  event->accept();
}

void Graph::draw() {
  Lexem_stack in, rpn;
  int incrtd = 0, rpncrtd = 0;
  int rtn = 0;
  QByteArray ba = graph_text.toLocal8Bit();
  const char *str = ba.data();
  double range = high_x - low_x;

  QVector<double> x(MAX_PIXELS * precision), y(MAX_PIXELS * precision);

  rtn = create_lexem_stack(&in);
  if (!rtn) {
    incrtd = 1;
    rtn = create_lexem_stack(&rpn);
  }
  if (!rtn) rpncrtd = 1;

  if (!rtn) rtn = parse_string(str, &in);

  if (!rtn) rtn = in_to_rpn(&in, &rpn);

  for (int i = 0; i < precision * MAX_PIXELS && !rtn; ++i) {
    double di = (double)i / (precision * MAX_PIXELS) * range + low_x;
    double result = di;
    x[i] = di;
    rtn = calculate(rpn, &result);
    if (!rtn) {
      y[i] = result;
    }
  }

  if (!rtn) {
    plot->graph(0)->setData(x, y);
    plot->replot();
  } else {
    x.clear();
    y.clear();
    plot->graph(0)->setData(x, y);
    plot->replot();
  }

  if (incrtd) remove_lexem_stack(&in);
  if (rpncrtd) remove_lexem_stack(&rpn);
}

void Graph::redraw(const QString &text) {
  if (isVisible()) {
    graph_text = text;
    draw();
  } else {
    graph_text = text;
  }
}

void Graph::low_x_changed(const QString &text) {
  double temp;
  bool ok = 1;
  temp = text.toDouble(&ok);
  if (ok) {
    low_x = temp;
    draw();
  } else {
    low_x = LOW_X_DEFAULT;
  }
}

void Graph::high_x_changed(const QString &text) {
  double temp;
  bool ok = 1;
  temp = text.toDouble(&ok);
  if (ok) {
    high_x = temp;
    draw();
  } else {
    high_x = HIGH_X_DEFAULT;
  }
}

void Graph::precision_changed(const QString &text) {
  double temp;
  bool ok = 1;
  temp = text.toDouble(&ok);
  if (ok) {
    precision = temp;
    draw();
  } else {
    precision = PRECISION_DEFAULT;
  }
}

void Graph::show() {
  QWidget::show();
  draw();
}
