// mesh.h: interface for the mesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESH_H__07F55FBE_D4DD_451E_B587_680CFA0DE9C4__INCLUDED_)
#define AFX_MESH_H__07F55FBE_D4DD_451E_B587_680CFA0DE9C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>

using namespace std;

class material
{
public:
	float Ka[4];
	float Kd[4];
	float Ks[4];
	float Ns;		// shiness

	material()
	{ 
		for (int i=0;i<4;i++)
			Ka[i] = Kd[i] = Ks[i] = 1;
		Ns = 0;
	}
};

class mesh  
{
	class Vertex		// �x�s�� vertex �� property
	{
	public:
		int v;		// vertex (index of vList)
		int n;		// normal (index of nList)
		int t;		// texture (index of tList)
		int m;		// material (index of material)
		Vertex() {};
		Vertex(int v_index, int n_index, int t_index=0, int m_index=0)
		{
			v = v_index;
			n = n_index;
			t = t_index;
			m = m_index;
		}
	};

	class Vec3		// vList, nList, tList �� structure
	{
	public:
		GLfloat ptr[3];
		Vec3 (GLfloat *v) 
		{
			for (int i=0;i<3;i++)
				ptr[i] = v[i];
		}
		GLfloat& operator[](int index)
		{
			return ptr[index];
		}
	};

	class FACE		// faceList �� structure
	{
	public:
		Vertex v[3];		// 3 vertex for each face
		FACE (Vertex &v1, Vertex &v2, Vertex &v3) 
		{
			v[0] = v1; 
			v[1] = v2;
			v[2] = v3;
		}
		Vertex& operator[](int index)
		{
			return v[index];
		}
	};

public:
	/////////////////////////////////////////////////////////////////////////////
	// Loading Object
	/////////////////////////////////////////////////////////////////////////////

	FILE	*scene, *texture;
	string  s_file, t_file;
	char	mat_file[50];		// matetial file name

	int		matTotal;	// total material 
	map<string,int> matMap;		// matMap[material_name] = material_ID
	material	mat[100];	// material ID (�C�� mesh �̦h�� 100 �� material)	

	vector<Vec3>	vList;		// Vertex List (Position) - world cord.
	vector<Vec3>	nList;		// Normal List
	vector<Vec3>	tList;		// Texture List
	vector<FACE>	faceList;	// Face List

	int	vTotal, tTotal, nTotal, fTotal;

	void	LoadMesh(string scene_file);
	void	LoadTex(string tex_file);

	mesh();
	mesh(const char* obj_file);
	virtual ~mesh();

	void Init(const char* obj_file);
};

#endif // !defined(AFX_MESH_H__07F55FBE_D4DD_451E_B587_680CFA0DE9C4__INCLUDED_)
