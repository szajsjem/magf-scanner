#pragma once
#include "renderincl.h"

class punkt;
class okno3d;
class obrot {
protected:
	friend punkt;
	friend okno3d;
	friend oknort;
	friend oknocl;
	flpo calx = 0;
	flpo caly = 0;
	flpo calz = 0;
	flpo sinx = 0;
	flpo cosx = 1;
	flpo siny = 0;
	flpo cosy = 1;
	flpo sinz = 0;
	flpo cosz = 1;
public:
	void update() {
		if (x != calx) {
			sinx = sin(x);
			cosx = cos(x);
			calx = x;
		}

		if (y != caly) {
			siny = sin(y);
			cosy = cos(y);
			caly = y;
		}

		if (z != calz) {
			sinz = sin(z);
			cosz = cos(z);
			calz = z;
		}
	}
	flpo x = 0;
	flpo y = 0;
	flpo z = 0;
};

punkt obr(punkt a, int os, flpo kat);
punkt obr(punkt a, int os, flpo sina, flpo cosa);

class punkt {
public:
	flpo x = 0;
	flpo y = 0;
	flpo z = 0;
	flpo odleglosc(const punkt b) {
		return sqrt(odleglosckw(b));

	}
	flpo odleglosckw(const punkt b) {
		flpo ox = (this->x - b.x);
		flpo oy = (this->y - b.y);
		flpo oz = (this->z - b.z);
		return (ox*ox) + (oy*oy) + (oz*oz);

	}
	punkt operator+(const punkt a) {
		punkt w;
		w.x = x + a.x;
		w.y = y + a.y;
		w.z = z + a.z;
		return w;
	}
	punkt operator-(const punkt a) {
		punkt w;
		w.x = x - a.x;
		w.y = y - a.y;
		w.z = z - a.z;
		return w;
	}
	punkt operator/(const punkt a) {
		punkt w;
		w.x = x / a.x;
		w.y = y / a.y;
		w.z = z / a.z;
		return w;
	}
	punkt operator*(const punkt a) {
		punkt w;
		w.x = x * a.x;
		w.y = y * a.y;
		w.z = z * a.z;
		return w;
	}
	punkt operator*(const flpo a) {
		punkt w;
		w.x = x * a;
		w.y = y * a;
		w.z = z * a;
		return w;
	}
	punkt operator/(const flpo a) {
		punkt w;
		w.x = x / a;
		w.y = y / a;
		w.z = z / a;
		return w;
	}
	punkt& operator*=(const punkt a) {
		x *= a.x;
		y *= a.y;
		z *= a.z;
		return *this;
	}
	punkt& operator/=(const punkt a) {
		x /= a.x;
		y /= a.y;
		z /= a.z;
		return *this;
	}
	punkt& operator*=(const flpo a) {
		if (a == 1)return *this;
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}
	punkt& operator/=(const flpo a) {
		if (a == 1)return *this;
		x /= a;
		y /= a;
		z /= a;
		return *this;
	}
	void ppp(const flpo a) {
		if (a == 1)return;
		x /= a;
		y /= a;
	}
	punkt& operator+=(const punkt a) {
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	punkt& operator-=(const punkt a) {
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}
	bool operator!=(const punkt o) {
		if (abs(x - o.x) > 0.0000001 || abs(y - o.y) > 0.0000001 || abs(z - o.z) > 0.0000001)
			return 1;
		return 0;
	}
	bool operator==(const punkt o) {
		if (abs(x - o.x) < 0.0000001 && abs(y - o.y) < 0.0000001 && abs(z - o.z) < 0.0000001)
			return 1;
		return 0;
	}
	void obroc(int os, flpo kat) {
		if (kat == 0.0)return;
		*this = obr(*this, os, kat);
	}
	void obrocp(int os, flpo kat) {
		*this = obr(*this, os, kat);
	}
	void obrocopt(int os, flpo sina, flpo cosa) {
		*this = obr(*this, os, sina, cosa);
	}
	void obroc(obrot a) {
		a.update();
		if (a.z != 0)obrocopt(os::Z, a.sinz, a.cosz);
		if (a.x != 0)obrocopt(os::X, a.sinx, a.cosx);
		if (a.y != 0)obrocopt(os::Y, a.siny, a.cosy);
	}
	flpo dot(const punkt o) {
		return (x * o.x) + (y * o.y) + (z * o.z);
	}
	punkt cross(const punkt o) {
		punkt result;
		result.x = y * o.z - z * o.y;
		result.y = z * o.x - x * o.z;
		result.z = x * o.y - y * o.x;
		return result;
	}
	void normalize() {
		flpo o = sqrt((x * x) + (y * y) + (z * z));
		x /= o;
		y /= o;
		z /= o;
	}
	flpo kwodl() {
		return sqrt((x * x) + (y * y) + (z * z));
	}
};


punkt obr(punkt a, int os, flpo kat) {
	punkt w;
	flpo cosTheta = cos(kat);
	flpo sinTheta = sin(kat);
	switch (os)
	{
	case(Z):
		w.x = a.x*(cosTheta)+a.y*(-sinTheta);
		w.y = a.y*(cosTheta)+a.x*(sinTheta);
		w.z = a.z;
		break;
	case(X):
		w.x = a.x;
		w.y = a.y*(cosTheta)+a.z*(-sinTheta);
		w.z = a.z*(cosTheta)+a.y*(sinTheta);
		break;
	case(Y):
		w.x = a.x*(cosTheta)+a.z*(sinTheta);
		w.y = a.y;
		w.z = a.z*(cosTheta)+a.x*(-sinTheta);
		break;
	default:
		break;
	}
	return w;
}

punkt obr(punkt a, int os, flpo sina, flpo cosa) {
	punkt w;
	flpo cosTheta = cosa;
	flpo sinTheta = sina;
	switch (os)
	{
	case(Z):
		w.x = a.x*(cosTheta)+a.y*(-sinTheta);
		w.y = a.y*(cosTheta)+a.x*(sinTheta);
		w.z = a.z;
		break;
	case(X):
		w.x = a.x;
		w.y = a.y*(cosTheta)+a.z*(-sinTheta);
		w.z = a.z*(cosTheta)+a.y*(sinTheta);
		break;
	case(Y):
		w.x = a.x*(cosTheta)+a.z*(sinTheta);
		w.y = a.y;
		w.z = a.z*(cosTheta)+a.x*(-sinTheta);
		break;
	default:
		break;
	}
	return w;
}
























