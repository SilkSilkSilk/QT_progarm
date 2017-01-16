#include "wan_sendfile.h"

wan_sendfile::wan_sendfile(QString *filename, QHostAddress *addr, quint16 port, QObject * parent) :
	QObject(parent),
	bytesWritten(0),
	loadSize(4*1024)
{
	localfile = new QFile(*filename);
	if (!localfile->open(QFile::ReadOnly))
	{
		qDebug() << "open file error!";
		return;
	}

	filesize = localfile->size();
	havesize = filesize;

	QFileInfo info(*filename);
	localfilename = info.fileName();
	qDebug() << localfilename;

	clientsock = new QTcpSocket(this);
	clientsock->connectToHost(*addr, port);

	QByteArray data(REQUEST);
	clientsock->write(data);																						//发送请求

	connect(clientsock, &QTcpSocket::readyRead, this, &wan_sendfile::readdata);
	connect(clientsock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

wan_sendfile::~wan_sendfile() 
{
	delete clientsock;
	delete localfile;
}

void wan_sendfile::readdata()
{
	QByteArray data = clientsock->readAll();
	if (strcmp(data.data(), RESPONSE) == 0)
	{
		QString datasend = QString::number(filesize) + QString(" ") + localfilename + QString("\n");
		bytesWritten -= datasend.size();
		clientsock->write(datasend.toLocal8Bit(), datasend.size());

		connect(clientsock, &QTcpSocket::bytesWritten, this, &wan_sendfile::filedatasend);
	}
	else
	{
		QByteArray data(REQUEST);
		clientsock->write(data);
		qDebug() << "send REQUEST agin";
	}
}

void wan_sendfile::filedatasend(qint64 numByte)
{
	bytesWritten += numByte;
	if (havesize > 0)
	{
		localfile->seek(bytesWritten);
		outBlock = localfile->read(qMin(loadSize, havesize));

		havesize -= clientsock->write(outBlock);
	}
	else
	{
		/*emit finishsend(addr.toString());*/
	}
}

void wan_sendfile::displayError(QAbstractSocket::SocketError)
{
	qDebug() << clientsock->errorString();
}