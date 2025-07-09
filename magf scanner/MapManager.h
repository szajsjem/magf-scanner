#pragma once
#include "Queue.h"
#include <list>
#include "pnoise.h"
#include <fstream>
#include <string>
#include <mutex>

#include <optional>
#include <unordered_map>

#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <algorithm>

#define chunkx 8
#define chunky 8
#define chunkz 8

enum Chunkstate {
	unknown = 0,
	generated = 1,
	optimized = 2,
	generatedandoptimized = 3,
	chunkempty = 4,
	generatedandoptimizedandempty = 7,
};
void operator|=(Chunkstate &L,const Chunkstate P) {
	if (L == generated)
		if (P == optimized)
			L = generatedandoptimized;
	if (L == generatedandoptimized)
		if (P == chunkempty)
			L = generatedandoptimizedandempty;
}

enum ChunkAction {
	load = 0,
	generate = 1,
	optimize = 2,
	rmfrommemory=3,
};

struct Chunk {
	float v[chunkx+1][chunky+1][chunkz+1];
	bool render[chunkx][chunky][chunkz];
	Chunkstate state;
};

struct Pos {
	long long x, y, z;
	Pos() {
		x = 0;
		y = 0;
		z = 0;
	}
	Pos(long long xx,long long yy,long long zz) {
		x = xx;
		y = yy;
		z = zz;
	}
	Pos(long long xx[3]) {
		x = xx[0];
		y = xx[1];
		z = xx[2];
	}
	constexpr bool operator==(const Pos &o) const{
		if (x != o.x)return 0;
		if (y != o.y)return 0;
		if (z != o.z)return 0;
		return 1;
	}
	Pos operator+(const Pos& o) {
		Pos w = o;
		w.x += x;
		w.y += y;
		w.z += z;
		return w;
	}
	constexpr bool operator()(const Pos& lhs, const Pos& rhs) const
	{
		return lhs == rhs;
	}
	double odl(const Pos o) const{
		long long dx = o.x - x;
		long long dy = o.y - y;
		long long dz = o.z - z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}
};




namespace std {
	template<>
	class hash<Pos> {
	public:
		size_t operator()(const Pos &p) const noexcept{
			size_t w = hash<long long>{}(p.x);
			w = w xor hash<long long>{}(p.z)<<8;
			w = w xor hash<long long>{}(p.y)<<16;
			return w;
		}
	};
}

class MapgenNoise{
	packednoise p;
	packednoise h;
public:
	MapgenNoise(long long seed):p(2, 0.5, 8, 0.2, seed),h(0.15,4,8,0.5,seed+1){

	}
	double getheight(long long x, long long z) {
		constexpr double bigterrainscale = 333.333;
		constexpr double smallterrainscale = 30;
		constexpr double heightmultipler = 20;
		constexpr double bigheightmultipler = 100;

		double mult = (h.noise(x/ bigterrainscale, z/ bigterrainscale));// / 1000.00;//  x
		//https://www.desmos.com/calculator/lp85ilbnao?lang=plpl

		double hmult = 0.2 * tanh(3 * mult) + 0.4 * ((mult)/(pow(mult,4)+1)) + 0.08 * pow(mult, 3);
		double kmult = abs(hmult - pow(mult/1.5, 5))+0.1*abs(mult)+0.3;
		kmult = 0.9 + log(0.7 + log(0.3 + pow(kmult,4))/log(10))/log(10);
		return hmult * bigheightmultipler + kmult * heightmultipler * p.noise(x / smallterrainscale, z / smallterrainscale);

		//return hmult*100 + pow(abs(hmult),pmult) + heightmultipler * p.noise(x / smallterrainscale, z / smallterrainscale);
	}
	float noise(long long x, long long y, long long z, double height) {
		constexpr double xnoisescale = 138.254;
		constexpr double ynoisescale = 134.342;
		constexpr double znoisescale = 136.666;

		double v = 0;
		if (y < height) {
			if (height - y < 1) {
				v = height - y;
			}
			else {
				double tt = tanh((y - height) / 30) / 2;
				v = p.noise((x) / xnoisescale, (y) / ynoisescale, (z) / znoisescale) + tt;
				if (v < 0)
					v = 0;
				else
					v = 1;
			}
		}

		return v;
	}

};

class Chunkholder {
public:
	std::unique_ptr<Chunk> ch;
	const Pos position;
	int playermark;
	ChunkAction wtd;
	Chunkholder(Pos p): position(p){
		ch = std::unique_ptr<Chunk>(new Chunk);
		playermark = 1000;
		ch->state = unknown;
	}
	void generate(MapgenNoise *n) {
		for(long long x=0;x<=chunkx;x++)
			for (long long z = 0; z <= chunkz; z++) {
				double height = n->getheight(x + position.x * chunkx, z + position.z * chunkz);
				for (long long y = 0; y <= chunky; y++) {
					ch->v[x][y][z] = n->noise(x + position.x * chunkx, y + position.y * chunky, z + position.z * chunkz, height);
					//ch->g[x][y][z] = n->noise(x, height - y, z, height);
					//ch->b[x][y][z] = n->noise(z, height - y, x, height);
				}
			}
		ch->state = generated;
	}
	//1 on error
	bool load() {
		try {
			std::string filename = std::string("D:/world/") + std::to_string(position.x) + "_" + std::to_string(position.y) + "_" + std::to_string(position.z) + ".chunk";
			std::fstream f(filename.c_str(), std::ios::in | std::ios::binary);

			if (f.is_open()) {
				f.read((char*)ch.get(), sizeof(Chunk));
				f.close();
				return 0;
			}
			f.close();
		}
		catch (std::exception e) {
			printf("%s\n",e.what());
		}
		return 1;
	}
	void save() {
		std::string filename = std::string("D:/world/") + std::to_string(position.x) + "_" + std::to_string(position.y) + "_" + std::to_string(position.z) + ".chunk";
		std::fstream f(filename, std::ios::out | std::ios::binary);
		f.write((char*)ch.get(), sizeof(Chunk));
		f.flush();
		f.close();
	}
};

template<class K, class V>
class hashmap : public std::unordered_map<K, V> {
	mutable std::mutex m;
	std::unordered_map<K, V> map;
public:
	void lock() {
		m.lock();
	}
	void unlock() {
		m.unlock();
	}
	auto& operator[](const K& k) {
		std::lock_guard<std::mutex> lock(m);
		return map[k];
	}
	auto insert(const K& key, const V& value) {
		std::lock_guard<std::mutex> lock(m);
		return map.insert({ key, value });
	}
	void assign(const K& key, const V& value) {
		std::lock_guard<std::mutex> lock(m);
		map[key] = value;
	}
	std::optional<V> getValue(const K& key) const {
		std::lock_guard<std::mutex> lock(m);
		auto it = map.find(key);
		if (it != map.end()) {
			return it->second;
		}
		return std::nullopt;
	}
	size_t erase(const K& key) {
		std::lock_guard<std::mutex> lock(m);
		return map.erase(key);
	}
	size_t size() const {
		return map.size();
	}
	bool empty() const {
		std::lock_guard<std::mutex> lock(m);
		return map.empty();
	}
	void clear() {
		std::lock_guard<std::mutex> lock(m);
		map.clear();
	}
};

class MapManager {
	Queue<Pos> loadchunkq;
	Queue<Pos> genchunkq;
	Queue<Pos> optchunkq;
	Queue<Chunkholder*> savechunkq;

	hashmap<Pos,Chunkholder*> chunkss;

	void loadchunk(Chunkholder*chunk) {
		if (chunk->load()) {
			if(chunk->wtd>load)
				genchunkq.insert(chunk->position);
		}
	}
	void savechunk(Chunkholder*chunk) {
		if (chunk != NULL) {
			chunk->save();
			if (chunk->wtd == rmfrommemory) {
				Chunkholder* cd;
				if ((cd=chunkss[chunk->position]) != NULL)printf("?");
				if(chunk!=cd)
					delete chunk;
			}
		}
	}
	void genchunk(Chunkholder*chunk) {
		chunk->generate(&noise);
		if (chunk->wtd > generate) {
			optchunkq.insert(chunk->position);
		}
		else {
			savechunkq.insert(chunk);
		}
	}
	void gench(Pos p) {
		if (chunkss[p] == NULL) {
			Chunkholder* t = new Chunkholder(p);
			t->wtd = generate;
			loadchunkq.insert(t->position);
			chunkss[p] = t;
		}
	}
	bool optimizechunk(Pos czunk) {
		Chunkholder* srodek = chunkss[czunk];
		if (srodek == NULL){
			gench(czunk);
			return 1;
		}
		if (srodek->ch->state == optimized)return 0;
		if (!(srodek->ch->state & generated)) {
			if (srodek->wtd < optimize) {
				srodek->wtd = optimize;
				loadchunkq.insert(srodek->position);
			}
			return 1;
		}
		Chunkholder* gora = chunkss[czunk + Pos({0, 1, 0})];
		if (gora == NULL) {
			gench(czunk);
			return 1;
		}
		if (!(gora->ch->state & generated)) return 1;
		Chunkholder* wschod = chunkss[czunk + Pos({ 0, 0, 1 })];
		if (wschod == NULL) {
			gench(czunk);
			return 1;
		}
		if (!(wschod->ch->state & generated)) return 1;
		Chunkholder* zachod = chunkss[czunk + Pos({ 0, 0, -1 })];
		if (zachod == NULL) {
			gench(czunk);
			return 1;
		}
		if (!(zachod->ch->state & generated)) return 1;
		Chunkholder* polnoc = chunkss[czunk + Pos({ 1, 0, 0 })];
		if (polnoc == NULL) {
			gench(czunk);
			return 1;
		}
		if (!(polnoc->ch->state & generated)) return 1;
		Chunkholder* poludnie = chunkss[czunk + Pos({ -1, 0, 0 })];
		if (poludnie == NULL) {
			gench(czunk);
			return 1;
		}
		if (!(poludnie->ch->state & generated)) return 1;
		Chunkholder* dol = chunkss[czunk + Pos({ 0, -1, 0 })];
		if (dol == NULL) {
			gench(czunk);
			return 1;
		}
		if (!(dol->ch->state & generated)) return 1;
		bool cos = 0;
		for (int x = 0; x < chunkx; x++)
			for (int y = 0; y < chunky; y++)
				for (int z = 0; z < chunkz; z++) {
					if (srodek->ch->v[x][y][z] == 0) {
						srodek->ch->render[x][y][z] = 0;
						continue;
					}
					if (x < (chunkx - 1)) {
						if (srodek->ch->v[x + 1][y][z] == 0) {
							srodek->ch->render[x][y][z] = 1; cos = 1;
							continue;
						}
					}
					else {
						if (polnoc->ch->v[0][y][z] == 0) {
							srodek->ch->render[x][y][z] = 1; cos = 1;
							continue;
						}
					}
					if (x > 0) {
						if (srodek->ch->v[x - 1][y][z] == 0) {
							srodek->ch->render[x][y][z] = 1; cos = 1;
							continue;
						}
					}
					else {
						if (poludnie->ch->v[(chunkx - 1)][y][z] == 0) {
							srodek->ch->render[x][y][z] = 1; cos = 1;
							continue;
						}
					}
					if (y < (chunky - 1)) {
						if (srodek->ch->v[x][y + 1][z] == 0) {
							srodek->ch->render[x][y][z] = 1; cos = 1;
							continue;
						}
					}
					else {
						if (gora->ch->v[x][0][z] == 0) {
							srodek->ch->render[x][y][z] = 1; cos = 1;
							continue;
						}
					}
					if (y > 0) {
						if (srodek->ch->v[x][y - 1][z] == 0) {
							srodek->ch->render[x][y][z] = 1; cos = 1;
							continue;
						}
					}
					else {
						if (dol->ch->v[x][(chunky - 1)][z] == 0) {
							srodek->ch->render[x][y][z] = 1; cos = 1;
							continue;
						}
					}
					if (z < (chunkz - 1)) {
						if (srodek->ch->v[x][y][z + 1] == 0) {
							srodek->ch->render[x][y][z] = 1; cos = 1;
							continue;
						}
					}
					else {
						if (wschod->ch->v[x][y][0] == 0) {
							srodek->ch->render[x][y][z] = 1; cos = 1;
							continue;
						}
					}
					if (z > 0) {
						if (srodek->ch->v[x][y][z - 1] == 0) {
							srodek->ch->render[x][y][z] = 1; cos = 1;
							continue;
						}
					}
					else {
						if (zachod->ch->v[x][y][(chunkz - 1)] == 0) {
							srodek->ch->render[x][y][z] = 1; cos = 1;
							continue;
						}
					}
					srodek->ch->render[x][y][z] = 0;
				}
		srodek->ch->state |= optimized;
		if(!cos)srodek->ch->state |= chunkempty;
		return 0;
	}

public:
	MapManager(long long seed):noise(seed){
		chunkss.reserve(32000000);
		loadchunkq.start_exec([&](const Pos& p) {
			Chunkholder* c = chunkss[p];
			if (c != NULL)
				((MapManager*)(this))->loadchunk(c);
			//printf("load %lld,%lld,%lld\n", p->x, p->y, p->z);
			return false; });
		genchunkq.start_exec([&](const Pos& p) {
			Chunkholder* c = chunkss[p];
			if (c != NULL)
				((MapManager*)(this))->genchunk(c);
			//printf("generate %lld,%lld,%lld\n", p->x, p->y, p->z);
			return 0; });
		optchunkq.start_exec([&](const Pos& p) {
			if (((MapManager*)(this))->optimizechunk(p)) {
				return 1;
			}
			//printf("optimize1 %lld,%lld,%lld\n", p->x, p->y, p->z);
			Chunkholder* c = chunkss[p];
			savechunkq.insert(c);
			return 0;
			});
		optchunkq.start_exec([&](const Pos& p) {
			if (((MapManager*)(this))->optimizechunk(p)) {
				return 1;
			}
			//printf("optimize2 %lld,%lld,%lld\n", p->x, p->y, p->z);
			Chunkholder* c = chunkss[p];
			savechunkq.insert(c);
			return 0;
			});
		optchunkq.start_exec([&](const Pos& p) {
			if (((MapManager*)(this))->optimizechunk(p)) {
				return 1;
			}
			//printf("optimize3 %lld,%lld,%lld\n", p->x, p->y, p->z);
			savechunkq.insert(chunkss[p]);
			return 0;
			});
		savechunkq.start_exec([&](Chunkholder*& c) {
			((MapManager*)(this))->savechunk((Chunkholder*)c);
			//printf("save %lld,%lld,%lld\n", c->position.x, c->position.y, c->position.z);
			return false; });
	}
	//playerpos
	void uptate(Pos p,int rd) {
		p.x /= chunkx;
		p.y /= chunky;
		p.z /= chunkz;
		//loadchunkq.sort();
		/*chunkss.lock();
		for (auto &a : chunkss) {
			if(a.second!=NULL)
				if (a.first.odl(p) > 3 * rd) {
					a.second->wtd = rmfrommemory;
					savechunkq.insert(a.second);
					chunkss[a.first] = NULL;
					loadchunkq.remove(a.second->position);
					genchunkq.remove(a.second->position);
					optchunkq.remove(a.second->position);
				}
		}
		chunkss.unlock();*/
		std::vector<Chunkholder*> c;
		for (long long xr = -rd; xr < rd; xr++) {
			for (long long yr = -rd; yr < rd; yr++) {
				for (long long zr = -rd; zr < rd; zr++) {
					Chunkholder* ch = chunkss[p + Pos({ xr, yr, zr })];
					if (ch != NULL) {
						if (ch->ch->state & optimized) {
							if (ch->ch->state & chunkempty)
								continue;
							else
								c.push_back(ch);
						}
						else if (ch->wtd < optimize) {
							ch->wtd = optimize;
							//loadchunkq.insert(&ch->position);
						}
					}
					else {
						ch = new Chunkholder(p + Pos({ xr, yr, zr }));
						ch->wtd = optimize;
						loadchunkq.insert(ch->position);
						chunkss[ch->position] = ch;
					}
				}
			}
		}
		renderchunks.swap(c);
	}

	Chunkholder* getchunk(Pos p) {
		Chunkholder* ch = chunkss[p];
		if (ch == NULL){
			ch = new Chunkholder(p);
			ch->wtd = optimize;
			loadchunkq.insert(ch->position);
			chunkss[ch->position] = ch;
		}
		if (ch->ch->state&generated) {
			optchunkq.insert(ch->position);
			return ch;
		}
		return NULL;
	}

	float& blockat(Pos blockpos) {
		long long posx = blockpos.x;
		long long posy = blockpos.y;
		long long posz = blockpos.z;

		long long chx = posx / chunkx; if (posx < 0)chx -= 1;
		long long chy = posy / chunky; if (posy < 0)chy -= 1;
		long long chz = posz / chunkz; if (posz < 0)chz -= 1;

		if (posx < 0)posx = -posx;
		if (posy < 0)posy = -posy;
		if (posz < 0)posz = -posz;

		posx = posx % chunkx;
		posy = posy % chunky;
		posz = posz % chunkz;

		if (blockpos.x < 0)posx = chunkx-1 - posx;
		if (blockpos.y < 0)posy = chunky-1 - posy;
		if (blockpos.z < 0)posz = chunkz-1 - posz;

		Chunkholder* ch = getchunk({ chx,chy,chz });
		if (ch == NULL) {
			float t = -1;
			return t;
		}
		
		return ch->ch->v[posx][posy][posz];
	}

	MapgenNoise noise;
	std::vector<Chunkholder*> renderchunks;

};


