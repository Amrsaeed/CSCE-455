#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <string>
#include <vector>
#include "Loader.h"
#include "point.h"
#include <GL/glut.h>


class world {
private:
	std::vector<point> V, Vn, Vt;
	std::vector<std::vector<std::vector<int>>> faces;
	template <class T>
	friend class Loader;
	Loader<world> MapLoader;
public:
	world();
	void Load(std::string path);
	void Draw();
	point center;
	double maxx = -89888;
	double maxy = -857633;
	double minx = 9879835;
	double miny = 45645345;
	double maxz = -45645465;
	double minz = 5464563;
	double aspect;
};

#endif