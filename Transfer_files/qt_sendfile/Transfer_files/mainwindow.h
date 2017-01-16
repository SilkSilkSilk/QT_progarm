#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QRadioButton"
#include "WAN_send.h"
#include "WAN_recv.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_chooseButton_clicked();

private:
    Ui::MainWindow *ui;
	WAN_send *wansend_widgt;
	WAN_recv *wanrecv_widgt;
};

#endif // MAINWINDOW_H
