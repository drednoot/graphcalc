#ifndef NUMBUTTON_H
#define NUMBUTTON_H

#include <QMenu>
#include <QMouseEvent>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <cstring>

#include "action.h"

#define NUMBER_GRID \
  { {"7", "8", "9"}, {"4", "5", "6"}, {"1", "2", "3"}, {"0", ".", "e"}, }
#define NUMBER_GRID_MAX_STR_SIZE 2
#define NUMBER_GRID_ROWS 4
#define NUMBER_GRID_COLUMNS 3

#define ACTIONS_GRID                                          \
  {                                                           \
    {"+", "-", "(", ")"}, {"/", "*", "^", "mod"},             \
        {"sqrt", "x", "cos", "acos"}, {"ln", "C", "<-", "="}, \
  }
#define ACTIONS_GRID_SHOWN                                        \
  {                                                               \
    {L"+", L"-", L"(", L")"}, {L"÷", L"×", L"^", L"mod"},         \
        {L"√", L"𝑥", L"cos", L"acos"}, {L"ln", L"C", L"←", L"="}, \
  }
#define ACTIONS_GRID_MULTICHOICE                                       \
  {                                                                    \
    {}, {}, {{}, {}, {"cos", "sin", "tan"}, {"acos", "asin", "atan"}}, \
        {{"ln", "log"}, {}, {}, {}},                                   \
  }
#define ACTIONS_GRID_MULTICHOICE_SHOWN                                       \
  {                                                                          \
    {}, {}, {{}, {}, {L"cos", L"sin", L"tan"}, {L"acos", L"asin", L"atan"}}, \
        {{L"ln", L"log"}, {}, {}, {}},                                       \
  }
#define ACTIONS_GRID_MULTICHOICE_COUNT \
  { {}, {}, {0, 0, 3, 3}, {2, 0, 0, 0}, }
#define ACTIONS_GRID_MAX_STR_SIZE 5
#define ACTIONS_GRID_ROWS 4
#define ACTIONS_GRID_MAX_CHOICE 4
#define ACTIONS_GRID_COLUMNS 4

class Numbutton : public QPushButton {
  Q_OBJECT

 public:
  Numbutton(const QString &text, QWidget *parent = nullptr);
  Numbutton(int ai, int aj, QWidget *parent = nullptr);

 private:
  QString append_str;

 public slots:
  void pressed_text_slot();

 signals:
  void pressed_text(const QString &text);
};

class Equalsign : public Numbutton {
  Q_OBJECT
 public:
  Equalsign(int ai, int aj, QWidget *parent = nullptr);
};

class Backspace : public Numbutton {
  Q_OBJECT
 public:
  Backspace(int ai, int aj, QWidget *parent = nullptr);
};

class Clear : public Numbutton {
  Q_OBJECT
 public:
  Clear(int ai, int aj, QWidget *parent = nullptr);
};

class Multichoice : public Numbutton {
  Q_OBJECT
 public:
  Multichoice(int ai, int aj, QWidget *parent = nullptr);

 private:
  QMenu *menu;
  char choices[ACTIONS_GRID_MAX_CHOICE][ACTIONS_GRID_MAX_STR_SIZE];
  const char *cur_choice;
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

 public slots:
  void change_choice(MultichoiceAction *choice);
  void pressed_text_slot();
};

class ExpandBtn : public QPushButton {
  Q_OBJECT
 public:
  ExpandBtn(const QString &text, QWidget *parent = nullptr);

 private:
  int is_expanded;

 public slots:
  void expand_action();
};

#endif
