// CamIDDialog1.cpp : implementation file
//

#include "stdafx.h"
#include "MsEdit.h"
#include "CamIDDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCamIDDialog dialog


CCamIDDialog::CCamIDDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCamIDDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCamIDDialog)
	m_nCamID = 0;
	//}}AFX_DATA_INIT
}


void CCamIDDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamIDDialog)
	DDX_Text(pDX, IDC_EDIT1, m_nCamID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCamIDDialog, CDialog)
	//{{AFX_MSG_MAP(CCamIDDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamIDDialog message handlers
