#ifndef WIDGET_H
#define WIDGET_H

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "lineedit.h"
#include "numbutton.h"
#include "plot.h"

class Widget : public QWidget {
  Q_OBJECT

 public:
  Widget(QWidget *parent = nullptr);

 protected:
  void closeEvent(QCloseEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

 private:
  Mainfield *input_line;
  int is_expanded;

  QGridLayout *number_grid;
  Numbutton *numbutton_arr[NUMBER_GRID_ROWS][NUMBER_GRID_COLUMNS];
  void generate_numberline();

  QGridLayout *actions_grid;
  Numbutton *actions_arr[ACTIONS_GRID_ROWS][ACTIONS_GRID_COLUMNS];
  void generate_actions();
  void put_buttons_small();

  QPushButton *extend_btn;
  void expand_buttons();
  void contract_buttons();
  void hide_expanded();
  void show_expanded();

  QHBoxLayout *lineedit_hbox;
  QPushButton *graph_btn;
  void generate_inputline_buttons();

  int is_graph_shown;
  Graph *graph;

  QFont f;

 public slots:
  void toggle_expansion();
  void toggle_graph();
  void set_graph_not_shown();
};

#endif  // WIDGET_H
