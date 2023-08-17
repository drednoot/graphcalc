#ifndef ACTIONS_H
#define ACTIONS_H

#include <QAction>

class MultichoiceAction : public QAction {
  Q_OBJECT
 public:
  MultichoiceAction(const QString &text, const char *choice, QWidget *parent);
  const char *choice;

 public slots:
  void resend_to_choice();

 signals:
  void triggered_choice(MultichoiceAction *choice);
};

#endif
