
// AppManager.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAppManagerApp:
// �йش����ʵ�֣������ AppManager.cpp
//

class CAppManagerApp : public CWinApp
{
public:
	CAppManagerApp();

// ��д
public:
	virtual BOOL InitInstance();

public:
	BOOL IsFirstInstance(CString title); //�жϳ����Ƿ��״�����

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAppManagerApp theApp;