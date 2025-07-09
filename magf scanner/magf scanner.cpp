#include "MapManager.h"
#include <iostream>
#include "rand.h"
#include "glrender.h"
#include <vector>
#include <thread>
#include <chrono>
//#include "../../../C++/include/rendermgrauto.h"
/*
	okno3d ok(800, 600, defkybd);

		for (auto xx : punkty)
			ok.rys({ xx.x,xx.y,xx.z }, 0xffffff);
		ok.wyswietl();
*/

struct WaterDrop {
	Pos pos;
	float velocity = 0.0f;
	float water = 1.0f;    // Start with a full unit of water
	float sediment = 0.0f; // Start with no sediment
};

void erosion_thread(MapManager& m) {
	// --- Simulation Parameters (tweak these to change the erosion style) ---
	const int MAX_LIFESPAN = 80;   // Max steps for a drop before it dies
	const float GRAVITY = 9.81f;   // Influences acceleration
	const float INERTIA = 0.1f;    // How much velocity is preserved
	const float MIN_SLOPE = 0.01f; // Minimum slope to be considered downhill

	// Erosion and Deposition
	const float EROSION_RATE = 0.1f;      // How easily sediment is picked up
	const float DEPOSITION_RATE = 0.2f;   // How easily sediment is dropped
	const float CAPACITY_FACTOR = 8.0f;   // How much sediment water can hold
	const float EVAPORATION_RATE = 0.05f; // How much water is lost each step

	// Main loop to continuously create new water drops
	while (true) {
		// 1. INITIALIZE THE DROP
		// Start at a random position on the terrain surface
		Pos start_pos(
			rnd.rndi() % 1000 - 500ll,
			0,
			rnd.rndi() % 1000 - 500ll
		);
		start_pos.y = m.noise.getheight(start_pos.x, start_pos.z);

		WaterDrop drop{ start_pos };

		// Simulate the drop's life for a fixed number of steps
		for (int i = 0; i < MAX_LIFESPAN; ++i) {
			float& current_block_ref = m.blockat(drop.pos);
			if (current_block_ref < 0) break; // Fell out of the world

			float current_height = current_block_ref + drop.pos.y;

			// 2. FIND LOWEST NEIGHBOR (GRADIENT)
			Pos neighbor_offsets[] = {
				{1, 0, 0}, {-1, 0, 0}, {0, 0, 1}, {0, 0, -1}
			};
			Pos next_pos = drop.pos;
			float min_neighbor_height = current_height;

			for (const auto& offset : neighbor_offsets) {
				Pos neighbor_pos = drop.pos + offset;
				float& neighbor_block_ref = m.blockat(neighbor_pos);

				// Ignore "air" blocks
				if (neighbor_block_ref < 0) continue;

				float neighbor_height = neighbor_block_ref + neighbor_pos.y;
				if (neighbor_height < min_neighbor_height) {
					min_neighbor_height = neighbor_height;
					next_pos = neighbor_pos;
				}
			}

			// 3. EROSION & DEPOSITION
			float height_delta = current_height - min_neighbor_height;

			// If there's a downhill slope, erode or deposit
			if (height_delta > MIN_SLOPE) {
				// Calculate sediment capacity based on velocity, water, and slope
				float capacity =
					height_delta * drop.velocity * drop.water * CAPACITY_FACTOR;

				// If carrying more sediment than capacity, DEPOSIT
				if (drop.sediment > capacity) {
					float amount_to_deposit =
						(drop.sediment - capacity) * DEPOSITION_RATE;
					current_block_ref += amount_to_deposit;
					drop.sediment -= amount_to_deposit;
				}
				// Otherwise, ERODE
				else {
					// Erode an amount proportional to the remaining capacity
					// but don't erode more than the height difference itself
					float amount_to_erode = std::min(
						(capacity - drop.sediment) * EROSION_RATE,
						height_delta
					);
					current_block_ref -= amount_to_erode;
					drop.sediment += amount_to_erode;
				}
			}
			// If on flat ground or in a pit, deposit sediment
			else {
				float amount_to_deposit = drop.sediment * DEPOSITION_RATE;
				current_block_ref += amount_to_deposit;
				drop.sediment -= amount_to_deposit;
			}

			// 4. UPDATE DROP STATE
			// Update velocity based on gravity and the height delta
			drop.velocity = std::sqrt(
				drop.velocity * drop.velocity + height_delta * GRAVITY
			);
			// Evaporate some water
			drop.water *= (1.0f - EVAPORATION_RATE);

			// 5. MOVE THE DROP
			drop.pos = next_pos;

			// End simulation if drop has evaporated or stopped moving
			if (drop.water < 0.01f || drop.velocity < 0.01f) {
				break;
			}
		}

		// After the loop, deposit any remaining sediment at the final position
		float& final_block_ref = m.blockat(drop.pos);
		if (final_block_ref >= 0) {
			final_block_ref += drop.sediment;
		}
	}
}

int main() {

	std::vector<glm::vec3> punkty;
	std::vector<glm::vec3> kolor;
	std::vector<glm::vec4> cub;
	std::vector<glm::vec4> cub2;
	MapManager m(69420);


	m.uptate({ 0,(long long)m.noise.getheight(0,0) + 2,0 }, 12);
	bool newpoints = 0;


	oknogl o; shader s("vshader.vert", "fshader.frag" , "gshader.geom");//	);//
	o.playerpos = { 0,m.noise.getheight(0,0) + 2,0 };
	buffer glpoints;
	buffer glcolors;
	buffer glC,glC2;
	Pos watp = { 0,(long long)m.noise.getheight(0,0) + 5,0 };

	std::thread tw([&]() {
		while (1) {
			std::chrono::milliseconds timespan(100);
			std::this_thread::sleep_for(timespan);
			printf("plpos x:%lf y:%lf z:%lf\n", o.playerpos.x, o.playerpos.y, o.playerpos.z);
			printf("watpos x:%lld y:%lld z:%lld\n", watp.x, watp.y, watp.z);
			Pos plpos = { (long long)o.playerpos.x,(long long)o.playerpos.y ,(long long)o.playerpos.z };
			m.uptate(plpos, 1);
			std::vector<glm::vec3> pointss;
			std::vector<glm::vec3> colrss;
			std::vector<glm::vec4> cv;
			std::vector<glm::vec4> cv2;
			for (auto chh : m.renderchunks) {
				for (int x = 0; x < chunkx; x++) {
					for (int y = 0; y < chunky; y++) {
						for (int z = 0; z < chunkz; z++) {
							if (chh->ch->render[x][y][z]) {
								float zxxx = chh->ch->v[x][y][z];
								pointss.push_back({ (chh->position.x * chunkx) + (float)x,(chh->position.y * chunky) + (float)y + zxxx, (chh->position.z * chunkz) + (float)z });
								colrss.push_back({ zxxx,zxxx,zxxx });
								cv.push_back(glm::vec4(
									chh->ch->v[x][y][z],     // Corner 0 - cv.x
									chh->ch->v[x + 1][y][z], // Corner 1 - cv.y
									chh->ch->v[x + 1][y][z + 1], // Corner 2 - cv.z
									chh->ch->v[x][y][z + 1]  // Corner 3 - cv.w
								));
								cv2.push_back(glm::vec4(
									chh->ch->v[x][y + 1][z],   // Corner 4 - cv2.x
									chh->ch->v[x + 1][y + 1][z], // Corner 5 - cv2.y
									chh->ch->v[x + 1][y + 1][z + 1], // Corner 6 - cv2.z
									chh->ch->v[x][y + 1][z + 1] // Corner 7 - cv2.w
								));
							}
						}
					}
				}
			}
			/*for (int x = -100; x < 100; x++) {
				for (int z = -100; z < 100; z++) {
					pointss.push_back({ (float)x+plpos.x,(float)m.noise.getheight(x+ plpos.x,z+ plpos.z),(float)z+ plpos.z });
					colrss.push_back({ 1.0f,1.0f,1.0f });
				}
			}*/

			pointss.push_back({ watp.x,watp.y + 0.1,watp.z });
			colrss.push_back({ 1.0f,0.0f,1.0f });
			cv.push_back(glm::vec4(1,0,0,0));
			cv2.push_back(glm::vec4(0,0,0,0));

			if (!newpoints) {
				punkty.swap(pointss);
				kolor.swap(colrss);
				cub.swap(cv);
				cub2.swap(cv2);
				newpoints = 1;
			}
		}
		});
	std::thread watrdrop(erosion_thread, std::ref(m));
	std::thread watrdropold([&]() {
		return;
		while (1) {
			int kk = 80;
			while (1) {
				kk--;
				if (kk == 0)break;
				//Sleep(1);
				float& b = m.blockat(watp);
				//printf("\n%f ", b);
				if (b < 0)continue;
				if (b < 0.00001) {
					//printf("\n");
					watp.y -= 1;
					continue;
				}
				float k[4] = { m.blockat(watp + Pos({1,0,0})),m.blockat(watp + Pos({-1,0,0})),m.blockat(watp + Pos({0,0,1})),m.blockat(watp + Pos({0,0,-1})) };
				int nai = 0;
				float v = k[0];
				for (int i = 0; i < 4; i++) {
					//printf("%f ", k[i]);
					if (k[i] < v) {
						v = k[i];
						nai = i;
					}
				}
				//printf("\n");
				if (v < 0)continue;

				if (v > 0) {
					if (v >= b) {
						b = (v + b - 0.00001) / 2;
						if (b < 0)b = 0;
						break;
					}
					b = v + 0.00001;

					switch (nai)
					{
					case(0):
						watp.x += 1;
						break;
					case(1):
						watp.x -= 1;
						break;
					case(2):
						watp.z += 1;
						break;
					case(3):
						watp.z -= 1;
						break;
					}
				}
				else {
					b = 0;
					watp.y -= 1;

					float k[4] = { m.blockat(watp + Pos({1,0,0})),m.blockat(watp + Pos({-1,0,0})),m.blockat(watp + Pos({0,0,1})),m.blockat(watp + Pos({0,0,-1})) };
					int nai = 0;
					float v = k[0];
					for (int i = 0; i < 4; i++) {
						//printf("%f ", k[i]);
						if (k[i] < v) {
							v = k[i];
							nai = i;
						}
					}
					//printf("\n");
					if (v < 0)continue;

					switch (nai)
					{
					case(0):
						watp.x += 1;
						break;
					case(1):
						watp.x -= 1;
						break;
					case(2):
						watp.z += 1;
						break;
					case(3):
						watp.z -= 1;
						break;
					}
				}
			}
			watp = Pos(rnd.rndi() % 1000 - 500ll, 0, rnd.rndi() % 1000 - 500ll);//new random position
			watp.y = m.noise.getheight(watp.x, watp.z) + chunky;//get original height, low chance of beig filled with sediment
		}
		});

	int torender = 0;
	while (glfwGetKey(o.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(o.window) == 0) {

		if (newpoints) {
			torender = punkty.size();
			glpoints.loadbuff(punkty.data(), punkty.size() * sizeof(glm::vec3));
			glcolors.loadbuff(kolor.data(), kolor.size() * sizeof(glm::vec3));
			glC.loadbuff(cub.data(), cub.size() * sizeof(glm::vec4));
			glC2.loadbuff(cub2.data(), cub2.size() * sizeof(glm::vec4));
			newpoints = 0;
		}

		o.startframe(s);
		o.setarg(0, glpoints, 3);
		o.setarg(1, glcolors, 3);
		o.setarg(2, glC, 4);
		o.setarg(3, glC2, 4);
		o.draw(torender, GL_POINTS);
		o.wyswietl();
	}
	exit(0);
	return 0;
}


