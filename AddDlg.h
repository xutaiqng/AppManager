#pragma once


// AddDlg 对话框

class CAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg)

public:
	CAddDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDlg();

// 对话框数据
	enum { IDD = IDD_ADD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
