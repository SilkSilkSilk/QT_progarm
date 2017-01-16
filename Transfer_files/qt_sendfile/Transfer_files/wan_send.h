#ifndef WAN_SEND_H
#define WAN_SEND_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QHostAddress>
#include "wan_sendfile.h"
#include <QTableWidgetItem>
#include <QMessageBox>

namespace Ui {
class WAN_send;
}

class WAN_send : public QWidget
{
    Q_OBJECT

public:
    explicit WAN_send(QWidget *parent = 0);
    ~WAN_send();

private slots:
	void on_sendfileButton_clicked();

private:
    Ui::WAN_send *ui;
	QString filename;
	QHostAddress *addr;
	quint16 port;
	wan_sendfile *client_sendfile;
	QMessageBox *mesgbox;
};

#endif // WAN_SEND_H
