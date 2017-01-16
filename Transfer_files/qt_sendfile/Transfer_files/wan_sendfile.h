#ifndef __WAN_SENDFILE_H__
#define __WAN_SENDFILE_H__

#include <QObject>
#include "QTcpSocket"
#include "QFileInfo"

class wan_sendfile : public QObject {
	Q_OBJECT

public:
	explicit wan_sendfile(QString *filename, QHostAddress *addr, quint16 port, QObject* parent = Q_NULLPTR);
	~wan_sendfile();

private slots:
	void readdata();
	void filedatasend(qint64 numByte);
	void displayError(QAbstractSocket::SocketError);

private:
	const char* REQUEST = "request";
	const char* RESPONSE = "response";

	QTcpSocket *clientsock;
	QFile *localfile;
	QString localfilename;
	qint64 filesize;
	qint64 havesize;																								//还剩数据大小
	qint64 bytesWritten;																							//已发送数据大小
	qint64 loadSize;
	QByteArray outBlock;
};
















#endif // __WAN_SENDFILE_H__

