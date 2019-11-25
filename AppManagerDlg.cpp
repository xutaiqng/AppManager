
// AppManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AppManager.h"
#include "AppManagerDlg.h"
#include "afxdialogex.h"
#include "AddDlg.h"
#include "version.h"

#include <VersionHelpers.h>

// Header Files for Jump List features
#include <objectarray.h>
#include <shobjidl.h>
#include <propkey.h>
#include <propvarutil.h>
#include <knownfolders.h>
#include <shlobj.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CString onlyTitle;
// CAppManagerDlg �Ի���


//��Buffer����IntervalCharΪ�ָ������뵽TabString������
int GetTab(CString Buffer, CArray<CString>& TabString, CString separator)
{
	int pos = 0;
	int cnt = 0;
	int left;
	CString sTmp;
	TabString.RemoveAll();

	while ( true )
	{
		left = Buffer.Find(separator, pos);
		if ( left == -1 ) break;
		sTmp = Buffer.Mid(pos, left - pos);
		TabString.Add( sTmp );
		pos = left + separator.GetLength();
		cnt++;
	}
	return cnt;
}

CAppManagerDlg::CAppManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAppManagerDlg::IDD, pParent)
	, m_lvIsChange(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	m_imgList.Create(16, 16, ILC_COLOR32, 0, 100);
}

CAppManagerDlg::~CAppManagerDlg()
{
	CoUninitialize();
}

void CAppManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_APP, m_appList);
}

BEGIN_MESSAGE_MAP(CAppManagerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ADD, &CAppManagerDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CAppManagerDlg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_UP, &CAppManagerDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, &CAppManagerDlg::OnBnClickedBtnDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_APP, &CAppManagerDlg::OnDblclkListApp)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CAppManagerDlg::OnBnClickedBtnModify)
END_MESSAGE_MAP()


// CAppManagerDlg ��Ϣ�������

BOOL CAppManagerDlg::OnInitDialog()
{
	/*OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	::GetVersionEx((OSVERSIONINFO*)&osvi);
	if (osvi.dwMajorVersion < 6 || (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion < 1))
	{
		MessageBox(L"�ó���ֻ����Windows7������ϵͳ���У�", L"��ʾ", MB_OK | MB_ICONERROR);
		EndDialog(IDCANCEL);
		return FALSE;
	}*/

	if ( false == IsWindows7OrGreater() )
	{
		MessageBox(L"�ó���ֻ����Windows7������ϵͳ���У�", L"��ʾ", MB_OK | MB_ICONERROR);
		EndDialog(IDCANCEL);
		return FALSE;
	}

	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//SetWindowText(onlyTitle);
	CString sTitle = onlyTitle + L" " + g_version + L"[" + g_versiondate + L"]";
	SetWindowText(sTitle);

	//��ȡ�����ļ��е�Ӧ�ó����б�
	WCHAR szCurrPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szCurrPath, MAX_PATH);
	CString strCurrPath(szCurrPath);
	int iPos = strCurrPath.ReverseFind('\\');
	strCurrPath = strCurrPath.Left(iPos);
	m_cfgFileName = strCurrPath + L"\\AppManager.dat";

	CRect rect;
	//��ȡ�б�ؼ���λ�úʹ�С
	m_appList.GetClientRect(&rect);
	//Ϊ�б�ؼ����ȫ��ѡ����Сͼ����
	m_appList.SetExtendedStyle(m_appList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_appList.SetImageList(&m_imgList, LVSIL_SMALL);
	//Ϊ�б�ؼ������
	m_appList.InsertColumn(0, _T("����"), 0, rect.Width()/4, 0);
	m_appList.InsertColumn(1, _T("�ļ�·��"), 0, rect.Width()/5*3, 1);
	m_appList.InsertColumn(2, _T("���Ӳ���"), 0, rect.Width()/5*2, 2);

	/*CStdioFile cfgFile;
	if ( !cfgFile.Open((LPTSTR)(LPCTSTR)m_cfgFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate | CFile::typeText) )
	{
		MessageBox(L"�������ļ�ʧ�ܣ�", L"��ʾ");
	}*/
	CFile cfgFile;
	if ( !cfgFile.Open((LPTSTR)(LPCTSTR)m_cfgFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate) )
	{
		MessageBox(L"�������ļ�ʧ�ܣ�", L"��ʾ");
		EndDialog(IDCANCEL);
		return FALSE;
	}

	UINT iLen = static_cast<UINT>(cfgFile.GetLength());
	
	if ( iLen > 0)
	{
		char* buf = new char[iLen + 1];
		memset(buf, 0, iLen + 1);
		cfgFile.Read(buf, iLen);

		CString sTemp(buf);
		//�ж����һ�������Ƿ���س����з���û������ϣ�����ᵼ�����һ������ȡ����
		if (sTemp.Right(2) != "\r\n")
		{
			sTemp += "\r\n";
		}
		CArray<CString> itemList;
		GetTab(sTemp, itemList, L"\r\n");
		CString sName, sPath, sParam;

		for (int i=0; i<itemList.GetSize(); i++)
		{
			iPos = itemList[i].Find('=');
			sName = itemList[i].Left(iPos);
			sTemp = itemList[i].Mid(iPos + 1);

			iPos = sTemp.Find(L"@@");
			sPath = sTemp.Left(iPos);
			sParam = sTemp.Mid(iPos + 2);
			if ( PathFileExists((LPTSTR)(LPCTSTR)sPath) )
			{
				CreateItem(sName, sPath, sParam);
			}
		}
		delete[] buf;
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAppManagerDlg::OnPaint()
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
HCURSOR CAppManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAppManagerDlg::OnBnClickedBtnAdd()
{
	CAddDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		CreateItem(dlg.m_appName, dlg.m_appPath, dlg.m_appParam);

		//ListView�����б�������´���JumpList�б�
		DeleteJumpList(); //ɾ��ԭ����JumpList
		CreateJumpList(); //��������ListView���ݴ����µ�JumpList
		m_lvIsChange = true; //��Ϊtrue�������˳�ʱ���������ļ�����
	}
}

void CAppManagerDlg::CreateItem(CString sName, CString sPath, CString sParam)
{
	if ( !PathFileExists((LPTSTR)(LPCTSTR)sPath) )
	{
		return;
	}
	
	HICON hIcon = ExtractIcon(AfxGetInstanceHandle(), (LPTSTR)(LPCTSTR)sPath, 0);
	int nIndex = m_imgList.Add(hIcon);
	
	LVITEM item = {0};
	item.iItem = nIndex;
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.cchTextMax = sName.GetLength() + 1;
	item.pszText = (LPTSTR)(LPCTSTR)sName;
	item.iImage = nIndex;
	m_appList.InsertItem(&item);

	m_appList.SetItemText(nIndex, 1, sPath);
	m_appList.SetItemText(nIndex, 2, sParam);
}


int CAppManagerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CenterWindow();

	return 0;
}


void CAppManagerDlg::OnBnClickedBtnDel()
{
	int nIndex = m_appList.GetSelectionMark();
	if (nIndex >= 0)
	{
		if( m_appList.GetItemState(nIndex, LVIS_SELECTED) == LVIS_SELECTED )
		{
			m_appList.DeleteItem(nIndex);

			//ListView�����б�������´���JumpList�б�
			DeleteJumpList(); //ɾ��ԭ����JumpList
			CreateJumpList(); //��������ListView���ݴ����µ�JumpList
			m_lvIsChange = true; //��Ϊtrue�������˳�ʱ���������ļ�����
		}
	}
}


void CAppManagerDlg::OnDblclkListApp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	*pResult = 0;

	CString strPath, strParam;
	NMLISTVIEW *pNMListView = (NMLISTVIEW *)pNMHDR;

	if ( -1 != pNMListView->iItem)
	{
		strPath = m_appList.GetItemText(pNMListView->iItem, 1);
		strParam = m_appList.GetItemText(pNMListView->iItem, 2);
		//ShellExecute(m_hWnd, L"open", (LPTSTR)(LPCTSTR)strPath, L"", L"", SW_SHOW);
		ShellExecute(m_hWnd, L"open", (LPTSTR)(LPCTSTR)strPath, (LPTSTR)(LPCTSTR)strParam, L"", SW_SHOW);
	}
}


void CAppManagerDlg::OnBnClickedBtnUp()
{
	int nIndex = m_appList.GetSelectionMark();
	if (nIndex >= 0) //��ѡ��ĳ����
	{
		if( m_appList.GetItemState(nIndex, LVIS_SELECTED) == LVIS_SELECTED )
		{
			//nIndex=0��ʾѡ����ǵ�һ�У����������ϣ����Բ�������
			if (nIndex == 0) return;

			HICON tmpIcon1 = m_imgList.ExtractIcon(nIndex-1);
			HICON tmpIcon2 = m_imgList.ExtractIcon(nIndex);
			m_imgList.Replace(nIndex, tmpIcon1);
			m_imgList.Replace(nIndex-1, tmpIcon2);

			CString sAppName, sAppPath, sAppPram;
			sAppName = m_appList.GetItemText(nIndex, 0);
			sAppPath = m_appList.GetItemText(nIndex, 1);
			sAppPram = m_appList.GetItemText(nIndex, 2);
			
			m_appList.SetItemText(nIndex, 0, m_appList.GetItemText(nIndex-1, 0));
			m_appList.SetItemText(nIndex, 1, m_appList.GetItemText(nIndex-1, 1));
			m_appList.SetItemText(nIndex, 2, m_appList.GetItemText(nIndex-1, 2));

			m_appList.SetItemText(nIndex-1, 0, sAppName);
			m_appList.SetItemText(nIndex-1, 1, sAppPath);
			m_appList.SetItemText(nIndex-1, 2, sAppPram);

			//ListView�����б�������´���JumpList�б�
			DeleteJumpList(); //ɾ��ԭ����JumpList
			CreateJumpList(); //��������ListView���ݴ����µ�JumpList
			m_lvIsChange = true; //��Ϊtrue�������˳�ʱ���������ļ�����
		}
	}
}


void CAppManagerDlg::OnBnClickedBtnDown()
{
	int nIndex = m_appList.GetSelectionMark();
	if (nIndex >= 0) //��ѡ��ĳ����
	{
		int nCount = m_appList.GetItemCount();
		if( m_appList.GetItemState(nIndex, LVIS_SELECTED) == LVIS_SELECTED )
		{
			//nIndex=nCount-1��ʾѡ��������һ�У����������£����Բ�������
			if (nIndex == nCount-1 ) return;

			HICON tmpIcon1 = m_imgList.ExtractIcon(nIndex);
			HICON tmpIcon2 = m_imgList.ExtractIcon(nIndex+1);
			m_imgList.Replace(nIndex, tmpIcon2);
			m_imgList.Replace(nIndex+1, tmpIcon1);

			CString sAppName, sAppPath, sAppPram;
			sAppName = m_appList.GetItemText(nIndex, 0);
			sAppPath = m_appList.GetItemText(nIndex, 1);
			sAppPram = m_appList.GetItemText(nIndex, 2);

			m_appList.SetItemText(nIndex, 0, m_appList.GetItemText(nIndex+1, 0));
			m_appList.SetItemText(nIndex, 1, m_appList.GetItemText(nIndex+1, 1));
			m_appList.SetItemText(nIndex, 2, m_appList.GetItemText(nIndex+1, 2));

			m_appList.SetItemText(nIndex+1, 0, sAppName);
			m_appList.SetItemText(nIndex+1, 1, sAppPath);
			m_appList.SetItemText(nIndex+1, 2, sAppPram);

			//ListView�����б�������´���JumpList�б�
			DeleteJumpList(); //ɾ��ԭ����JumpList
			CreateJumpList(); //��������ListView���ݴ����µ�JumpList
			m_lvIsChange = true; //��Ϊtrue�������˳�ʱ���������ļ�����
		}
	}
}

void CAppManagerDlg::CreateJumpList()
{
	ICustomDestinationList *pcdl;
	HRESULT hr = CoCreateInstance(CLSID_DestinationList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pcdl));
	if (SUCCEEDED(hr))
	{
		//������Ϊ��ݲ˵�����AppID
		//hr = pcdl->SetAppID(c_szAppID);
		if (SUCCEEDED(hr))
		{
			UINT cMinSlots;
			IObjectArray *poaRemoved;
			hr = pcdl->BeginList(&cMinSlots, IID_PPV_ARGS(&poaRemoved));
			if (SUCCEEDED(hr))
			{
				// Add content to the Jump List.
				hr = _AddTasksToList(pcdl);
				if (SUCCEEDED(hr))
				{
					// Commit the list-building transaction.
					hr = pcdl->CommitList();
				}
				poaRemoved->Release();
			}
			pcdl->Release();
		}
	}
}

void CAppManagerDlg::DeleteJumpList()
{
	ICustomDestinationList *pcdl;
	HRESULT hr = CoCreateInstance(CLSID_DestinationList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pcdl));
	if (SUCCEEDED(hr))
	{
		hr = pcdl->DeleteList(NULL);
		pcdl->Release();
	}
}

// Creates a CLSID_ShellLink to insert into the Tasks section of the Jump List.  This type of Jump
// List item allows the specification of an explicit command line to execute the task.
HRESULT CAppManagerDlg::_CreateShellLink(PCWSTR pszTitle, PCWSTR pszArguments, PCWSTR pszAppPath, IShellLink **ppsl)
{
	IShellLink *psl;
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&psl));
	if (SUCCEEDED(hr))
	{
		// Determine our executable's file path so the task will execute this application
		hr = psl->SetPath(pszAppPath);
		if (SUCCEEDED(hr))
		{
			hr = psl->SetIconLocation(pszAppPath, 0);
			hr = psl->SetArguments(pszArguments);
			if (SUCCEEDED(hr))
			{
				// The title property is required on Jump List items provided as an IShellLink
				// instance.  This value is used as the display name in the Jump List.
				IPropertyStore *pps;
				hr = psl->QueryInterface(IID_PPV_ARGS(&pps));
				if (SUCCEEDED(hr))
				{
					PROPVARIANT propvar;
					hr = InitPropVariantFromString(pszTitle, &propvar);
					if (SUCCEEDED(hr))
					{
						hr = pps->SetValue(PKEY_Title, propvar);
						if (SUCCEEDED(hr))
						{
							hr = pps->Commit();
							if (SUCCEEDED(hr))
							{
								hr = psl->QueryInterface(IID_PPV_ARGS(ppsl));
							}
						}
						PropVariantClear(&propvar);
					}
					pps->Release();
				}
			}
		}
		psl->Release();
	}
	return hr;
}

HRESULT CAppManagerDlg::_AddTasksToList(ICustomDestinationList *pcdl)
{
	IObjectCollection *poc;
	HRESULT hr = CoCreateInstance(CLSID_EnumerableObjectCollection, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&poc));
	if (SUCCEEDED(hr))
	{
		IShellLink * psl;
		
		int iItemCount = m_appList.GetItemCount();
		CString appName, appPath, appPram;
		for (int i=0; i<iItemCount; i++)
		{
			appName = m_appList.GetItemText(i, 0);
			appPath = m_appList.GetItemText(i, 1);
			appPram = m_appList.GetItemText(i, 2);
			/*if (appPram.IsEmpty())
				appPram = "/";*/

			hr = _CreateShellLink((LPTSTR)(LPCTSTR)appName, (LPTSTR)(LPCTSTR)appPram, (LPTSTR)(LPCTSTR)appPath, &psl);
			if (SUCCEEDED(hr))
			{
				hr = poc->AddObject(psl);
				psl->Release();
			}
		}
		
		if (SUCCEEDED(hr))
		{
			IObjectArray * poa;
			hr = poc->QueryInterface(IID_PPV_ARGS(&poa));
			if (SUCCEEDED(hr))
			{
				// Add the tasks to the Jump List. Tasks always appear in the canonical "Tasks"
				// category that is displayed at the bottom of the Jump List, after all other
				// categories.
				hr = pcdl->AddUserTasks(poa);
				poa->Release();
			}
		}
		poc->Release();
	}
	return hr;
}


void CAppManagerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if ( m_lvIsChange )
	{
		CFile cfgFile;
		if ( !cfgFile.Open((LPTSTR)(LPCTSTR)m_cfgFileName, CFile::modeCreate | CFile::modeReadWrite) )
		{
			MessageBox(L"�������ļ�ʧ�ܣ�", L"��ʾ");
			return;
		}

		int iItemCount = m_appList.GetItemCount();
		CString appName, appPath, appPram;
		for (int i=0; i<iItemCount; i++)
		{
			appName = m_appList.GetItemText(i, 0);
			appPath = m_appList.GetItemText(i, 1);
			appPram = m_appList.GetItemText(i, 2);

			CString sItem = appName + "=" + appPath + "@@" + appPram;
			//Unicodeת����ANSI��д���ļ�
			int nLen = WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)sItem, -1, NULL, 0, NULL, NULL);
			char *ansiStr = new char[nLen];
			memset(ansiStr, 0, nLen);
			WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)sItem, -1, ansiStr, nLen, NULL, NULL);
			
			cfgFile.Write(ansiStr, strlen(ansiStr));
			cfgFile.Write("\r\n", strlen("\r\n"));
		}
		cfgFile.Close();
	}
}


void CAppManagerDlg::OnBnClickedBtnModify()
{
	int nIndex = m_appList.GetSelectionMark();
	if (nIndex < 0) return;

	if (m_appList.GetItemState(nIndex, LVIS_SELECTED) == LVIS_SELECTED)
	{
		CAddDlg dlg;
		dlg.m_appName = m_appList.GetItemText(nIndex, 0);
		dlg.m_appPath = m_appList.GetItemText(nIndex, 1);
		dlg.m_appParam = m_appList.GetItemText(nIndex, 2);
		dlg.m_opType = 1; //�޸�����

		if (IDOK == dlg.DoModal())
		{
			m_appList.SetItemText(nIndex, 0, dlg.m_appName);
			m_appList.SetItemText(nIndex, 2, dlg.m_appParam);

			//ListView�����б�������´���JumpList�б�
			DeleteJumpList(); //ɾ��ԭ����JumpList
			CreateJumpList(); //��������ListView���ݴ����µ�JumpList
			m_lvIsChange = true; //��Ϊtrue�������˳�ʱ���������ļ�����
		}
	}
}
