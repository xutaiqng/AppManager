
// AppManagerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CAppManagerDlg 对话框
class CAppManagerDlg : public CDialogEx
{
// 构造
public:
	CAppManagerDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CAppManagerDlg(); //析构函数

// 对话框数据
	enum { IDD = IDD_APPMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_appList;
	CImageList m_imgList;

private:
	bool m_lvIsChange; //ListView列表数据是否有变化 (如有变化退出程序时会将ListView数据重新写入配置文件)
	CString m_cfgFileName; //配置文件名
	
	void CreateItem(CString sName, CString sPath, CString sParam);
	void CreateJumpList();
	void DeleteJumpList();
	HRESULT _CreateShellLink(PCWSTR pszTitle, PCWSTR pszArguments, PCWSTR pszAppPath, IShellLink **ppsl);
	HRESULT _AddTasksToList(ICustomDestinationList *pcdl);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
	afx_msg void OnDblclkListApp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnModify();
};
