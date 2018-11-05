#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtClient.h"
#include <qfile.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <QtWidgets\QMessageBox>
#include <Windows.h>
class QtClient : public QMainWindow
{
	Q_OBJECT

public:
	QtClient(QWidget *parent = Q_NULLPTR);
	void Init();
	int InitSocket();
	void SocketAccept(DWORD ip, int port);
	static DWORD WINAPI ReceiveThread(LPVOID lpParam);
signals:
	void EmitBytes(int num);
signals:
	void EmitMsg(QString str);
public slots:
	void BuildClick();
	void EndClick();
	void ShowMsg(QString str);
	void UpdateBytes(int num);
	void BeginSocket();
private:
	DWORD ip;
	int port;
	bool endFlag;
	Ui::QtClientClass ui;
};
