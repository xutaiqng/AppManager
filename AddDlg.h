#pragma once


// AddDlg �Ի���

class CAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg)

public:
	CAddDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddDlg();

// �Ի�������
	enum { IDD = IDD_ADD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedOk();
	CString m_appName;
	CString m_appPath;
	CString m_appParam;
	UINT    m_opType;

public:
	virtual BOOL OnInitDialog();
};
