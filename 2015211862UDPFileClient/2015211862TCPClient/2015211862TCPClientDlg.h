
// 2015211862TCPClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CMy2015211862TCPClientDlg �Ի���
class CMy2015211862TCPClientDlg : public CDialogEx
{
// ����
public:
	CMy2015211862TCPClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY2015211862TCPCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	CIPAddressCtrl ServerIP;
	CString m_strSendMessage;
	CString m_strRecvMessage;
	afx_msg void OnBnClickedButtonSend();
};
