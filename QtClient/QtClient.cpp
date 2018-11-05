#include "QtClient.h"

struct SendFileInfo {
	char name[40] = { 0 };
	long size;
};
struct ConnectParam {
	DWORD ip;
	int port;
};
QtClient::QtClient(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	Init();
}

void QtClient::Init()
{
	//Build
	auto res = QObject::connect(ui.BuildBtn, SIGNAL(clicked()), this, SLOT(BuildClick()));

	//Stop
	res = QObject::connect(ui.EndBtn, SIGNAL(clicked()), this, SLOT(EndClick()));

	//update bytes
	res = QObject::connect(this, SIGNAL(EmitBytes(int)), this, SLOT(UpdateBytes(int)));
	//finished
	res = QObject::connect(this, SIGNAL(EmitMsg(QString)), this, SLOT(ShowMsg(QString)));

	endFlag = false;
}

int QtClient::InitSocket()
{
	//加载套接字库
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}

	return 1;
}

void QtClient::SocketAccept(DWORD ip,int port)
{

	unsigned long long file_size = 0;

	//加载套接字库
	WORD wVersionRequested;
	//这个结构被用来存储被WSAStartup函数调用后返回的Windows Sockets数据。它包含Winsock.dll执行的数据。
	WSADATA wsaData;
	int err;
	//声明调用1.1版本的winsock。MAKEWORD(2,2)是2.2版本
	wVersionRequested = MAKEWORD(1, 1);
	//
	err = WSAStartup(wVersionRequested, &wsaData);


	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_port = htons(port);
	addrSrv.sin_family = AF_INET;
	//::connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	//recv(sockClient,(char*)&file_size,sizeof(unsigned long long)+1,NULL);

	if (!::connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)))
	{
		//GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T(""));
		//GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T("连接服务器成功！\r\n"));

		QString path=ui.FileText->text();
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}

		SendFileInfo* info;
		char recvBuffer[1024];
		//获取文件信息
		recv(sockClient, recvBuffer, sizeof(recvBuffer), NULL);
		info = (SendFileInfo *)recvBuffer;
		//这里少了一步判断路径有没有/ fix me

		path+=info->name;
		file_size = info->size;
		QFile file(path);
		file.open(QIODevice::WriteOnly);

		unsigned short maxvalue = file_size;    //此处不太稳妥 当数据很大时可能会出现异常
		//m_progress->SetRange(0, maxvalue);

		if (file_size>0)
		{
			/*GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T(""));
			GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T("文件下载到本地 d：\\test.zip \r\n"));*/

			DWORD dwNumberOfBytesRecv = 0;
			DWORD dwCountOfBytesRecv = 0;

			char Buffer[1024];
			
			do
			{
				//结束多线程
				if (endFlag) {
					endFlag = false;
					break;
				}
				//m_progress->SetPos(dwCountOfBytesRecv);//更新进度条

				dwNumberOfBytesRecv = ::recv(sockClient, Buffer, sizeof(Buffer), 0);
				file.write(Buffer, dwNumberOfBytesRecv);
				dwCountOfBytesRecv += dwNumberOfBytesRecv;
				emit EmitBytes(dwCountOfBytesRecv);
			} while ((file_size - dwCountOfBytesRecv) && dwNumberOfBytesRecv>0);


			
			//GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T(""));
			//GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T("文件接收完毕！\r\n"));
			//AfxMessageBox(_T("文件接收完毕!"));//醒目可以注释
			emit EmitMsg("file received");
			
		}
		else
		{
			emit EmitMsg("get file size failed");
			//AfxMessageBox(_T("获取文件总大小失败！"));
		}
		file.close();

	}
	else
	{
		emit EmitMsg("connect server failed");
		//AfxMessageBox(_T("连接服务器失败、请确认IP地址或端口号！"));
	}


	closesocket(sockClient);//关闭套接字

}

void QtClient::BeginSocket()
{
}

DWORD QtClient::ReceiveThread(LPVOID lpParam)
{
	QtClient* param = (QtClient*)lpParam;
	param->SocketAccept(param->ip, param->port);
	return 0;
}

void QtClient::EndClick()
{
	endFlag = true;
}
void QtClient::ShowMsg(QString str)
{
	QMessageBox::about(this, "Information", str);
}
void QtClient::UpdateBytes(int num)
{
	ui.FileSizeTxt->setText(QString("%1").arg(num));
}

void QtClient::BuildClick()
{
	 auto ipchar= ui.ServerIPTxt->text().toLatin1().data();
	 int port = ui.PortEdit->text().toInt();
	 DWORD ip = inet_addr(ipchar);
	 this->ip = ip;
	 this->port = port;
	 HANDLE hThread = CreateThread(NULL, 0, QtClient::ReceiveThread, this, 0, NULL);
}