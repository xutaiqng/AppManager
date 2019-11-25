
// AppManagerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CAppManagerDlg �Ի���
class CAppManagerDlg : public CDialogEx
{
// ����
public:
	CAppManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CAppManagerDlg(); //��������

// �Ի�������
	enum { IDD = IDD_APPMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_appList;
	CImageList m_imgList;

private:
	bool m_lvIsChange; //ListView�б������Ƿ��б仯 (���б仯�˳�����ʱ�ὫListView��������д�������ļ�)
	CString m_cfgFileName; //�����ļ���
	
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
