
// MFC_CustomDlg.h : header file
//

#pragma once
#include "GridCtrl_src/GridCtrl.h"


// CMFCCustomDlg dialog
class CMFCCustomDlg : public CDialogEx
{
// Construction
public:
	CMFCCustomDlg(CWnd* pParent = nullptr);	// standard constructor

	CGridCtrl m_Grid;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_CUSTOM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
