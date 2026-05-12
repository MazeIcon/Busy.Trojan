#include "resource.h" 

DWORD WINAPI shader1(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	HWND wnd = GetDesktopWindow();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmp = { 40, w, h, 1, 24 };
	PRGBTRIPLE2 rgbtriple;
	while (true) {
		hdc = GetDC(0);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP hbit = CreateDIBSection(hdc, &bmp, 0, (void**)&rgbtriple, 0, 0);
		SelectObject(mdc, hbit);
		BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < w * h; i++) {
			rgbtriple[i].rgbt2Red -= 40;
			rgbtriple[i].rgbt2Green += 35;
			rgbtriple[i].rgbt2Blue -= 60;
		}
		BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
		ReleaseDC(wnd, hdc);
		DeleteDC(hdc); DeleteDC(mdc); DeleteObject(hbit); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&w); DeleteObject(&h); DeleteObject(&bmp);
	}
}

DWORD WINAPI shader2(LPVOID lpParam)
{
	HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	BITMAPINFO bmi = { 40, sw, sh, 1, 24 };
	PRGBTRIPLE2 rgbtriple;
	for (;;) {
		desk = GetDC(0);
		HDC deskMem = CreateCompatibleDC(desk);
		HBITMAP scr = CreateDIBSection(desk, &bmi, 0, (void**)&rgbtriple, 0, 0);
		SelectObject(deskMem, scr);
		BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
		for (int i = 0; i < sw * sh; i++) {
			rgbtriple[i].rgbt2Red = (rgbtriple[i].rgbt2Red * 1) % (RGB(10, 0, 240));
			rgbtriple[i].rgbt2Green = (rgbtriple[i].rgbt2Green * 2) % (RGB(50, 0, 5));
			rgbtriple[i].rgbt2Blue = (rgbtriple[i].rgbt2Blue * 3) % (RGB(0, 60, 25));
		}
		BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);
		ReleaseDC(wnd, desk);
		DeleteDC(desk); DeleteDC(deskMem); DeleteObject(scr); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&bmi);
	}
}

DWORD WINAPI shader3(LPVOID lpParam) {
	int time = GetTickCount();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(NULL);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;
		BYTE byte = 0;
		if ((GetTickCount() - time) > 60000)
			byte = rand() % 0xff;
		for (int i = 0; w * h > i; i++) {
			int x = i % w, y = i / h;
			if (i % h == 0 && rand() % 100 == 0)
				v = rand() % 2;
			*((BYTE*)data + 4 * i + v) += x ^ y | x & y;
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
		DeleteObject(hbm);
		DeleteObject(hdcdc);
		DeleteObject(desk);
	}
	return 0;
}

DWORD WINAPI shader4(LPVOID lpvd) //credits to fr4ctalz
{
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				FLOAT fx = ((x + (i * 8)) - (y ^ (i & 8))) ^ ((x << 2) & (y >> 1));
				FLOAT fx2 = ((x + (i * 8)) + (y ^ (i & 8))) & ((x << 2) & (y >> 1));

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				
				hslcolor.h += fmod(fx / 1500.f + y / h * .2f, .7f);
				hslcolor.s = fmod(fx2 / 120.f + y / h * .1f, 1.5f);
				hslcolor.s += 1.f;

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}

	return 0x00;
}

DWORD WINAPI shader4dot1(LPVOID lpParam) {
	HDC hdc;
	int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	float ri = 5.0893, ro = 2.8946, ax = 0.5, ay = 2.0286, bx = 2.8099, by = 3.4485, cx = 1.3855, cy = 1.5639, dx = 3.2052, dy = 2.2124, ex = 1.9535, ey = 0.7409, fx = 3.8662, fy = 0.4694, gx = 2.0741, gy = 0.2518, hx = 3.6415, hy = 1.381, ix = 1.7195, iy = 1.1869, jx = 7.5826, jy = 2.9152, kx = 0.9709, ky = 1.85, lx = 0.932, ly = 3.7814;
	for (;;) {
		hdc = GetWindowDC(0);
		int x = rand() % w, y = rand() % h, z = 1 + rand() % 128, m = rand() % 256, n = rand() % 256, o = rand() % 256;
		POINT point[26] = { x,y + (ro * z),x + (ax * z),y + (ay * z),x + (bx * z),y + (by * z),x + (cx * z),y + (cy * z),x + (dx * z),y + (dy * z),x - (ex * z),y + (ey * z),x + (fx * z),y + (fy * z),x - (gx * z),y + (gy * z),x - (hx * z),y - (hy * z),x - (ix * z),y - (iy * z),x - (jx * z),y - (jy * z),x - (kx * z),y - (ky * z),x - (lx * z),y - (ly * z),x,y - (ri * z),x + (lx * z),y - (ly * z),x + (kx * z),y - (ky * z),x + (jx * z),y - (jy * z),x + (ix * z),y - (iy * z),x + (hx * z),y - (hy * z),x + (gx * z),y - (gy * z),x + (fx * z),y + (fy * z),x + (ex * z),y + (ey * z) ,x + (dx * z),y + (dy * z),x + (cx * z),y + (cy * z),x + (bx * z),y + (by * z),x + (ax * z),y + (ay * z) };
		SelectObject(hdc, CreateSolidBrush(RGB(m, n, o)));
		Polygon(hdc, point, 26);
		DeleteObject;
		ReleaseDC(NULL, hdc);
		Sleep(1);
	}
	return 0;
}

DWORD WINAPI shader5(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / h, t = y ^ y | x;
            data[i].r += i;
            data[i].g ^= i;
            data[i].b -= i;
            data[i].rgb += 1500 | i + i;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, desk);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI shader6(LPVOID lpParam) { // by the no skid
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    int a = 0;
    int b = 0;
    int c = 0;
    while (1) {
        HDC hdc = GetDC(0);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 1000, rand() % 1000, rand() % 1000));
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, 0, w, h, hdc, -c, b, 0x987654);
        a += 3;
        b = sin(a / 100.f) * 10;
        c = cos(a / 100.f) * b;
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shader6dot1(LPVOID lpParam) {
    int sw = GetSystemMetrics(0);
    int sh = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        POINT p[3] = { rand() % sw, rand() % sh, rand() % sw, rand() % sh,  rand() % sw, rand() % sh };
        HPEN hPen = CreatePen(PS_DASH, 10, RGB(rand() % 1000, rand() % 1000, rand() % 1000));
        SelectObject(hdc, hPen);
        Polygon(hdc, p, 3);
        DeleteObject(hPen);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
}

DWORD WINAPI shader7(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 30;
	DOUBLE angle = 3240.f;

	int ws = w / 2;
	int hs = h / 2;

	DWORD startTime = GetTickCount();

	while (true)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCAND);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				int cx = (x - (w / 245));
				int cy = (y - (h / 245));

				int zx = cos(angle) * cx - sin(angle) * cy;
				int zy = sin(angle) * cx + cos(angle) * cy;

				int fx = ((zx + i) ^ (zy - i)) / 500;
				int fx2 = ((zx - i) & (zy + i)) / 500;

				rgbquad[index].rgbRed -= (fx - 46);
				rgbquad[index].rgbGreen -= (fx + 50);
				rgbquad[index].rgbBlue += (fx + 96);
				rgbquad[index].rgbRed += (fx2 - 50);
				rgbquad[index].rgbGreen -= (fx2 + 75);
				rgbquad[index].rgbBlue ^= (fx2 + 99);
			}
		}

		i++; angle += 200.34701f;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, ws, hs, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0;
}

DWORD WINAPI shader8(LPVOID lpParam) {
while (1) {
		HDC hdc = GetDC(HWND_DESKTOP);
		int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN);
		int a = rand() % sw, b = rand() % sh;
		BitBlt(hdc, rand() % 5, rand() % 5, rand() % sw, rand() % sh, hdc, rand() % 5, rand() % 5, 0x999999);
		BitBlt(hdc, a, b, 200, 200, hdc, a + rand() % 21 - 10, b + rand() % 21 - 10, SRCAND);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI mama(LPVOID lpParam) { //idk fuck?
    HDC desk = GetDC(0); //why 2

    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);

    while (1) {

        desk = GetDC(0); //why 2?

        int radius = 5, rx = rand() % sw, ry = rand() % sh, num = rand() % 30;

        for (int t = 0; t < sw + sh; ++t) {
            // Very cool float, ngl LMFAOOOO
            int x = (int)(float)(radius + t * tan(t + radius * 10) + rx),

                y = (int)(float)(radius + t * cos(t + radius * 10) + ry);

            if (rand() % 5 == 0) { DrawIcon(desk, x, y, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2))); Sleep(0.99); }

            if (rand() % 5 == 0) { DrawIcon(desk, x, y, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2))); Sleep(0.99); }

            if (rand() % 5 == 0) { DrawIcon(desk, x, y, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2))); Sleep(0.99); }

            if (rand() % 5 == 0) { DrawIcon(desk, x, y, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2))); Sleep(0.99); }

            if (rand() % 5 == 0) { DrawIcon(desk, x, y, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2))); Sleep(0.99); }

        }

        InvalidateRect(0, 0, 0);

    }

}

DWORD WINAPI shader9(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);

		// Increase the random displacements to intensify the shake effect
		int shakeIntensity = 10; // This is the increased intensity

								// BitBlt with random offsets, now within the range of -5 to +5
		BitBlt(hdc, rand() % (shakeIntensity * 2) - shakeIntensity, rand() % (shakeIntensity * 2) - shakeIntensity,
			w, h, hdc, rand() % (shakeIntensity * 2) - shakeIntensity, rand() % (shakeIntensity * 2) - shakeIntensity, NOTSRCCOPY);

		Sleep(10);  // Delay between shake movements
		ReleaseDC(0, hdc);
	}
	return 0x00;
}

DWORD WINAPI shader10(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;
    DOUBLE angle = 0.37f;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int cx = (x - (w / 3.4));
                int cy = (y - (h / 1.5));

                int zx = tan(angle) * cx - sin(angle) * cy;
                int zy = sin(angle) * cx + tan(angle) * cy;

                INT fx = (zx - i) ^ (zy + i) + 7 | (zx - i) & (zy + i) + 5;
                INT fx2 = (zx + i | zy - i) ^ (zy + i | zx - i) + 7 | (zx + i | zy + i) & (zy + i | zx + i) + 5;

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);

				         hslcolor.h += fmod(fx / 1500.f + y / h * .2f, .7f);
				         hslcolor.s = fmod(fx2 / 120.f + y / h * .1f, 1.5f);
                hslcolor.h += (FLOAT)fmod((DOUBLE)hslcolor.h + (DOUBLE)(fx) / 10000.0 + 0.09, 1.0);
                hslcolor.h = (FLOAT)fmod((DOUBLE)hslcolor.h + (DOUBLE)(fx2) / 10000.0 + 0.09, 1.0);
                hslcolor.s += 1.f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        StretchBlt(hdc, -i, -i, w + i * 2, h + i * 2, hdc, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
}

DWORD WINAPI shader11(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 222;
	DOUBLE angle = 66701.f;

	DWORD startTime = GetTickCount();

	while (true)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;

				int cx = (x - (screenWidth / 132));
				int cy = (y - (screenHeight / 123));

				int fx = (sqrt((x - (i * 428)) ^ (y + (i * 392))) * 1382) / 2900;
				int fx1 = (sqrt((x + (i * 132)) & (y - (i * 224))) * 114) / 412;
				int fx2 = (sqrt((x - (i * 1124)) + (y + (i * 339))) * 730) / 241;
				int fx3 = (sqrt((x + (i * 542)) - (y - (i * 392))) * 1832) / 2900;

				rgbquad[index].rgbRed ^= fx;
				rgbquad[index].rgbGreen += fx1;
				rgbquad[index].rgbBlue -= fx2;
				rgbquad[index].rgbRed += fx;
				rgbquad[index].rgbGreen -= fx3 + 1000;
				rgbquad[index].rgbBlue += fx2;
				rgbquad[index].rgbRed ^= fx3;
				rgbquad[index].rgbGreen += fx1 + 500;
				rgbquad[index].rgbBlue -= fx2;
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0;
}