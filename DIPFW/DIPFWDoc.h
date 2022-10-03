// DIPFWDoc.h : CDIPFWDoc 클래스의 인터페이스
//


#pragma once

typedef unsigned char BYTE;

class CDIPFWDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CDIPFWDoc();
	DECLARE_DYNCREATE(CDIPFWDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();

// 구현입니다.
public:
	virtual ~CDIPFWDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	int	   getWidth();
	int	   getHeight();
	BYTE*  getOpenImg();
	BYTE** getRGBImg();

	void InitImage();
	void ReadyDisplayImage();
	void CopyClipboard(BYTE* m_pImgSrc, int nWidth, int nHeight);
	BYTE** CreateResultImage(int nWidth, int nHeight);
	void CreateNewImage(BYTE*** pImg, int nWidth, int nHeight);
	void ReleaseImage(BYTE** pImg);
	void MakeNewWindow(BYTE* m_pImgResult, int nWidth, int nHeight);
	void MakeNewWindow(BYTE** m_pImgResult, int nWidth, int nHeight);

protected:
	BYTE*	m_pImgOpen1D;	// 1D open image
	BYTE**  m_pImgOpen;		// 2D open image
	BYTE*	m_pImgResult1D;	// for temporary storing
	BYTE**	m_pImgResult;	// for temporary storing	
	BYTE**	m_pImgRGB;		// for display

	int		m_nWidth;		// image width
	int		m_nHeight;		// image height

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnPointprocessingTestcase();
	afx_msg void OnPointprocessingTestcase32789();
	afx_msg void StoreImg1();
	afx_msg void StoreImg2();
	afx_msg void Moravec();
	afx_msg void HOG();
	afx_msg void OnPointprocessingOninverse();
	afx_msg void OnPointprocessingBright();
	afx_msg void OnPointprocessingDark();
};


