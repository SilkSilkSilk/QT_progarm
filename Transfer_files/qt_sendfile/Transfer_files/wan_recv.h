#ifndef WAN_RECV_H
#define WAN_RECV_H

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class WAN_recv;
}

class WAN_recv : public QWidget
{
    Q_OBJECT

public:
    explicit WAN_recv(QWidget *parent = nullptr);
    ~WAN_recv();

private slots:
	void on_changeButton_clicked();
	void newclientconnect();
	void readbuf();

private:
	const char* REQUEST = "request";
	const char* RESPONSE = "response";
	const qint16 loadsize = 4096;
    Ui::WAN_recv *ui;
	QString file_pos;
	QHostAddress addr_client;
	QTcpServer *recv_server;
	QTcpSocket *recv_clien;
	qint16 flag_recv;																								//readbuf时三件事的区分
	qint64 filesize;
	QString filename;
};

#endif // WAN_RECV_H
