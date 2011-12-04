#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <QDialog>

namespace Ui {
    class debugger;
}

class debugger : public QDialog
{
    Q_OBJECT

public:
    explicit debugger(QWidget *parent = 0);
    ~debugger();

private:
    Ui::debugger *ui;
};

#endif // DEBUGGER_H
