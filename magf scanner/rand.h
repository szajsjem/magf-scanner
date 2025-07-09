#pragma once
#include <time.h>
#include <math.h>
#include <string>
#include <mutex>

class randengine {
	long long sed;
	long long a, b;
	unsigned char c;
public:
	randengine(long long seed=time(NULL),long long mult=123456789,long long add=12345,unsigned char bitsh=45) {
		sed = seed;
		a = mult;
		b = add;
		c = bitsh;
	}
	unsigned short rndi() {//0 - 32767
		sed = sed * a + b;
		unsigned short w = (sed >> c) & 0x7fff;
		return w;
	}
	double rndd() {//0-1
		return ((double)rndi()) / 0x7fff;
	}
	template <class t>
	t* shuffleincrint(t* tab, int il) {
		for (int i = 0; i < il; i++)tab[i] = i;
		shuffle(tab, il);
		return tab;
	}
	template<class t>
	void shuffle(t* tab,int il) {
		for (int i = 0; i < il; i++)std::swap(tab[i], tab[rndi() % il]);
	}
	char*rdst(int l) {
		static const char* sl = "0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";//,./<>?;':\"[] {}-=_+!@#$%^&*()~`\\|
		char *out = new char[l+1L];
		out[l] = 0;
		for (int i = 0; i < l; i++) {
			out[i] = sl[rndi() % 62];
		}
		return out;
	}
	char* rdst(int l, char *out) {
		static const char* sl = "0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
		out[l] = 0;
		for (int i = 0; i < l; i++) {
			out[i] = sl[rndi() % 62];
		}
		return out;
	}
	void morernd(char*seed,int l) {
		int j = l / 3;
		for (int i = 0; i < j; i++) {
			int sh1 = rndi() % 48 + 1;
			int sh2 = rndi() % 48 + 1;
			int sh3 = rndi() % 48 + 1;
			sed ^= ((long long)seed[i * 3] )<< sh1;
			a ^= ((long long)seed[i * 3 + 1] )<< sh2;
			b ^= ((long long)seed[i * 3 + 2] )<< sh3;
		}
		l -= j * 3;
		if (l > 0) {
			for (int i = 0; i < l; i++) {
				int sh1 = rndi() % 48 + 1;
				sed ^= ((long long)seed[i * 3] )<< sh1;

			}
		}
	}
};


randengine rnd;
