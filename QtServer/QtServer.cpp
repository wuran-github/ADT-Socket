#include "QtServer.h"
#include <QtWidgets/QFileDialog>
#include <qfile.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <QtWidgets\QMessageBox>

struct SocketParam
{
	QtServer* server;
	SOCKET* client;
};
struct SendFileInfo {
	char name[40] = { 0 };
	long size;
};
QtServer::QtServer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	Init();
}

void QtServer::Init()
{
	//File
	auto res=QObject::connect(ui.FileBtn, SIGNAL(clicked()), this, SLOT(OpenFileDialog()));

	//listen
	res = QObject::connect(ui.ListenBtn, SIGNAL(clicked()), this,SLOT(BeginSocket()));

	//stop
	res = QObject::connect(ui.SendBtn, SIGNAL(clicked()), this, SLOT(CloseSocket()));
	closeFlag = false;

	//getIP
	GetIps();
	if (ips.size() > 0) {
		ui.IPTxt->setText(ips[0]);
	}
}


void QtServer::BeginSocket() {
	InitSocket();
	UpdateListenLabel();
	HANDLE hThread = CreateThread(NULL, 0, QtServer::AcceptThread, this, 0, NULL);
}

DWORD QtServer::AcceptThread(LPVOID lpParam)
{
	QtServer* p = (QtServer*)lpParam;
	p->SocketAccept();
	return 0;
}
DWORD QtServer::SendThread(LPVOID lpParam)
{
	SocketParam* param = (SocketParam*)lpParam;
	param->server->SendFile(param->client);
	return 0;
}
int QtServer::InitSocket()
{
	//�����׽��ֿ�
	WORD wVersionRequested;
	//����ṹ�������洢��WSAStartup�������ú󷵻ص�Windows Sockets���ݡ�������Winsock.dllִ�е����ݡ�
	WSADATA wsaData;
	int err;
	//��������1.1�汾��winsock��MAKEWORD(2,2)��2.2�汾
	wVersionRequested = MAKEWORD(1, 1);
	//����WSA��֮ǰclientûִ�������һֱ����
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return FALSE;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return FALSE;
	}

	//�����׽���
	//SOCKET m_socket=socket(AF_INET,SOCK_STREAM,0);
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		QMessageBox::about(this,"warning","socket Create Failed��");
		return FALSE;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8099);

	err = bind(m_socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));    //�󶨱��ض˿�
	if (err == SOCKET_ERROR)
	{
		closesocket(m_socket);
		return FALSE;
	}
	listen(m_socket, 5);//��������

	return 1;
}


void QtServer::SocketAccept()
{
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (true)
	{
		SOCKET sockConn = accept(m_socket, (SOCKADDR*)&addrClient, &len);

		if (sockConn == INVALID_SOCKET) {
			wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
			closesocket(m_socket);
			WSACleanup();
			return ;
		}
		//��ͻ��˵ķ�����ÿ���յ�һ��accept���ڿ���һ���߳�
		//������ֵ
		SocketParam param;
		param.client = &sockConn;
		param.server = this;


		//�յ�һ������Ϳ���һ���̴߳���
		HANDLE hThread = CreateThread(NULL, 0, QtServer::SendThread, &param, 0, NULL);


	
	}

	
}

void QtServer::GetIps()
{
	for each (QHostAddress address in QNetworkInterface().allAddresses())
	{
		if (address.protocol() == QAbstractSocket::IPv4Protocol)
		{
			QString ip = address.toString();
			ips.push_back(ip);
		}
	}
}

void QtServer::SendFile(SOCKET* client) {
	//�Ѷ����ܴ�С
	long totalBytes = 0;
	int actualSendBytes = 0;
	char infochar[1024];
	//��ȡ�ļ���Ϣ
	SendFileInfo fileInfo;
	QFileInfo info((this->fileName));
	auto qname = info.fileName().toLatin1();
	auto name = qname.data();
	memcpy(fileInfo.name, name, strlen(name));
	fileInfo.size = info.size();

	//�ṹ��ת��char[]
	memcpy(infochar, &fileInfo, sizeof(fileInfo));

	QFile file((this->fileName));
	file.open(QFile::ReadOnly);

	QDataStream stream(&file);

	char Buffer[1024];
	DWORD dwNumberOfBytesRead;

	//��һ���ȷ����ļ���Ϣ
	send(*client, infochar, 1024, NULL);

	do
	{
		//::ReadFile(hFile, Buffer, sizeof(Buffer), &dwNumberOfBytesRead, NULL);
		if (closeFlag) {
			break;
		}
		dwNumberOfBytesRead=stream.readRawData(Buffer, sizeof(Buffer));
		actualSendBytes=::send(*client, Buffer, dwNumberOfBytesRead, 0);

		//��ֹ�����������ʧ����
		totalBytes += actualSendBytes;
		stream.device()->seek(totalBytes);
	} while (dwNumberOfBytesRead && actualSendBytes>0);
	
	file.close();
}
void QtServer::GetFileBytes(QString str)
{
	QFileInfo info(str);
	if (info.exists()) {
		if (info.isFile()) {
			ui.FileSizeTxt->setText(QString("%1").arg(info.size()));
		}
	}

}
void QtServer::UpdateListenLabel()
{
	ui.ConnectTxt->setText("listening...");
}
void QtServer::CloseSocket()
{
	this->closeFlag = true;
	closesocket(this->m_socket);
	WSACleanup();
	ui.ConnectTxt->setText("closed...");
	closeFlag = false;
}
void QtServer::OpenFileDialog() {
	//�����ļ��Ի�����
	QFileDialog *fileDialog = new QFileDialog(this);
	//�����ļ��Ի������
	fileDialog->setWindowTitle(tr("Select Send File"));
	//����Ĭ���ļ�·��
	fileDialog->setDirectory(".");
	//�����ļ�������
	//fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
	//���ÿ���ѡ�����ļ�,Ĭ��Ϊֻ��ѡ��һ���ļ�QFileDialog::ExistingFiles
	//fileDialog->setFileMode(QFileDialog::ExistingFiles);
	//������ͼģʽ
	fileDialog->setViewMode(QFileDialog::Detail);
	//��ӡ����ѡ����ļ���·��
	QStringList fileNames;
	if (fileDialog->exec())
	{
		fileNames = fileDialog->selectedFiles();
	}

	if (fileNames.size() != 0) {
		ui.FileText->setText(fileNames[0]);
		this->GetFileBytes(fileNames[0]);
		this->fileName = fileNames[0];
	}
	
}


