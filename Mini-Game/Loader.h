#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <string>
#include <thread>
#include <map>
#include <mutex>
#include <GL/glut.h>
#include "SOIL.h"
using namespace std;

template <class T>
class Loader
{
public:
	Loader() {}

	void LoadData(std::string image, T* DP)
	{
		path = image;
		DataPtr = DP;
		LoadDataPriv();
	}

	std::string path;

	map<string, GLuint> MtlToTex;
	map<int, GLuint> TexRange;
	T* DataPtr;
	void loadmtl(string file) {
		ifstream in(file);
		if (in.is_open())
		{
			string t, object, photo;
			GLuint y;
			while (!in.eof())
			{
				getline(in, t);
				if (t.find("newmtl") != string::npos)
					object = t.substr(7);
				else if (t.find("map_Ka") != string::npos || t.find("map_Kd") !=
					string::npos)
				{
					if (t[7] == ' ')
						photo = t.substr(8);
					else
						photo = t.substr(7);
					char*u = new char[photo.length() + 1];
					strcpy(u, photo.c_str());
					y = LoadTexture(u);
					delete[] u;
					MtlToTex[object] = y;
				}
			}
			in.close();
		}
		else
			cout << file << " is a bad file\n";
	}

	GLuint LoadTexture(char* ImageName)
	{
		cout << ImageName << endl;
		GLuint texture;
		texture = SOIL_load_OGL_texture
			(
				ImageName,
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
				SOIL_FLAG_COMPRESS_TO_DXT);
		if (!texture)
			cerr << "TEXTURE FILE COULD NOT BE LOADED!\n";

		//glBindTexture(GL_TEXTURE_2D, texture);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return texture;
	}

	void LoadDataPriv()
		{
			std::ifstream in(path);

			if (in.is_open())
			{
				vector<thread> par;
				while (in.good())
				{
					std::string Line;
					double vectors[3]; char input;
					//Extract First Line
					getline(in, Line);
					std::stringstream DataStream(Line);
					DataStream >> input;
					if (Line.find("mtllib") != std::string::npos)
						//par.push_back(thread([&]() { loadmtl(Line.substr(7)); }));
						loadmtl(Line.substr(7));
					else if (Line.find("usemtl") != std::string::npos)
						TexRange[DataPtr->faces.size()] = MtlToTex[Line.substr(7)];
					if (Line[0] == 'v')
					{
						// cout << " v\n";
						if (Line[1] == 'n')
						{
							DataStream >> input;
							DataStream >> vectors[0] >> vectors[1] >> vectors[2];
							DataPtr->Vn.push_back(vectors);
						}
						else if (Line[1] == 't')
						{
							DataStream >> input;
							DataStream >> vectors[0] >> vectors[1] >> vectors[2];
							vectors[2] = 0;
							DataStream >> vectors[2];
							DataPtr->Vt.push_back(vectors);
						}
						else
						{
							DataStream >> vectors[0] >> vectors[1] >> vectors[2];
							DataPtr->V.push_back(vectors);
							DataPtr->maxx = max(DataPtr->maxx, vectors[0]);
							DataPtr->maxy = max(DataPtr->maxy, vectors[1]);
							DataPtr->maxz = max(DataPtr->maxz, vectors[2]);
							DataPtr->minx = min(DataPtr->minx, vectors[0]);
							DataPtr->miny = min(DataPtr->miny, vectors[1]);
							DataPtr->minz = min(DataPtr->minz, vectors[2]);
						}
					}
					else if (Line[0] == 'f')
					{
						std::vector<std::vector<int>> Face;
						std::vector<int> Point;
						std::string Data = "";

						for (unsigned int i = 2; i < Line.size(); i++)
							if (Line[i] == '/')
							{
								if (Data.size())
									Point.push_back(stoi(Data) - 1);
								else
									Point.push_back(-1);

								Data.clear();
							}
							else if (Line[i] == ' ')
							{
								Point.push_back(stoi(Data) - 1);
								if (Point.size() == 1)
									Point.push_back(-1);
								if (Point.size() == 2)
									Point.push_back(-1);
								Face.push_back(Point);
								Data.clear();
								Point.clear();
							}
							else if (i == Line.size() - 1)
							{
								if (Line[i] != '/')
								{
									Data.push_back(Line[i]);
									Point.push_back(stoi(Data) - 1);
									if (Point.size() == 1)
										Point.push_back(-1);
									if (Point.size() == 2)
										Point.push_back(-1);
									Face.push_back(Point);
								}
								DataPtr->faces.push_back(Face);
							}
							else
								Data.push_back(Line[i]);
					}
				}
				DataPtr->aspect = abs(DataPtr->maxx - DataPtr->minx) / abs(DataPtr->maxy -
					DataPtr->miny);
				DataPtr->center = { DataPtr->maxx - DataPtr->minx,DataPtr->maxy -
					DataPtr->miny,DataPtr->maxz - DataPtr->minz };
			}
			else
				printf("Bad file!\n");
		}
};

#endif