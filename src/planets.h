#pragma once

static const char*	texture_planet_path[64] = {
	"../bin/textures/sun.jpg",		// 0 (no shading)
	"../bin/textures/mercury.jpg",
	"../bin/textures/venus.jpg",
	"../bin/textures/earth.jpg",	// 3 (dwarfs)
	"../bin/textures/mars.jpg",
	"../bin/textures/jupiter.jpg",	// 5 (dwarfs)
	"../bin/textures/saturn.jpg",	// 6 (ring)
	"../bin/textures/uranus.jpg",	// 7 (dwarfs,ring)
	"../bin/textures/neptune.jpg",	// 8 (dwarfs)
	"../bin/textures/moon.jpg"		// 9 (texture for all dwarfs)
};
static const char*	texture_ring_path[64] = {
	"../bin/textures/saturn-ring.jpg",
	"../bin/textures/uranus-ring.jpg"
};

struct planet
{
	float radius;		//�༺ ũ��
	float rotate;		//���� �ֱ�
	float revolve;		//���� �ֱ�
	float distance;		//�¾���� �Ÿ�
};

struct planet planets[9] = {
	{3.0f, 0.10f, 0.0f, 0.0f},	//0 �¾�
	{0.3f, 0.08f, 2.0f, 5.5f},	//1 ����
	{0.5f, 0.05f, 1.3f, 8.0f},	//2 �ݼ�
	{1.0f, 0.5f, 0.9f, 10.0f},	//3 ����
	{0.8f, 0.5f, 0.4f, 13.4f},	//4 ȭ��
	{1.8f, 0.8f, 0.33f, 20.7f},	//5 ��
	{1.4f, 0.8f, 0.25f, 31.0f},	//6 �伺
	{1.1f, 0.7f, 0.17f, 37.1f},	//7 õ�ռ�
	{1.2f, 0.6f, 0.10f, 43.6f}	//8 �ؿռ�
};

struct dwarf
{
	uint planet;
	struct planet info;
};

struct dwarf dwarfs[12] = {
	{3, {0.25f, 0.4f, 2.5f, 2.2f}}, //����
	{5, {0.27f, 0.7f, 5.8f, 2.0f}}, //��
	{5, {0.22f, 0.3f, 7.7f, 2.3f}},
	{5, {0.25f, 0.9f, 8.0f, 2.6f}},
	{5, {0.23f, 0.6f, 6.5f, 2.9f}},
	{7, {0.15f, 0.7f, 5.8f, 2.0f}}, //õ�ռ�
	{7, {0.14f, 0.3f, 7.7f, 2.2f}},
	{7, {0.16f, 0.9f, 8.0f, 2.4f}},
	{7, {0.13f, 0.6f, 3.0f, 2.6f}},
	{7, {0.15f, 0.6f, 6.5f, 2.8f}},
	{8, {0.12f, 0.6f, 4.5f, 2.5f}}, //�ؿռ�
	{8, {0.12f, 0.6f, 7.0f, 3.0f}}
};

struct ring
{
	uint planet;
	float scale;
};

struct ring rings[2] = {
	{6, 1.9f}, //�伺
	{7, 1.6f}, //õ�ռ�
};