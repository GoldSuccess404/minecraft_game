#pragma once
#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_

typedef struct
{
	float x, y, z;
}TCell; //структура вершин для поля

typedef struct
{
	float u, v;
}TUV; //структура вершин для координат текстуры поля

typedef struct
{
	float x, y, z;
	float Xrot, Zrot;
}camera; //структура камеры

typedef struct
{
	float x, y, z;
	unsigned int type;
	float scale;
}TObject; //структура объектов

typedef struct
{
	int objIndexMas;
	int colorIndex;
}TSelect; //структура для выбранных объектов

typedef struct
{
	TObject* obj;
	float dx, dy, dz;
	int cnt;
}TAnim; //структура для анимации

typedef struct
{
	int type;
}TSlot;

#define mapW 100 //ширина поля
#define mapH 100 //высота

TCell map[mapW][mapH];  //карта
TUV mapTex[mapW][mapH];  //цвета клеток 99х99

GLuint mapIndex[mapW - 1][mapH - 1][6]; //массив индексов (каждая клетка - два трекголника по 6 вершин)
int mapIndexCnt = sizeof(mapIndex) / sizeof(GLuint);

camera player{50,50,0,30,0}; //игрок

unsigned int tex_pole, tex_trava, tex_grib, tex_tree, tex_tree2, tex_flower, tex_flower2; //текстуры

/*трехмерные текстуры*/
float tex_points3D[]{ -0.5,0,0,  0.5,0,0,  0.5,0,1,  -0.5,0,1,
					0,-0.5,0,  0,0.5,0,  0,0.5,1,  0,-0.5,1 };

float tex_coord3D[]{ 0,1,  1,1,  1,0,  0,0,  0,1,  1,1,  1,0,  0,0 };
GLuint tex_index3D[12]{ 0,1,2,  2,3,0,  4,5,6,  6,7,4 };
/*трехмерные текстуры*/

TObject* objMas = NULL; //динамический массив объектов
int objCnt = 0;  //кол-во объектов в динамическом массиве


BOOL selectMode = FALSE; //режим выбора
TSelect selectMas[255]; //массив выбранных объектов
int selectCnt = 0; //кол-во объектов в массиве выбора

TAnim animation;  //объект для анимации

POINT windSize;  //координаты окна
float windKoef;  //коэффициент w/h

#define slotsCnt 16  //кол-во слотов
TSlot slots[slotsCnt];  //массив слотов

float slotRect[]{ 0,0,  1,0,  1,1,  0,1 };  //массив координат слота
float slotRectCoord[]{ 0,0,  1,0,  1,1,  0,1 };  //массив текстурных координат слота

#endif // !_GLOBAL_VARIABLES_H_

