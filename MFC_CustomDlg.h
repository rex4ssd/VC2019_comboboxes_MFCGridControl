
// MFC_CustomDlg.h : header file
//

#pragma once
#include "GridCtrl_src/GridCtrl.h"


#define GRID_C0_WIDTH	150
#define GRID_C1_WIDTH	250
#define ITEM_LIMIT		50
#define ITEM_LEN		20
#define ITEMS_LIMIT		10

struct _sz_v { 
	char v[ITEM_LEN];//value
};

struct _int_v { 
	int v;			//value 
};

typedef struct _grid_ini {
	int id;
	char title[ITEM_LEN];
	int option; //0 = no sub item, 1 = exist sub item
	int type;	//0 = char, 1 = int;
	union {		//default value
		struct _sz_v sz_v;
		struct _int_v int_v;
	};
	char d[ITEM_LEN];//description
}grid_ini, *pGrid_ini;

struct f_grid_ini { //find grid info
	int id;

	f_grid_ini(int id) : id(id) {}
	bool operator () (const grid_ini& g) const
	{
		return g.id == id;
	}
};

//suport ITEM_LEN sub item
struct _sz_item {
	char i[ITEM_LEN];
};

struct _int_item {
	int i;
};

typedef struct _grid_option {//sub item
	int id;
	union {
		struct _sz_item sz_item[ITEMS_LIMIT];
		struct _int_item int_item[ITEMS_LIMIT];
	};
}grid_option, *pGrid_option;


struct f_grid_option { //find grid option(sub item)
	int id;
	f_grid_option(int id) : id(id) {}
	bool operator () (const grid_option& g) const
	{
		return g.id == id;
	}
};


// CMFCCustomDlg dialog
class CMFCCustomDlg : public CDialogEx
{
// Construction
public:
	CMFCCustomDlg(CWnd* pParent = nullptr);	// standard constructor


	CStatic	m_GridRect;
	int		m_nFixCols;
	int		m_nFixRows;
	int		m_nCols;
	int		m_nRows;
	BOOL	m_bEditable;
	BOOL	m_bHorzLines;
	BOOL	m_bListMode;
	BOOL	m_bVertLines;
	BOOL	m_bSelectable;
	BOOL	m_bAllowColumnResize;
	BOOL	m_bAllowRowResize;
	BOOL	m_bHeaderSort;
	BOOL	m_bItalics;
	BOOL	m_btitleTips;
	BOOL	m_bSingleSelMode;
    BOOL    m_bSingleColSelMode;
	BOOL	m_bTrackFocus;
	BOOL	m_bFrameFocus;
	BOOL	m_bVirtualMode;
	BOOL	m_bCallback;
    BOOL    m_bVertical;
	//}}AFX_DATA
	CGridCtrl m_Grid;
	CSize m_OldSize;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_CUSTOM_DIALOG };
#endif

    virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	static BOOL CALLBACK GridCallback(GV_DISPINFO* pDispInfo, LPARAM lParam);
	void OnGridLines();
	void UpdateMenuUI();
	void create_Ini_Data();
	void InitGrid(int rowCount = 10);
	void gridSetCombo(int row, char* value, CStringArray &op);
	void GetAllItems(IN grid_option& go, IN int type, OUT CStringArray& op);
private:

// Implementation
protected:
	HICON m_hIcon;
	// CImageList m_ImageList;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnUpdateEditCols();
	afx_msg void OnUpdateEditFixcols();
	afx_msg void OnUpdateEditFixrows();
	afx_msg void OnUpdateEditRows();
	afx_msg void OnHorzGridLines();
	afx_msg void OnVertGridLines();
	afx_msg void OnListmode();
	afx_msg void OnEditable();
	afx_msg void OnPrintButton();
	afx_msg void OnAllowSelection();
	afx_msg void OnRowResize();
	afx_msg void OnColResize();
	afx_msg void OnFontButton();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHeaderSort();
	afx_msg void OnEditSelectall();
	afx_msg void OnAppAbout();
	afx_msg void OnItalics();
	afx_msg void OnTitletips();
	afx_msg void OnInsertRow();
	afx_msg void OnDeleteRow();
	afx_msg void OnSingleselmode();
	afx_msg void OnSingleColMode();
	afx_msg void OnExpandToFit();
	afx_msg void OnAutoSize();
	afx_msg void OnFill();
	afx_msg void OnCellUrl();
	afx_msg void OnCellNormal();
	afx_msg void OnCellNormal(int row);
	afx_msg void OnCellCombo();
	afx_msg void OnCellReadonly();
	afx_msg void OnTrackfocus();
	afx_msg void OnFramefocus();
	afx_msg void OnExpandLast();
	afx_msg void OnSetFocus11();
	afx_msg void OnVirtualMode();
	afx_msg void OnCallbackFunction();
	afx_msg void OnFixedRowSelection();
	afx_msg void OnFixedColSelection();

    afx_msg void OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);

    afx_msg void OnVertical();
    	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
};
