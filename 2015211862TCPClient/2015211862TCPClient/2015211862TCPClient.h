
// 2015211862TCPClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy2015211862TCPClientApp: 
// �йش����ʵ�֣������ 2015211862TCPClient.cpp
//

class CMy2015211862TCPClientApp : public CWinApp
{
public:
	CMy2015211862TCPClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy2015211862TCPClientApp theApp;