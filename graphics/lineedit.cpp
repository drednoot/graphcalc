#include "lineedit.h"

#include <QInputDialog>
#include <QMessageBox>
#include <cstring>

#include "../backend.h"

Mainfield::Mainfield(QWidget *parent) : QLineEdit(parent) {
  setMinimumHeight(50);
  setMaxLength(MAX_LENGTH);
  setPlaceholderText("0");
  setAlignment(Qt::AlignRight);
  x = 0;
}

void Mainfield::add_string(const QString &str) {
  if (text() == "0") {
    clear();
  }
  insert(str);
}
void Mainfield::backspace_slot() { backspace(); }
void Mainfield::clear_slot() { clear(); }

void Mainfield::calculate_result() {
  QByteArray ba = text().toLocal8Bit();
  const char *str = ba.data();
  int rtn = 0;
  double result = 0;
  char output[MAX_LENGTH] = "";

  if (!text().isEmpty()) {
    rtn = form_answer(str, &result);
  }
  if (!rtn && text().contains("x")) {
    bool ok;
    result = QInputDialog::getDouble(this, "Input 'x' value", "'x' value:", x,
                                     -1.7e308, 1.7e308, 6, &ok);
    if (ok) {
      x = result;
      rtn = form_answer(str, &result);
    } else {
      rtn = INCORRECT_X_ERROR;
    }
  }

  if (!rtn) {
    std::sprintf(output, "%g", result);
    setText(output);
  } else {
    show_error(rtn);
  }
}

void Mainfield::show_error(int code) {
  const char error_message[][MAX_ERROR_LENGTH] = ERROR_MESSAGES;
  QMessageBox *err = new QMessageBox(this);
  err->setText(error_message[code]);
  err->exec();
  delete err;
}
