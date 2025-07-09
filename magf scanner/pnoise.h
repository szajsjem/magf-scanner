#pragma once
#include <math.h>

class pnoise {
	int p[512];
public:
	pnoise(unsigned int seed = 0) {
		for (int i = 0; i < 256; i++)p[i] = i;
		for (int i = 0; i < 256; i++) {
			seed *= 123456789;
			seed += 12345;
			int t = (seed >> 23) & 0xff;
			int n = p[t];
			p[t] = p[i];
			p[i] = n;
		}
		for (int i = 0; i < 256; i++)p[i + 256] = p[i];
	}
	double noise(double x, double y, double z) {
		// Find the unit cube that contains the point
		int X = (int)floor(x) & 255;
		int Y = (int)floor(y) & 255;
		int Z = (int)floor(z) & 255;

		// Find relative x, y,z of point in cube
		x -= floor(x);
		y -= floor(y);
		z -= floor(z);

		// Compute fade curves for each of x, y, z
		double u = fade(x);
		double v = fade(y);
		double w = fade(z);

		// Hash coordinates of the 8 cube corners
		int A = p[X] + Y;
		int AA = p[A] + Z;
		int AB = p[A + 1] + Z;
		int B = p[X + 1] + Y;
		int BA = p[B] + Z;
		int BB = p[B + 1] + Z;

		// Add blended results from 8 corners of cube
		double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)), lerp(u, grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z))), lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), grad(p[BA + 1], x - 1, y, z - 1)), lerp(u, grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1))));
		return (res + 1.0) / 2.0;
	}
	double noise(double x, double y) {
		// Find the unit square that contains the point
		int X = (int)floor(x) & 255;
		int Y = (int)floor(y) & 255;

		// Find relative x, y of point in square
		x -= floor(x);
		y -= floor(y);

		// Compute fade curves for each of x, y
		double u = fade(x);
		double v = fade(y);

		// Hash coordinates of the 4 square corners
		int A = p[X] + Y;
		int B = p[X + 1] + Y;

		// Add blended results from 4 corners of square
		double res = lerp(
			v,
			lerp(u, grad(p[A], x, y), grad(p[B], x - 1, y)),
			lerp(u, grad(p[A + 1], x, y - 1), grad(p[B + 1], x - 1, y - 1))
		);

		return (res + 1.0) / 2.0;
	}
	float lin_inter(float x, float y, float s)
	{
		return x + s * (y - x);
	}

	float smooth_inter(float x, float y, float s)
	{
		return lin_inter(x, y, s * s * (3 - 2 * s));
	}
	int noise3(int x, int y, int z)
	{
		int tmp = p[z];
		tmp = p[y + tmp];
		return p[tmp + x];
	}
	double noise3dn(double x, double y, double z) {
		// Find the unit cube that contains the point
		int X = x;
		int Y = y;
		int Z = z;

		// Find relative x, y,z of point in cube
		x -= X;
		y -= Y;
		z -= Z;

		X &= 255;
		Y &= 255;
		Z &= 255;

		// Compute fade curves for each of x, y, z
		double a = noise3(X, Y, Z);
		double b = noise3(X + 1, Y, Z);
		double c = noise3(X, Y + 1, Z);
		double d = noise3(X + 1, Y + 1, Z);
		double e = noise3(X, Y, Z + 1);
		double f = noise3(X + 1, Y, Z + 1);
		double g = noise3(X, Y + 1, Z + 1);
		double h = noise3(X + 1, Y + 1, Z + 1);

		// Hash coordinates of the 8 cube corners
		double ae = smooth_inter(a, e, z);
		double bf = smooth_inter(b, f, z);
		double cg = smooth_inter(c, g, z);
		double dh = smooth_inter(d, h, z);

		double aecg = smooth_inter(ae, cg, y);
		double bfdh = smooth_inter(bf, dh, y);

		double res = smooth_inter(aecg, bfdh, x);

		return (res + 1.0) / 2.0;
	}
	double fade(double t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	double lerp(double t, double a, double b) {
		return a + t * (b - a);
	}

	double grad(int hash, double x, double y, double z) {
		int h = hash & 15;
		// Convert lower 4 bits of hash into 12 gradient directions
		double u = h < 8 ? x : y,
			v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}

	double grad(int hash, double x, double y) {
		// We only need 2 bits of the hash to select one of 4 gradient vectors.
		int h = hash & 3;
		double u = (h & 1) == 0 ? x : -x;
		double v = (h & 2) == 0 ? y : -y;

		return u + v;
	}
};

class packednoise {
	pnoise* n;
	int octaves;
public:
	double lacunarity;
	double persistence;
	double hob;
	packednoise(double lac, double pers, int oct, double hmin, unsigned long long seed = 2176234) {
		lacunarity = lac;
		persistence = pers;
		octaves = oct;
		hob = hmin;
		n = new pnoise[octaves];
		for (int i = 0; i < octaves; i++) {
			seed *= 123456789;
			seed += 12345;
			n[i] = pnoise((seed >> 30) & 0x7fffffff);
		}
	}
	double noise(double x, double y, double z) {
		double lac = 1;
		double pers = 1;
		double wyn = pers * (n[0].noise(x * lac, y * lac, z * lac) - hob);
		for (int i = 1; i < octaves; i++) {
			lac *= lacunarity;
			pers *= persistence;
			wyn += pers * (n[i].noise(x * lac, y * lac, z * lac) - hob);
		}
		return wyn;
	}
	double noise(double x, double y) {
		double lac = 1;
		double pers = 1;
		double wyn = pers * (n[0].noise(x * lac, y * lac) - hob);
		for (int i = 1; i < octaves; i++) {
			lac *= lacunarity;
			pers *= persistence;
			wyn += pers * (n[i].noise(x * lac, y * lac) - hob);
		}
		return wyn;
	}
};