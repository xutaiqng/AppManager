// AddDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AppManager.h"
#include "AddDlg.h"
#include "afxdialogex.h"


// AddDlg 对话框

IMPLEMENT_DYNAMIC(CAddDlg, CDialogEx)

CAddDlg::CAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddDlg::IDD, pParent)
	, m_appName(_T(""))
	, m_appPath(_T(""))
	, m_opType(0)
{

}

CAddDlg::~CAddDlg()
{
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_appName);
	DDX_Text(pDX, IDC_EDIT_PATH, m_appPath);
	DDX_Text(pDX, IDC_EDIT_PARAM, m_appParam);
}


BEGIN_MESSAGE_MAP(CAddDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CAddDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDOK, &CAddDlg::OnBnClickedOk)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// AddDlg 消息处理程序


void CAddDlg::OnBnClickedBtnOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *tmp = GetDlgItem(IDC_EDIT_PATH);
	GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);
	//将各控件中的数据保存到相应的变量
	UpdateData(TRUE);
	CFileDialog fileDlg(TRUE, NULL, NULL, 0, L"可执行程序(*.exe)|*.exe|所有文件(*.*)|*.*|");
	if ( IDOK == fileDlg.DoModal() )
	{
		m_appPath = fileDlg.GetPathName();
		if (m_appName.IsEmpty())
		{
			int iPos = m_appPath.ReverseFind('\\');
			m_appName = m_appPath.Mid(iPos + 1);
		}
	}
	//根据各变量的值更新相应的控件
	UpdateData(FALSE);
}


void CAddDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_appName.IsEmpty())
	{
		MessageBox(L"请输入显示名称", L"提示");
		return;
	}
	if (m_appPath.IsEmpty())
	{
		MessageBox(L"请输入程序路径", L"提示");
		return;
	}
	if (!PathFileExists((LPTSTR)(LPCTSTR)m_appPath))
	{
		MessageBox(L"指定的文件不存在", L"提示");
		return;
	}
	CDialogEx::OnOK();
}


int CAddDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CenterWindow();

	return 0;
}

BOOL CAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (1 == m_opType) //修改数据时不允许修改路径，所以禁用这两个控件
	{
		SetWindowText(L"修改应用程序");
		CEdit *pEditPath = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
		pEditPath->SetReadOnly(TRUE);
		GetDlgItem(IDC_BTN_OPEN)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
