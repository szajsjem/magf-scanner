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

bool running = true;

struct WaterDrop {
	Pos pos;
	float velocity = 0.0f;
	float water = 1.0f;    // Start with a full unit of water
	float sediment = 0.0f; // Start with no sediment
};

void erosion_thread_lambda(MapManager& m) {
	return;
	// --- Simulation Parameters ---
	const int MAX_LIFESPAN = 100;
	const float GRAVITY = 9.81f;

	const float EROSION_RATE = 0.05f;
	const float DEPOSITION_RATE = 0.1f;
	const float CAPACITY_FACTOR = 6.0f;
	const float EVAPORATION_RATE = 0.02f;
	const float MIN_EROSION_DIFF = 0.01f; // Min height diff to erode

	while (running) {
		// 1. INITIALIZE THE DROP
		Pos start_pos(
			rnd.rndi() % 1000 - 500ll,
			0,
			rnd.rndi() % 1000 - 500ll
		);
		start_pos.y = m.noise.getheight(start_pos.x, start_pos.z) + 20;

		// Find the first air block above the terrain to place the drop
		// m.blockat is called for reading, which is fine here.
		while (m.blockat(start_pos) > 0 && start_pos.y < 255) {
			start_pos.y++;
		}
		// If start_pos ends up in a solid block (e.g., maxHeight reached),
		// it's safer to break or choose a new position.
		if (m.blockat(start_pos) > 0) {
		    // Optionally handle error or re-attempt
		    continue;
		}


		WaterDrop drop{start_pos};

		for (int i = 0; i < MAX_LIFESPAN; ++i) {
			Pos ground_pos = drop.pos + Pos{0, -1, 0};
			float ground_block_value = m.blockat(ground_pos); // Read value

			// A. GRAVITY: If the block below is air, just fall.
			if (ground_block_value == 0) {
				drop.pos.y -= 1;
				drop.velocity += GRAVITY * 0.1f; // Accelerate while falling
				continue; // Skip to next iteration
			}

			// B. SURFACE FLOW: If the block below is solid, flow over it.
			float current_surface_height = ground_pos.y + ground_block_value;
			Pos next_pos = drop.pos;
			float min_neighbor_height = current_surface_height;

			// Find the lowest valid neighbor on the surface
			for (int dx = -1; dx <= 1; ++dx) {
				for (int dz = -1; dz <= 1; ++dz) {
					if (dx == 0 && dz == 0) continue;

					Pos neighbor_air_pos = drop.pos + Pos{dx, 0, dz};
					// Don't flow into solid walls (m.blockat is read-only here)
					if (m.blockat(neighbor_air_pos) > 0) continue;

					Pos neighbor_ground_pos = neighbor_air_pos + Pos{0, -1, 0};
					float neighbor_surface_height =
						neighbor_ground_pos.y + m.blockat(neighbor_ground_pos); // Read-only

					if (neighbor_surface_height < min_neighbor_height) {
						min_neighbor_height = neighbor_surface_height;
						next_pos = neighbor_air_pos;
					}
				}
			}

			float height_delta = current_surface_height - min_neighbor_height;

			// C. ERODE OR DEPOSIT on the ground block BENEATH the drop
			// If height_delta <= 0, we are in a pit or on flat ground.
			if (height_delta <= 0) {
				// Deposit sediment aggressively if stuck or in a pit
				float amount_to_deposit = drop.sediment * DEPOSITION_RATE;
				float new_ground_value = ground_block_value + amount_to_deposit;
				m.setblock(ground_pos, min(1.0f, new_ground_value)); // Write back
				drop.sediment -= amount_to_deposit;
				break; // Stop drop, it's stuck
			} else { // Moving downhill, so erode or deposit based on capacity
				float capacity =
					height_delta * drop.velocity * drop.water * CAPACITY_FACTOR;

				if (drop.sediment > capacity) {
					// Deposit sediment if over capacity
					float amount_to_deposit = (drop.sediment - capacity) * DEPOSITION_RATE;
					float new_ground_value = ground_block_value + amount_to_deposit;
					m.setblock(ground_pos, min(1.0f, new_ground_value)); // Write back
					drop.sediment -= amount_to_deposit;
				} else if (height_delta > MIN_EROSION_DIFF) {
					// Erode if under capacity and slope is steep enough
					float amount_to_erode = min(
						(capacity - drop.sediment) * EROSION_RATE,
						height_delta / 2.0f // Don't erode more than half the slope
					);
					// Clamp erosion so block doesn't go below 0
					amount_to_erode = min(amount_to_erode, ground_block_value);
					float new_ground_value = ground_block_value - amount_to_erode;
					m.setblock(ground_pos, max(0.0f, new_ground_value)); // Write back
					drop.sediment += amount_to_erode;
				}
			}

			// D. UPDATE DROP STATE AND MOVE
			drop.velocity = std::sqrt(
				max(0.0f, drop.velocity * drop.velocity + height_delta * GRAVITY)
			);
			drop.water *= (1.0f - EVAPORATION_RATE);
			drop.pos = next_pos; // Move the drop to the new air block position

			if (drop.water < 0.01f || drop.velocity < 0.01f) {
				break;
			}
		}

		// Deposit any leftover sediment at the final ground position
		Pos final_ground_pos = drop.pos + Pos{0, -1, 0};
		float final_ground_value = m.blockat(final_ground_pos); // Read final value
		if (final_ground_value > 0) { // Check if it's solid (or partially solid) to deposit on
			float new_final_ground_value = final_ground_value + drop.sediment;
			m.setblock(final_ground_pos, min(1.0f, new_final_ground_value)); // Write final value
		}
	}
}

int main() {

	std::vector<glm::vec3> punkty;
	std::vector<glm::vec3> kolor;
	std::vector<glm::vec3> nor;
	MapManager m(69420);

	//m.noise.printData();

	m.uptate({ 0,(long long)m.noise.getheight(0,0) + 2,0 }, 6);
	bool newpoints = 0;


	oknogl o; 
	shader s("vshader.vert", "fshader.frag");//, "gshader.geom");//	);//
	o.playerpos = { 0,m.noise.getheight(0,0) + 2,0 };
	buffer glpoints;
	buffer glcolors;
	buffer glnorm;
	Pos watp = { 0,(long long)m.noise.getheight(0,0) + 5,0 };

	float scrool = 100.0f;
	float scroolshift =10.0f;

	o.scrool = &scrool;
	o.scroolshift = &scroolshift;

	std::thread tw([&]() {
		while (running) {
			std::chrono::milliseconds timespan(100);
			std::this_thread::sleep_for(timespan);
			printf("plpos x:%lf y:%lf z:%lf\n", o.playerpos.x, o.playerpos.y, o.playerpos.z);
			printf("scrool:%lf,scroolshift:%lf\n", scrool, scroolshift);
			Pos plpos = { (long long)o.playerpos.x,(long long)o.playerpos.y ,(long long)o.playerpos.z };
			m.uptate(plpos, 15);
			printf("chunks:%lld\n", m.renderchunks.size());
			std::vector<glm::vec3> pointss;
			std::vector<glm::vec3> norm;
			std::vector<glm::vec3> colrss;
			for(auto& chh : m.renderchunks) {
				if (chh->ch->state == chunkempty)continue;
				std::lock_guard<std::mutex> lock(chh->ch->vtxlock);
				/*for (auto& vert : chh->ch->vertices) {
					pointss.push_back({ vert.position.x + chh->position.x * chunkx, vert.position.y + chh->position.y * chunky, vert.position.z + chh->position.z * chunkz });
					colrss.push_back({ 1, 1, 1 });
					norm.push_back({ vert.normal.x, vert.normal.y, vert.normal.z });
				}*/
				for (int x = 0; x < chunkx; x++) {
					for (int y = 0; y < chunky; y++) {
						for (int z = 0; z < chunkz; z++) {
							float v = chh->ch->v[x][y][z];
							if (v > scrool) {
								v -= scrool;
								if (v > scroolshift)
									v = 1;
								else
									v /= scroolshift;
								pointss.push_back({ (chh->position.x * chunkx) + (float)x, (chh->position.y * chunky) + (float)y, (chh->position.z * chunkz) + (float)z });
								colrss.push_back({v,v,v });
								norm.push_back({ 0.0f, 1.0f, 0.0f }); // Assuming flat surface normals for simplicity
							}
						}
					}
				}
			}
			//for (auto chh : m.renderchunks) {
			//	for (int x = 0; x < chunkx; x++) {
			//		for (int y = 0; y < chunky; y++) {
			//			for (int z = 0; z < chunkz; z++) {
			//				if (chh->ch->render[x][y][z]) {
			//					float zxxx = chh->ch->v[x][y][z];
			//					pointss.push_back({ (chh->position.x * chunkx) + (float)x,(chh->position.y * chunky) + (float)y + zxxx, (chh->position.z * chunkz) + (float)z });
			//					colrss.push_back({ zxxx,zxxx,zxxx });
			//					cv.push_back(glm::vec4(
			//						chh->ch->v[x][y][z],     // Corner 0 - cv.x
			//						chh->ch->v[x + 1][y][z], // Corner 1 - cv.y
			//						chh->ch->v[x + 1][y][z + 1], // Corner 2 - cv.z
			//						chh->ch->v[x][y][z + 1]  // Corner 3 - cv.w
			//					));
			//					cv2.push_back(glm::vec4(
			//						chh->ch->v[x][y + 1][z],   // Corner 4 - cv2.x
			//						chh->ch->v[x + 1][y + 1][z], // Corner 5 - cv2.y
			//						chh->ch->v[x + 1][y + 1][z + 1], // Corner 6 - cv2.z
			//						chh->ch->v[x][y + 1][z + 1] // Corner 7 - cv2.w
			//					));
			//				}
			//			}
			//		}
			//	}
			//}
			/*for (int x = -100; x < 100; x++) {
				for (int z = -100; z < 100; z++) {
					pointss.push_back({ (float)x+plpos.x,(float)m.noise.getheight(x+ plpos.x,z+ plpos.z),(float)z+ plpos.z });
					colrss.push_back({ 1.0f,1.0f,1.0f });
				}
			}*/

			for (auto& sender : m.noise.getSenderPos()) {
				pointss.push_back({ sender.x,sender.y + 0.1,sender.z });
				colrss.push_back({ 1.0f,0.0f,1.0f });
				norm.push_back({ 0.0f,1.0f,0.0f });
				/*pointss.push_back({ sender.x,sender.y - 0.1,sender.z });
				colrss.push_back({ 1.0f,0.0f,1.0f });
				norm.push_back({ 0.0f,1.0f,0.0f });
				pointss.push_back({ sender.x + 0.1,sender.y,sender.z + 0.1 });
				colrss.push_back({ 1.0f,0.0f,1.0f });
				norm.push_back({ 0.0f,1.0f,0.0f });*/
			}
			for(auto& receiver: m.noise.getReceiverPos()) {
				pointss.push_back({ receiver.x,receiver.y ,receiver.z + 0.1 });
				colrss.push_back({ 0.0f,1.0f,1.0f });
				norm.push_back({ 0.0f,1.0f,0.0f });
				/*pointss.push_back({ receiver.x,receiver.y ,receiver.z - 0.1 });
				colrss.push_back({ 0.0f,1.0f,1.0f });
				norm.push_back({ 0.0f,1.0f,0.0f });
				pointss.push_back({ receiver.x + 0.1,receiver.y + 0.1,receiver.z  });
				colrss.push_back({ 0.0f,1.0f,1.0f });
				norm.push_back({ 0.0f,1.0f,0.0f });*/
			}
			for(auto& reflector: m.noise.getReflectorsPos()) {
				pointss.push_back({ reflector.x + 0.1,reflector.y ,reflector.z });
				colrss.push_back({ 1.0f,1.0f,0.0f });
				norm.push_back({ 0.0f,1.0f,0.0f });
				/*pointss.push_back({ reflector.x - 0.1,reflector.y ,reflector.z });
				colrss.push_back({ 1.0f,1.0f,0.0f });
				norm.push_back({ 0.0f,1.0f,0.0f });
				pointss.push_back({ reflector.x ,reflector.y + 0.1,reflector.z + 0.1 });
				colrss.push_back({ 1.0f,1.0f,0.0f });
				norm.push_back({ 0.0f,1.0f,0.0f });*/
			}


			if (!newpoints) {
				printf("points:%lld\n", pointss.size());
				punkty.swap(pointss);
				kolor.swap(colrss);
				nor.swap(norm);
				newpoints = 1;
			}
		}
		});
	std::thread watrdrop(erosion_thread_lambda, std::ref(m));
	std::thread watrdropold([&]() {
		return;
		while (running) {
			int kk = 80;
			float b = m.blockat(watp);
			auto watpp = watp;
			while (1) {
				kk--;
				if (kk == 0)break;
				//Sleep(1);
				m.setblock(watpp, b); 
				watpp = watp;
				b = m.blockat(watp);
				//printf("\n%f ", b);
				if (b < 0)continue;
				if (b <= 0.0) {
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
			glnorm.loadbuff(nor.data(), nor.size() * sizeof(glm::vec3));
			newpoints = 0;
		}

		o.startframe(s);
		o.setarg(0, glpoints, 3);
		o.setarg(1, glcolors, 3);
		o.setarg(2, glnorm, 3);
		o.draw(torender, GL_POINTS);
		o.drawXR(torender, GL_POINTS);
		o.wyswietl();
	}
	running = false;
	tw.join();
	watrdrop.join();
	watrdropold.join();
	o.cleanup();
	m.cleanup();
	glfwTerminate();
	exit(0);
	return 0;
}


