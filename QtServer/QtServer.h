#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtServer.h"
#include <Windows.h>
class QtServer : public QMainWindow
{
	Q_OBJECT

public:
	QtServer(QWidget *parent = Q_NULLPTR);
	void Init();
	int InitSocket();
	void SocketAccept();
	void SendFile(SOCKET* client);
	static DWORD WINAPI AcceptThread(LPVOID lpParam);
	static DWORD WINAPI SendThread(LPVOID lpParam);
private:
	void GetFileBytes(QString str);
public slots:
	void OpenFileDialog();
	void UpdateListenLabel();
	void CloseSocket();
	void BeginSocket();
signals:
	void EmitStartListen();
	void EmitClose();

public:
	QString  fileName;
private:
	Ui::QtServerClass ui;
	SOCKET m_socket;
	bool closeFlag;
	//client num
	int threadNum;
	
};
