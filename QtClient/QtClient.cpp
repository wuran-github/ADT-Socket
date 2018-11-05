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
	//�����׽��ֿ�
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

	//�����׽��ֿ�
	WORD wVersionRequested;
	//����ṹ�������洢��WSAStartup�������ú󷵻ص�Windows Sockets���ݡ�������Winsock.dllִ�е����ݡ�
	WSADATA wsaData;
	int err;
	//��������1.1�汾��winsock��MAKEWORD(2,2)��2.2�汾
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
		//GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T("���ӷ������ɹ���\r\n"));

		QString path=ui.FileText->text();
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}

		SendFileInfo* info;
		char recvBuffer[1024];
		//��ȡ�ļ���Ϣ
		recv(sockClient, recvBuffer, sizeof(recvBuffer), NULL);
		info = (SendFileInfo *)recvBuffer;
		//��������һ���ж�·����û��/ fix me

		path+=info->name;
		file_size = info->size;
		QFile file(path);
		file.open(QIODevice::WriteOnly);

		unsigned short maxvalue = file_size;    //�˴���̫���� �����ݺܴ�ʱ���ܻ�����쳣
		//m_progress->SetRange(0, maxvalue);

		if (file_size>0)
		{
			/*GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T(""));
			GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T("�ļ����ص����� d��\\test.zip \r\n"));*/

			DWORD dwNumberOfBytesRecv = 0;
			DWORD dwCountOfBytesRecv = 0;

			char Buffer[1024];
			
			do
			{
				//�������߳�
				if (endFlag) {
					endFlag = false;
					break;
				}
				//m_progress->SetPos(dwCountOfBytesRecv);//���½�����

				dwNumberOfBytesRecv = ::recv(sockClient, Buffer, sizeof(Buffer), 0);
				file.write(Buffer, dwNumberOfBytesRecv);
				dwCountOfBytesRecv += dwNumberOfBytesRecv;
				emit EmitBytes(dwCountOfBytesRecv);
			} while ((file_size - dwCountOfBytesRecv) && dwNumberOfBytesRecv>0);


			
			//GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T(""));
			//GetDlgItem(IDC_SHOWINFO)->SetWindowText(_T("�ļ�������ϣ�\r\n"));
			//AfxMessageBox(_T("�ļ��������!"));//��Ŀ����ע��
			emit EmitMsg("file received");
			
		}
		else
		{
			emit EmitMsg("get file size failed");
			//AfxMessageBox(_T("��ȡ�ļ��ܴ�Сʧ�ܣ�"));
		}
		file.close();

	}
	else
	{
		emit EmitMsg("connect server failed");
		//AfxMessageBox(_T("���ӷ�����ʧ�ܡ���ȷ��IP��ַ��˿ںţ�"));
	}


	closesocket(sockClient);//�ر��׽���

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