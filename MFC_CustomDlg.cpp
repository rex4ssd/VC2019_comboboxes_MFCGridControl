
// MFC_CustomDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC_Custom.h"
#include "MFC_CustomDlg.h"
#include "afxdialogex.h"
#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"


#include <algorithm>
#include <vector>

#include <random>

#include "CWlog.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//global
vector<grid_ini> vgis;
vector<grid_option> vgos;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCCustomDlg dialog



CMFCCustomDlg::CMFCCustomDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_CUSTOM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_OldSize = CSize(-1, -1);

	//{{AFX_DATA_INIT(CGridCtrlDemoDlg)
	m_nFixCols = 1;
	m_nFixRows = 1;
	m_nCols = 2;
	m_nRows = 20;
	m_bEditable = FALSE;
	m_bHorzLines = FALSE;
	m_bVertLines = FALSE;
	m_bListMode = TRUE;
	m_bHeaderSort = FALSE;
	m_bSingleSelMode = TRUE;
	m_bSingleColSelMode = TRUE;
	m_bSelectable = FALSE;
	m_bAllowColumnResize = FALSE;
	m_bAllowRowResize = FALSE;
	m_bItalics = FALSE;
	m_btitleTips = FALSE;
	m_bTrackFocus = FALSE;
	m_bFrameFocus = FALSE;
	m_bVirtualMode = TRUE;
	m_bCallback = TRUE;
	m_bVertical = TRUE;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCCustomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);             // associate the grid window with a C++ object
}

BEGIN_MESSAGE_MAP(CMFCCustomDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCCustomDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCCustomDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCCustomDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &CMFCCustomDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCCustomDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCCustomDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCCustomDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCCustomDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCCustomDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CMFCCustomDlg message handlers

BOOL CMFCCustomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	create_Ini_Data();

	CRect rect;
	GetClientRect(rect);
	m_OldSize = CSize(rect.Width(), rect.Height());

	// m_ImageList.Create(MAKEINTRESOURCE(IDB_IMAGES), 16, 1, RGB(255,255,255));
	// m_Grid.SetImageList(&m_ImageList);

	m_Grid.EnableDragAndDrop(FALSE);
	m_Grid.GetDefaultCell(TRUE, TRUE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

    OnEditable();

    InitGrid(50);
    //OnVirtualMode();    // Sets the grid mode, fills the grid

	OnListmode();
    //OnCellReadonly();
    //OnItalics();
    //OnTitletips();
    //OnTrackfocus();
    OnFramefocus();
    OnRowResize();
    OnColResize();
    OnSingleselmode();
    OnSingleColMode();
    OnVertGridLines();
    OnHorzGridLines();

    //R, Disable Select multi line
  //  OnAllowSelection();
    OnCallbackFunction();
    OnVertical();
    
    //R, Disable Sorting func
//    OnHeaderSort();


    m_Grid.SetFixedColumnSelection(FALSE);
    m_Grid.SetFixedRowSelection(FALSE);

	//R, Set ColumnWidth
	m_Grid.SetColumnWidth(0, GRID_C0_WIDTH);
	m_Grid.SetColumnWidth(1, GRID_C1_WIDTH);
    UpdateMenuUI();

    //m_Grid.AutoSize();

    //m_Grid.SetSortColumn(100);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCCustomDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCCustomDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCCustomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//..............................................................
//...............................................................

void CMFCCustomDlg::OnGridLines()
{
	if (!m_bHorzLines && !m_bVertLines)
		m_Grid.SetGridLines(GVL_NONE);
	else if (m_bHorzLines && !m_bVertLines)
		m_Grid.SetGridLines(GVL_HORZ);
	else if (!m_bHorzLines && m_bVertLines)
		m_Grid.SetGridLines(GVL_VERT);
	else
		m_Grid.SetGridLines(GVL_BOTH);

	UpdateMenuUI();
}

void CMFCCustomDlg::OnVertGridLines() 
{
    m_bVertLines = !m_bVertLines;
    OnGridLines();
}

void CMFCCustomDlg::OnHorzGridLines() 
{
    m_bHorzLines = !m_bHorzLines;
    OnGridLines();
}

void CMFCCustomDlg::OnListmode() 
{
    m_bListMode = !m_bListMode;
	m_Grid.SetListMode(m_bListMode);
    UpdateMenuUI();
}

void CMFCCustomDlg::OnHeaderSort() 
{
	m_bHeaderSort = !m_bHeaderSort;
	m_Grid.SetHeaderSort(m_bHeaderSort);
    UpdateMenuUI();
}

void CMFCCustomDlg::OnSingleselmode() 
{
	m_bSingleSelMode = !m_bSingleSelMode;
	m_Grid.SetSingleRowSelection(m_bSingleSelMode);
    UpdateMenuUI();
}

void CMFCCustomDlg::OnSingleColMode() 
{
	m_bSingleColSelMode = !m_bSingleColSelMode;
	m_Grid.SetSingleColSelection(m_bSingleColSelMode);
    UpdateMenuUI();
}

void CMFCCustomDlg::OnEditable() 
{
	m_bEditable = !m_bEditable;
	m_Grid.SetEditable(m_bEditable);
    UpdateMenuUI();
}

void CMFCCustomDlg::OnAllowSelection() 
{
	m_bSelectable = !m_bSelectable;
	m_Grid.EnableSelection(m_bSelectable);
    UpdateMenuUI();
}

void CMFCCustomDlg::OnRowResize() 
{
	m_bAllowRowResize = !m_bAllowRowResize;
	m_Grid.SetRowResize(m_bAllowRowResize);
    UpdateMenuUI();
}

void CMFCCustomDlg::OnColResize() 
{
	m_bAllowColumnResize = !m_bAllowColumnResize;
	m_Grid.SetColumnResize(m_bAllowColumnResize);
    UpdateMenuUI();
}



BOOL CALLBACK EnumProc(HWND hwnd, LPARAM lParam)
{
	CWnd* pWnd = CWnd::FromHandle(hwnd);
	CSize* pTranslate = (CSize*) lParam;

	CMFCCustomDlg* pDlg = (CMFCCustomDlg*) pWnd->GetParent();
	if (!pDlg) return FALSE;

	CRect rect;
	pWnd->GetWindowRect(rect);
	//if (hwnd == pDlg->m_Grid.GetSafeHwnd())
	//	TRACE(_T("Wnd rect: %d,%d - %d,%d\n"),rect.left,rect.top, rect.right, rect.bottom);

	pDlg->ScreenToClient(rect);
	//if (hwnd == pDlg->m_Grid.GetSafeHwnd())
	//	TRACE(_T("Scr rect: %d,%d - %d,%d\n"),rect.left,rect.top, rect.right, rect.bottom);

	if (hwnd == pDlg->m_Grid.GetSafeHwnd())
	{
		if (  ((rect.top >= 7 && pTranslate->cy > 0) || rect.Height() > 20) &&
			  ((rect.left >= 7 && pTranslate->cx > 0) || rect.Width() > 20)   )
			pDlg->m_Grid.MoveWindow(rect.left, rect.top, 
									rect.Width()+pTranslate->cx, 
									rect.Height()+pTranslate->cy, FALSE);
		else
			pWnd->MoveWindow(rect.left+pTranslate->cx, rect.top+pTranslate->cy, 
							 rect.Width(), rect.Height(), FALSE);
	}
	else 
	{
        // if (pWnd->GetDlgCtrlID() == IDC_SIZEBOX)
       	// 	pWnd->MoveWindow(rect.left+pTranslate->cx, rect.top+pTranslate->cy, 
        //    					 rect.Width(), rect.Height(), FALSE);
        // else
   		   //  pWnd->MoveWindow(rect.left+pTranslate->cx, rect.top, rect.Width(), rect.Height(), FALSE);
	}
	pDlg->Invalidate();

	return TRUE;
}


void CMFCCustomDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (cx <= 1 || cy <= 1 ) 
        return;

#ifdef _WIN32_WCE
    m_Grid.MoveWindow(0,0, cx,cy, FALSE);
#else
    if (IsWindow(m_Grid.GetSafeHwnd()))
    {
        CRect rect;
        m_Grid.GetWindowRect(rect);
        ScreenToClient(rect);
        m_Grid.MoveWindow(7,7, rect.Width(), cy-14, FALSE);
    }

	CSize Translate(cx - m_OldSize.cx, cy - m_OldSize.cy);
	::EnumChildWindows(GetSafeHwnd(), EnumProc, (LPARAM)&Translate);
	m_OldSize = CSize(cx,cy);
#endif
}



// (Thanks to Koay Kah Hoe for this)
BOOL CMFCCustomDlg::PreTranslateMessage(MSG* pMsg) 
{
    if( pMsg->message == WM_KEYDOWN )
    {
        if(pMsg->wParam == VK_RETURN
            || pMsg->wParam == VK_ESCAPE )
        {
            ::TranslateMessage(pMsg);
            ::DispatchMessage(pMsg);
            return TRUE;                    // DO NOT process further
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}	

void CMFCCustomDlg::OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
    if (pItem->iRow < 0)
        return;

    TRACE2("Double Clicked on row %d, col %d\n", pItem->iRow, pItem->iColumn);
}

void CMFCCustomDlg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
    if (pItem->iRow < 0)
        return;

    TRACE2("Clicked on row %d, col %d\n", pItem->iRow, pItem->iColumn);
}

void CMFCCustomDlg::OnExpandToFit() 
{
    m_Grid.ExpandToFit();
}

void CMFCCustomDlg::OnAutoSize() 
{
    m_Grid.AutoSize();
}

void CMFCCustomDlg::OnFill() 
{
    m_Grid.DeleteAllItems();
    m_Grid.AutoFill();
}

void CMFCCustomDlg::OnCellNormal() 
{
    m_Grid.SetCellType(1,1, RUNTIME_CLASS(CGridCell));
    m_Grid.SetItemText(1,1, _T("1"));
    m_Grid.SetItemState(1,1, m_Grid.GetItemState(1,1) & ~GVIS_READONLY);
    m_Grid.Invalidate();
}

void CMFCCustomDlg::OnCellNormal(int row)
{
	m_Grid.SetCellType(row, 1, RUNTIME_CLASS(CGridCell));
	//m_Grid.SetItemText(1, 1, _T("1"));
	m_Grid.SetItemState(row, 1, m_Grid.GetItemState(row, 1) & ~GVIS_READONLY);

	//m_Grid.SetItemState(1,1, m_Grid.GetItemState(1,1) | GVIS_READONLY);
	m_Grid.Invalidate();
}


void CMFCCustomDlg::OnCellReadonly() 
{
    OnCellNormal();

    m_Grid.SetItemState(1,1, m_Grid.GetItemState(1,1) | GVIS_READONLY);
}

void CMFCCustomDlg::OnCellUrl() 
{
    OnCellNormal();

    if (!m_Grid.SetCellType(1,1, RUNTIME_CLASS(CGridURLCell)))
        return;
    m_Grid.SetItemText(1,1, _T("www.codeproject.com"));
}

void CMFCCustomDlg::OnCellCombo() 
{
    OnCellNormal();

    if (!m_Grid.SetCellType(1,1, RUNTIME_CLASS(CGridCellCombo)))
        return;

    m_Grid.SetItemText(1,1, _T("www.codeproject.com"));

    CStringArray options;
    options.Add(_T("Option 1"));
    options.Add(_T("Option 2"));
    options.Add(_T("Option 3"));

    CGridCellCombo *pCell = (CGridCellCombo*) m_Grid.GetCell(1,1);
    pCell->SetOptions(options);
}

void CMFCCustomDlg::OnTrackfocus() 
{
    m_bTrackFocus = !m_bTrackFocus;
    m_Grid.SetTrackFocusCell(m_bTrackFocus);
    m_Grid.Invalidate();
    UpdateMenuUI();
}

void CMFCCustomDlg::OnFramefocus() 
{
    m_bFrameFocus = !m_bFrameFocus;
    m_Grid.SetFrameFocusCell(m_bFrameFocus);
    m_Grid.Invalidate();
    UpdateMenuUI();
}

void CMFCCustomDlg::OnExpandLast() 
{
    m_Grid.ExpandLastColumn();	
}

void CMFCCustomDlg::OnSetFocus11() 
{
    CCellID cell(1,1);

    m_Grid.SetFocusCell(cell);

    if (m_Grid.GetListMode())
        m_Grid.SetSelectedRange(1, m_Grid.GetFixedColumnCount(), 1, m_Grid.GetColumnCount()-1, TRUE);
    else
        m_Grid.SetSelectedRange(1,1,1,1, TRUE);
}

void CMFCCustomDlg::OnCallbackFunction() 
{
    m_bCallback = !m_bCallback;
    m_Grid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
    m_Grid.Refresh();
    UpdateMenuUI();
}

//.............................................................
void CMFCCustomDlg::GetAllItems(IN grid_option& go, IN int type, OUT CStringArray& op)
{
	std::vector< grid_option >::iterator it_vgos;
	it_vgos = std::find_if(vgos.begin(), vgos.end(), f_grid_option(go.id));

	char szOption[ITEM_LEN];
	::memset(szOption, 0, ITEM_LEN);

	for (int i = 0 ; i < ITEMS_LIMIT; i++)
	{
		::memset(szOption, 0, ITEM_LEN);
		if (0 == type)	//char
		{
			::sprintf(szOption, "%s", it_vgos->sz_item[i].i);
			
		}
		else {			//int 
			::sprintf(szOption, "%d", it_vgos->int_item[i].i);
		}

		//if empty, exit
		if (strlen(szOption) > 0)
			op.Add(szOption);
		else break;
			
	}
}

//.............................................................
void CMFCCustomDlg::InitGrid(int rowCount)
{
	UpdateData();
	m_bVirtualMode = !m_bVirtualMode;
	m_Grid.SetVirtualMode(m_bVirtualMode);

    //m_bHeaderSort = m_Grid.GetHeaderSort();
    m_bEditable = m_Grid.IsEditable();
	//m_bEditable = true;
    m_nFixCols = 1;
    m_nFixRows = 1;
    m_nCols = 2;
    m_nRows = rowCount;

    m_Grid.SetEditable(m_bEditable);
    //m_Grid.SetAutoSizeStyle();
	//m_Grid.SetItemS
	//SetItemState(nRow, nCol, GetItemState(nRow, nCol) | GVIS_MODIFIED);
    TRY {
	    m_Grid.SetRowCount(m_nRows);
	    m_Grid.SetColumnCount(m_nCols);
	    m_Grid.SetFixedRowCount(m_nFixRows);
	    m_Grid.SetFixedColumnCount(m_nFixCols);
    }
    CATCH (CMemoryException, e)
    {
    	e->ReportError();
		return;
    }
    END_CATCH

	grid_ini gi;
	grid_option go;
	int OptionType = 0;
	CString str0;	//title
	CString str1;	//value

	GV_ITEM Item0;
	GV_ITEM Item1;

	
    // fill rows/cols with text
    for (int row = 0; row < m_Grid.GetRowCount(); row++)
    {
		Item0.mask = GVIF_TEXT;
		Item0.row = row;
    	Item0.col = 0;

		Item1.mask = GVIF_TEXT;
		Item1.row = row;
    	Item1.col = 1;	    	

	    if (row == 0){
        	str0.Format(_T("- Content -       "));
        	str1.Format(_T(" - Parameter - "));
	    }
		else
		{
			gi.id = row;
			go.id = row;
			std::vector< grid_ini >::iterator it;
			it = std::find_if(vgis.begin(), vgis.end(), f_grid_ini(gi.id));
			
			if (it != vgis.end()) {
				//get it
				TRACE("find data : id = %d, titile  = %s, option = %d, type = %d\n",
					it->id, it->title, it->option, it->type);

				OptionType = it->type;
				str0.Format(_T("%s"), it->title);

				//default value
				if (it->type == 0) {
					//show char
					TRACE("char Value = %s\n", it->sz_v.v);
					str1.Format(_T("%s"), it->sz_v.v);
					//if (it_vgos != vgos.end()) {
					//	//get option value 
					//	TRACE("find option : id = %d, item0  = %s, item1 = %s, item2 = %s\n",
					//		it_vgos->id, it_vgos->sz_item.i0, it_vgos->sz_item.i1, it_vgos->sz_item.i2);
					//}
					//else TRACE("X, find option : id = %d\n", go.id);

				}
				else
				{
					//show int
					TRACE("int Value = %d\n", it->int_v.v);
					str1.Format(_T("%d"), it->int_v.v);
					//if (it_vgos != vgos.end()) {
					//	//get option value 
					//	TRACE("find option : id = %d, item0  = %d, item1 = %d, item2 = %d\n",
					//		it_vgos->id, it_vgos->int_item.i0, it_vgos->int_item.i1, it_vgos->int_item.i2);
					//}
					//else TRACE("X, find option : id = %d\n", go.id);
				}


				//find sub item
				if (it->option == 1)
				{
					//std::vector< grid_option >::iterator it_vgos;
					//it_vgos = std::find_if(vgos.begin(), vgos.end(), f_grid_option(go.id));				
					
					//get all option
					CStringArray op;
					char szOption[ITEM_LEN];
					::memset(szOption, 0, ITEM_LEN);
					//start from here Rex
					GetAllItems(go, OptionType, op);
					char z[] = "ABC";
					gridSetCombo(row, z, op);
				}//end of if

			}
			else TRACE("X, find id = %d\n", gi.id);		
		}// end of if

        Item0.strText = str0;
		m_Grid.SetItem(&Item0);

        Item1.strText = str1;
        m_Grid.SetItem(&Item1);
    }//end of for

    m_Grid.GetDefaultCell(TRUE, TRUE)->SetFormat(DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS);
    UpdateData(FALSE);    
}

//.............................................................
void CMFCCustomDlg::OnVirtualMode() 
{
    UpdateData();

    m_bVirtualMode = !m_bVirtualMode;
	m_Grid.SetVirtualMode(m_bVirtualMode);

    //m_bHeaderSort = m_Grid.GetHeaderSort();
    //m_bEditable = m_Grid.IsEditable();
    m_bEditable = true;
	//m_bVirtualMode = true;
    if (m_bVirtualMode)
    {
        m_Grid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);

        m_nFixCols = 1;
	    m_nFixRows = 1;
	    m_nCols = 2;
	    m_nRows = 100000;

	    TRY {
		    m_Grid.SetRowCount(m_nRows);
		    m_Grid.SetColumnCount(m_nCols);
		    m_Grid.SetFixedRowCount(m_nFixRows);
		    m_Grid.SetFixedColumnCount(m_nFixCols);
	    }
	    CATCH (CMemoryException, e)
	    {
	    	e->ReportError();
    		return;
	    }
        END_CATCH
    }
    else
    {
		//R, go this way
        m_nFixCols = 1;
	    m_nFixRows = 1;
	    m_nCols = 2;
	    m_nRows = 20;

	    m_Grid.SetEditable(m_bEditable);
        //m_Grid.SetAutoSizeStyle();
		//m_Grid.SetItemS
		//SetItemState(nRow, nCol, GetItemState(nRow, nCol) | GVIS_MODIFIED);
	    TRY {
		    m_Grid.SetRowCount(m_nRows);
		    m_Grid.SetColumnCount(m_nCols);
		    m_Grid.SetFixedRowCount(m_nFixRows);
		    m_Grid.SetFixedColumnCount(m_nFixCols);
	    }
	    CATCH (CMemoryException, e)
	    {
	    	e->ReportError();
    		return;
	    }
        END_CATCH

	    // fill rows/cols with text
	    for (int row = 0; row < m_Grid.GetRowCount(); row++)
        {
	    	for (int col = 0; col < m_Grid.GetColumnCount(); col++)
		    { 
                CString str;

			    GV_ITEM Item;

    			Item.mask = GVIF_TEXT;
	    		Item.row = row;
		    	Item.col = col;

			    if (row < m_nFixRows){
			    	//R, Skip first Column
			    	if(col == 0)
                    	str.Format(_T("Content - Row       "));
			    	else if(col == 1)
                    	str.Format(_T("Parameter - Col "));
			    }
                else if (col < m_nFixCols) 
                    str.Format(_T("Content - Row %d"), row);
                else 
				    str.Format(_T("Content1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8 9  10 11 12 13 14 15 16 %d"),row*col);

                Item.strText = str;


				//R, Disable some effect
				/*
    			if (rand() % 10 == 1)
	    		{
                    COLORREF clr = RGB(rand()%128 + 128, rand()%128 + 128, rand()%128 + 128);
                    Item.crBkClr = clr;             // or - m_Grid.SetItemBkColour(row, col, clr);
                    Item.crFgClr = RGB(255,0,0);    // or - m_Grid.SetItemFgColour(row, col, RGB(255,0,0));				    
                    Item.mask    |= (GVIF_BKCLR|GVIF_FGCLR);
    			}

    			if (col < m_Grid.GetFixedColumnCount())
               	{
                   Item.iImage = rand()%m_ImageList.GetImageCount();
                    Item.mask  |= (GVIF_IMAGE);
               	}
               	*/

        		m_Grid.SetItem(&Item);
	    	}
        }
    }

    //m_Grid.GetDefaultCell(FALSE,FALSE)->SetFormat(DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS);
    m_Grid.GetDefaultCell(TRUE, TRUE)->SetFormat(DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS);

    // Having weird Autosize problems with CE 2.11 - possibly just an emulation problem
#if !defined(_WIN32_WCE) || (_WIN32_WCE < 211)
	m_Grid.AutoSize();
#endif

    UpdateMenuUI();

    UpdateData(FALSE);
}

BOOL CMFCCustomDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
    if (wParam == (WPARAM)m_Grid.GetDlgCtrlID())
    {
        *pResult = 1;
        GV_DISPINFO *pDispInfo = (GV_DISPINFO*)lParam;
        if (GVN_GETDISPINFO == pDispInfo->hdr.code)
        {
            //TRACE2("Getting Display info for cell %d,%d\n", pDispInfo->item.row, pDispInfo->item.col);
            pDispInfo->item.strText.Format(_T("Message %d,%d"),pDispInfo->item.row, pDispInfo->item.col);
            return TRUE;
        }
        else if (GVN_ODCACHEHINT == pDispInfo->hdr.code)
        {
            GV_CACHEHINT *pCacheHint = (GV_CACHEHINT*)pDispInfo;
            TRACE(_T("Cache hint received for cell range %d,%d - %d,%d\n"),
                  pCacheHint->range.GetMinRow(), pCacheHint->range.GetMinCol(),
                  pCacheHint->range.GetMaxRow(), pCacheHint->range.GetMaxCol());
        }
    }
    
	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL CALLBACK CMFCCustomDlg::GridCallback(GV_DISPINFO *pDispInfo, LPARAM /*lParam*/) 
{
    pDispInfo->item.strText.Format(_T("Callback %d,%d"),pDispInfo->item.row, pDispInfo->item.col);
    return TRUE;
}

void CMFCCustomDlg::UpdateMenuUI()
{
#ifndef _WIN32_WCE
    BOOL bHorzLines = m_Grid.GetGridLines() & GVL_HORZ;
    BOOL bVertLines = m_Grid.GetGridLines() & GVL_VERT;
    GetMenu()->CheckMenuItem(IDC_HORZ_LINES, MF_BYCOMMAND | (bHorzLines)? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_VERT_LINES, MF_BYCOMMAND | (bVertLines)? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->EnableMenuItem(IDC_SINGLESELMODE, MF_BYCOMMAND | (m_Grid.GetListMode())? MF_ENABLED: MF_DISABLED|MF_GRAYED);
    GetMenu()->CheckMenuItem(IDC_HEADERSORT, MF_BYCOMMAND | (m_Grid.GetHeaderSort())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_SINGLESELMODE, MF_BYCOMMAND | (m_Grid.GetSingleRowSelection())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_SINGLECOLSELMODE, MF_BYCOMMAND | (m_Grid.GetSingleColSelection())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_EDITABLE, MF_BYCOMMAND | (m_Grid.IsEditable())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_ALLOW_SELECTION, MF_BYCOMMAND | (m_Grid.IsSelectable())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_ROW_RESIZE, MF_BYCOMMAND | (m_Grid.GetRowResize())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_COL_RESIZE, MF_BYCOMMAND | (m_Grid.GetColumnResize())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_ITALICS, MF_BYCOMMAND | (m_bItalics)? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_TITLETIPS, MF_BYCOMMAND | (m_Grid.GetTitleTips())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_TRACKFOCUS, MF_BYCOMMAND | (m_Grid.GetTrackFocusCell())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_FRAMEFOCUS, MF_BYCOMMAND | (m_Grid.GetFrameFocusCell())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_CALLBACK_FUNCTION, MF_BYCOMMAND | (m_Grid.GetCallbackFunc()!=NULL)? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_VIRTUAL_MODE, MF_BYCOMMAND | (m_Grid.GetVirtualMode())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->EnableMenuItem(IDC_CALLBACK_FUNCTION, MF_BYCOMMAND | (m_Grid.GetVirtualMode())? MF_ENABLED: MF_DISABLED|MF_GRAYED);
    GetMenu()->CheckMenuItem(IDC_FIXEDROW_SELECTION, MF_BYCOMMAND | (m_Grid.GetFixedRowSelection())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_FIXEDCOL_SELECTION, MF_BYCOMMAND | (m_Grid.GetFixedColumnSelection())? MF_CHECKED: MF_UNCHECKED);
    GetMenu()->CheckMenuItem(IDC_VERTICAL_TEXT, MF_BYCOMMAND | (m_bVertical)? MF_CHECKED: MF_UNCHECKED);
#endif
}

void CMFCCustomDlg::OnFixedRowSelection() 
{
    m_Grid.SetFixedRowSelection(!m_Grid.GetFixedRowSelection());
    UpdateMenuUI();
}

void CMFCCustomDlg::OnFixedColSelection() 
{
    m_Grid.SetFixedColumnSelection(!m_Grid.GetFixedColumnSelection());
    UpdateMenuUI();
}

void CMFCCustomDlg::OnVertical() 
{
    m_bVertical = !m_bVertical;

    // Set reasonable format
    //m_Grid.GetDefaultCell(TRUE, FALSE)->SetFormat(DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM);
    //m_Grid.GetDefaultCell(TRUE, TRUE)->SetFormat(DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM);

    // Get the font
    CFont* pFont = m_Grid.GetFont();
	LOGFONT lf;
    pFont->GetLogFont(&lf);
    
    // Make it true-type
    memcpy(lf.lfFaceName, _T("Arial"), 6);

    // Vertical or horizontal
    if (m_bVertical)
    {
        lf.lfEscapement = 900;
        lf.lfOrientation = 900;
    }
    else
    {
        lf.lfEscapement = 0;
        lf.lfOrientation = 0;
    }

    // Set the fixed row defaults as this new font
    m_Grid.GetDefaultCell(TRUE, TRUE)->SetFont(&lf);

    // Set cell 1,1 as the more normal font so URL cells look OK
    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    m_Grid.SetItemFont(0,1, &lf);


    m_Grid.AutoSize(GVS_HEADER);
    if (m_btitleTips)
    {
        m_btitleTips = !m_bVertical;
        m_Grid.EnableTitleTips(!m_bVertical);
    }

    UpdateMenuUI();
}

void CMFCCustomDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	OnCellNormal();

	if (!m_Grid.SetCellType(1, 1, RUNTIME_CLASS(CGridCellCombo)))
		return;

	m_Grid.SetItemText(1, 1, _T("www.codeproject.com"));

	CStringArray options;
	options.Add(_T("Option 1"));
	options.Add(_T("Option 2"));
	options.Add(_T("Option 3"));

	CGridCellCombo* pCell = (CGridCellCombo*)m_Grid.GetCell(1, 1);
	pCell->SetOptions(options);


	if (!m_Grid.SetCellType(3, 1, RUNTIME_CLASS(CGridCellCombo)))
		return;

	m_Grid.SetItemText(3, 1, _T("www.codeproject.com"));

	 options.RemoveAll();
	options.Add(_T("Optiona 1"));
	options.Add(_T("Optionb 2"));

	pCell = (CGridCellCombo*)m_Grid.GetCell(3, 1);
	pCell->SetOptions(options);
	CString strTempText = pCell->GetText();
	TRACE("pCell->GetText() = %s\n", pCell->GetText());
}

void CMFCCustomDlg::gridSetCombo(int row, char *value, CStringArray& op)
{
	OnCellNormal(row);

	if (!m_Grid.SetCellType(row, 1, RUNTIME_CLASS(CGridCellCombo)))
		return;

	//default value 
	//m_Grid.SetItemText(row, 1, (LPCTSTR)value);

	//insert option
	CGridCellCombo* pCell = (CGridCellCombo*)m_Grid.GetCell(row, 1);
	pCell = (CGridCellCombo*)m_Grid.GetCell(row, 1);
	pCell->SetOptions(op);
}

//------------------------------------------------------------------------------------------------------
void CMFCCustomDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CString s1 = "";
	for (int row = 1; row < m_Grid.GetRowCount(); row++)
	{
		CString str;
		str.Format(_T("RE %d"), row);
		str = m_Grid.GetItemText(row, 1);
		s1 = s1 + str + "\n";
	}
	
	CEdit* wEdit;
	wEdit = (CEdit*)GetDlgItem(IDC_STATIC);
	wEdit->SetWindowText(s1); //設定

}

//------------------------------------------------------------------------------------------------------
void CMFCCustomDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

	for (int row = 1; row < m_Grid.GetRowCount(); row++)
	{
		CString str;
		str.Format(_T("RE %d"), row);
		m_Grid.SetItemText(row, 1, str);
		m_Grid.Refresh();
	}

}


void CMFCCustomDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CMFCCustomDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here

	grid_ini gi;
	grid_option go;
	::ZeroMemory(&gi, sizeof(grid_ini));
	::ZeroMemory(&go, sizeof(grid_option));

	//init data 
	//vector<grid_ini> vgis;
	for (int i = 0; i < 10; i++)
	{
		//grid_ini
		gi.id = i; //id 0~ max
		sprintf(gi.title, "t_%03d", i);
		gi.option = 1;
		vgis.push_back(gi);

		//grid_option
		go.id = i; 
		sprintf(go.sz_item[0].i, "Item0A_%03d", i);
		sprintf(go.sz_item[1].i, "Item1A_%03d", i);
		sprintf(go.sz_item[2].i, "Item2B_%03d", i);
		vgos.push_back(go);
	}

	//find option by id
	gi.id = 2;
	std::vector< grid_ini >::iterator it;
	it = std::find_if(vgis.begin(), vgis.end(), f_grid_ini(gi.id));

	if (it != vgis.end()) {
		//get it
		TRACE("find data : id = %d, titile  = %s, option = %d\n", it->id, it->title, it->option);

		//modify data
		it->id = 77;
		sprintf(it->title, "%s", "update77");
		it->option = 77;

		TRACE("after modify data : id = %d, titile  = %s, option = %d\n", it->id, it->title, it->option);
	}

	//find data by id
	go.id = 2;
	std::vector< grid_option >::iterator it_vgos;
	it_vgos = std::find_if(vgos.begin(), vgos.end(), f_grid_option(go.id));

	if (it_vgos != vgos.end()) {
		//get it
		TRACE("find option : id = %d, item0  = %s, item1 = %s\n", it_vgos->id, it_vgos->sz_item[0].i, it_vgos->sz_item[1].i);
	}
	else
	{
		TRACE("find option : Nothing\n");
	}

	//find again
	gi.id = 2;
	it = std::find_if(vgis.begin(), vgis.end(), f_grid_ini(gi.id));

	if (it != vgis.end()) {
		//get it
		TRACE("find data : id = %d, titile  = %s, option = %d\n", it->id, it->title, it->option);

		//modify data
		it->id = 77;
		sprintf(it->title, "%s", "update77");
		it->option = 77;

		TRACE("after modify data : id = %d, titile  = %s, option = %d\n", it->id, it->title, it->option);
	}
	else
	{
		TRACE("Find nothing !!\n");
	}
}


void CMFCCustomDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	TRACE("Size = %d\n", vgis.size());
	//show all vector content
	CString s = "";
	std::vector< grid_ini >::iterator it;
	for (it = vgis.begin(); it != vgis.end(); it++)
	{
		s.Format("%sid = %d, title = %s, value = %d\n",
			s, it->id, it->title, it->option);
	}
	 
	CEdit* wEdit;
	wEdit = (CEdit*)GetDlgItem(IDC_STATIC);
	wEdit->SetWindowText(s); //設定
}

//---------------------------------------------------------------------------------------
void random_find_data()
{
	grid_ini gi;
	grid_option go;
	::ZeroMemory(&gi, sizeof(grid_ini));
	::ZeroMemory(&go, sizeof(grid_option));

	std::random_device rd;
	//std::mt19937_64 generator(rd());
	std::default_random_engine gen = std::default_random_engine(rd());
	std::uniform_int_distribution<int> dis(0, ITEM_LIMIT -1);
	int iRanSec = dis(gen);

	TRACE("Random = %d\n", iRanSec);
	//find option by id
	gi.id = iRanSec;
	std::vector< grid_ini >::iterator it;
	it = std::find_if(vgis.begin(), vgis.end(), f_grid_ini(gi.id));

	//find data by id
	go.id = iRanSec;
	std::vector< grid_option >::iterator it_vgos;
	it_vgos = std::find_if(vgos.begin(), vgos.end(), f_grid_option(go.id));

	if (it != vgis.end()) {
		//get it
		TRACE("find data : id = %d, titile  = %s, option = %d, type = %d\n",
			it->id, it->title, it->option, it->type);

		if (it->type == 1) {
			//show char
			TRACE("char Value = %s\n", it->sz_v.v);

			if (it_vgos != vgos.end()) {
				//get option value 
				TRACE("find option : id = %d, item0  = %s, item1 = %s, item2 = %s\n",
					it_vgos->id, it_vgos->sz_item[0].i, it_vgos->sz_item[1].i, it_vgos->sz_item[2].i);
			}
			else
			{
				TRACE("X, find option : id = %d\n", go.id);
			}
		}
		else
		{
			//show int
			TRACE("int Value = %d\n", it->int_v.v);


			if (it_vgos != vgos.end()) {
				//get option value 
				TRACE("find option : id = %d, item0  = %d, item1 = %d, item2 = %d\n",
					it_vgos->id, it_vgos->int_item[0].i, it_vgos->int_item[1].i, it_vgos->int_item[2].i);
			}
			else
			{
				TRACE("X, find option : id = %d\n", go.id);
			}
		}
	}
	else
	{
		TRACE("X, find id = %d\n", gi.id);
	}
}


//---------------------------------------------------------------------------------------
void CMFCCustomDlg::create_Ini_Data()
{
	grid_ini gi;
	grid_option go;


	vgis.clear();
	vgos.clear();
	//init data 
	int int_item_base = 9900;
	for (int i = 0; i < ITEM_LIMIT; i++)
	{
		::ZeroMemory(&gi, sizeof(grid_ini));
		::ZeroMemory(&go, sizeof(grid_option));

		gi.id = i; //id 0~ max
		go.id = i;//grid_option id
		if (i % 2 == 0) {
			//char part
			//grid_ini

			gi.option = 1;
			gi.type = 0;//char
			sprintf(gi.title, "char_%03d", i);
			sprintf(gi.sz_v.v, "szValue_%03d", i);

			//grid_option
			//go.id = i;
			sprintf(go.sz_item[0].i, "szItem0A_%03d", i);
			sprintf(go.sz_item[1].i, "szItem1A_%03d", i);
			sprintf(go.sz_item[2].i, "szItem2B_%03d", i);
		}
		else if (i % 3 == 0)
		{
			//int part
			gi.option = 1;
			gi.type = 1;//int 
			sprintf(gi.title, "int_%03d", i);
			gi.int_v.v = int_item_base + i * 2;

			go.int_item[0].i = int_item_base + i;
			go.int_item[1].i = int_item_base + i + 1;
			go.int_item[2].i = int_item_base + i + 2;
		}
		else {
			//char part and no subItem
			gi.option = 0; //no sub item
			gi.type = 0;//char
			sprintf(gi.title, "charII_%03d", i);
			sprintf(gi.sz_v.v, "szValue_%03d", i);
		}

		vgis.push_back(gi);
		vgos.push_back(go);
	}
	TRACE("INIT DATA DONE\n");
}

void CMFCCustomDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	create_Ini_Data();

	random_find_data();
}


void CMFCCustomDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	char sz[] = "ABC";

	CStringArray op;
	op.Add(_T("Option 1"));
	op.Add(_T("Option 2"));
	op.Add(_T("Option 3"));

	gridSetCombo(3, sz, op);



}



std::string random_string()
{
	std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	std::random_device rd;
	std::mt19937 generator(rd());

	std::shuffle(str.begin(), str.end(), generator);

	return str.substr(0, 32);    // assumes 32 < number of characters in str         
}

void CMFCCustomDlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here

	//generate random log string
	CCWLog cw0;
	std:string sRan;
	char szbuff[256];
	for (UINT i = 0; i < 1024 * 1024 * 1024; i++)
	{
		sRan =  random_string();
		snprintf(szbuff, sizeof(szbuff), "%u - %s", i, sRan.c_str());
		cw0.WLog(szbuff);
		//std::string buffAsStdStr = buff;
	}
	//...........................................
	
	//create With Full path
	CCWLog cw1("E:\\win7_utility\\TEST.log");
	cw1.WLog("CW1 a123");
	std::string s = ("CW1 A123456789 123456789");
	cw1.WLog(s.c_str()); //input std::string

	//create with current path and default log name(current date)
	CCWLog cw2;
	cw2.WLog("cw2 a123");
	std::string s1 = ("CW2 A123456789 123456789");
	cw2.WLog((char*)s1.c_str());

	//create with current path and log name = "TEST"
	CCWLog cw3("TEST.log");
	cw3.WLog("cw3 12"); //input char


}
