#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	wansend_widgt(nullptr),
	wanrecv_widgt(nullptr)
{
    ui->setupUi(this);

    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_chooseButton_clicked()
{
	if (ui->wanButton->isChecked() && ui->sendButton_3->isChecked())
	{
		if (wanrecv_widgt != nullptr)
		{
			wanrecv_widgt->hide();
		}

		if (wansend_widgt == nullptr)
		{
			wansend_widgt = new WAN_send(this);

			wansend_widgt->move(QPoint(10, 20));
			wansend_widgt->raise();
			wansend_widgt->show();
			qDebug() << "wan send";
		}
		if (wansend_widgt->isHidden())
		{
			wansend_widgt->raise();
			wansend_widgt->show();
		}
	}
	else if (ui->wanButton->isChecked() && ui->recvButton_4->isChecked())
	{
		if (wansend_widgt != nullptr)
		{
			wansend_widgt->hide();
		}
		
		if (wanrecv_widgt == nullptr)
		{
			wanrecv_widgt = new WAN_recv(this);

            wanrecv_widgt->move(QPoint(6, 0));
			wanrecv_widgt->raise();
			wanrecv_widgt->show();
			qDebug() << "wan recv";
		}
		if (wanrecv_widgt->isHidden())
		{
			wanrecv_widgt->raise();
			wanrecv_widgt->show();
		}
	}
	else if (ui->localButton_2->isChecked() && ui->sendButton_3->isChecked())
	{
		qDebug() << "local send";
	}
	else if(ui->localButton_2->isChecked() && ui->recvButton_4->isChecked())
	{
		qDebug() << "loacl recv";
	}
}
