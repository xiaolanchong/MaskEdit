#pragma once


// CLayerDlg

class CLayerDlg : public CWnd
{
	DECLARE_DYNAMIC(CLayerDlg)

public:
	CLayerDlg();
	virtual ~CLayerDlg();

protected:
	DECLARE_MESSAGE_MAP()
};


#pragma once


// CLayerDlg dialog

class CLayerDlg : public CDialog
{
	DECLARE_DYNAMIC(CLayerDlg)

public:
	CLayerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLayerDlg();

// Dialog Data
	enum { IDD = IDD_LAYERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
