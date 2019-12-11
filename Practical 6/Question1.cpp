#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

float dif[] = { 1.0, 0.0, 0.0 };
float pos[] = { 0.0, 0.0, 0.0 };
int lightSwitch = 0;
int objShape = 0;
float rotate = 0.0;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		else if (wParam == 'W')
			pos[1] += 0.1;
		else if (wParam == 'S')
			pos[1] -= 0.1;
		else if (wParam == 'A')
			pos[0] -= 0.1;
		else if (wParam == 'D')
			pos[0] += 0.1;
		else if (wParam == 'E')
			pos[2] -= 0.1;
		else if (wParam == 'Q')
			pos[2] += 0.1;
		else if (wParam == VK_UP) {
			rotate += 1;
			glRotatef(rotate, 1, 1, 1);
		}
		else if (wParam == VK_DOWN) {
			rotate += 1;
			glRotatef(-(rotate), 1, 1, 1);
		}
		else if (wParam == VK_SPACE) {
			if (lightSwitch == 0)
				lightSwitch = 1;
			else
				lightSwitch = 0;
		}
		else if (wParam == 'O')
			objShape = 0;
		else if (wParam == 'P')
			objShape = 1;

		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------
void sphere() {
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, ambM);
	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_LINE);
	gluSphere(sphere, 0.5, 30, 30);
	gluDeleteQuadric(sphere);
}

void pyramid() {
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0.5, 0);
	glVertex3f(0.5, 0.5, 0);
	glVertex3f(0.5, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, 0);
	glVertex3f(0.25, 0.25, 0.25);
	glVertex3f(0, 0.5, 0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0.5, 0);
	glVertex3f(0.25, 0.25, 0.25);
	glVertex3f(0.5, 0.5, 0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, 0.5, 0);
	glVertex3f(0.25, 0.25, 0.25);
	glVertex3f(0.5, 0, 0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, 0, 0);
	glVertex3f(0.25, 0.25, 0.25);
	glVertex3f(0, 0, 0);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT0);

	if (lightSwitch == 1)
		glDisable(GL_LIGHT0);

	if (objShape == 0) {
		sphere();
	}
	else if (objShape == 1) {
		pyramid();
	}

}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));


	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------

