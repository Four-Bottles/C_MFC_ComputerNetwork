
// 2015211862TCPSeverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "2015211862TCPSever.h"
#include "2015211862TCPSeverDlg.h"
#include "afxdialogex.h"

#define DEFAULT_PORT 2000//服务器监听端口
SOCKET m_Listening;//监听socket
HANDLE m_hListenThread;//线程句柄

char * CSt2char(CString str)
{
	//注意：以下n和len的值大小不同,n是按字符计算的，len是按字节计算的
	int n = str.GetLength(); // n = 14, len = 18
	//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL);
	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * buff = new char[len + 1]; //以字节为单位
	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP, 0, str, str.GetLength() + 1, buff, len + 1, NULL, NULL);
	buff[len + 1] = '\0'; //多字节字符以'/0'结束
	return buff;
}
CString char2CSt(char * szBuf)
{
	//计算char *数组大小，以字节为单位，一个汉字占两个字节
	int charLen = strlen(szBuf);
	//计算多字节字符的大小，按字符计算。
	int len = MultiByteToWideChar(CP_ACP, 0, szBuf, charLen, NULL, 0);
	//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
	TCHAR *buf = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码
	MultiByteToWideChar(CP_ACP, 0, szBuf, charLen, buf, len);
	buf[len] = '\0'; //添加字符串结尾，注意不是len+1
	//将TCHAR数组转换为CString
	CString pWideChar;
	pWideChar.Append(buf);
	//删除缓冲区
	delete[]buf;
	return pWideChar;
}
DWORD WINAPI ListenThread(LPVOID lpparam)
{
	CMy2015211862TCPSeverDlg* pDlg = (CMy2015211862TCPSeverDlg*)lpparam;
	if (pDlg == NULL) return 0;//获取当前主对话框的指针
	SOCKET ConnectSocket;//记录捕捉到的连接
	sockaddr_in ClientAddr;//记录数据包的IP地址
	int nLen = sizeof(sockaddr);
	if (listen(m_Listening, 40) == SOCKET_ERROR)//开始监听是否有客户端连接。
	{
		return 0;
	}
	char szBuf[MAX_PATH];//数据缓冲区
	memset(szBuf, 0, MAX_PATH);//初始化缓冲区
	while (1)
	{
		ConnectSocket = accept(m_Listening, (sockaddr*)&ClientAddr, &nLen);//阻塞直到有客户端连
	    //	接，不然多浪费CPU资源
		char *pAddrname = inet_ntoa(ClientAddr.sin_addr);//得到客户端的IP地址
		memset(szBuf, 0, MAX_PATH);//每次接收数据前清空缓冲区
		if (recv(ConnectSocket, szBuf, sizeof szBuf, 0) != SOCKET_ERROR)
		{
			pDlg->m_edtMessage.SetWindowTextW(char2CSt(szBuf));
			char buff[MAX_PATH] = "我是勘探队，我是勘探队，收到请回答！(";
			strcat_s(buff, pAddrname);
			strcat_s(buff, "）");
			send(ConnectSocket, buff, sizeof buff, 0);
		}
	}
	return 0;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy2015211862TCPSeverDlg 对话框



CMy2015211862TCPSeverDlg::CMy2015211862TCPSeverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy2015211862TCPSeverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy2015211862TCPSeverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_HOST, HostIP);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_edtMessage);
}

BEGIN_MESSAGE_MAP(CMy2015211862TCPSeverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMy2015211862TCPSeverDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CMy2015211862TCPSeverDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMy2015211862TCPSeverDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CMy2015211862TCPSeverDlg 消息处理程序

BOOL CMy2015211862TCPSeverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	WSADATA wsaData;//结构被用来保存函数WSAStartup返回的Windows Socket 初始化信息
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		AfxMessageBox(_T("WSAStartup error！"));
		return FALSE;
	}//当一个应用程序调用WSAStartup函数时，操作系统根据请求的Socket版本来搜索相应的Socket
	    //库，然后绑定找到的Socket库到该应用程序中
		//表示要使用1.1版本的Socket
	GetLocalIP();//获取IP 地址

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy2015211862TCPSeverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy2015211862TCPSeverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy2015211862TCPSeverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy2015211862TCPSeverDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	WSACleanup();//释放资源
	CDialogEx::OnCancel();
}


void CMy2015211862TCPSeverDlg::GetLocalIP()
{
	char HostName[128];//记录主机名
	char *IPAddress;//记录IP地址
	if (gethostname(HostName, 128) == 0)//获取主机名成功
	{
		struct hostent *pHost;
		pHost = gethostbyname(HostName);//用域名或主机名获取IP地址
		IPAddress = pHost->h_addr_list[0];//取第一个地址
		HostIP.SetAddress(*IPAddress, *(IPAddress + 1), *(IPAddress + 2), *(IPAddress + 3));//设置IP
		    //地址
		UpdateData(FALSE);//刷新界面
	}
}


void CMy2015211862TCPSeverDlg::OnBnClickedButtonStart()
{
	// TODO:  在此添加控件通知处理程序代码
	sockaddr_in local;//使用sockaddr_in结构指定IP地址和端口信息
	local.sin_family = AF_INET;
	local.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT。将整型变量从主机字
	    //节顺序转变成网络字节顺序
	char IPAddress[MAX_PATH];//记录IP地址
	BYTE add1, add2, add3, add4;
	HostIP.GetAddress(add1, add2, add3, add4);
	sprintf_s(IPAddress, "%d.%d.%d.%d", add1, add2, add3, add4);
	local.sin_addr.S_un.S_addr = inet_addr(IPAddress);//将一个点分十进制的IP转换成一个长整数型
	    //数(u_long类型), 返回的整数形式是网络字节序
	m_Listening = socket(AF_INET, SOCK_STREAM, 0);//初始化Socket，采用TCP方式-SOCK_STREAM
	if (m_Listening == INVALID_SOCKET)//创建失败
	{
		AfxMessageBox(_T("初始化socket失败！"));
		return;
	}
	if (bind(m_Listening, (LPSOCKADDR)& local, sizeof(local)) == SOCKET_ERROR)//将IP地址和端
		//口绑定到所创建的套接字上
	{
		AfxMessageBox(_T("绑定失败！"));
		closesocket(m_Listening);
		return;
	}
		//创建监听线程
	DWORD dwThreadID = 0;
	m_hListenThread = ::CreateThread(NULL, 0, ListenThread, this, 0, &dwThreadID);//ListenThread为
	    //线程函数，this为当前主对话框的指针
	m_btnStart.EnableWindow(FALSE);
	m_btnStop.EnableWindow(TRUE);
}


void CMy2015211862TCPSeverDlg::OnBnClickedButtonStop()
{
	// TODO:  在此添加控件通知处理程序代码
	TerminateThread(m_hListenThread, 0);//关闭线程
	CloseHandle(m_hListenThread);//释放资源
	shutdown(m_Listening, 2);//关闭读写通道
	closesocket(m_Listening);//释放socket
	m_btnStart.EnableWindow(TRUE);
	m_btnStop.EnableWindow(FALSE);
}
