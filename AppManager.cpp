
// AppManager.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "AppManager.h"
#include "AppManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString onlyTitle = L"Ӧ�ó����ݷ�ʽ����";
// CAppManagerApp

BEGIN_MESSAGE_MAP(CAppManagerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAppManagerApp ����

CAppManagerApp::CAppManagerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAppManagerApp ����

CAppManagerApp theApp;


// CAppManagerApp ��ʼ��

BOOL CAppManagerApp::InitInstance()
{
	if (!IsFirstInstance(onlyTitle))
		return FALSE;
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CAppManagerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

BOOL CAppManagerApp::IsFirstInstance(CString title)
{
	CWnd *pWndPrev, *pWndChild;
	if (pWndPrev = CWnd::FindWindow(NULL, title))
	{
		pWndChild = pWndPrev->GetLastActivePopup();
		if (pWndPrev->IsIconic())
			pWndPrev->ShowWindow(SW_RESTORE);
		pWndChild->SetForegroundWindow();
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}