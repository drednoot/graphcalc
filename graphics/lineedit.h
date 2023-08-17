#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include <cstdio>

#define MAX_LENGTH 256
#define ERROR_MESSAGES                                                    \
  {                                                                       \
    "OK.", "Not enough memory.",                                          \
        "Something went wrong while parsing your string (your string is " \
        "incorrect).",                                                    \
        "Stack turned out to be empty.",                                  \
        "Found closed bracket without an opening bracket.",               \
        "Found opened bracket without a closing bracket.",                \
        "Some operator didn't get enough arguments.",                     \
        "Numbers left on the stack (syntactic error).",                   \
        "Please enter 'x' value.",                                        \
  }
#define MAX_ERROR_LENGTH 256

class Mainfield : public QLineEdit {
  Q_OBJECT

 public:
  Mainfield(QWidget *parent = nullptr);

 public slots:
  void add_string(const QString &str);
  void backspace_slot();
  void clear_slot();
  void calculate_result();
  void show_error(int code);

 private:
  double x;
};

#endif
