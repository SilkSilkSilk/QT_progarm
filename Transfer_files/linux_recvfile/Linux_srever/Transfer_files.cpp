#include "unp.h"
#include <iostream>

const char* REQUEST = "request";
const char* RESPONSE = "response";
#define LOADSIZE 4*1024

int response(int con_fd);
int recvfile(int con_fd);

int main(int argc, char *argv[], char *envp[])
{
	int listen_fd;																									//�����׽���
	int con_fd;																										//�ͻ��׽���
	int on = 1;																										//enable

	struct sockaddr_in addr;																						//��������ip�Ͷ˿�
	bzero(&addr, sizeof(addr));
	struct sockaddr_in clienaddr;																					//accecp���Ŀͻ�ip�Ͷ˿�
	struct sockaddr_in serveraddr;
	bzero(&clienaddr, sizeof(clienaddr));
	bzero(&serveraddr, sizeof(serveraddr));
	socklen_t clien_len = sizeof(clienaddr);
	socklen_t server_len = sizeof(serveraddr);

	listen_fd = Socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(50000);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
//	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);

	Setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));												//����ip��ַ

	Bind(listen_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

	Listen(listen_fd, SOMAXCONN);																					//listen�����������г���

	Getsockname(listen_fd, reinterpret_cast<struct sockaddr*>(&serveraddr), &server_len);							//��ȡ�����������ַ�ṹ
	printf("server ip is:%s		 port is:%d\n", inet_ntoa(serveraddr.sin_addr), ntohs(serveraddr.sin_port));		//��ӡ����ip�Ͷ˿�

	con_fd = Accept(listen_fd, reinterpret_cast<sockaddr*>(&clienaddr), &clien_len);
	printf("client ip is:%s		 port is:%d\n", inet_ntoa(clienaddr.sin_addr), ntohs(clienaddr.sin_port));			//��ӡ���Ӷοͻ�ip�Ͷ˿�

	if(response(con_fd) == 0)																						//��Ӧresponse
	{
		exit(1);
	}

	recvfile(con_fd);

	exit(0);
}

int response(int con_fd)
{
	int ret;
	char buf[1024];

	memset(buf, 0, sizeof(buf));

	while (true)
	{
		if ((ret = read(con_fd, buf, sizeof(buf))) < 0)													
			err_sys("Readline error");
		else if (ret == 0)																					
		{
			printf("client close!\n");
			Close(con_fd);
			return 0;
		}

		if (memcmp(REQUEST, buf, strlen(REQUEST)) == 0)
		{
			printf("get %s\n", buf);
			strcpy(buf, RESPONSE);
			writen(con_fd, buf, strlen(buf));
			break;
		}
		else
		{
			printf("get server data but not RESPONSE data is:%s\n", buf);
		}
	}
	return 1;
}

int recvfile(int con_fd)
{
	int ret;
	char buf[1024];
	char *p1;
	u_int64_t filesize;
	char filename[40];
	char loadbuf[LOADSIZE];
	u_int64_t nread;
	int file_fd;

	memset(buf, 0, sizeof(buf));

	if ((ret = Readline(con_fd, buf, sizeof(buf))) < 0)
		err_sys("Readline error");
	else if (ret == 0)
	{
		printf("client close!\n");
		Close(con_fd);
		return 0;
	}

//	p1 = strchr(buf, ' ');
//	*p1++ = '\0';
//
//	filesize = atoi(buf);
//	strcpy(filename, p1);
	sscanf(buf, "%d %s", &filesize, filename);
	printf("filename:%s filesize=%ld\n", filename, filesize);

	if (access(filename, F_OK) == 0) 		    																	//�ж��ļ��Ƿ��Ѵ���
	{
		unlink(filename);																							//ɾ���ļ�
	}

	if ((file_fd = open(filename, O_CREAT | O_EXCL | O_RDWR, 0666)) < 0)
	{
		err_sys("open file");
	}

	nread = filesize;
	printf("download %.1f%\n", (static_cast<float>(filesize - nread) / filesize) * 100);

	while (nread > 0)
	{
		if ((ret = readn(con_fd, loadbuf, MIN(LOADSIZE, nread))) < 0)
			err_sys("readn error");
		else if (ret == 0)
		{
			printf("client close!\n");
			Close(con_fd);
			return 0;
		}
		else if(ret != MIN(LOADSIZE, nread))
		{
			printf("readn has:%d size not %d\n", ret, MIN(LOADSIZE, nread));
		}

		write(file_fd, loadbuf, ret);
		nread -= ret;

		printf("download %.1f%\n", (static_cast<float>(filesize - nread) / filesize) * 100);
	}
	printf("recvfile file [%s] success\n", filename);

	return 1;
}