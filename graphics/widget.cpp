#include "widget.h"

#include <iostream>

#include "lineedit.h"
#include "numbutton.h"
#include "plot.h"

Widget::Widget(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *mainvbox = new QVBoxLayout(this);
  QHBoxLayout *mainhbox = new QHBoxLayout(this);
  lineedit_hbox = new QHBoxLayout(this);
  input_line = new Mainfield(this);
  number_grid = new QGridLayout(this);
  QFrame *vline = new QFrame(this);
  actions_grid = new QGridLayout(this);
  extend_btn = new ExpandBtn("+", this);
  graph = new Graph();
  is_expanded = 0;
  is_graph_shown = 0;

  f.setPointSize(f.pointSize() + 2);
  setFont(f);

  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  mainvbox->setSizeConstraint(QLayout::SetFixedSize);
  mainhbox->setSizeConstraint(QLayout::SetMinAndMaxSize);
  number_grid->setSizeConstraint(QLayout::SetMinAndMaxSize);

  vline->setFrameShape(QFrame::VLine);
  vline->setFrameShadow(QFrame::Sunken);
  vline->setMinimumWidth(3);
  vline->setMaximumWidth(3);
  vline->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

  lineedit_hbox->addWidget(input_line, 8);
  QFont lefont("Courier New");
  lefont.setStyleHint(QFont::Monospace);
  lefont.setPointSize(f.pointSize() + 3);
  input_line->setFont(lefont);

  generate_numberline();
  generate_actions();
  generate_inputline_buttons();
  put_buttons_small();

  QObject::connect(extend_btn, &ExpandBtn::pressed, this,
                   &Widget::toggle_expansion);

  QObject::connect(input_line, &Mainfield::textChanged, graph, &Graph::redraw);

  mainhbox->addLayout(number_grid);
  mainhbox->addWidget(vline);
  mainhbox->addLayout(actions_grid);

  mainvbox->addLayout(lineedit_hbox);
  mainvbox->addLayout(mainhbox);
  mainvbox->addWidget(extend_btn, 0, Qt::AlignRight);

  setLayout(mainvbox);
  setFocusPolicy(Qt::ClickFocus);
}

void Widget::generate_numberline() {
  const char number_grid_str[][NUMBER_GRID_COLUMNS][NUMBER_GRID_MAX_STR_SIZE] =
      NUMBER_GRID;

  for (int i = 0; i < NUMBER_GRID_ROWS; ++i) {
    for (int j = 0; j < NUMBER_GRID_COLUMNS; ++j) {
      const char *str = number_grid_str[i][j];
      numbutton_arr[i][j] = new Numbutton(str, this);
      number_grid->addWidget(numbutton_arr[i][j], i, j);
      QObject::connect(numbutton_arr[i][j], &Numbutton::pressed_text,
                       input_line, &Mainfield::add_string);
    }
  }
}

void Widget::generate_actions() {
  {
    // regular buttons
    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 4; ++j) {
        actions_arr[i][j] = new Numbutton(i, j, this);
        QObject::connect(actions_arr[i][j], &Numbutton::pressed_text,
                         input_line, &Mainfield::add_string);
      }
    }
    for (int i = 0; i < 2; ++i) {
      actions_arr[2][i] = new Numbutton(2, i, this);
      QObject::connect(actions_arr[2][i], &Numbutton::pressed_text, input_line,
                       &Mainfield::add_string);
    }
    QFont xfont = f;
    xfont.setPointSize(f.pointSize() + 5);
    actions_arr[2][1]->setFont(xfont);  // x
  }
  {
    // multichoice buttons
    for (int i = 0; i < 2; ++i) {
      actions_arr[2][2 + i] = new Multichoice(2, 2 + i, this);
      QObject::connect(actions_arr[2][2 + i], &Multichoice::pressed_text,
                       input_line, &Mainfield::add_string);
    }
    actions_arr[3][0] = new Multichoice(3, 0, this);
    QObject::connect(actions_arr[3][0], &Multichoice::pressed_text, input_line,
                     &Mainfield::add_string);
  }
  {
    // special buttons
    actions_arr[3][1] = new Clear(3, 1, this);
    QObject::connect(actions_arr[3][1], &Clear::pressed, input_line,
                     &Mainfield::clear_slot);

    actions_arr[3][3] = new Equalsign(3, 3, this);
    QObject::connect(actions_arr[3][3], &Equalsign::pressed, input_line,
                     &Mainfield::calculate_result);
    QFont eqfont = f;
    eqfont.setPointSize(f.pointSize() + 5);
    actions_arr[3][3]->setFont(eqfont);  // x

    actions_arr[3][2] = new Backspace(3, 2, this);
    QObject::connect(actions_arr[3][2], &Backspace::pressed, input_line,
                     &Mainfield::backspace_slot);
  }
}

void Widget::put_buttons_small() {
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      actions_grid->addWidget(actions_arr[i][j], i, j);
    }
  }
  actions_grid->addWidget(actions_arr[3][1], 2, 0);
  actions_grid->addWidget(actions_arr[3][2], 3, 0);
  actions_grid->addWidget(actions_arr[3][3], 2, 1, 2, 1);

  lineedit_hbox->addWidget(graph_btn, 1);

  hide_expanded();
}

void Widget::toggle_expansion() {
  if (is_expanded) {
    contract_buttons();
    is_expanded = 0;
  } else {
    expand_buttons();
    is_expanded = 1;
  }
}

void Widget::toggle_graph() {
  if (is_graph_shown) {
    graph->hide();
    is_graph_shown = 0;
  } else {
    graph->show();
    is_graph_shown = 1;
  }
}

void Widget::set_graph_not_shown() { is_graph_shown = 0; }

void Widget::show_expanded() {
  for (int i = 0; i < 2; ++i) {
    for (int j = 2; j < 4; ++j) {
      actions_arr[i][j]->show();
    }
  }
  for (int i = 0; i < 4; ++i) {
    actions_arr[2][i]->show();
  }
  actions_arr[3][0]->show();
  graph_btn->show();
}

void Widget::hide_expanded() {
  for (int i = 0; i < 2; ++i) {
    for (int j = 2; j < 4; ++j) {
      actions_arr[i][j]->hide();
    }
  }
  for (int i = 0; i < 4; ++i) {
    actions_arr[2][i]->hide();
  }
  actions_arr[3][0]->hide();
  graph_btn->hide();
}

void Widget::expand_buttons() {
  for (int i = 0; i < 2; ++i) {
    for (int j = 2; j < 4; ++j) {
      actions_grid->addWidget(actions_arr[i][j], i, j);
    }
  }

  actions_grid->removeWidget(actions_arr[3][1]);  // C
  actions_grid->addWidget(actions_arr[3][1], 0, 4);
  actions_grid->removeWidget(actions_arr[3][2]);  // <-
  actions_grid->addWidget(actions_arr[3][2], 1, 4);
  actions_grid->removeWidget(actions_arr[3][3]);  // =
  actions_grid->addWidget(actions_arr[3][3], 2, 4, 2, 1);

  actions_grid->addWidget(actions_arr[2][0], 2, 0);        // sqrt
  actions_grid->addWidget(actions_arr[2][1], 2, 1);        // x
  actions_grid->addWidget(actions_arr[2][2], 2, 2, 1, 2);  // cos
  actions_grid->addWidget(actions_arr[3][0], 3, 0, 1, 2);  // ln
  actions_grid->addWidget(actions_arr[2][3], 3, 2, 1, 2);  // cos

  lineedit_hbox->addWidget(graph_btn);

  show_expanded();
}

void Widget::contract_buttons() {
  for (int i = 0; i < 2; ++i) {
    for (int j = 2; j < 4; ++j) {
      actions_grid->removeWidget(actions_arr[i][j]);
    }
  }

  actions_grid->removeWidget(actions_arr[2][0]);  // sqrt
  actions_grid->removeWidget(actions_arr[2][1]);  // x
  actions_grid->removeWidget(actions_arr[2][2]);  // cos
  actions_grid->removeWidget(actions_arr[3][0]);  // ln
  actions_grid->removeWidget(actions_arr[2][3]);  // cos

  actions_grid->removeWidget(actions_arr[3][1]);  // C
  actions_grid->addWidget(actions_arr[3][1], 2, 0);
  actions_grid->removeWidget(actions_arr[3][2]);  // <-
  actions_grid->addWidget(actions_arr[3][2], 3, 0);
  actions_grid->removeWidget(actions_arr[3][3]);  // =
  actions_grid->addWidget(actions_arr[3][3], 2, 1, 2, 1);

  lineedit_hbox->removeWidget(graph_btn);

  hide_expanded();
}

void Widget::generate_inputline_buttons() {
  graph_btn = new QPushButton(this);
  graph_btn->setIcon(QIcon(":/graph.svg"));
  graph_btn->setIconSize(QSize(30, 30));
  graph_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  QObject::connect(graph_btn, &QPushButton::pressed, this,
                   &Widget::toggle_graph);
  QObject::connect(graph, &Graph::set_graph_not_shown, this,
                   &Widget::set_graph_not_shown);
}

void Widget::closeEvent(QCloseEvent *event) {
  delete graph;
  event->accept();
}

void Widget::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_1) {
    emit numbutton_arr[2][0]->pressed();
  } else if (event->key() == Qt::Key_2) {
    emit numbutton_arr[2][1]->pressed();
  } else if (event->key() == Qt::Key_3) {
    emit numbutton_arr[2][2]->pressed();
  } else if (event->key() == Qt::Key_4) {
    emit numbutton_arr[1][0]->pressed();
  } else if (event->key() == Qt::Key_5) {
    emit numbutton_arr[1][1]->pressed();
  } else if (event->key() == Qt::Key_6) {
    emit numbutton_arr[1][2]->pressed();
  } else if (event->key() == Qt::Key_7) {
    emit numbutton_arr[0][0]->pressed();
  } else if (event->key() == Qt::Key_8) {
    emit numbutton_arr[0][1]->pressed();
  } else if (event->key() == Qt::Key_9) {
    emit numbutton_arr[0][2]->pressed();
  } else if (event->key() == Qt::Key_0) {
    emit numbutton_arr[3][0]->pressed();
  } else if (event->key() == Qt::Key_Period || event->key() == Qt::Key_Comma) {
    emit numbutton_arr[3][1]->pressed();
  } else if (event->key() == Qt::Key_E) {
    emit numbutton_arr[3][2]->pressed();
  } else if (event->key() == Qt::Key_Plus &&
             event->modifiers() & Qt::ControlModifier) {
    if (!is_expanded) {
      emit extend_btn->pressed();
    }
  } else if (event->key() == Qt::Key_Plus) {
    emit actions_arr[0][0]->pressed();
  } else if (event->key() == Qt::Key_Minus &&
             event->modifiers() & Qt::ControlModifier) {
    if (is_expanded) {
      emit extend_btn->pressed();
    }
  } else if (event->key() == Qt::Key_Minus) {
    emit actions_arr[0][1]->pressed();
  } else if (event->key() == Qt::Key_Equal || event->key() == Qt::Key_Return) {
    emit actions_arr[3][3]->pressed();
  } else if (event->key() == Qt::Key_Backspace &&
             event->modifiers() & Qt::ControlModifier) {
    emit actions_arr[3][1]->pressed();
  } else if (event->key() == Qt::Key_Backspace) {
    emit actions_arr[3][2]->pressed();
  } else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ||
             event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
    input_line->setFocus();
  } else if (event->key() == Qt::Key_Slash || event->key() == Qt::Key_Colon) {
    emit actions_arr[1][0]->pressed();
  } else if (event->key() == Qt::Key_Asterisk) {
    emit actions_arr[1][1]->pressed();
  } else if (event->key() == Qt::Key_AsciiCircum) {
    emit actions_arr[1][3]->pressed();
  } else if (event->key() == Qt::Key_X) {
    emit actions_arr[2][1]->pressed();
  } else if (event->key() == Qt::Key_ParenLeft) {
    emit actions_arr[0][2]->pressed();
  } else if (event->key() == Qt::Key_ParenRight) {
    emit actions_arr[0][3]->pressed();
  }
}
