#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include <SDL.h>
#include "map.h"
#include "renderer.h"
#include "menu.h"
#include "player.h"
#include <ctime>
using namespace std;

Map::Map() : speed(1.2), damage(0), enemy_count(0), map(NULL), dist(NULL), sprites(std::vector<Sprite>()), doors(std::vector<Door>())
{
	// ���������� ��������� ��������� �����
	srand(time(NULL));

	// �������� ��������� ����� - �������
	//int number = 1 + rand() % (3 - 1 + 1);

	const string map_load =  "map" + to_string(1 + rand() % (3 - 1 + 1)) + ".bmp";
	SDL_Surface* map_tex = SDL_LoadBMP(map_load.c_str());

	 //SDL_Surface* map_tex = SDL_LoadBMP("test.bmp");
	//Loading the map texture

	//Error handling for texture loading
	if (!map_tex)
	{
		std::cerr << "Couldn't load texture file " << SDL_GetError() << std::endl;
		return;
	}

	if (map_tex->format->BytesPerPixel != 3)
	{
		std::cerr << "Map must be using 8bpp format." << std::endl;
		return;
	}

	if (map_tex->w != 32 || map_tex->h != 32)
	{
		std::cerr << "Map must be 32x32." << std::endl;
		return;
	}

	w = map_tex->w;
	h = map_tex->h;

	map = new char[w * h];
	dist = new unsigned short[w * h];

	std::cout << w << ';' << h << "\n";

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int id = y * w + x;
			Uint32 pixel = get_pixel(map_tex, x, y);
			//std::cout << pixel << std::endl;

			if (pixel == 0) //black 0,0,0 : grass
				map[id] = '0';
			else if (pixel == 65280) //green 0,255,0 : bricks
				map[id] = '1';
			else if (pixel == 255) //blue 0,0,255 : cobblestone
				map[id] = '2';
			else if (pixel == 16776960) //yellow 255,255,0 : door
			{
				map[id] = '3';

				unsigned int index = doors.size();
				doors.push_back(Door());
				doors.at(index).x = x;
				doors.at(index).y = y;
				doors.at(index).animationState = 1;
			}
			else if (pixel == 16711680) //red 255,0,0 : enemy
			{
				map[id] = ' ';

				unsigned int index = sprites.size();
				sprites.push_back(Sprite());
				sprites.at(index).x = x + (std::rand() % 100) / 100.0;
				sprites.at(index).y = y + (std::rand() % 100) / 100.0;
				sprites.at(index).itex = std::rand() % 2 == 1 ? 1 : 4;
				sprites.at(index).type = Enemy;
				sprites.at(index).size = 500 + ((id * 50) % 100);
				enemy_count++;
			}
			else if (pixel == 65535) //cyan 0,255,255 : smth
			{
				map[id] = ' ';

				for (int i = 0; i < 1; i++)
				{
					unsigned int index = sprites.size();
					sprites.push_back(Sprite());
					sprites.at(index).x = x + (std::rand() % 100) / 100.0;
					sprites.at(index).y = y + (std::rand() % 100) / 100.0;
					sprites.at(index).itex = 1 + rand() % (4 - 1 + 1);
					// 1 + rand() % (4 - 1 + 1);
					sprites.at(index).type = Decoration;
					if (sprites.at(index).itex == 1) {
						sprites.at(index).size = 900;
					}
					if (sprites.at(index).itex == 2) {
						sprites.at(index).size = 500;
					}
					if (sprites.at(index).itex == 3) {
						sprites.at(index).size = 700;
					}
					if (sprites.at(index).itex == 4) {
						sprites.at(index).size = 900;
					}
				}
			}
			else if (pixel == 16711935) //magenta 255,0,255 : key
			{
				map[id] = ' ';

				unsigned int index = sprites.size();
				sprites.push_back(Sprite());
				sprites.at(index).x = x;
				sprites.at(index).y = y;
				sprites.at(index).itex = 5;
				sprites.at(index).type = Key;
				sprites.at(index).size = 400;
			}
			else //empty
				map[id] = ' ';

			std::cout << get_tile(x, y);
		}
		std::cout << std::endl;
	}
	std::cout << "Level loaded with " << sprites.size() << " sprites" << std::endl;
	SDL_FreeSurface(map_tex);

	update_dist_map(2, 2);
}

char Map::get_tile(int x, int y)
{
	if (x >= w || y >= h)
		return '0';

	return map[y * w + x];
}

void Map::set_tile(int x, int y, char tile)
{
	if (x > w || y > h)
		return;

	map[y * w + x] = tile;
}

Door Map::get_door(int x, int y)
{
	unsigned int i = 0;
	for (i = 0; i < doors.size() - 1; i++)
	{
		if (doors.at(i).x == x && doors.at(i).y == y)
			break;
	}
	return doors.at(i);
}

//returns true if the player opened a door
bool Map::update_doors(float player_x, float player_y, float dt)
{
	for (unsigned int i = doors.size(); i > 0; i--)
	{
		float sqr_dist = pow(player_x - doors.at(i - 1).x, 2) + pow(player_y - doors.at(i - 1).y, 2);
		if (sqr_dist < 10)
		{
			doors.at(i - 1).animationState -= dt;

			if (doors.at(i - 1).animationState < 0.01)
			{
				set_tile(doors.at(i - 1).x, doors.at(i - 1).y, ' ');
				doors.erase(doors.begin() + i - 1);
				update_dist_map(player_x, player_y);
				return true;
			}
		}
		else
			doors.at(i - 1).animationState = 1;
	}

	return false;
}

void Map::sort_sprites(float player_x, float player_y)
{
	if (sprites.size() < 2)
		return;

	for (unsigned int i = sprites.size() - 1; i > 0; i--)
	{
		sprites.at(i).sqr_dist = pow(player_x - sprites.at(i).x, 2) + pow(player_y - sprites.at(i).y, 2);

		if (sprites.at(i).type == Temporary && sprites.at(i).start_time + 500 < SDL_GetTicks())
			delete_sprite(i);
	}
	std::sort(sprites.begin(), sprites.end());
}

int Map::damage_player()
{
	int amount = 0;
	for (unsigned int i = 0; i < sprites.size(); i++)
	{
		if (sprites.at(i).type == Enemy && sprites.at(i).sqr_dist < 2) {
			amount += damage;
		}
	}
	return amount;
}

bool Map::pickup_keys()
{
	if (sprites.size() == 0)
		return false;

	for (unsigned int i = sprites.size() - 1; i > 0; i--)
	{
		if (sprites.at(i).type == Key && sprites.at(i).sqr_dist < 2)
		{
			delete_sprite(i);
			return true;
		}
	}

	return false;
}

void Map::animate_sprites()
{
	string filename("mode.txt");
	int number;
	int elect = 0;

	ifstream input_file(filename);
	if (!input_file.is_open()) {
		cerr << "Could not open the file - '" << filename << "'" << endl;
		//  return EXIT_FAILURE;
	}

	while (input_file >> number) {
		elect = number;
	}
	if (elect == 666) {
		for (unsigned int i = 0; i < sprites.size(); i++)
		{
			if (sprites.at(i).type == Enemy)
				sprites.at(i).itex = sprites.at(i).itex == 9 ? 10 : sprites.at(i).itex == 10 ? 11 : sprites.at(i).itex == 11 ? 12 : 9;
			else if (sprites.at(i).type == Death && sprites.at(i).itex != 20 && sprites.at(i).itex == 15)
				sprites.at(i).itex = 16;
			else if (sprites.at(i).type == Death && sprites.at(i).itex != 20 && sprites.at(i).itex == 16) {
				int choose = 1 + rand() % (4 - 1 + 1);
				if (choose == 3) {
					sprites.at(i).itex = 20;
				}
				else {
					sprites.at(i).itex = 17;
				}
			}
			else if (sprites.at(i).type == Death && sprites.at(i).itex != 20 && sprites.at(i).itex == 17) {
				sprites.at(i).itex = 18;
			}
			else if (sprites.at(i).type == Death && sprites.at(i).itex != 20 && sprites.at(i).itex == 18) {
				sprites.at(i).itex = 19;
			}
			//else if (sprites.at(i).type == Fire_) {
			//	sprites.at(i).itex = sprites.at(i).itex == 13 ? 14 : 13;
			//}
		}
	}
	else {
		for (unsigned int i = 0; i < sprites.size(); i++)
		{
			if (sprites.at(i).type == Enemy)
				sprites.at(i).itex = sprites.at(i).itex == 9 ? 10 : sprites.at(i).itex == 10 ? 11 : sprites.at(i).itex == 11 ? 12 : 9;
			else if (sprites.at(i).type == Death && sprites.at(i).itex != 20 && sprites.at(i).itex == 15)
				sprites.at(i).itex = 16;
			else if (sprites.at(i).type == Death && sprites.at(i).itex != 20 && sprites.at(i).itex == 16)
				sprites.at(i).itex = 17;
			else if (sprites.at(i).type == Death && sprites.at(i).itex != 20 && sprites.at(i).itex == 17) {
				sprites.at(i).itex = 18;
			}
			else if (sprites.at(i).type == Death && sprites.at(i).itex != 20 && sprites.at(i).itex == 18) {
				sprites.at(i).itex = 19;
			}
			else if (sprites.at(i).type == Death && sprites.at(i).itex != 20 && sprites.at(i).itex == 19)
				sprites.at(i).itex = 20;
			//else if (sprites.at(i).type == Fire_) {
			//	sprites.at(i).itex = sprites.at(i).itex == 13 ? 14 : 13;
			//}
		}
	}
	input_file.close();

}

std::vector<Sprite> const& Map::get_sprites()
{
	return sprites;
}

void Map::delete_sprite(int id)
{
	sprites.erase(sprites.begin() + id);
}

void Map::add_temp_sprite(int itex, float x, float y, int size, int type)
{
	if (type == 0) {
		unsigned int index = sprites.size();
		sprites.push_back(Sprite());
		sprites.at(index).x = x;
		sprites.at(index).y = y;
		sprites.at(index).itex = itex;
		sprites.at(index).type = Temporary;
		sprites.at(index).size = size;
	}
	if (type == 1) {
		unsigned int index = sprites.size();
		sprites.push_back(Sprite());
		sprites.at(index).x = x;
		sprites.at(index).y = y;
		sprites.at(index).itex = itex;
		sprites.at(index).type = Death;
		sprites.at(index).size = size;
	}
}

void Map::update_sprites(float player_x, float player_y, float dt)
{
	//player position
	int px = int(player_x);
	int py = int(player_y);
	if (dist[px + py * w] != 0)
		update_dist_map(px, py);

	for (unsigned int i = 0; i < sprites.size(); i++)
	{
		if (sprites.at(i).type == Enemy || sprites.at(i).type == Fire_)
		{
			int x = int(sprites.at(i).x);
			int y = int(sprites.at(i).y);

			int d = dist[x + y * w];

			if (d > dist[x + (y - 1) * w])
			{
			//	sprites.at(i).type = Enemy;
				sprites.at(i).y -= speed * dt;
				sprites.at(i).x += (x - sprites.at(i).x + 0.5) * speed * dt;
			}
			else if (d > dist[x + (y + 1) * w])
			{
			//	sprites.at(i).type = Enemy;
				sprites.at(i).y += speed * dt;
				sprites.at(i).x += (x - sprites.at(i).x + 0.5) * speed * dt;
			}
			else if (d > dist[(x + 1) + y * w])
			{
			//	sprites.at(i).type = Enemy;
				sprites.at(i).x += speed * dt;
				sprites.at(i).y += (y - sprites.at(i).y + 0.5) * speed * dt;
			}
			else if (d > dist[(x - 1) + y * w])
			{
			//	sprites.at(i).type = Fire_;
				sprites.at(i).x -= speed * dt;
				sprites.at(i).y += (y - sprites.at(i).y + 0.5) * speed * dt;
			}
		}
		else if (sprites.at(i).type == Temporary)
		{
			sprites.at(i).size += 15;
		}
		//else if (sprites.at(i).type == Death) {
		//	sprites.at(i).itex = 15;
		//	//sprites.at(i).itex = 16;
		//	//sprites.at(i).itex = 17;
		//	//sprites.at(i).itex = 18;
		//	//sprites.at(i).itex = 19;
		//	//sprites.at(i).itex = 20;
		//}
	}
}

void Map::update_dist_map(int px, int py)
{
	//initialize the distance map
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			dist[j * w + i] = 1000;
		}
	}

	//all for neighbors
	const int nx[4] = { 0, -1, 1, 0 };
	const int ny[4] = { -1, 0, 0, 1 };

	dist[px + py * w] = 0; //sets player's distance to 0
	for (int iter = 0; iter < 10; iter++)
	{
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				int idx = x + y * w;
				if (map[idx] == ' ')
				{
					//for each neighbor
					for (int i = 0; i < 4; i++)
					{
						int tidx = idx + nx[i] + ny[i] * w;
						if (map[tidx] == ' ' && dist[tidx] > dist[idx])
							dist[tidx] = dist[idx] + 1;
					}
				}
			}
		}
	}

	/*
	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			int idx = i + j * w;
			std::cout<<(char)(dist[idx] > 9 ? '-' : dist[idx] + '0');
		}
		std::cout<<std::endl;
	}
	*/
}

//Gets a pixel from the texture file
Uint32 Map::get_pixel(SDL_Surface* source, int x, int y)
{
	if (x >= h || y >= w)
		return 0;

	Uint8* p = (Uint8*)source->pixels + y * source->pitch + x * source->format->BytesPerPixel;
	return p[0] | p[1] << 8 | p[2] << 16;
}

Map::~Map()
{
	delete dist;
	delete map;
	std::cout << "Map deleted" << std::endl;
}