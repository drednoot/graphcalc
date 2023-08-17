#include "numbutton.h"

Numbutton::Numbutton(const QString &text, QWidget *parent)
    : QPushButton(text, parent) {
  setMinimumSize(50, 50);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  QObject::connect(this, &Numbutton::pressed, this,
                   &Numbutton::pressed_text_slot);
  QByteArray ba = text.toLocal8Bit();
  const char *ctext = ba.data();
  append_str = ctext;
}

Numbutton::Numbutton(int ai, int aj, QWidget *parent) : QPushButton(parent) {
  setMinimumSize(50, 50);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  QObject::connect(this, &Numbutton::pressed, this,
                   &Numbutton::pressed_text_slot);
  const wchar_t wstr[][ACTIONS_GRID_COLUMNS][ACTIONS_GRID_MAX_STR_SIZE] =
      ACTIONS_GRID_SHOWN;
  const char str[][ACTIONS_GRID_COLUMNS][ACTIONS_GRID_MAX_STR_SIZE] =
      ACTIONS_GRID;
  const wchar_t *text = wstr[ai][aj];
  setText(QString::fromWCharArray(text));
  append_str = str[ai][aj];
}

void Numbutton::pressed_text_slot() { emit pressed_text(append_str); }

Multichoice::Multichoice(int ai, int aj, QWidget *parent)
    : Numbutton(ai, aj, parent) {
  menu = new QMenu(this);
  const wchar_t actions_shown[][ACTIONS_GRID_COLUMNS][ACTIONS_GRID_MAX_CHOICE]
                             [ACTIONS_GRID_MAX_STR_SIZE] =
                                 ACTIONS_GRID_MULTICHOICE_SHOWN;
  const char choices_all[][ACTIONS_GRID_COLUMNS][ACTIONS_GRID_MAX_CHOICE]
                        [ACTIONS_GRID_MAX_STR_SIZE] = ACTIONS_GRID_MULTICHOICE;
  int actions_count[][ACTIONS_GRID_COLUMNS] = ACTIONS_GRID_MULTICHOICE_COUNT;

  for (int i = 0; i < actions_count[ai][aj]; ++i) {
    std::strcpy(choices[i], choices_all[ai][aj][i]);
    MultichoiceAction *action = new MultichoiceAction(
        QString::fromWCharArray(actions_shown[ai][aj][i]), choices[i], this);
    menu->addAction(action);
    QObject::connect(action, &MultichoiceAction::triggered_choice, this,
                     &Multichoice::change_choice);
  }
  cur_choice = choices[0];
  QObject::disconnect(this, &Numbutton::pressed, this,
                      &Numbutton::pressed_text_slot);
  QObject::connect(this, &Multichoice::pressed, this,
                   &Multichoice::pressed_text_slot);
  setMenu(menu);
}

void Multichoice::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    emit showMenu();
  } else if (event->button() == Qt::LeftButton) {
    setMenu(nullptr);
    emit pressed();
    setDown(true);
  } else {
    Numbutton::mousePressEvent(event);
  }
}

void Multichoice::mouseReleaseEvent(QMouseEvent *event) {
  emit released();
  setMenu(menu);
  Numbutton::mouseReleaseEvent(event);
}

void Multichoice::change_choice(MultichoiceAction *choice) {
  cur_choice = choice->choice;
  setText(choice->text());
}

void Multichoice::pressed_text_slot() { emit pressed_text(cur_choice); }

// special buttons

Equalsign::Equalsign(int ai, int aj, QWidget *parent)
    : Numbutton(ai, aj, parent) {
  setMinimumSize(50, 50);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

Backspace::Backspace(int ai, int aj, QWidget *parent)
    : Numbutton(ai, aj, parent) {}

Clear::Clear(int ai, int aj, QWidget *parent) : Numbutton(ai, aj, parent) {}

// expand button

ExpandBtn::ExpandBtn(const QString &text, QWidget *parent)
    : QPushButton(text, parent) {
  QObject::connect(this, &ExpandBtn::pressed, this, &ExpandBtn::expand_action);
  is_expanded = 0;
  setMaximumSize(15, 15);
}

void ExpandBtn::expand_action() {
  if (is_expanded) {
    setText("+");
    is_expanded = 0;
  } else {
    setText("-");
    is_expanded = 1;
  }
}
