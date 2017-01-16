#include "wan_recv.h"
#include "ui_wan_recv.h"

WAN_recv::WAN_recv(QWidget *parent) :
    QWidget(parent),
	file_pos("D:\\downloadfile"),																					// ��\\��ת�ƺ��ʾ"\"
	recv_clien(nullptr),
	flag_recv(0),
    ui(new Ui::WAN_recv)
{
    ui->setupUi(this);

	ui->lineEdit->setText(file_pos);
	
	recv_server = new QTcpServer(this);
	if (!recv_server->listen(QHostAddress::AnyIPv4, 50000))
	{
		qDebug() << recv_server->errorString();
		recv_server->close();
	}

	connect(recv_server, &QTcpServer::newConnection, this, &WAN_recv::newclientconnect);
}

WAN_recv::~WAN_recv()
{
    delete ui;
}

void WAN_recv::on_changeButton_clicked()
{
	QFileDialog download_dialog(this);
	download_dialog.setFileMode(QFileDialog::Directory);															//����Ŀ¼ģʽ
	download_dialog.setViewMode(QFileDialog::Detail);																//������ϸģʽ
	QStringList fileNames;	
	if (download_dialog.exec())
		fileNames = download_dialog.selectedFiles();																//����ѡ���Ŀ¼				
	if (fileNames.size() > 1)
	{
		QMessageBox msgbox(this);
		msgbox.setText("choose to many directory,please choose one directory!");
	}
	ui->lineEdit->setText(fileNames.at(0));
	qDebug() << "fileName:" << fileNames;


}

void WAN_recv::newclientconnect()
{
	if (recv_clien != nullptr)
	{
		delete recv_clien;																							//����
	}
	recv_clien = this->recv_server->nextPendingConnection();
	addr_client = recv_clien->peerAddress();
	connect(recv_clien, &QTcpSocket::readyRead, this, &WAN_recv::readbuf);
}

void WAN_recv::readbuf()
{
	QByteArray buf(loadsize, 0);
	buf = recv_clien->readAll();

	if (flag_recv == 0)
	{
		if (strcmp(buf.data(), RESPONSE) == 0)
		{
			QByteArray data(RESPONSE);
			recv_clien->write(data);
			qDebug() << "RESPONSE to client";
			flag_recv = 1;																							//Ӧ����ɣ�ȥ�����ļ����Ⱥ�����
		}
	}
	else if (flag_recv == 1)
	{
//		QString str(buf.constData());
//		filesize = str.left(str.indexOf(' ')).toLongLong();
//		filename= str.mid(str.indexOf(' ') + 1);
		sscanf(buf.data(), "%ld %s", &filesize, filename.data());
		qDebug() << "filesize:" << filesize << "	" << filename;
		flag_recv = 2;																								//�����ļ����Ⱥ�������ɣ�ȥ�����ļ�
	}
	else if (flag_recv == 2)
	{

		flag_recv = 0;																								//�ļ�������ɣ�ȥѭ��Ӧ��
	}
	
}