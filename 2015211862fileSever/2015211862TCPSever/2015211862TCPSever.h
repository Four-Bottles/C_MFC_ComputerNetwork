
// 2015211862TCPSever.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy2015211862TCPSeverApp: 
// �йش����ʵ�֣������ 2015211862TCPSever.cpp
//

class CMy2015211862TCPSeverApp : public CWinApp
{
public:
	CMy2015211862TCPSeverApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy2015211862TCPSeverApp theApp;