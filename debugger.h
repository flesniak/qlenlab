#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
    class debugger;
}

class debugger : public QDialog
{
    Q_OBJECT

public:
    explicit debugger(QWidget *parent = 0);
    ~debugger();

public slots:
    void process_incoming(QByteArray data);

private:
    Ui::debugger *ui;

private slots:
    void update_editfields(int index);
    void send_command();
};

#endif // DEBUGGER_H
