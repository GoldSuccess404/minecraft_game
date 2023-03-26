#pragma once
#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_

typedef struct
{
	float x, y, z;
}TCell; //��������� ������ ��� ����

typedef struct
{
	float u, v;
}TUV; //��������� ������ ��� ��������� �������� ����

typedef struct
{
	float x, y, z;
	float Xrot, Zrot;
}camera; //��������� ������

typedef struct
{
	float x, y, z;
	unsigned int type;
	float scale;
}TObject; //��������� ��������

typedef struct
{
	int objIndexMas;
	int colorIndex;
}TSelect; //��������� ��� ��������� ��������

typedef struct
{
	TObject* obj;
	float dx, dy, dz;
	int cnt;
}TAnim; //��������� ��� ��������

typedef struct
{
	int type;
}TSlot;

#define mapW 100 //������ ����
#define mapH 100 //������

TCell map[mapW][mapH];  //�����
TUV mapTex[mapW][mapH];  //����� ������ 99�99

GLuint mapIndex[mapW - 1][mapH - 1][6]; //������ �������� (������ ������ - ��� ����������� �� 6 ������)
int mapIndexCnt = sizeof(mapIndex) / sizeof(GLuint);

camera player{50,50,0,30,0}; //�����

unsigned int tex_pole, tex_trava, tex_grib, tex_tree, tex_tree2, tex_flower, tex_flower2; //��������

/*���������� ��������*/
float tex_points3D[]{ -0.5,0,0,  0.5,0,0,  0.5,0,1,  -0.5,0,1,
					0,-0.5,0,  0,0.5,0,  0,0.5,1,  0,-0.5,1 };

float tex_coord3D[]{ 0,1,  1,1,  1,0,  0,0,  0,1,  1,1,  1,0,  0,0 };
GLuint tex_index3D[12]{ 0,1,2,  2,3,0,  4,5,6,  6,7,4 };
/*���������� ��������*/

TObject* objMas = NULL; //������������ ������ ��������
int objCnt = 0;  //���-�� �������� � ������������ �������


BOOL selectMode = FALSE; //����� ������
TSelect selectMas[255]; //������ ��������� ��������
int selectCnt = 0; //���-�� �������� � ������� ������

TAnim animation;  //������ ��� ��������

POINT windSize;  //���������� ����
float windKoef;  //����������� w/h

#define slotsCnt 16  //���-�� ������
TSlot slots[slotsCnt];  //������ ������

float slotRect[]{ 0,0,  1,0,  1,1,  0,1 };  //������ ��������� �����
float slotRectCoord[]{ 0,0,  1,0,  1,1,  0,1 };  //������ ���������� ��������� �����

#endif // !_GLOBAL_VARIABLES_H_

