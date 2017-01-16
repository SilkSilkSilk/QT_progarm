#include "wan_send.h"
#include "ui_wan_send.h"

WAN_send::WAN_send(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WAN_send),
	mesgbox(nullptr),
	client_sendfile(nullptr)
{
    ui->setupUi(this);

	ui->tableWidget->setRowCount(5);																				//设置行数
	ui->tableWidget->setColumnCount(2);																				//设置列数
	ui->tableWidget->setColumnWidth(0, 120);																		//设置列宽
	ui->tableWidget->setColumnWidth(1, 50);																			//设置列宽
	ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QString("115.159.100.211")));								//指定位置写数据
	ui->tableWidget->setItem(0, 1, new QTableWidgetItem(QString("50000")));
}

WAN_send::~WAN_send()
{
	delete client_sendfile;
	delete mesgbox;
	delete addr;
	delete ui;
}

void WAN_send::on_sendfileButton_clicked()
{
	auto row = ui->tableWidget->currentRow();
	qDebug() << "row:" << row;

	QTableWidgetItem *item = ui->tableWidget->item(row, 0);
	if (item == nullptr)
	{
		if (mesgbox == nullptr)
		{
			mesgbox = new QMessageBox(this);
			mesgbox->setText("no have data!");
		}
		mesgbox->exec();
		return;
	}
	addr = new QHostAddress(item->text());
	item = ui->tableWidget->item(row, 1);
	if (item == nullptr)
	{
		if (mesgbox == nullptr)
		{
			mesgbox = new QMessageBox(this);
			mesgbox->setText("no have data!");
		}
		mesgbox->exec();
		return;
	}
	port = item->text().toInt();
	qDebug() << "ip is:" << addr->toString() << "	" << "port is:" << port;

	filename = QFileDialog::getOpenFileName(this, tr("select file to send"));
	qDebug() << filename;

	if (filename.isNull())
	{
		return;
	}

	if (client_sendfile == nullptr)
	{
		client_sendfile->deleteLater();
		delete client_sendfile;
	}
	client_sendfile = new wan_sendfile(&filename, addr, port);
}
