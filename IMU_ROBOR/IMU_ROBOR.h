
// IMU_ROBOR.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CIMU_ROBORApp:
// See IMU_ROBOR.cpp for the implementation of this class
//

class CIMU_ROBORApp : public CWinApp
{
public:
	CIMU_ROBORApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CIMU_ROBORApp theApp;