#pragma once
#include <math.h>
#include <cstdlib>
#include <string>
#include <chrono>
#include <Windows.h>

#ifndef flpo
#define flpo float
#endif


#include <SDL2/SDL.h>

#define PPI 1.570796326794896619
#define PI 3.141592653589793238
#define PI2 6.28318530717958648

#include "punkt.h"

struct d {
	int a, b, c;
	bool wl;
	void *wsk[7];

	Uint32* wysswtab;
	float*depthhtab;
	int sztabb;

	int rysscalercx;
	int rysscalercy;
	int xd, yd;
	int rysscaler, sccx, sccy;

	std::string adwys;

	bool fullsc;
};

;

class pkty {
public:
	punkt *p = NULL;
	int il = 0;
	pkty(int ilosc) {
		il = ilosc;
		p = new punkt[il];
		memset(p, 0, sizeof(punkt)*il);
	}
	pkty(int rozm, punkt *pk, int ilp) {
		il = rozm;
		memcpy(p, pk, sizeof(punkt)*ilp);
		memset(&p[sizeof(punkt)*ilp], 0, sizeof(punkt)*(rozm - ilp));
	}
	void operator+=(punkt pp) {
		for (int i = 0; i < il; i++) {
			p[i] += pp;
		}
	}
	punkt& operator[](int indx) {
		return p[indx];
	}
};

void presssset(d* a) {
	a->xd = a->a / 2;
	a->yd = a->b / 2;
	if (a->xd < a->yd) {
		a->rysscaler = a->xd;
		a->sccx = 0;
		a->sccy = (a->yd - a->xd);
	}
	else {
		a->rysscaler = a->yd;
		a->sccy = 0;
		a->sccx = (a->xd - a->yd);
	}
	a->rysscalercx = a->rysscaler + a->sccx;
	a->rysscalercy = a->rysscaler + a->sccy;

	a->depthhtab = (float*)(((SDL_Surface**)(a->wsk[6]))[0]->pixels);
	a->wysswtab = (Uint32*)(a->wsk[4]);
	a->sztabb = a->a;
}

static int stw3d(void *arg) {
	d* a = (d*)arg;
	SDL_Window *win;
	SDL_Renderer *red;
	SDL_Texture *txk;
	SDL_Surface *sc, *dh;
	SDL_CreateWindowAndRenderer(a->a, a->b, SDL_WINDOW_RESIZABLE, &win, &red);
	{
		int xx, yy;
		SDL_GetRendererOutputSize(red, &xx, &yy);
		if (xx != a->a || yy != a->b){
			a->a = xx;
			a->b = yy;
			//SDL_DestroyTexture(bitmapTex);
			SDL_DestroyRenderer(red);
			SDL_DestroyWindow(win);
			SDL_CreateWindowAndRenderer(a->a, a->b, SDL_WINDOW_RESIZABLE, &win, &red);
		}
	}
	SDL_RenderSetLogicalSize(red, a->a, a->b);
	SDL_SetRenderDrawColor(red, 0, 0, 0, 255);
	SDL_RenderClear(red);
	SDL_RenderPresent(red);
	sc = SDL_CreateRGBSurface(0, a->a, a->b, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	dh = SDL_CreateRGBSurface(0, a->a, a->b, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	txk = SDL_CreateTexture(red, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, a->a, a->b);
	a->wsk[0] = win;
	a->wsk[1] = red;
	a->wsk[2] = txk;
	a->wsk[3] = sc;
	a->wsk[4] = sc->pixels;
	bool ttt = 0;
	a->wsk[5] = &ttt;
	a->wsk[6] = &dh;
	presssset(a);
	a->wl = 1;
	auto st = std::chrono::high_resolution_clock::now();
#ifdef NoC
	memset(sc->pixels, 0, a->a*a->b * 4);
	memset(dh->pixels, 0, a->a*a->b * 4);
#endif // NoC

	while ((a->wl))
	{
		if (ttt) {
			SDL_UpdateTexture(txk, NULL, sc->pixels, sc->pitch);
#ifndef NoC
			memset(sc->pixels, 0, a->a*a->b * 4);
			memset(dh->pixels, 255, a->a*a->b * 4);
#endif // !NoC
			{
				int xx, yy;
				SDL_GetRendererOutputSize(red, &xx, &yy);
				if (xx != a->a || yy != a->b) {
					a->a = xx;
					a->b = yy;
					SDL_DestroyTexture(txk);
					SDL_FreeSurface(sc);
					SDL_FreeSurface(dh);
					SDL_DestroyRenderer(red);
					SDL_DestroyWindow(win);
					SDL_CreateWindowAndRenderer(a->a, a->b, SDL_WINDOW_RESIZABLE | ((a->fullsc)? SDL_WINDOW_FULLSCREEN:0), &win, &red);
					SDL_RenderSetLogicalSize(red, a->a, a->b);
					SDL_SetRenderDrawColor(red, 0, 0, 0, 255);
					SDL_RenderClear(red);
					SDL_RenderPresent(red);
					sc = SDL_CreateRGBSurface(0, a->a, a->b, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
					dh = SDL_CreateRGBSurface(0, a->a, a->b, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
					txk = SDL_CreateTexture(red, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, a->a, a->b);
					a->wsk[0] = win;
					a->wsk[1] = red;
					a->wsk[2] = txk;
					a->wsk[3] = sc;
					a->wsk[4] = sc->pixels;
					a->wsk[5] = &ttt;
					a->wsk[6] = &dh;
					presssset(a);
				}
			}
			ttt = 0;
			auto now = std::chrono::high_resolution_clock::now();
			SDL_SetWindowTitle(win, (std::string("FPS: ") + std::to_string(1000000000.0 / (now - st).count()) + " " + a->adwys).data());
			st = now;
		}
		SDL_RenderClear(red);
		SDL_RenderCopy(red, txk, NULL, NULL);
		SDL_RenderPresent(red);
		SDL_PumpEvents();
		SDL_Delay(10);
	}
	return 0;
}

struct input {
	void(*inp)(obrot&, punkt&, flpo&, okno3d*);
	obrot *o;
	punkt *p;
	flpo *scale;
	okno3d*oo;
};

static int klawobs(void *a) {
	((input*)a)->inp(*((input*)a)->o, *((input*)a)->p, *((input*)a)->scale, ((input*)a)->oo);
	return 0;
}

class okno3d {
private:
	struct actobr {
		flpo a[9];
	};

	bool p = 1;
	actobr opt;
	actobr optrt;

	SDL_Thread * ok;
	SDL_Thread*kk;
	d dane;
	
	void odsw() {
		p = 0;
		obr.update();
		obrrt.x = -obr.x;
		obrrt.y = -obr.y;
		obrrt.z = -obr.z;
		obrrt.update();

		optrt.a[0] = (obrrt.cosz * obrrt.cosy + obrrt.sinz * obrrt.sinx * obrrt.siny) ;//x na x//
		optrt.a[1] = (obrrt.cosx * obrrt.sinz) ;//x na y//
		optrt.a[2] = (obrrt.cosy * obrrt.sinx * obrrt.sinz - obrrt.siny * obrrt.cosz);//x na z//
		optrt.a[3] = (obrrt.sinx * obrrt.siny * obrrt.cosz - obrrt.cosy * obrrt.sinz) ;//y na x//
		optrt.a[4] = (obrrt.cosx * obrrt.cosz) ;//y na y//
		optrt.a[5] = (obrrt.siny * obrrt.sinz + obrrt.sinx * obrrt.cosy * obrrt.cosz);//y na z//
		optrt.a[6] = (obrrt.siny * obrrt.cosx) ;//z na x//
		optrt.a[7] = (-obrrt.sinx) ;//z na y//
		optrt.a[8] = (obrrt.cosy * obrrt.cosx);//z na z//

		opt.a[0] = (obr.cosz * obr.cosy + obr.sinz * obr.sinx * obr.siny)*scale*dane.rysscaler;;//x na x//
		opt.a[1] = (obr.cosx * obr.sinz)*scale*dane.rysscaler;;//x na y//
		opt.a[2] = (obr.cosy * obr.sinx * obr.sinz - obr.siny * obr.cosz);//x na z//
		opt.a[3] = (obr.sinx * obr.siny * obr.cosz - obr.cosy * obr.sinz)*scale*dane.rysscaler;;//y na x//
		opt.a[4] = (obr.cosx * obr.cosz)*scale*dane.rysscaler;;//y na y//
		opt.a[5] = (obr.siny * obr.sinz + obr.sinx * obr.cosy *  obr.cosz);//y na z//
		opt.a[6] = (obr.siny * obr.cosx)*scale*dane.rysscaler;;//z na x//
		opt.a[7] = (-obr.sinx)*scale*dane.rysscaler;;//z na y//
		opt.a[8] = (obr.cosy* obr.cosx);//z na z//
		playerpos = playerpost;
	}
	int zmien(punkt &s) {
		if (p)odsw();
		s += playerpos;
		punkt obl;
		obl.z = s.x*opt.a[2] + s.y*opt.a[5] + s.z*opt.a[8];
		if (obl.z <= 0)return 0;
		obl.x = s.x * opt.a[0] + s.y * opt.a[3] + s.z * opt.a[6];
		obl.x = obl.x / obl.z;
		int x = obl.x + dane.rysscalercx;
		if (x >= dane.a)return 0;
		if (x <= 0)return 0;
		obl.y = s.x *opt.a[1] + s.y * opt.a[4] + s.z * opt.a[7];
		obl.y = obl.y / obl.z;
		int y = obl.y + dane.rysscalercy;
		if (y >= dane.b)return 0;
		if (y <= 0)return 0;
		s = obl;
		return y * dane.sztabb + x;
	}
	int zmiendm(punkt &s) {
		if (p)odsw();
		s += playerpos;
		punkt obl;
		obl.z = s.x*opt.a[2] + s.y*opt.a[5] + s.z*opt.a[8];
		if (obl.z <= 0)return 0;
		obl.x = s.x * opt.a[0] + s.y * opt.a[3] + s.z * opt.a[6];
		obl.x = obl.x / obl.z;
		int x = obl.x + dane.rysscalercx;
		if (x >= dane.a)return 0;
		if (x <= 0)return 0;
		obl.y = s.x *opt.a[1] + s.y * opt.a[4] + s.z * opt.a[7];
		obl.y = obl.y / obl.z;
		int y = obl.y + dane.rysscalercy;
		if (y >= dane.b)return 0;
		if (y <= 0)return 0;
		s = obl;
		return y/10 * dane.sztabb + x/10;
	}
	int zmiennnp(punkt &s) {
		if (p)odsw();
		punkt obl;
		obl.z = s.x*opt.a[2] + s.y*opt.a[5] + s.z*opt.a[8];
		if (obl.z <= 0)return 0;
		obl.x = s.x * opt.a[0] + s.y * opt.a[3] + s.z * opt.a[6];
		obl.x = obl.x / obl.z;
		int x = obl.x + dane.rysscalercx;
		if (x >= dane.a)return 0;
		if (x <= 0)return 0;
		obl.y = s.x *opt.a[1] + s.y * opt.a[4] + s.z * opt.a[7];
		obl.y = obl.y / obl.z;
		int y = obl.y + dane.rysscalercy;
		if (y >= dane.b)return 0;
		if (y <= 0)return 0;
		s.x = x;
		s.y = y;
		s.z = obl.z;
		return y * dane.sztabb + x;
	}
public:
	obrot obr = {};
	obrot obrrt = {};
	punkt playerpos;
	punkt playerpost;
	flpo scale = 1;
	std::string adwyss;
	okno3d(int szerokosc, int wysokosc, void(*inp)(obrot&, punkt&, flpo&,okno3d*)) {
		dane.a = szerokosc;
		dane.b = wysokosc;
		dane.fullsc = 0;
		dane.wl = 0;
		SDL_SetHint(SDL_HINT_RENDER_VSYNC,"1");
		ok = SDL_CreateThread(stw3d, "okno", &dane);
		while (dane.wl == 0)SDL_Delay(1);
		if (inp != NULL) {
			input i;
			i.inp = inp;
			i.o = &obr;
			i.p = &playerpost;
			i.scale = &scale;
			i.oo = this;
			kk = SDL_CreateThread(klawobs, "klawiatora", &i);
			SDL_Delay(100);
		}
	}
	punkt obrottt(punkt s) {
		if (p)odsw();
		punkt obl;
		obl.x = s.x * optrt.a[0] + s.y * optrt.a[3] + s.z * optrt.a[6];
		obl.y = s.x * optrt.a[1] + s.y * optrt.a[4] + s.z * optrt.a[7];
		obl.z = s.x * optrt.a[2] + s.y * optrt.a[5] + s.z * optrt.a[8];
		return obl;
	}
	flpo* getobrtab() {
		return optrt.a;
	}
	int sizy() {
		return dane.b;
	}
	int sizx() {
		return dane.a;
	}
	int rsclr() {
		return dane.rysscaler;
	}
	int rsclrcx() {
		return dane.rysscalercx;
	}
	int rsclrcy() {
		return dane.rysscalercy;
	}
	bool fullscreen(bool v) {
		dane.fullsc = v;
		dane.b--;
		return dane.fullsc;
	}
	bool fullscreen() {
		return dane.fullsc;
	}
	~okno3d() {
		dane.wl = 0;
		SDL_WaitThread(ok, 0);
	}
	Uint32* pixels() {
		return (Uint32*)dane.wsk[4];
	}
	Uint32* pixelss() {
		return (Uint32*)dane.wsk[6];
	}
	SDL_Window *Window() {
		return (SDL_Window*)dane.wsk[0];
	}
	SDL_Renderer *Renderer() {
		return (SDL_Renderer*)dane.wsk[1];
	}
	SDL_Texture *Texture() {
		return (SDL_Texture *)dane.wsk[2];
	}
	SDL_Surface *Surface() {
		return (SDL_Surface*)dane.wsk[3];
	}
	bool gets() {
		return *((bool*)dane.wsk[5]);
	}
	void setready() {
		*((bool*)dane.wsk[5]) = 1;
	}
	void wyswietl() {
		p = true;
		dane.adwys = "x:" + std::to_string(playerpos.x) + " y:" + std::to_string(playerpos.y) + " z:" + std::to_string(playerpos.z) + " " +adwyss;
		setready();
		while (gets())SDL_Delay(1);
	}
	void waitmainth() {
		if (p)odsw();
		while (p == 0)SDL_Delay(1);
	}
	void rysd(punkt p, Uint32 k) {
		int indx = zmien(p);
		int indxd = zmiendm(p);
		if (indx > 0) {
			if (dane.depthhtab[indxd] <= p.z)return;
			dane.wysswtab[indx] = k;
			dane.depthhtab[indxd] = p.z;
		}
	}
	void rys(punkt p, Uint32 k) {
		int indx = zmien(p);
		if (indx > 0) {
			if (dane.depthhtab[indx] <= p.z)return;
			dane.wysswtab[indx] = k;
			dane.depthhtab[indx] = p.z;
		}
	}
	void rysnp(punkt p, Uint32 k) {
		int indx = zmiennnp(p);
		if (indx > 0) {
			if (dane.depthhtab[indx] <= p.z)return;
			dane.wysswtab[indx] = k;
			dane.depthhtab[indx] = p.z;
		}
	}
	void ryscc(punkt p, Uint32 k) {
		int x = p.x + dane.rysscalercx;
		int y = p.y + dane.rysscalercy;
		int indx= y * dane.sztabb + x;
		if (indx > 0) {
			if (dane.depthhtab[indx] <= p.z)return;
			dane.wysswtab[indx] = k;
			dane.depthhtab[indx] = p.z;
		}
	}
	void rysbig(punkt p, Uint32 k) {
		int indx = zmien(p);
		if (indx > 0) {
			if (dane.depthhtab[indx] > p.z) {
				dane.wysswtab[indx] = k;
				dane.depthhtab[indx] = p.z;
			}
			if (dane.depthhtab[indx-1] > p.z) {
				dane.wysswtab[indx-1] = k;
				dane.depthhtab[indx-1] = p.z;
			}
			if (dane.depthhtab[indx - dane.sztabb] > p.z) {
				dane.wysswtab[indx - dane.sztabb] = k;
				dane.depthhtab[indx - dane.sztabb] = p.z;
			}
			if (dane.depthhtab[indx + 1] > p.z) {
				dane.wysswtab[indx + 1] = k;
				dane.depthhtab[indx + 1] = p.z;
			}
			if (dane.depthhtab[indx + dane.sztabb] > p.z) {
				dane.wysswtab[indx + dane.sztabb] = k;
				dane.depthhtab[indx + dane.sztabb] = p.z;
			}
		}
	}
	void drawline(punkt a, punkt b, Uint32 k) {
		punkt r = a - b;
		int il = dane.rysscaler * scale;
		r /= il;
		for (int i = 0; i < il; i++) {
			a -= r;
			rys(a, k);
		}
	}
};

void defkbmoto(obrot & o, punkt & p, flpo & s, okno3d * l)
{
	int nacporid = -1, nacobrid = -1, nacscid = -1, nacgdid = -1;

	double x = 0, y = 0, xg = 0, yg = 0, odlst = 0, scs = s;

	bool wc = 0;
	int sx = 0, sy = 0;

	flpo popx = 0, popz = 0, popox = 0;
	punkt przysp = { 0 }, playerspeed = { 0 };
	while (1)
	{
		SDL_Delay(8);

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_MOUSEWHEEL)
			{
				s += event.wheel.y / 50.0;
			}
			if (event.type == SDL_FINGERDOWN)
			{
				if (nacporid == -1)
					if (event.tfinger.x < 0.5)
					{
						nacporid = event.tfinger.fingerId;
						x = event.tfinger.x;
						y = event.tfinger.y;
						continue;
					}
				if (nacgdid == -1)
					if (event.tfinger.x > 0.5)
						if (event.tfinger.y > 0.5)
						{
							nacgdid = event.tfinger.fingerId;
							xg = event.tfinger.x;
							yg = event.tfinger.y;
							continue;
						}
				if (nacobrid == -1)
				{
					nacobrid = event.tfinger.fingerId;
					continue;
				}
				if (nacscid == -1) {
					nacscid = event.tfinger.fingerId;
					odlst = event.tfinger.x - event.tfinger.y;
					scs = s;
					continue;
				}
			}
			if (event.type == SDL_FINGERUP)
			{
				if (nacobrid == event.tfinger.fingerId)
					nacobrid = -1;
				if (nacgdid == event.tfinger.fingerId) {
					nacgdid = -1;
					przysp.y = 0;
				}
				if (nacscid == event.tfinger.fingerId)
					nacscid = -1;
				if (nacporid == event.tfinger.fingerId)
				{
					nacporid = -1;
					przysp.x = 0;
					przysp.z = 0;
				}
			}
			if (event.type == SDL_FINGERMOTION)
			{
				if (nacscid == event.tfinger.fingerId) {
					s = scs + odlst - (event.tfinger.x - event.tfinger.y);
				}
				if (nacgdid == event.tfinger.fingerId)
				{
					//4d//przysp.x = x - event.tfinger.x;
					o.y -= event.tfinger.dx * 5;
					przysp.y = y - event.tfinger.y;
				}
				if (nacporid == event.tfinger.fingerId)
				{
					przysp.x = x - event.tfinger.x;
					przysp.z = event.tfinger.y - y;
				}
				if (nacobrid == event.tfinger.fingerId)
				{
					punkt oo;
					oo.x = event.tfinger.dy * 5;
					oo.y = event.tfinger.dx * 5;

					popox += oo.x;
					o.x -= popx;
					o.z -= popz;

					o.y -= oo.y;
					popx = popox * cos(o.y);
					popz = popox * sin(o.y);

					if (popx > PPI)
						popx = PPI;
					if (popx < -PPI)
						popx = -PPI;
					if (popz > PPI)
						popz = PPI;
					if (popz < -PPI)
						popz = -PPI;

					o.x += popx;
					o.z += popz;
				}
			}

		}
		const Uint8 *k = SDL_GetKeyboardState(NULL);
		if (k[SDL_SCANCODE_A])if (k[SDL_SCANCODE_LCTRL])przysp.x += 0.02; else przysp.x += 0.01;
		if (k[SDL_SCANCODE_S])if (k[SDL_SCANCODE_LCTRL])przysp.z += 0.02; else przysp.z += 0.01;
		if (k[SDL_SCANCODE_D])if (k[SDL_SCANCODE_LCTRL])przysp.x -= 0.02; else przysp.x -= 0.01;
		if (k[SDL_SCANCODE_W])if (k[SDL_SCANCODE_LCTRL])przysp.z -= 0.02; else przysp.z -= 0.01;
		if (k[SDL_SCANCODE_LSHIFT])if (k[SDL_SCANCODE_LCTRL])przysp.y -= 0.02; else przysp.y -= 0.01;
		if (k[SDL_SCANCODE_SPACE])if (k[SDL_SCANCODE_LCTRL])przysp.y += 0.02; else przysp.y += 0.01;

		przysp.obroc(Y, -o.y);
		playerspeed += przysp;
		p += playerspeed;
		if (k[SDL_SCANCODE_LCTRL]) {
			playerspeed *= 0.9;
		}
		else {
			playerspeed *= 0.8;
		}


		int x, y;
		if (SDL_GetMouseState(&y, &x) == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			if (wc == 1) {
				punkt oo;
				oo.x = (x - sx) / 200.0;
				oo.y = (y - sy) / 200.0;

				popox += oo.x;

				o.x -= popx;
				o.z -= popz;

				o.y -= oo.y;
				popx = popox * cos(o.y);
				popz = popox * sin(o.y);

				if (popx > PPI)popx = PPI;
				if (popx < -PPI)popx = -PPI;
				if (popz > PPI)popz = PPI;
				if (popz < -PPI)popz = -PPI;

				o.x += popx;
				o.z += popz;
			}
			else {
				wc = 1;
			}
		}
		else {
			wc = 0;
		}
		sx = x;
		sy = y;

		if (o.z >= PI2)
			o.z -= PI2;
		if (o.z < 0)
			o.z += PI2;
		if (o.y >= PI2)
			o.y -= PI2;
		if (o.y < 0)
			o.y += PI2;
		if (o.x >= PI2)
			o.x -= PI2;
		if (o.x < 0)
			o.x += PI2;
	}
}

//smcoords d3map;
//punkt pointingat;
//double rdist = 20;
//double rst = 0;
//double gst = 0;
//double bst = 0;


double ee = 1;

#define MAX(a,b) a>b?a:b

void defkybd(obrot& o, punkt& p, flpo& s,okno3d* l) {
	//d3map.startramcontrol(&(p.x), &(p.y), &(p.z), &rdist);
	bool wc = 0;
	int sx = 0, sy = 0;
	flpo dxx = 0, dyy = 0;

	flpo popx = 0, popz = 0, popox = 0;
	punkt przysp = { 0 }, playerspeed = {};

	bool fccc = 0;
	bool flsclck=0;
	auto sttim = std::chrono::high_resolution_clock::now();
	while (1) {
		SDL_Delay(1);
		double deltat=0.01;
		//{
		//	auto edtim = std::chrono::high_resolution_clock::now();
		//	deltat = (std::chrono::duration<double>(edtim - sttim)).count();
		//	sttim = edtim;
		//	//printf("%f\n", deltat);
		//}
		const Uint8 *k = SDL_GetKeyboardState(NULL);
		if (k[SDL_SCANCODE_A])przysp.x += 1;
		if (k[SDL_SCANCODE_S])przysp.z += 1;
		if (k[SDL_SCANCODE_D])przysp.x -= 1;
		if (k[SDL_SCANCODE_W])przysp.z -= 1;
		if (k[SDL_SCANCODE_LSHIFT])przysp.y -= 1.5;
		if (k[SDL_SCANCODE_SPACE])przysp.y += 1.5;
		if (k[SDL_SCANCODE_LCTRL])przysp *= 2;

		//przysp.y -= 0.01;//gravity
		przysp.obroc(Y, -o.y);
		playerspeed += przysp*deltat;
		przysp = { 0 };
		//{
		//	//collision
		//	punkt pozp = punkt({ 0,0,0 }) -(p+playerspeed);
		//	double h = d3map.geth(pozp.x, pozp.y, pozp.z);
		//	if (h < ee) {
		//		p += playerspeed;
		//	}
		//}
		p += playerspeed * deltat;
		if (k[SDL_SCANCODE_LCTRL]) {
			playerspeed *= 0.98;
		}
		else {
			playerspeed *= 0.90;
		}
		//if (k[SDL_SCANCODE_1])o.x += 0.05;//przod na dol
		//if (k[SDL_SCANCODE_2])o.x -= 0.05;//przod do gory
		//if (k[SDL_SCANCODE_3])o.y += 0.05;//przod w lewo
		//if (k[SDL_SCANCODE_4])o.y -= 0.05;//przod w prawo
		//if (k[SDL_SCANCODE_5])o.z += 0.05;//gora w prawo
		//if (k[SDL_SCANCODE_6])o.z -= 0.05;//gora w lewo
		//if (k[SDL_SCANCODE_7])s *= 1.001;//przyblizanie
		//if (k[SDL_SCANCODE_8])s *= 0.999;//oddalanie
		//if (k[SDL_SCANCODE_9])ee *= 1.001;
		//if (k[SDL_SCANCODE_0])ee *= 0.999;
		//if (k[SDL_SCANCODE_L])lac *= 1.001;
		//if (k[SDL_SCANCODE_K])lac *= 0.999;
		//if (k[SDL_SCANCODE_P])pers *= 1.001;
		//if (k[SDL_SCANCODE_O])pers *= 0.999;
		//if (k[SDL_SCANCODE_M])hob *= 1.001;
		//if (k[SDL_SCANCODE_N])hob *= 0.999;
		/*if (k[SDL_SCANCODE_V])rdist *= 1.001;
		if (k[SDL_SCANCODE_B])rdist *= 0.999;*/
		if (k[SDL_SCANCODE_F1]) {
			o = {};
			p = {};
			s = 0.65;
		}
		if (k[SDL_SCANCODE_F11]) {
			if (flsclck == 0) {
				flsclck = 1;
				l->fullscreen(!l->fullscreen());
			}
		}
		else {
			flsclck = 0;
		}
		//if (k[SDL_SCANCODE_ESCAPE])exit(0);
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_MOUSEWHEEL)
			{
				s += event.wheel.y / 100.0;//fov
			}
		}
		int x, y;
		if (SDL_GetMouseState(&y, &x) == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			if (wc == 1) {
				punkt oo;
				oo.x = (x - sx) / 200.0;
				oo.y = (y - sy) / 200.0;

				popox += oo.x;

				obrot to = o;

				to.x -= popx;
				to.z -= popz;

				to.y -= oo.y;
				popx = popox *cos(to.y);
				popz = popox * sin(to.y);

				if (popx > PPI)popx = PPI;
				if (popx < -PPI)popx = -PPI;
				if (popz > PPI)popz = PPI;
				if (popz < -PPI)popz = -PPI;

				to.x += popx;
				to.z += popz;

				o = to;
			}
			else {
				wc = 1;
			}
		}
		else {
			wc = 0;
		}
		sx = x;
		sy = y;

		if (o.z >= PI2)o.z -= PI2;
		if (o.z < 0)o.z += PI2;
		if (o.y >= PI2)o.y -= PI2;
		if (o.y < 0)o.y += PI2;
		if (o.x >= PI2)o.x -= PI2;
		if (o.x < 0)o.x += PI2;
	}
}








/*
waste code


void rszr(int n, int x, int y, int z, bool *rdc[120][120], int num) {
	if (n >= rd)return;
	rdc[x][y][z] = 0;
	switch (num)
	{
	case(0):
			rszr(n + 1, x + 1, y, z, rdc, 0);
			rszr(n + 1, x + 1, y + 1, z, rdc, 0);
			rszr(n + 1, x + 1, y - 1, z, rdc, 0);
			rszr(n + 1, x + 1, y, z + 1, rdc, 0);
			rszr(n + 1, x + 1, y, z - 1, rdc, 0);
			rszr(n + 1, x + 1, y + 1, z + 1, rdc, 0);
			rszr(n + 1, x + 1, y + 1, z - 1, rdc, 0);
			rszr(n + 1, x + 1, y - 1, z + 1, rdc, 0);
			rszr(n + 1, x + 1, y - 1, z - 1, rdc, 0);

	case(1):
			rszr(n + 1, x - 1, y, z, rdc, 1);
			rszr(n + 1, x - 1, y + 1, z, rdc, 1);
			rszr(n + 1, x - 1, y - 1, z, rdc, 1);
			rszr(n + 1, x - 1, y, z + 1, rdc, 1);
			rszr(n + 1, x - 1, y, z - 1, rdc, 1);
			rszr(n + 1, x - 1, y + 1, z + 1, rdc, 1);
			rszr(n + 1, x - 1, y + 1, z - 1, rdc, 1);
			rszr(n + 1, x - 1, y - 1, z + 1, rdc, 1);
			rszr(n + 1, x - 1, y - 1, z - 1, rdc, 1);
		break;
	case(2):
			rszr(n + 1, x, y + 1, z, rdc, 2);
			rszr(n + 1, x + 1, y + 1, z, rdc, 2);
			rszr(n + 1, x + 1, y + 1, z + 1, rdc, 2);
			rszr(n + 1, x + 1, y + 1, z - 1, rdc, 2);
			rszr(n + 1, x, y + 1, z + 1, rdc, 2);
			rszr(n + 1, x, y + 1, z - 1, rdc, 2);
			rszr(n + 1, x - 1, y + 1, z - 1, rdc, 2);
			rszr(n + 1, x - 1, y + 1, z, rdc, 2);
			rszr(n + 1, x - 1, y + 1, z - 1, rdc, 2);
		break;
	case(3):
		rszr(n + 1, x, y - 1, z, rdc, 3);
		rszr(n + 1, x + 1, y - 1, z, rdc, 3);
		rszr(n + 1, x + 1, y - 1, z + 1, rdc, 3);
		rszr(n + 1, x + 1, y - 1, z - 1, rdc, 3);
		rszr(n + 1, x, y - 1, z + 1, rdc, 3);
		rszr(n + 1, x, y - 1, z - 1, rdc, 3);
		rszr(n + 1, x - 1, y - 1, z - 1, rdc, 3);
		rszr(n + 1, x - 1, y - 1, z, rdc, 3);
		rszr(n + 1, x - 1, y - 1, z - 1, rdc, 3);
		break;
	case(4):
			rszr(n + 1, x + 1, y, z + 1, rdc, 4);
			rszr(n + 1, x + 1, y + 1, z + 1, rdc, 4);
			rszr(n + 1, x + 1, y - 1, z + 1, rdc, 4);
			rszr(n + 1, x - 1, y, z + 1, rdc, 4);
			rszr(n + 1, x, y, z + 1, rdc, 4);
			rszr(n + 1, x - 1, y + 1, z + 1, rdc, 4);
			rszr(n + 1, x, y + 1, z + 1, rdc, 4);
			rszr(n + 1, x - 1, y - 1, z + 1, rdc, 4);
			rszr(n + 1, x, y - 1, z + 1, rdc, 4);
		break;
	case(5):
		rszr(n + 1, x + 1, y, z - 1, rdc, 5);
		rszr(n + 1, x + 1, y + 1, z - 1, rdc, 5);
		rszr(n + 1, x + 1, y - 1, z - 1, rdc, 5);
		rszr(n + 1, x - 1, y, z - 1, rdc, 5);
		rszr(n + 1, x, y, z - 1, rdc, 5);
		rszr(n + 1, x - 1, y + 1, z - 1, rdc, 5);
		rszr(n + 1, x, y + 1, z - 1, rdc, 5);
		rszr(n + 1, x - 1, y - 1, z - 1, rdc, 5);
		rszr(n + 1, x, y - 1, z - 1, rdc, 5);
		break;
	}
}

void recrdsp(int n,int x,int y,int z, bool *rdc[120][120],int num) {
	if (n >= rd)return;
	switch (num)
	{
	case(0):
		if (rdc[x][y][z]) {
			rszr(n + 1, x+1, y, z, rdc, 0);
			rszr(n + 1, x+1, y+1, z, rdc, 0);
			rszr(n + 1, x+1, y-1, z, rdc, 0);
			rszr(n + 1, x+1, y, z+1, rdc, 0);
			rszr(n + 1, x+1, y, z-1, rdc, 0);
			rszr(n + 1, x+1, y+1, z+1, rdc, 0);
			rszr(n + 1, x+1, y+1, z-1, rdc, 0);
			rszr(n + 1, x+1, y-1, z+1, rdc, 0);
			rszr(n + 1, x+1, y-1, z-1, rdc, 0);
		}
		else {
			recrdsp(n + 1, x + 1, y, z, rdc, 0);
			recrdsp(n + 1, x + 1, y + 1, z, rdc, 0);
			recrdsp(n + 1, x + 1, y - 1, z, rdc, 0);
			recrdsp(n + 1, x + 1, y, z + 1, rdc, 0);
			recrdsp(n + 1, x + 1, y, z - 1, rdc, 0);
			recrdsp(n + 1, x + 1, y + 1, z + 1, rdc, 0);
			recrdsp(n + 1, x + 1, y + 1, z - 1, rdc, 0);
			recrdsp(n + 1, x + 1, y - 1, z + 1, rdc, 0);
			recrdsp(n + 1, x + 1, y - 1, z - 1, rdc, 0);
		}
		break;
	case(1):
		if (rdc[x][y][z]) {
			rszr(n + 1, x - 1, y, z, rdc, 1);
			rszr(n + 1, x - 1, y + 1, z, rdc, 1);
			rszr(n + 1, x - 1, y - 1, z, rdc, 1);
			rszr(n + 1, x - 1, y, z + 1, rdc, 1);
			rszr(n + 1, x - 1, y, z - 1, rdc, 1);
			rszr(n + 1, x - 1, y + 1, z + 1, rdc, 1);
			rszr(n + 1, x - 1, y + 1, z - 1, rdc, 1);
			rszr(n + 1, x - 1, y - 1, z + 1, rdc, 1);
			rszr(n + 1, x - 1, y - 1, z - 1, rdc, 1);
		}
		else {
			recrdsp(n + 1, x - 1, y, z, rdc, 1);
			recrdsp(n + 1, x - 1, y + 1, z, rdc, 1);
			recrdsp(n + 1, x - 1, y - 1, z, rdc, 1);
			recrdsp(n + 1, x - 1, y, z + 1, rdc, 1);
			recrdsp(n + 1, x - 1, y, z - 1, rdc, 1);
			recrdsp(n + 1, x - 1, y + 1, z + 1, rdc, 1);
			recrdsp(n + 1, x - 1, y + 1, z - 1, rdc, 1);
			recrdsp(n + 1, x - 1, y - 1, z + 1, rdc, 1);
			recrdsp(n + 1, x - 1, y - 1, z - 1, rdc, 1);
		}
		break;
	case(2):
		if (rdc[x][y][z]) {
			rszr(n + 1, x, y + 1, z, rdc, 2);
			rszr(n + 1, x + 1, y + 1, z, rdc, 2);
			rszr(n + 1, x + 1, y + 1, z + 1, rdc, 2);
			rszr(n + 1, x + 1, y + 1, z - 1, rdc, 2);
			rszr(n + 1, x, y + 1, z + 1, rdc, 2);
			rszr(n + 1, x, y + 1, z - 1, rdc, 2);
			rszr(n + 1, x - 1, y + 1, z - 1, rdc, 2);
			rszr(n + 1, x - 1, y + 1, z, rdc, 2);
			rszr(n + 1, x - 1, y + 1, z - 1, rdc, 2);
		}
		else {
			recrdsp(n + 1, x, y + 1, z, rdc, 2);
			recrdsp(n + 1, x + 1, y + 1, z, rdc, 2);
			recrdsp(n + 1, x + 1, y + 1, z + 1, rdc, 2);
			recrdsp(n + 1, x + 1, y + 1, z - 1, rdc, 2);
			recrdsp(n + 1, x, y + 1, z + 1, rdc, 2);
			recrdsp(n + 1, x, y + 1, z - 1, rdc, 2);
			recrdsp(n + 1, x - 1, y + 1, z - 1, rdc, 2);
			recrdsp(n + 1, x - 1, y + 1, z, rdc, 2);
			recrdsp(n + 1, x - 1, y + 1, z - 1, rdc, 2);
		}
		break;
	case(3):
		if (rdc[x][y][z]) {
			rszr(n + 1, x, y - 1, z, rdc, 3);
			rszr(n + 1, x + 1, y - 1, z, rdc, 3);
			rszr(n + 1, x + 1, y - 1, z + 1, rdc, 3);
			rszr(n + 1, x + 1, y - 1, z - 1, rdc, 3);
			rszr(n + 1, x, y - 1, z + 1, rdc, 3);
			rszr(n + 1, x, y - 1, z - 1, rdc, 3);
			rszr(n + 1, x - 1, y - 1, z - 1, rdc, 3);
			rszr(n + 1, x - 1, y - 1, z, rdc, 3);
			rszr(n + 1, x - 1, y - 1, z - 1, rdc, 3);
		}
		else {
			recrdsp(n + 1, x, y - 1, z, rdc, 3);
			recrdsp(n + 1, x + 1, y - 1, z, rdc, 3);
			recrdsp(n + 1, x + 1, y - 1, z + 1, rdc, 3);
			recrdsp(n + 1, x + 1, y - 1, z - 1, rdc, 3);
			recrdsp(n + 1, x, y - 1, z + 1, rdc, 3);
			recrdsp(n + 1, x, y - 1, z - 1, rdc, 3);
			recrdsp(n + 1, x - 1, y - 1, z - 1, rdc, 3);
			recrdsp(n + 1, x - 1, y - 1, z, rdc, 3);
			recrdsp(n + 1, x - 1, y - 1, z - 1, rdc, 3);
		}
		break;
	case(4):
		if (rdc[x][y][z]) {
			rszr(n + 1, x + 1, y, z + 1, rdc, 4);
			rszr(n + 1, x + 1, y + 1, z + 1, rdc, 4);
			rszr(n + 1, x + 1, y - 1, z + 1, rdc, 4);
			rszr(n + 1, x - 1, y, z + 1, rdc, 4);
			rszr(n + 1, x, y, z + 1, rdc, 4);
			rszr(n + 1, x - 1, y + 1, z + 1, rdc, 4);
			rszr(n + 1, x, y + 1, z + 1, rdc, 4);
			rszr(n + 1, x - 1, y - 1, z + 1, rdc, 4);
			rszr(n + 1, x, y - 1, z + 1, rdc, 4);
		}
		else {
			recrdsp(n + 1, x + 1, y, z + 1, rdc, 4);
			recrdsp(n + 1, x + 1, y + 1, z + 1, rdc, 4);
			recrdsp(n + 1, x + 1, y - 1, z + 1, rdc, 4);
			recrdsp(n + 1, x - 1, y, z + 1, rdc, 4);
			recrdsp(n + 1, x, y, z + 1, rdc, 4);
			recrdsp(n + 1, x - 1, y + 1, z + 1, rdc, 4);
			recrdsp(n + 1, x, y + 1, z + 1, rdc, 4);
			recrdsp(n + 1, x - 1, y - 1, z + 1, rdc, 4);
			recrdsp(n + 1, x, y - 1, z + 1, rdc, 4);
		}
		break;
	case(5):
		if (rdc[x][y][z]) {
			rszr(n + 1, x + 1, y, z - 1, rdc, 5);
			rszr(n + 1, x + 1, y + 1, z - 1, rdc, 5);
			rszr(n + 1, x + 1, y - 1, z - 1, rdc, 5);
			rszr(n + 1, x - 1, y, z - 1, rdc, 5);
			rszr(n + 1, x, y, z - 1, rdc, 5);
			rszr(n + 1, x - 1, y + 1, z - 1, rdc, 5);
			rszr(n + 1, x, y + 1, z - 1, rdc, 5);
			rszr(n + 1, x - 1, y - 1, z - 1, rdc, 5);
			rszr(n + 1, x, y - 1, z - 1, rdc, 5);
		}
		else {
			recrdsp(n + 1, x + 1, y, z - 1, rdc, 5);
			recrdsp(n + 1, x + 1, y + 1, z - 1, rdc, 5);
			recrdsp(n + 1, x + 1, y - 1, z - 1, rdc, 5);
			recrdsp(n + 1, x - 1, y, z - 1, rdc, 5);
			recrdsp(n + 1, x, y, z - 1, rdc, 5);
			recrdsp(n + 1, x - 1, y + 1, z - 1, rdc, 5);
			recrdsp(n + 1, x, y + 1, z - 1, rdc, 5);
			recrdsp(n + 1, x - 1, y - 1, z - 1, rdc, 5);
			recrdsp(n + 1, x, y - 1, z - 1, rdc, 5);
		}
		break;
	}
}







































































































*/