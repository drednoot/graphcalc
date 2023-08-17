#include "action.h"

MultichoiceAction::MultichoiceAction(const QString &text, const char *choice,
                                     QWidget *parent)
    : QAction(text, parent) {
  this->choice = choice;
  QObject::connect(this, &MultichoiceAction::triggered, this,
                   &MultichoiceAction::resend_to_choice);
}

void MultichoiceAction::resend_to_choice() { emit triggered_choice(this); }
