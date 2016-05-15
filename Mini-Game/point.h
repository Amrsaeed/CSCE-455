#ifndef POINT_H
#define POINT_H

class point {
private:
	double x;
	double y;
	double z;
public:
	point(double Points[3]) :x(Points[0]), y(Points[1]), z(Points[2]) {} //Construct From Array
	point(double PointX = 0, double PointY = 0, double PointZ = 0) :x(PointX), y(PointY), z(PointZ) {} //Constructor with 3 arguments
	point(point PointA, point PointB) :x(PointB[0] - PointA[0]), y(PointB[1] - PointA[1]), z(PointB[2] - PointA[2]) {} //Merge two points into one

	double operator[](int index)
	{
		if (index == 0)
			return x;

		if (index == 1)
			return y;

		if (index == 2)
			return z;

		return -1;
	}

	point& point::operator=(const point& a)
	{
		x = a.x;
		y = a.y;
		z = a.z;

		return *this;
	}

	bool operator==(const point& a)
	{
		if (x != a.x)
			return false;

		if (y != a.y)
			return false;

		if (z != a.z)
			return false;

		return true;
	}

	bool operator!=(const point&a)
	{
		return !(*this == a);
	}

	double* point::data()
	{
		double a[] = { x,y,z };
		return a;
	}

	/*void print() {
	cout << x << " " << y << " " << z << endl;
	}*/

};

#endif