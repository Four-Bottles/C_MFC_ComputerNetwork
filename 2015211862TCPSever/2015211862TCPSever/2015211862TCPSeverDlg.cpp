
// 2015211862TCPSeverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "2015211862TCPSever.h"
#include "2015211862TCPSeverDlg.h"
#include "afxdialogex.h"

#define DEFAULT_PORT 2000//�����������˿�
SOCKET m_Listening;//����socket
HANDLE m_hListenThread;//�߳̾��

char * CSt2char(CString str)
{
	//ע�⣺����n��len��ֵ��С��ͬ,n�ǰ��ַ�����ģ�len�ǰ��ֽڼ����
	int n = str.GetLength(); // n = 14, len = 18
	//��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
	int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	char * buff = new char[len + 1]; //���ֽ�Ϊ��λ
	//���ֽڱ���ת���ɶ��ֽڱ���
	WideCharToMultiByte(CP_ACP, 0, str, str.GetLength() + 1, buff, len + 1, NULL, NULL);
	buff[len + 1] = '\0'; //���ֽ��ַ���'/0'����
	return buff;
}
CString char2CSt(char * szBuf)
{
	//����char *�����С�����ֽ�Ϊ��λ��һ������ռ�����ֽ�
	int charLen = strlen(szBuf);
	//������ֽ��ַ��Ĵ�С�����ַ����㡣
	int len = MultiByteToWideChar(CP_ACP, 0, szBuf, charLen, NULL, 0);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ķ��ֽ��ַ���С
	TCHAR *buf = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP, 0, szBuf, charLen, buf, len);
	buf[len] = '\0'; //����ַ�����β��ע�ⲻ��len+1
	//��TCHAR����ת��ΪCString
	CString pWideChar;
	pWideChar.Append(buf);
	//ɾ��������
	delete[]buf;
	return pWideChar;
}
DWORD WINAPI ListenThread(LPVOID lpparam)
{
	CMy2015211862TCPSeverDlg* pDlg = (CMy2015211862TCPSeverDlg*)lpparam;
	if (pDlg == NULL) return 0;//��ȡ��ǰ���Ի����ָ��
	SOCKET ConnectSocket;//��¼��׽��������
	sockaddr_in ClientAddr;//��¼���ݰ���IP��ַ
	int nLen = sizeof(sockaddr);
	if (listen(m_Listening, 40) == SOCKET_ERROR)//��ʼ�����Ƿ��пͻ������ӡ�
	{
		return 0;
	}
	char szBuf[MAX_PATH];//���ݻ�����
	memset(szBuf, 0, MAX_PATH);//��ʼ��������
	while (1)
	{
		ConnectSocket = accept(m_Listening, (sockaddr*)&ClientAddr, &nLen);//����ֱ���пͻ�����
	    //	�ӣ���Ȼ���˷�CPU��Դ
		char *pAddrname = inet_ntoa(ClientAddr.sin_addr);//�õ��ͻ��˵�IP��ַ
		memset(szBuf, 0, MAX_PATH);//ÿ�ν�������ǰ��ջ�����
		if (recv(ConnectSocket, szBuf, sizeof szBuf, 0) != SOCKET_ERROR)
		{
			pDlg->m_edtMessage.SetWindowTextW(char2CSt(szBuf));
			char buff[MAX_PATH] = "���ǿ�̽�ӣ����ǿ�̽�ӣ��յ���ش�(";
			strcat_s(buff, pAddrname);
			strcat_s(buff, "��");
			send(ConnectSocket, buff, sizeof buff, 0);
		}
	}
	return 0;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMy2015211862TCPSeverDlg �Ի���



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


// CMy2015211862TCPSeverDlg ��Ϣ�������

BOOL CMy2015211862TCPSeverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	WSADATA wsaData;//�ṹ���������溯��WSAStartup���ص�Windows Socket ��ʼ����Ϣ
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		AfxMessageBox(_T("WSAStartup error��"));
		return FALSE;
	}//��һ��Ӧ�ó������WSAStartup����ʱ������ϵͳ���������Socket�汾��������Ӧ��Socket
	    //�⣬Ȼ����ҵ���Socket�⵽��Ӧ�ó�����
		//��ʾҪʹ��1.1�汾��Socket
	GetLocalIP();//��ȡIP ��ַ

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy2015211862TCPSeverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy2015211862TCPSeverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy2015211862TCPSeverDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	WSACleanup();//�ͷ���Դ
	CDialogEx::OnCancel();
}


void CMy2015211862TCPSeverDlg::GetLocalIP()
{
	char HostName[128];//��¼������
	char *IPAddress;//��¼IP��ַ
	if (gethostname(HostName, 128) == 0)//��ȡ�������ɹ�
	{
		struct hostent *pHost;
		pHost = gethostbyname(HostName);//����������������ȡIP��ַ
		IPAddress = pHost->h_addr_list[0];//ȡ��һ����ַ
		HostIP.SetAddress(*IPAddress, *(IPAddress + 1), *(IPAddress + 2), *(IPAddress + 3));//����IP
		    //��ַ
		UpdateData(FALSE);//ˢ�½���
	}
}


void CMy2015211862TCPSeverDlg::OnBnClickedButtonStart()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	sockaddr_in local;//ʹ��sockaddr_in�ṹָ��IP��ַ�Ͷ˿���Ϣ
	local.sin_family = AF_INET;
	local.sin_port = htons(DEFAULT_PORT);//���õĶ˿�ΪDEFAULT_PORT�������ͱ�����������
	    //��˳��ת��������ֽ�˳��
	char IPAddress[MAX_PATH];//��¼IP��ַ
	BYTE add1, add2, add3, add4;
	HostIP.GetAddress(add1, add2, add3, add4);
	sprintf_s(IPAddress, "%d.%d.%d.%d", add1, add2, add3, add4);
	local.sin_addr.S_un.S_addr = inet_addr(IPAddress);//��һ�����ʮ���Ƶ�IPת����һ����������
	    //��(u_long����), ���ص�������ʽ�������ֽ���
	m_Listening = socket(AF_INET, SOCK_STREAM, 0);//��ʼ��Socket������TCP��ʽ-SOCK_STREAM
	if (m_Listening == INVALID_SOCKET)//����ʧ��
	{
		AfxMessageBox(_T("��ʼ��socketʧ�ܣ�"));
		return;
	}
	if (bind(m_Listening, (LPSOCKADDR)& local, sizeof(local)) == SOCKET_ERROR)//��IP��ַ�Ͷ�
		//�ڰ󶨵����������׽�����
	{
		AfxMessageBox(_T("��ʧ�ܣ�"));
		closesocket(m_Listening);
		return;
	}
		//���������߳�
	DWORD dwThreadID = 0;
	m_hListenThread = ::CreateThread(NULL, 0, ListenThread, this, 0, &dwThreadID);//ListenThreadΪ
	    //�̺߳�����thisΪ��ǰ���Ի����ָ��
	m_btnStart.EnableWindow(FALSE);
	m_btnStop.EnableWindow(TRUE);
}


void CMy2015211862TCPSeverDlg::OnBnClickedButtonStop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TerminateThread(m_hListenThread, 0);//�ر��߳�
	CloseHandle(m_hListenThread);//�ͷ���Դ
	shutdown(m_Listening, 2);//�رն�дͨ��
	closesocket(m_Listening);//�ͷ�socket
	m_btnStart.EnableWindow(TRUE);
	m_btnStop.EnableWindow(FALSE);
}
