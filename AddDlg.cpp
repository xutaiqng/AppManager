// AddDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AppManager.h"
#include "AddDlg.h"
#include "afxdialogex.h"


// AddDlg �Ի���

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


// AddDlg ��Ϣ�������


void CAddDlg::OnBnClickedBtnOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWnd *tmp = GetDlgItem(IDC_EDIT_PATH);
	GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);
	//�����ؼ��е����ݱ��浽��Ӧ�ı���
	UpdateData(TRUE);
	CFileDialog fileDlg(TRUE, NULL, NULL, 0, L"��ִ�г���(*.exe)|*.exe|�����ļ�(*.*)|*.*|");
	if ( IDOK == fileDlg.DoModal() )
	{
		m_appPath = fileDlg.GetPathName();
		if (m_appName.IsEmpty())
		{
			int iPos = m_appPath.ReverseFind('\\');
			m_appName = m_appPath.Mid(iPos + 1);
		}
	}
	//���ݸ�������ֵ������Ӧ�Ŀؼ�
	UpdateData(FALSE);
}


void CAddDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_appName.IsEmpty())
	{
		MessageBox(L"��������ʾ����", L"��ʾ");
		return;
	}
	if (m_appPath.IsEmpty())
	{
		MessageBox(L"���������·��", L"��ʾ");
		return;
	}
	if (!PathFileExists((LPTSTR)(LPCTSTR)m_appPath))
	{
		MessageBox(L"ָ�����ļ�������", L"��ʾ");
		return;
	}
	CDialogEx::OnOK();
}


int CAddDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CenterWindow();

	return 0;
}

BOOL CAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (1 == m_opType) //�޸�����ʱ�������޸�·�������Խ����������ؼ�
	{
		SetWindowText(L"�޸�Ӧ�ó���");
		CEdit *pEditPath = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
		pEditPath->SetReadOnly(TRUE);
		GetDlgItem(IDC_BTN_OPEN)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
