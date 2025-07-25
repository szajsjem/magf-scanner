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


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define chunkx 32
#define chunky 32
#define chunkz 32

#define iso 0.5f

const int edges[256] = {
	0x0,
	0x109,
	0x203,
	0x30a,
	0x406,
	0x50f,
	0x605,
	0x70c,
	0x80c,
	0x905,
	0xa0f,
	0xb06,
	0xc0a,
	0xd03,
	0xe09,
	0xf00,
	0x190,
	0x99,
	0x393,
	0x29a,
	0x596,
	0x49f,
	0x795,
	0x69c,
	0x99c,
	0x895,
	0xb9f,
	0xa96,
	0xd9a,
	0xc93,
	0xf99,
	0xe90,
	0x230,
	0x339,
	0x33,
	0x13a,
	0x636,
	0x73f,
	0x435,
	0x53c,
	0xa3c,
	0xb35,
	0x83f,
	0x936,
	0xe3a,
	0xf33,
	0xc39,
	0xd30,
	0x3a0,
	0x2a9,
	0x1a3,
	0xaa,
	0x7a6,
	0x6af,
	0x5a5,
	0x4ac,
	0xbac,
	0xaa5,
	0x9af,
	0x8a6,
	0xfaa,
	0xea3,
	0xda9,
	0xca0,
	0x460,
	0x569,
	0x663,
	0x76a,
	0x66,
	0x16f,
	0x265,
	0x36c,
	0xc6c,
	0xd65,
	0xe6f,
	0xf66,
	0x86a,
	0x963,
	0xa69,
	0xb60,
	0x5f0,
	0x4f9,
	0x7f3,
	0x6fa,
	0x1f6,
	0xff,
	0x3f5,
	0x2fc,
	0xdfc,
	0xcf5,
	0xfff,
	0xef6,
	0x9fa,
	0x8f3,
	0xbf9,
	0xaf0,
	0x650,
	0x759,
	0x453,
	0x55a,
	0x256,
	0x35f,
	0x55,
	0x15c,
	0xe5c,
	0xf55,
	0xc5f,
	0xd56,
	0xa5a,
	0xb53,
	0x859,
	0x950,
	0x7c0,
	0x6c9,
	0x5c3,
	0x4ca,
	0x3c6,
	0x2cf,
	0x1c5,
	0xcc,
	0xfcc,
	0xec5,
	0xdcf,
	0xcc6,
	0xbca,
	0xac3,
	0x9c9,
	0x8c0,
	0x8c0,
	0x9c9,
	0xac3,
	0xbca,
	0xcc6,
	0xdcf,
	0xec5,
	0xfcc,
	0xcc,
	0x1c5,
	0x2cf,
	0x3c6,
	0x4ca,
	0x5c3,
	0x6c9,
	0x7c0,
	0x950,
	0x859,
	0xb53,
	0xa5a,
	0xd56,
	0xc5f,
	0xf55,
	0xe5c,
	0x15c,
	0x55,
	0x35f,
	0x256,
	0x55a,
	0x453,
	0x759,
	0x650,
	0xaf0,
	0xbf9,
	0x8f3,
	0x9fa,
	0xef6,
	0xfff,
	0xcf5,
	0xdfc,
	0x2fc,
	0x3f5,
	0xff,
	0x1f6,
	0x6fa,
	0x7f3,
	0x4f9,
	0x5f0,
	0xb60,
	0xa69,
	0x963,
	0x86a,
	0xf66,
	0xe6f,
	0xd65,
	0xc6c,
	0x36c,
	0x265,
	0x16f,
	0x66,
	0x76a,
	0x663,
	0x569,
	0x460,
	0xca0,
	0xda9,
	0xea3,
	0xfaa,
	0x8a6,
	0x9af,
	0xaa5,
	0xbac,
	0x4ac,
	0x5a5,
	0x6af,
	0x7a6,
	0xaa,
	0x1a3,
	0x2a9,
	0x3a0,
	0xd30,
	0xc39,
	0xf33,
	0xe3a,
	0x936,
	0x83f,
	0xb35,
	0xa3c,
	0x53c,
	0x435,
	0x73f,
	0x636,
	0x13a,
	0x33,
	0x339,
	0x230,
	0xe90,
	0xf99,
	0xc93,
	0xd9a,
	0xa96,
	0xb9f,
	0x895,
	0x99c,
	0x69c,
	0x795,
	0x49f,
	0x596,
	0x29a,
	0x393,
	0x99,
	0x190,
	0xf00,
	0xe09,
	0xd03,
	0xc0a,
	0xb06,
	0xa0f,
	0x905,
	0x80c,
	0x70c,
	0x605,
	0x50f,
	0x406,
	0x30a,
	0x203,
	0x109,
	0x0
};

const int edgeConnections[12][2] = {
	{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5},
	{5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5},
	{2, 6}, {3, 7}
};


static const int triangulation[256][16] = {
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1 },
	{ 8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1 },
	{ 3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1 },
	{ 4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1 },
	{ 4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1 },
	{ 9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1 },
	{ 10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1 },
	{ 5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1 },
	{ 5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1 },
	{ 8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1 },
	{ 2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1 },
	{ 2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1 },
	{ 11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1 },
	{ 5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1 },
	{ 11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1 },
	{ 11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1 },
	{ 2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1 },
	{ 6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1 },
	{ 3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1 },
	{ 6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1 },
	{ 6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1 },
	{ 8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1 },
	{ 7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1 },
	{ 3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1 },
	{ 0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1 },
	{ 9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1 },
	{ 8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1 },
	{ 5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1 },
	{ 0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1 },
	{ 6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1 },
	{ 10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1 },
	{ 1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1 },
	{ 0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1 },
	{ 3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1 },
	{ 6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1 },
	{ 9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1 },
	{ 8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1 },
	{ 3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1 },
	{ 10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1 },
	{ 10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1 },
	{ 2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1 },
	{ 7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1 },
	{ 2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1 },
	{ 1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1 },
	{ 11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1 },
	{ 8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1 },
	{ 0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1 },
	{ 7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1 },
	{ 7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1 },
	{ 10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1 },
	{ 0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1 },
	{ 7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1 },
	{ 6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1 },
	{ 4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1 },
	{ 10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1 },
	{ 8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1 },
	{ 1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1 },
	{ 10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1 },
	{ 10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1 },
	{ 9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1 },
	{ 7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1 },
	{ 3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1 },
	{ 7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1 },
	{ 3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1 },
	{ 6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1 },
	{ 9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1 },
	{ 1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1 },
	{ 4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1 },
	{ 7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1 },
	{ 6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1 },
	{ 0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1 },
	{ 6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1 },
	{ 0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1 },
	{ 11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1 },
	{ 6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1 },
	{ 5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1 },
	{ 9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1 },
	{ 1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1 },
	{ 10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1 },
	{ 0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1 },
	{ 11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1 },
	{ 9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1 },
	{ 7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1 },
	{ 2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1 },
	{ 9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1 },
	{ 9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1 },
	{ 1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1 },
	{ 0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1 },
	{ 10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1 },
	{ 2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1 },
	{ 0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1 },
	{ 0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1 },
	{ 9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1 },
	{ 5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1 },
	{ 5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1 },
	{ 8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1 },
	{ 9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1 },
	{ 1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1 },
	{ 3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1 },
	{ 4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1 },
	{ 9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1 },
	{ 11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1 },
	{ 2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1 },
	{ 9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1 },
	{ 3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1 },
	{ 1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1 },
	{ 4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1 },
	{ 0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1 },
	{ 1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
};

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

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
};

struct Chunk {
	float v[chunkx+1][chunky+1][chunkz+1];
	std::vector<Vertex> vertices; mutable std::mutex vtxlock;
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
	void normalize(float r=1) {
		double d = sqrt(x * x + y * y + z * z) / r;
		if (d == 0) return;
		x /= d;
		y /= d;
		z /= d;
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
	double odlkw(const Pos o) const {
		long long dx = o.x - x;
		long long dy = o.y - y;
		long long dz = o.z - z;
		return (dx * dx + dy * dy + dz * dz);
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
	std::vector<Pos> receiverPos;
	std::vector<Pos> senderPos;
	std::vector<Pos> reflectorsPos;
	std::vector<std::vector<float>> signalsRxS;
	std::vector<std::vector<float>> signalsRxR;
	int receiverCount = 6;
	int senderCount = 12;
	int reflectorCount = 10;
	int sampleRate = 192000; // Hz
	int timeCount = sampleRate * 3;
	float speedOfLight = 299792.0f;// 299792458.0f; // m/s
	float peaksmooth = 0.7f;
public:
	Pos randomPointOnSphere(float radius) {
		float theta = ((float)rand() / RAND_MAX) * 2 * 3.1415;
		float phi = acos(1 - 2 * ((float)rand() / RAND_MAX));
		Pos p;
		p.x = radius * sin(phi) * cos(theta);
		p.y = radius * sin(phi) * sin(theta);
		p.z = radius * cos(phi);
		return p;
	}
	MapgenNoise(long long seed):p(2, 0.5, 8, 0.2, seed),h(0.15,4,8,0.5,seed+1){
		receiverPos.resize(receiverCount);
		senderPos.resize(senderCount);
		reflectorsPos.resize(reflectorCount);
		signalsRxS.resize(receiverCount * senderCount);
		signalsRxR.resize(receiverCount * receiverCount);
		for (int i = 0; i < receiverCount; i++) {
			/*receiverPos[i].x = rand() - RAND_MAX / 2;
			receiverPos[i].y = rand() - RAND_MAX / 2;
			receiverPos[i].z = rand() - RAND_MAX / 2;
			receiverPos[i].normalize(100);*/
			receiverPos[i] = randomPointOnSphere(100);
		}
		for (int i = 0; i < senderCount; i++) {
			/*senderPos[i].x = rand() - RAND_MAX / 2;
			senderPos[i].y = rand() - RAND_MAX / 2;
			senderPos[i].z = rand() - RAND_MAX / 2;
			senderPos[i].normalize(100);*/
			senderPos[i] = randomPointOnSphere(100);
		}
		for (int i = 0; i < reflectorCount; i++) {
			/*reflectorsPos[i].x = rand() - RAND_MAX / 2;
			reflectorsPos[i].y = rand() - RAND_MAX / 2;
			reflectorsPos[i].z = rand() - RAND_MAX / 2;
			reflectorsPos[i].normalize(50);*/
			reflectorsPos[i] = randomPointOnSphere(500);
		}
		for (int i = 0; i < receiverCount * senderCount; i++) {
			signalsRxS[i].resize(timeCount);
			for (int j = 0; j < timeCount; j++) {
				signalsRxS[i][j] = 0.001*rand()/(float)RAND_MAX;
			}
		}
		for (int i = 0; i < receiverCount * receiverCount; i++) {
			signalsRxR[i].resize(timeCount);
			for (int j = 0; j < timeCount; j++) {
				signalsRxR[i][j] = 0.001 * rand() / (float)RAND_MAX;
			}
		}
		//simulate magnetic field reflection correlation peaks
		for (int i = 0; i < receiverCount; i++) {
			for (int j = 0; j < senderCount; j++) {
				int signalIndex = i * senderCount + j;
				for( int l=0;l<reflectorCount;l++) {
					float pathLength = receiverPos[i].odl(reflectorsPos[l]) + reflectorsPos[l].odl(senderPos[j]);
					for (int k = 0; k < timeCount; k++) {
						float time = k / (float)sampleRate;
						float distance = pathLength - speedOfLight * time;
						distance *= distance;
						if (distance < 0 || distance >= signalsRxS[signalIndex].size()) {
							continue; // skip out of bounds
						}
						signalsRxS[signalIndex][k] += exp(-distance * peaksmooth);
					}
				}
			}
		}
		//simulate magnetic field emission correlation peaks
		for (int i = 0; i < receiverCount; i++) {
			for (int j = 0; j < receiverCount; j++) {
				int signalIndex = i * receiverCount + j;
				for (int l = 0; l < reflectorCount; l++) {
					float pathLength = receiverPos[i].odl(reflectorsPos[l]) + reflectorsPos[l].odl(receiverPos[j]);
					for (int k = 0; k < timeCount; k++) {
						float time = k / (float)sampleRate;
						float distance = pathLength - speedOfLight * time;
						distance *= distance;
						if (distance < 0 || distance >= signalsRxS[signalIndex].size()) {
							continue; // skip out of bounds
						}
						signalsRxR[signalIndex][k] += exp(-distance * peaksmooth);
					}
				}
			}
		}
	}
	double getheight(long long x, long long z) {
		return 0;
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
		return magprobablity(Pos(x, y, z));
		constexpr double xnoisescale = 138.254;
		constexpr double ynoisescale = 134.342;
		constexpr double znoisescale = 136.666;

		double v = (tanh((y - height) / 30)+1)/2;

		if (y < height) {
			double tt = tanh((y - height) / 30) / 2;
			v = p.noise((x) / xnoisescale, (y) / ynoisescale, (z) / znoisescale) + tt;
		}

		return v;
	}

	float interpolate(float prev, float next, float t) {
		return prev * (1.0f - t) + next * t;
	}
	float interpolate(float y0, float y1, float y2, float t) {
		// Fit parabola through y0, y1, y2 and evaluate at t (0 <= t < 1)
		float a = 0.5f * (y0 - 2.0f * y1 + y2);
		float b = 0.5f * (y2 - y0);
		float c = y1;
		return a * t * t + b * t + c;
	}
	float interpolate(float y0, float y1, float y2, float y3, float t) {
		float a0 = -0.5f * y0 + 1.5f * y1 - 1.5f * y2 + 0.5f * y3;
		float a1 = y0 - 2.5f * y1 + 2.0f * y2 - 0.5f * y3;
		float a2 = -0.5f * y0 + 0.5f * y2;
		float a3 = y1;
		return ((a0 * t + a1) * t + a2) * t + a3;
	}
	float magprobablity(Pos p) {
		//sum correlation peaks of all receivers and senders at position p
		float sum = 0;
		for (int i = 0; i < receiverPos.size(); i++) {
			for (int j = 0; j < senderPos.size(); j++) {
				float distance = p.odl(receiverPos[i]) + p.odl(senderPos[j]);
				float distFloat = distance * sampleRate / speedOfLight;

				int dist = static_cast<int>(distFloat);
				float t = distFloat - dist; // fractional part

				auto& signal = signalsRxS[i * senderPos.size() + j];

				// Ensure dist and dist+1 are within bounds
				if (dist < 1 || dist + 2 >= signal.size()) {
					continue;
				}
				float y0 = signal[dist - 1];
				float y1 = signal[dist];
				float y2 = signal[dist + 1];
				float y3 = signal[dist + 2];

				float interpolated = interpolate(y0, y1, y2, t);
				sum += interpolated;
			}
		}
		for (int i = 0; i < receiverPos.size(); i++) {
			for (int j = 0; j < receiverPos.size(); j++) {
				float distance = p.odl(receiverPos[i]) + p.odl(receiverPos[j]);
				float distFloat = distance * sampleRate / speedOfLight;
				int dist = static_cast<int>(distFloat);
				float t = distFloat - dist; // fractional part
				auto& signal = signalsRxR[i * receiverPos.size() + j];
				// Ensure dist and dist+1 are within bounds
				if (dist < 1 || dist + 2 >= signal.size()) {
					continue;
				}
				float y0 = signal[dist - 1];
				float y1 = signal[dist];
				float y2 = signal[dist + 1];
				float y3 = signal[dist + 2];

				float interpolated = interpolate(y0, y1, y2, t);
				sum += interpolated;
			}
		}
		return sum;
	}
	std::vector<Pos> getReceiverPos() const {
		return receiverPos;
	}
	std::vector<Pos> getSenderPos() const {
		return senderPos;
	}
	std::vector<Pos> getReflectorsPos() const {
		return reflectorsPos;
	}
	void printData() {
		int i = 0;
		int j = 0;
		for (int k = 0; k < timeCount; k++) {
			printf("%f\n", signalsRxS[i * receiverCount + j][k]);
		}
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


glm::vec3 interpolateVertex(glm::vec3 p1, glm::vec3 p2, float val1, float val2, float isoLevel) {
	if (std::abs(isoLevel - val1) < 0.00001f) return p1;
	if (std::abs(isoLevel - val2) < 0.00001f) return p2;
	if (std::abs(val1 - val2) < 0.00001f) return p1;
	float t = (isoLevel - val1) / (val2 - val1);
	return glm::mix(p1, p2, t);
}

void GenerateChunkMeshCPU(const Chunk& chunk, float isoLevel, std::vector<Vertex>& out_vertices) {
	std::lock_guard<std::mutex> lock(chunk.vtxlock);
	out_vertices.clear();
	return;

	const glm::vec3 cornerOffsets[] = {
		{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1},
		{0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}
	};

	for (int x = 0; x < chunkx; ++x) {
		for (int y = 0; y < chunky; ++y) {
			for (int z = 0; z < chunkz; ++z) {
				// Get 8 corner values
				float cornerValues[8];
				glm::vec3 cornerPositions[8];
				int cubeIndex = 0;

				for (int i = 0; i < 8; ++i) {
					glm::ivec3 cornerPos = glm::ivec3(x, y, z) + glm::ivec3(cornerOffsets[i]);
					cornerPositions[i] = glm::vec3(cornerPos);
					cornerValues[i] = chunk.v[cornerPos.x][cornerPos.y][cornerPos.z];
					if (cornerValues[i] < isoLevel) {
						cubeIndex |= (1 << i);
					}
				}

				if (cubeIndex == 0 || cubeIndex == 255) {
					continue;
				}

				glm::vec3 edgeVertices[12];
				for (int i = 0; i < 12; ++i) {
					int c1_idx = edgeConnections[i][0];
					int c2_idx = edgeConnections[i][1];
					edgeVertices[i] = interpolateVertex(
						cornerPositions[c1_idx], cornerPositions[c2_idx],
						cornerValues[c1_idx], cornerValues[c2_idx], isoLevel
					);
				}

				for (int i = 0; triangulation[cubeIndex][i] != -1; i += 3) {
					Vertex v1, v2, v3;
					v1.position = glm::vec4(edgeVertices[triangulation[cubeIndex][i]], 1.0f);
					v2.position = glm::vec4(edgeVertices[triangulation[cubeIndex][i + 1]], 1.0f);
					v3.position = glm::vec4(edgeVertices[triangulation[cubeIndex][i + 2]], 1.0f);

					glm::vec3 normal = glm::normalize(glm::cross(
						glm::vec3(v2.position) - glm::vec3(v1.position),
						glm::vec3(v3.position) - glm::vec3(v1.position)
					));

					v1.normal = glm::vec4(normal, 0.0f);
					v2.normal = glm::vec4(normal, 0.0f);
					v3.normal = glm::vec4(normal, 0.0f);

					out_vertices.push_back(v1);
					out_vertices.push_back(v2);
					out_vertices.push_back(v3);
				}
			}
		}
	}
}

void fixSinglePointMesh(
	const Chunk& chunk,
	int changedX,
	int changedY,
	int changedZ,
	std::vector<Vertex>& meshVertices,
	float isoLevel
) {
	if ((chunk.state & optimized) != optimized)return;

	std::lock_guard<std::mutex> lock(chunk.vtxlock);

	if (meshVertices.size() > 0) {
		// --- 1. REMOVE old triangles ---

		// The 8 cubes affected by the point (changedX, Y, Z) form a 2x2x2 block
		// of space. Any triangle generated by these cubes must lie entirely
		// within this bounding box.
		glm::vec3 minBound(changedX - 1.0f, changedY - 1.0f, changedZ - 1.0f);
		glm::vec3 maxBound(changedX + 1.0f, changedY + 1.0f, changedZ + 1.0f);

		// This is a slightly more complex but correct way to remove whole triangles.
		std::vector<Vertex> keptVertices;
		keptVertices.reserve(meshVertices.size());

		for (size_t i = 0; i < meshVertices.size(); i += 3) {
			Vertex& v1 = meshVertices[i];
			Vertex& v2 = meshVertices[i + 1];
			Vertex& v3 = meshVertices[i + 2];

			// If ALL THREE vertices of a triangle are inside the affected region,
			// then it's a triangle we need to remove. Otherwise, we keep it.
			bool isV1Inside = (v1.position.x >= minBound.x && v1.position.x <= maxBound.x && v1.position.y >= minBound.y && v1.position.y <= maxBound.y && v1.position.z >= minBound.z && v1.position.z <= maxBound.z);
			bool isV2Inside = (v2.position.x >= minBound.x && v2.position.x <= maxBound.x && v2.position.y >= minBound.y && v2.position.y <= maxBound.y && v2.position.z >= minBound.z && v2.position.z <= maxBound.z);
			bool isV3Inside = (v3.position.x >= minBound.x && v3.position.x <= maxBound.x && v3.position.y >= minBound.y && v3.position.y <= maxBound.y && v3.position.z >= minBound.z && v3.position.z <= maxBound.z);

			if (!(isV1Inside && isV2Inside && isV3Inside)) {
				keptVertices.push_back(v1);
				keptVertices.push_back(v2);
				keptVertices.push_back(v3);
			}
		}
		meshVertices.swap(keptVertices);

	}
	// --- 2. REGENERATE triangles for the 8 affected cubes ---

	std::vector<Vertex> newlyGeneratedVertices;
	const glm::vec3 cornerOffsets[] = {
		{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1},
		{0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}
	};

	// Iterate through the 8 cubes that share the changed point.
	// The origin of these cubes ranges from (x-1, y-1, z-1) to (x, y, z).
	for (int cx = changedX - 1; cx <= changedX; ++cx) {
		for (int cy = changedY - 1; cy <= changedY; ++cy) {
			for (int cz = changedZ - 1; cz <= changedZ; ++cz) {
				// Boundary check for the cube's origin
				if (cx < 0 || cy < 0 || cz < 0 || cx >= chunkx || cy >= chunky || cz >= chunkz) {
					continue;
				}

				// --- This is the standard Marching Cubes logic for one cube ---
				float cornerValues[8];
				glm::vec3 cornerPositions[8];
				int cubeIndex = 0;

				for (int i = 0; i < 8; ++i) {
					glm::ivec3 cornerPos = glm::ivec3(cx, cy, cz) + glm::ivec3(cornerOffsets[i]);
					cornerPositions[i] = glm::vec3(cornerPos);
					cornerValues[i] = chunk.v[cornerPos.x][cornerPos.y][cornerPos.z];
					if (cornerValues[i] <= isoLevel) {
						cubeIndex |= (1 << i);
					}
				}

				if (cubeIndex == 0 || cubeIndex == 255) {
					continue;
				}

				glm::vec3 edgeVertices[12];
				for (int i = 0; i < 12; ++i) {
					int c1_idx = edgeConnections[i][0];
					int c2_idx = edgeConnections[i][1];
					edgeVertices[i] = interpolateVertex(
						cornerPositions[c1_idx], cornerPositions[c2_idx],
						cornerValues[c1_idx], cornerValues[c2_idx], isoLevel
					);
				}

				for (int i = 0; triangulation[cubeIndex][i] != -1; i += 3) {
					Vertex v1, v2, v3;
					v1.position = glm::vec4(edgeVertices[triangulation[cubeIndex][i]], 1.0f);
					v2.position = glm::vec4(edgeVertices[triangulation[cubeIndex][i + 1]], 1.0f);
					v3.position = glm::vec4(edgeVertices[triangulation[cubeIndex][i + 2]], 1.0f);

					glm::vec3 normal = glm::normalize(glm::cross(
						glm::vec3(v2.position) - glm::vec3(v1.position),
						glm::vec3(v3.position) - glm::vec3(v1.position)
					));

					v1.normal = v2.normal = v3.normal = glm::vec4(normal, 0.0f);

					newlyGeneratedVertices.push_back(v1);
					newlyGeneratedVertices.push_back(v2);
					newlyGeneratedVertices.push_back(v3);
				}
			}
		}
	}

	if (meshVertices.size() == 0)
		meshVertices = newlyGeneratedVertices;
	else
		if (newlyGeneratedVertices.size() > 0)
			// --- 3. ADD the new triangles to the main mesh vector ---
			meshVertices.insert(meshVertices.end(), newlyGeneratedVertices.begin(), newlyGeneratedVertices.end());
}

class MapManager {
	Queue<Pos> loadchunkq;
	Queue<Pos> genchunkq;
	Queue<Pos> optchunkq;
	Queue<Chunkholder*> savechunkq;

	hashmap<Pos,Chunkholder*> chunkss;


	GLuint scalarFieldSSBO;
	GLuint vertexBufferSSBO;
	GLuint atomicCounterBuffer;
	GLuint computeProgram;
	GLuint renderProgram;
	GLuint vao;
	int generatedVertexCount = 0;

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
		}/*
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
		bool cos = 0;
		if (!(dol->ch->state & generated)) return 1;
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
		if (!cos)
			srodek->ch->state |= chunkempty;
		else */
			GenerateChunkMeshCPU(*srodek->ch, iso, srodek->ch->vertices);
		srodek->ch->state |= optimized;
		return 0;
	}

	void setup(const Chunk& chunk) {
		// Assume shader programs are already compiled and linked
		// computeProgram = ...;
		// renderProgram = ...;

		// Define chunk dimensions
		const int CHUNK_X = chunkx + 1;
		const int CHUNK_Y = chunky + 1;
		const int CHUNK_Z = chunkz + 1;

		// 1. Create the SSBO for the scalar field data
		glGenBuffers(1, &scalarFieldSSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, scalarFieldSSBO);
		glBufferData(
			GL_SHADER_STORAGE_BUFFER,
			sizeof(chunk.v),
			&chunk.v[0][0][0],
			GL_STATIC_DRAW
		);

		// 2. Create the output VBO/SSBO.
		// We must allocate a "worst-case" size. A cube can have 5 triangles.
		const int maxCubes = chunkx * chunky * chunkz;
		const int maxTriangles = maxCubes * 5;
		const int maxVertices = maxTriangles * 3;
		glGenBuffers(1, &vertexBufferSSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertexBufferSSBO);
		glBufferData(
			GL_SHADER_STORAGE_BUFFER,
			maxVertices * sizeof(Vertex),
			nullptr,
			GL_DYNAMIC_DRAW
		);

		// 3. Create the atomic counter buffer
		glGenBuffers(1, &atomicCounterBuffer);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicCounterBuffer);
		glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

		// 4. Setup VAO for rendering the result
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		// Bind the generated vertex buffer as a VBO
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferSSBO);
		// Position attribute
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		// Normal attribute
		glVertexAttribPointer(
			1,
			4,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			(void*)offsetof(Vertex, normal)
		);
		glEnableVertexAttribArray(1);

		// Unbind everything for safety
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
public:
	MapManager(long long seed):noise(seed){
		chunkss.reserve(320000);
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

	float blockat(Pos blockpos) {
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

	void setblock(Pos blockpos,float v) {
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

		if (blockpos.x < 0)posx = chunkx - 1 - posx;
		if (blockpos.y < 0)posy = chunky - 1 - posy;
		if (blockpos.z < 0)posz = chunkz - 1 - posz;

		Chunkholder* ch = getchunk({ chx,chy,chz });
		if (ch == NULL) {
			return;
		}

		ch->ch->v[posx][posy][posz]=v;
		//ch->ch->render[posx][posy][posz] = 1;
		if(posx==0)
			setblockp({ chx,chy,chz },{posx,posy,posz}, v,1,0,0);
		if(posy==0)
			setblockp({ chx,chy,chz }, { posx,posy,posz }, v, 0, 1, 0);
		if (posz == 0)
			setblockp({ chx,chy,chz }, { posx,posy,posz }, v, 0, 0, 1);
		if (posx == 0 && posy==0)
			setblockp({ chx,chy,chz }, { posx,posy,posz }, v, 1, 1, 0);
		if (posx == 0 && posz == 0)
			setblockp({ chx,chy,chz }, { posx,posy,posz }, v, 1, 0, 1);
		if (posy == 0 && posz == 0)
			setblockp({ chx,chy,chz }, { posx,posy,posz }, v, 0, 1, 1);
		if (posx == 0 && posy == 0 && posz == 0)
			setblockp({ chx,chy,chz }, { posx,posy,posz }, v, 1, 1, 1);

		fixSinglePointMesh(*ch->ch, posx, posy, posz, ch->ch->vertices, iso);
	}

	MapgenNoise noise;
	std::vector<Chunkholder*> renderchunks;

	private:
		void setblockp(Pos chs, Pos pos, float v, bool x, bool y, bool z) {

			long long chx = chs.x;
			long long chy = chs.y;
			long long chz = chs.z;

			int posx = pos.x % chunkx;
			int posy = pos.y % chunky;
			int posz = pos.z % chunkz;

			if (x) {
				chx--;
				posx = chunkx;
			}
			if (y) {
				chy--;
				posy = chunky;
			}
			if (z) {
				chz--;
				posz = chunkz;
			}

			Chunkholder* ch = getchunk({ chx,chy,chz });
			if (ch == NULL) {
				return;
			}

			ch->ch->v[posx][posy][posz] = v;
			fixSinglePointMesh(*ch->ch, posx, posy, posz, ch->ch->vertices, iso);
		}
};


