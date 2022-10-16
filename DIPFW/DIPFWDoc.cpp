// DIPFWDoc.cpp : CDIPFWDoc 클래스의 구현
//

#include "stdafx.h"
#include "DIPFW.h"

#include "DIPFWDoc.h"
#include "InSizeDlg.h"

#include <iostream>
#include "ImgData.h"
#include "Moravec.h"
#include "HOG.h"

#include <random>
#include <vector>


#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

using namespace std;


// CDIPFWDoc

IMPLEMENT_DYNCREATE(CDIPFWDoc, CDocument)

BEGIN_MESSAGE_MAP(CDIPFWDoc, CDocument)	
	ON_COMMAND(ID_POINTPROCESSING_STOREIMG1, &CDIPFWDoc::StoreImg1)
	ON_COMMAND(ID_POINTPROCESSING_STOREIMG2, &CDIPFWDoc::StoreImg2)
	ON_COMMAND(ID_POINTPROCESSING_MORAVEC, &CDIPFWDoc::Moravec)
	ON_COMMAND(ID_POINTPROCESSING_HOG, &CDIPFWDoc::HOG)
	ON_COMMAND(ID_POINTPROCESSING_ONINVERSE, &CDIPFWDoc::OnPointprocessingOninverse)
	ON_COMMAND(ID_POINTPROCESSING_BRIGHT, &CDIPFWDoc::OnPointprocessingBright)
	ON_COMMAND(ID_POINTPROCESSING_DARK, &CDIPFWDoc::OnPointprocessingDark)
	ON_COMMAND(ID_POINTPROCESSING_LPF, &CDIPFWDoc::OnPointprocessingLpf)
	ON_COMMAND(ID_POINTPROCESSING_HPF, &CDIPFWDoc::OnPointprocessingHpf)
	ON_COMMAND(ID_POINTPROCESSING_IMPULSENOISE, &CDIPFWDoc::OnPointprocessingImpulsenoise)
	ON_COMMAND(ID_POINTPROCESSING_MEDIAN32803, &CDIPFWDoc::OnPointprocessingMedian32803)
END_MESSAGE_MAP()


// CDIPFWDoc 생성/소멸

CDIPFWDoc::CDIPFWDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_pImgOpen1D = NULL;
	m_pImgOpen   = NULL;
	m_pImgResult1D = NULL;
	m_pImgResult = NULL;
	m_pImgRGB    = NULL;
}

CDIPFWDoc::~CDIPFWDoc()
{
}

BOOL CDIPFWDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
	::OpenClipboard(NULL);
	if (!IsClipboardFormatAvailable(CF_DIB)) return FALSE;
	HGLOBAL m_hImage = ::GetClipboardData(CF_DIB);
	::EmptyClipboard();
	::CloseClipboard();

	LPSTR pDIB = (LPSTR) ::GlobalLock((HGLOBAL)m_hImage);
	memcpy( &m_nHeight, pDIB,               sizeof(int) );
	memcpy( &m_nWidth,  pDIB+(sizeof(int)), sizeof(int) );

	InitImage();
	memcpy( m_pImgOpen1D, pDIB+(2*sizeof(int)), m_nHeight*m_nWidth );
	
	ReadyDisplayImage();
	return TRUE;
}

void CDIPFWDoc::CopyClipboard(BYTE* m_pImgSrc, int nWidth, int nHeight)
{
	DWORD dwBitsSize = nHeight * nWidth * sizeof(char) + (2*sizeof(int));
	HGLOBAL m_hImage = (HGLOBAL)::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	LPSTR pDIB = (LPSTR)::GlobalLock((HGLOBAL)m_hImage);

	memcpy( pDIB,                 &nHeight, sizeof(int) );
	memcpy( pDIB+(1*sizeof(int)), &nWidth,  sizeof(int) );
	memcpy( pDIB+(2*sizeof(int)), m_pImgSrc, nHeight*nWidth );

	::OpenClipboard(NULL);
	::SetClipboardData(CF_DIB, m_hImage);
	::CloseClipboard();

	::GlobalUnlock((HGLOBAL)m_hImage);
	GlobalFree(m_hImage);
}

// CDIPFWDoc serialization

void CDIPFWDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		//File* fp = ar.GetFile();
		ar.Write(m_pImgOpen1D, m_nWidth*m_nHeight);
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.		
		CInSizeDlg InsizeDlg;						// Size Dialog instance.

		if (InsizeDlg.DoModal() == IDOK)			// Dialog getting Image Size
		{
			BeginWaitCursor();						// Cursor waiting

			m_nWidth  = InsizeDlg.getWidth();
			m_nHeight = InsizeDlg.getHeight();

 			InitImage();							// Memory Allocation Images(Open,Result)
 			ar.Read(m_pImgOpen1D, m_nWidth*m_nHeight);
 			ReadyDisplayImage();

			UpdateAllViews(NULL);					// Update Document data to View
			EndWaitCursor();						// Cursor return
		}
	}
}

void CDIPFWDoc::DeleteContents()
{
	if (m_pImgOpen)
		delete [] m_pImgOpen;
	if (m_pImgOpen1D)
		delete [] m_pImgOpen1D;
	if (m_pImgResult)
		delete [] m_pImgResult;
	if (m_pImgResult1D)
		delete [] m_pImgResult1D;
	if (m_pImgRGB)
	{
		if (m_pImgRGB[0])
			delete [] (m_pImgRGB[0]);
		delete [] m_pImgRGB;
	}

	CDocument::DeleteContents();
}


// CDIPFWDoc 진단

#ifdef _DEBUG
void CDIPFWDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDIPFWDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDIPFWDoc 명령

int	CDIPFWDoc::getWidth()
{
	return m_nWidth;
}

int	CDIPFWDoc::getHeight()
{
	return m_nHeight;
}

BYTE** CDIPFWDoc::getRGBImg()
{
	return m_pImgRGB;
}

BYTE* CDIPFWDoc::getOpenImg()
{
	return m_pImgOpen1D;
}

void CDIPFWDoc::InitImage()
{
	int iY;
	if (!m_pImgOpen1D)
		m_pImgOpen1D = new BYTE [m_nWidth*m_nHeight];
	
	if (m_pImgOpen)
	{
		if (m_pImgOpen[0])
			delete [] (m_pImgOpen[0]);
		delete [] m_pImgOpen;
	}
	m_pImgOpen = new BYTE* [m_nHeight];
	for(iY = 0; iY < m_nHeight; iY++)
		m_pImgOpen[iY] = m_pImgOpen1D + (m_nWidth * iY);

	if (!m_pImgRGB)
	{
		m_pImgRGB    = new BYTE* [m_nHeight];
		m_pImgRGB[0] = new BYTE  [3*m_nWidth*m_nHeight];
	}	
	for(iY = 1; iY < m_nHeight; iY++)
		m_pImgRGB[iY] = *m_pImgRGB + (3 * m_nWidth * iY);
}

void CDIPFWDoc::ReadyDisplayImage()
{
	int	iY, iX;
	for(iY = 0; iY < m_nHeight; iY++) 
	{
		for(iX = 0; iX < m_nWidth; iX++)
		{
			m_pImgRGB[iY][3*iX+2] = m_pImgRGB[iY][3*iX+1] = m_pImgRGB[iY][3*iX] = m_pImgOpen1D[iY*m_nWidth+iX];
		}
	}
}

BYTE** CDIPFWDoc::CreateResultImage(int nWidth, int nHeight)
{
	if (m_pImgResult1D)
		delete [] m_pImgResult1D;
	if (m_pImgResult)
		delete [] m_pImgResult;

	m_pImgResult1D = new BYTE [nWidth * nHeight];
	m_pImgResult = new BYTE* [nHeight];
	for(int iY = 0; iY < nHeight; iY++)
		m_pImgResult[iY] = m_pImgResult1D + (nWidth * iY);

	return m_pImgResult;
}

void CDIPFWDoc::CreateNewImage(BYTE*** pImg, int nWidth, int nHeight)
{
	(*pImg)    = new BYTE* [nHeight];
	(*pImg)[0] = new BYTE  [nWidth * nHeight];
	for(int iY = 1; iY < nHeight; iY++)
		(*pImg)[iY] = (*pImg)[0] + (nWidth * iY);
}

void CDIPFWDoc::ReleaseImage(BYTE** pImg)
{
	if (pImg[0])
		delete [] (pImg[0]);
	if (pImg)
		delete [] pImg;
}


void CDIPFWDoc::MakeNewWindow(BYTE* m_pImgResult, int nWidth, int nHeight)
{
	CopyClipboard(m_pImgResult, nWidth, nHeight);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void CDIPFWDoc::MakeNewWindow(BYTE** m_pImgResult, int nWidth, int nHeight)
{
	BYTE* pImg = new BYTE [nWidth * nHeight];
	for(int iY = 0; iY < nHeight; iY++)
		memcpy((pImg + iY*nWidth), m_pImgResult[iY], sizeof(BYTE)*nWidth);
	CopyClipboard(pImg, nWidth, nHeight);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
	delete [] pImg;
}

// CDIPFWDoc 알고리즘 구현
Img1_Data data1;	Img2_Data data2;
Moravec moravec;
HOG hog;


// 소녀시대 1, 소녀시대 2 파일 전역변수에 저장.
BYTE img1[256][256];
int height1;	int width1;
void CDIPFWDoc::StoreImg1()
{
	height1 = m_nHeight;
	width1 = m_nWidth;

	for (int y = 0; y < m_nHeight; y++) 
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			img1[y][x] = m_pImgOpen[y][x];
		}
	}

//	MakeNewWindow(img1, m_nWidth, m_nHeight);
}

BYTE img2[256][256];
int height2;	int width2;
void CDIPFWDoc::StoreImg2()
{
	height2 = m_nHeight;
	width2 = m_nWidth;

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			img2[y][x] = m_pImgOpen[y][x];
		}
	}

	//MakeNewWindow(img2, m_nWidth, m_nHeight);
}

// Moravec 알고리즘
void CDIPFWDoc::Moravec()
{
	double _result[256][256] = { 0, };

	moravec.Moravec_Algorithm(img1, _result, m_nHeight, m_nWidth, 3);
	data1.Set_keypoint1(moravec.Select_N(img1, _result, m_nHeight, m_nWidth));
		
	moravec.Moravec_Algorithm(img2, _result, m_nHeight, m_nWidth, 3);
	data2.Set_keypoint2(moravec.Select_N(img2, _result, m_nHeight, m_nWidth));
	
}

// HOG 알고리즘
void CDIPFWDoc::HOG()
{
	//vector<HOG_Data> hog1_img =	hog.HOG_Algorithm(img1, data1.Get_keypoint1(), m_nHeight, m_nWidth);
	hog.HOG_Algorithm(m_nHeight, m_nWidth);
	//hog.HOG_Algorithm(img1, data1.Get_keypoint1(), m_nHeight, m_nWidth);
	//vector<HOG_Data> hog2_img = hog.HOG_Algorithm(img2, data2.Get_keypoint2(), m_nHeight, m_nWidth);
	//hog.HOG_Algorithm(img2, data2.Get_keypoint2(), m_nHeight, m_nWidth);

}


void CDIPFWDoc::OnPointprocessingOninverse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 색 반전
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	int i, j;
	for (i = 0; i < m_nHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			pImg[i][j] = 255 - m_pImgOpen[i][j];
		}
	}
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::OnPointprocessingBright()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 120% 밝게
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	int i, j;
	for (i = 0; i < m_nHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			if (m_pImgOpen[i][j] * 1.2<255) {
				pImg[i][j] = m_pImgOpen[i][j] * 1.2;
				
			}
			else {
				pImg[i][j] = 255;
			}
		}
	}
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::OnPointprocessingDark()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 50만큼 어둡게

	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	int i, j;
	for (i = 0; i < m_nHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			if (m_pImgOpen[i][j] - 50 > 0) {
				pImg[i][j] = m_pImgOpen[i][j] - 50;
			}
			else {
				pImg[i][j] = 0;
			}
		}
	}
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::OnPointprocessingLpf()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// low pass filter
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	int i, j, a, b, sum,con;
	for (i = 0; i < m_nHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			sum = 0;
			for (a = i - 1; a < i + 2; a++) {
				for (b = j - 1; b < j + 2; b++) {
					if (a<0 || b<0 || a>m_nWidth-1 || b>m_nHeight-1) {
						sum += 0;
						continue;
					}
					sum += m_pImgOpen[a][b] / 18;
				}
			}
			pImg[i][j] = (m_pImgOpen[i][j] / 18 * (9)) + sum;
		}
	}
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::OnPointprocessingHpf()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// high pass filter
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	int i, j, a, b, sum;
	for (i = 0; i < m_nHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			sum = (m_pImgOpen[i][j] * (9));
			for (a = i - 1; a < i + 2; a++) {
				for (b = j - 1; b < j + 2; b++) {
					if (a<0 || b<0 || a>m_nWidth - 1 || b>m_nHeight - 1) {
						sum -= 0;
						continue;
					}
					sum -= m_pImgOpen[a][b];
				}
			}
			
			if (sum < 0 ) {
				sum *= -1;
			}
			pImg[i][j] = sum;
		}
	}
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::OnPointprocessingImpulsenoise()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// Impulse noise 
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	int i,j, noise;
	noise = m_nWidth * m_nHeight * 5 / 100;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> disx(0, m_nWidth-1);
	std::uniform_int_distribution<int> disy(0, m_nHeight-1);
	
	for (i = 0; i < m_nHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			pImg[i][j] = m_pImgOpen[i][j];
		}
	}

	for ( i = 0; i < noise; i++)
	{
		if (i%2==0)
		{
			pImg[disx(gen)][disy(gen)]=255;
		}
		else
		{
			pImg[disx(gen)][disy(gen)] = 0;
		}
		
	}

	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::OnPointprocessingMedian32803()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// Median filter
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	int i, j, a, b;
	vector<int> vc;
	for (i = 0; i < m_nHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			vc.clear();
			for (a = i - 1; a < i + 2; a++) {
				for (b = j - 1; b < j + 2; b++) {
					if (a < 0 || b < 0 || a>m_nWidth-1 || b>m_nHeight-1) {
						vc.push_back(0);
						continue;
					}
					vc.push_back(m_pImgOpen[a][b]);
				}
			}
			sort(vc.begin(),vc.end());
			if (vc.size() % 2 == 1) {
				pImg[i][j]=vc[vc.size() / 2];
			}
			else {
				pImg[i][j] = vc[(vc.size()-1) / 2];
			}
		}
	}
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}
