#include <Windows.h>
#include <math.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "global_variables.h"



HWND hwnd; //дескриптор окна

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

void LoadTexture(const char* file_name, unsigned int* target);
void map_init();
void game_init();
void map_show();
void camera_rotation(float xAngle, float zAngle);
void PlayerMove();
void wnd_resize(int x, int y);
void TakeObject();
void Anim_Set(TAnim* an, TObject* obj);
void Anim_Move(TAnim* an);
void Show_Slots(int x, int y, float scale);
void Show_Invent();

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    WNDCLASSEX wcex;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    /* класс окна регистрации */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* создание окна */
    hwnd = CreateWindowEx(0,
        "GLSample",
        "OpenGL Sample",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        500,
        500,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd, nCmdShow);

    /* включите OpenGL для окна */
    EnableOpenGL(hwnd, &hDC, &hRC);

	game_init();


    /* основной цикл программы */
    while (!bQuit)
    {
        /* проверка сообщений */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* обрабатывать или отправлять сообщения */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* Код анимации OpenGL находится здесь */
         
            glPushMatrix();
                PlayerMove();
                map_show();
				Show_Invent();
            glPopMatrix();

            SwapBuffers(hDC);
            Sleep(1);
        }
    }

    /* завершение работы OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* явно уничтожьте окно */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_LBUTTONDOWN:
        TakeObject();
        break;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;

        case VK_CONTROL:
            SetWindowPos(hwnd, 0, 0, 0, 1608, 860, NULL);
            break;
        }
    }
    break;

    case WM_SIZE:
        wnd_resize(LOWORD(lParam), HIWORD(lParam));
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}


void map_init()
{
	for (int i = 0; i < slotsCnt; i++) slots[i].type = -1; //заполнение инвенторя

	srand(5);
	LoadTexture("textures/pole.png", &tex_pole);
	LoadTexture("textures/flower.png", &tex_flower);
	LoadTexture("textures/flower2.png", &tex_flower2);
	LoadTexture("textures/trava.png", &tex_trava);
	LoadTexture("textures/grib.png", &tex_grib);
	LoadTexture("textures/tree.png", &tex_tree);
	LoadTexture("textures/tree2.png", &tex_tree2);
	/*заполнение массива вершин*/
	for (int i = 0; i < mapW; i++) {
		for (int j = 0; j < mapH; j++) {
			map[i][j].x = i;
			map[i][j].y = j;
			map[i][j].z = 0;

			mapTex[i][j].u = i;
			mapTex[i][j].v = j;
		}
	}

	/*заполнения массива индексов*/
	for (int i = 0; i < mapW - 1; i++) {
		int pos = i * mapH;
		for (int j = 0; j < mapH - 1; j++) {
			mapIndex[i][j][0] = pos;
			mapIndex[i][j][1] = pos + 1;
			mapIndex[i][j][2] = pos + 1 + mapH;

			mapIndex[i][j][3] = pos + 1 + mapH;
			mapIndex[i][j][4] = pos + mapH;
			mapIndex[i][j][5] = pos;

			pos++;
		}
	}

	/*заполнение динамического массива объектов*/
	int travaN = 1000;
	int gribN = 40;
	int flowerN = 35;
	objCnt = travaN + gribN + flowerN;
	objMas = (TObject*)realloc(objMas, sizeof(*objMas) * objCnt);
	for (int i = 0; i < objCnt; i++) {
		if (i < travaN) {
			objMas[i].type = rand() % 10 != 0 ? tex_trava :
				(rand() % 2 == 0 ? tex_flower : tex_flower2);
			objMas[i].scale = 0.7 + (rand() % 5) * 0.1;
		}
		else if (i < travaN + gribN) {
			objMas[i].type = tex_grib;
			objMas[i].scale = 0.6;
		}
		else {
			objMas[i].type = rand() % 2 == 0 ? tex_tree : tex_tree2;
			objMas[i].scale = 8 + (rand() % 16);
		}
		objMas[i].x = rand() % (mapW - 1);
		objMas[i].y = rand() % (mapH - 1);
		objMas[i].z = 0;
	}

}

void game_init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.99);

	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 3, 200);

	map_init();
}

void map_show()
{
	//glLoadIdentity();
	//glFrustum(-windKoef, windKoef, -windKoef, windKoef, 3, 200);
	glEnable(GL_DEPTH_TEST);

	if (selectMode)glClearColor(0, 0, 0, 0);
	else glClearColor(0.6, 0.5, 1, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (selectMode) glDisable(GL_TEXTURE_2D);
	else glEnable(GL_TEXTURE_2D);

	Anim_Move(&animation);

	glPushMatrix();
	if (!selectMode) {
		/*земля*/
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_INDEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, map);
		glTexCoordPointer(2, GL_FLOAT, 0, mapTex);
		glColor3f(0.7, 0.7, 0.7);
		glBindTexture(GL_TEXTURE_2D, tex_pole);
		glDrawElements(GL_TRIANGLES, mapIndexCnt, GL_UNSIGNED_INT, mapIndex);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}



	/*объекты*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, tex_points3D);
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coord3D);
	glColor3f(0.7, 0.7, 0.7);

	selectCnt = 0;
	int selectColor = 1;
	for (int i = 0; i < objCnt; i++) {
		if (selectMode) {
			if ((objMas[i].type == tex_tree) || (objMas[i].type == tex_tree2)) continue;

			int radius = 3;
			if ((objMas[i].x > player.x - radius) && (objMas[i].x < player.x + radius) && (objMas[i].y > player.y - radius) && (objMas[i].y < player.y + radius))
			{
				glColor3ub(selectColor, 0, 0);
				selectMas[selectCnt].colorIndex = selectColor;
				selectMas[selectCnt].objIndexMas = i;
				selectColor++;
				selectCnt++;

				if (selectColor >= 255) break;
			}
			else continue;
		}

		glBindTexture(GL_TEXTURE_2D, objMas[i].type);
		glPushMatrix();
		glTranslatef(objMas[i].x, objMas[i].y, objMas[i].z);
		glScalef(objMas[i].scale, objMas[i].scale, objMas[i].scale);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, tex_index3D);
		glPopMatrix();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();
}

void camera_rotation(float xAngle, float zAngle)
{
	player.Zrot += zAngle;
	if (player.Zrot < 0) player.Zrot += 360;
	if (player.Zrot > 360) player.Zrot -= 360;
	player.Xrot += xAngle;
	if (player.Xrot < 0) player.Xrot = 0;
	if (player.Xrot > 180) player.Xrot = 180;
}

void PlayerMove()
{
	if (GetForegroundWindow() != hwnd) return;

	/*управление мышкой*/
	POINT cur;
	GetCursorPos(&cur);
	POINT base{ 804,430 };
	camera_rotation((base.y - cur.y) / 10.0, (base.x - cur.x) / 10.0);
	SetCursorPos(base.x, base.y);

	/*управление w-s-a-d*/
	float radUgol = -player.Zrot / 180 * 3.14;
	float speed = 0.0;
	if (GetKeyState('W') < 0) speed = 0.1;
	if (GetKeyState('S') < 0) speed = -0.1;
	if (GetKeyState('A') < 0) { speed = 0.1; radUgol -= 0.5 * 3.14; }
	if (GetKeyState('D') < 0) { speed = 0.1; radUgol += 0.5 * 3.14; }


	if (speed != 0) {
		player.x += sin(radUgol) * speed;
		player.y += cos(radUgol) * speed;
	}

	glRotatef(-player.Xrot, 1, 0, 0);
	glRotatef(-player.Zrot, 0, 0, 1);
	glTranslatef(-player.x, -player.y, -4);
}

void LoadTexture(const char* file_name, unsigned int* target)
{
	int widht, height, cnt;
	unsigned char* data = stbi_load(file_name, &widht, &height, &cnt, 0);
	glGenTextures(1, target);
	glBindTexture(GL_TEXTURE_2D, *target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, cnt == 4 ? GL_RGBA : GL_RGB, widht, height, 0,
		cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void wnd_resize(int x, int y)
{
	windSize.x = x;
	windSize.y = y;
	glViewport(0, 0, x, y);
	windKoef = x / (float)y;
	glLoadIdentity();
	glFrustum(-windKoef, windKoef, -windKoef, windKoef, 3, 200);
}

void TakeObject()
{
	selectMode = TRUE;
	map_show();
	selectMode = FALSE;


		for (int i = 0; i < selectCnt; i++) {
			if (1 == selectMas[i].colorIndex) {
				Anim_Set(&animation, objMas + selectMas[i].objIndexMas);
			}
		}

}

void Anim_Set(TAnim* an, TObject* obj)
{
	if (an->obj != NULL) return;
	an->obj = obj;
	an->cnt = 10;
	an->dx = (player.x - obj->x) / (float)an->cnt;
	an->dy = (player.y - obj->y) / (float)an->cnt;
	an->dz = (player.z - obj->z) / (float)an->cnt;
}

void Anim_Move(TAnim* an)
{
	if (an->obj != NULL)
	{
		an->obj->x += an->dx;
		an->obj->y += an->dy;
		an->obj->z += an->dz;
		an->cnt--;
		if (an->cnt < 1)
		{
			int i = 0;
			for (; i < slotsCnt; i++) {
				if (slots[i].type == -1) {
					slots[i].type = an->obj->type;
					break;
				}
			}
			if (i < slotsCnt) {
				an->obj->x = rand() % mapW;
				an->obj->y = rand() % mapH;
				an->obj->z = 0;
			}
			an->obj = NULL;
		}
	}
}

void Show_Invent()
{
	glLoadIdentity();
	glOrtho(0, windSize.x,  windSize.y, 0,   -1, 1);
	glDisable(GL_DEPTH_TEST);

	Show_Slots(250, 10, 50);
}

void Show_Slots(int x, int y, float scale)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, slotRect);
	glTexCoordPointer(2, GL_FLOAT, 0, slotRectCoord);

	for (int i = 0; i < slotsCnt; i++) {
		glPushMatrix();
			glTranslatef(x + i*scale, y, 0);
			glScalef(scale, scale, 1);
			glColor3f(0.3, 0.3, 0.3);
			glDisable(GL_TEXTURE_2D);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			if (slots[i].type != -1) {
				glColor3f(1, 1, 1);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, slots[i].type);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}

			glLineWidth(3);
			glColor3f(0.7, 0.7, 0.7);
			glDisable(GL_TEXTURE_2D);
			glDrawArrays(GL_LINE_LOOP, 0, 4);
		glPopMatrix();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}