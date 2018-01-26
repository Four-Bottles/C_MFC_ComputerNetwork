
// 2015211862TCPClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "2015211862TCPClient.h"
#include "2015211862TCPClientDlg.h"
#include "afxdialogex.h"

#define DEFAULT_PORT 2000//�����������˿ڣ�ע����������˱���һ��

#define MAX_LENGTH 1024*100//ÿ�ζ�ȡ�����ݿ��С

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


// CMy2015211862TCPClientDlg �Ի���



CMy2015211862TCPClientDlg::CMy2015211862TCPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy2015211862TCPClientDlg::IDD, pParent)
	, m_strSendMessage(_T("�о������п�̽�ӣ��о������п�̽�ӣ���ش�.."))
	, m_strRecvMessage(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy2015211862TCPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, ServerIP);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strSendMessage);
	DDX_Text(pDX, IDC_STATIC_RECV, m_strRecvMessage);
}

BEGIN_MESSAGE_MAP(CMy2015211862TCPClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMy2015211862TCPClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMy2015211862TCPClientDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CMy2015211862TCPClientDlg ��Ϣ�������

BOOL CMy2015211862TCPClientDlg::OnInitDialog()
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

	WSADATA wsaData;//�ṹ���������溯��WSAStartup ���ص�Windows Socket ��
	    //ʼ����Ϣ
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		AfxMessageBox(_T("WSAStartup error��"));
		return FALSE;
	}   //��һ��Ӧ�ó������WSAStartup ����ʱ������ϵͳ���������Socket �汾����
	    //����Ӧ��Socket �⣬Ȼ����ҵ���Socket �⵽��Ӧ�ó�����

	ServerIP.SetAddress(127, 0, 0, 1);
	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMy2015211862TCPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMy2015211862TCPClientDlg::OnPaint()
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
HCURSOR CMy2015211862TCPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy2015211862TCPClientDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	WSACleanup();//�ͷ���Դ
	CDialogEx::OnCancel();
}


void CMy2015211862TCPClientDlg::OnBnClickedButtonSend()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	sockaddr_in local;
	local.sin_family = AF_INET;//������AF_INET,��ʾ��socket��Internet���н���ͨ��
	local.sin_port = htons(DEFAULT_PORT);//�˿ں�
	char IPAddress[MAX_PATH];
	BYTE add1, add2, add3, add4;
	ServerIP.GetAddress(add1, add2, add3, add4);
	sprintf_s(IPAddress, "%d.%d.%d.%d", add1, add2, add3, add4);
	local.sin_addr.S_un.S_addr = inet_addr(IPAddress);//��������IP��ַ��
	SOCKET socketTmp = socket(AF_INET, SOCK_STREAM, 0);///��ʼ��Socket����������˱���һ��
	if (connect(socketTmp, (LPSOCKADDR)& local, sizeof(local)) != 0)//���ӷ�����
	{
		closesocket(socketTmp);
		AfxMessageBox(_T("���ӷ�����ʧ��"));
		return;
	}

	//��ȡPPT�ļ�
	FILE *fp = NULL;
	if (fopen_s(&fp, "test3.ppt", "wb") != 0)
	{
		AfxMessageBox(_T("Open PPT File Failed2!"));
		return;
	}
	int len = 0;//���յ������ݳ���
	int i = 1;
	char dataBuff[MAX_LENGTH];//���ݻ�����

	while (1)
	{
		memset(dataBuff, 0, MAX_LENGTH);
		len = recv(socketTmp, dataBuff, sizeof(dataBuff), 0);
		if (len <= 0)
		{
			//AfxMessageBox(_T("Recv error!"));

			shutdown(socketTmp, 2);//�رն�дͨ��
			closesocket(socketTmp);
			break;
		}
		else
			fwrite(dataBuff, 1, len, fp);

		Sleep(10);
	}
	fclose(fp);
	AfxMessageBox(_T("Recv finished!"));

	/*
	char szText[MAX_PATH];//���ݻ�����
	memset(szText, 0, MAX_PATH);
	UpdateData(TRUE);
	sprintf_s(szText, "%s", CSt2char(m_strSendMessage));
	send(socketTmp, szText, sizeof szText, 0);//������Ϣ
	memset(szText, 0, MAX_PATH);//��ջ�����
	if (recv(socketTmp, szText, sizeof szText, 0) != SOCKET_ERROR)//��ȡ�������˷��ص�����
	{
		m_strRecvMessage = char2CSt(szText);
		UpdateData(FALSE);//ˢ�½���
	}
	*/
	shutdown(socketTmp, 2);//�رն�дͨ��
	closesocket(socketTmp);
}
