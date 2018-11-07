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
	//加载套接字库
	WORD wVersionRequested;
	//这个结构被用来存储被WSAStartup函数调用后返回的Windows Sockets数据。它包含Winsock.dll执行的数据。
	WSADATA wsaData;
	int err;
	//声明调用1.1版本的winsock。MAKEWORD(2,2)是2.2版本
	wVersionRequested = MAKEWORD(1, 1);
	//启动WSA，之前client没执行这个就一直报错
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

	//创建套接字
	//SOCKET m_socket=socket(AF_INET,SOCK_STREAM,0);
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		QMessageBox::about(this,"warning","socket Create Failed！");
		return FALSE;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8099);

	err = bind(m_socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));    //绑定本地端口
	if (err == SOCKET_ERROR)
	{
		closesocket(m_socket);
		return FALSE;
	}
	listen(m_socket, 5);//开启监听

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
		//多客户端的方法就每接收到一个accept就在开启一个线程
		//参数赋值
		SocketParam param;
		param.client = &sockConn;
		param.server = this;


		//收到一个请求就开启一个线程处理
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
	//已读的总大小
	long totalBytes = 0;
	int actualSendBytes = 0;
	char infochar[1024];
	//读取文件信息
	SendFileInfo fileInfo;
	QFileInfo info((this->fileName));
	auto qname = info.fileName().toLatin1();
	auto name = qname.data();
	memcpy(fileInfo.name, name, strlen(name));
	fileInfo.size = info.size();

	//结构体转成char[]
	memcpy(infochar, &fileInfo, sizeof(fileInfo));

	QFile file((this->fileName));
	file.open(QFile::ReadOnly);

	QDataStream stream(&file);

	char Buffer[1024];
	DWORD dwNumberOfBytesRead;

	//第一次先发送文件信息
	send(*client, infochar, 1024, NULL);

	do
	{
		//::ReadFile(hFile, Buffer, sizeof(Buffer), &dwNumberOfBytesRead, NULL);
		if (closeFlag) {
			break;
		}
		dwNumberOfBytesRead=stream.readRawData(Buffer, sizeof(Buffer));
		actualSendBytes=::send(*client, Buffer, dwNumberOfBytesRead, 0);

		//防止缓冲区溢出丢失数据
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
	//定义文件对话框类
	QFileDialog *fileDialog = new QFileDialog(this);
	//定义文件对话框标题
	fileDialog->setWindowTitle(tr("Select Send File"));
	//设置默认文件路径
	fileDialog->setDirectory(".");
	//设置文件过滤器
	//fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
	//设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
	//fileDialog->setFileMode(QFileDialog::ExistingFiles);
	//设置视图模式
	fileDialog->setViewMode(QFileDialog::Detail);
	//打印所有选择的文件的路径
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


