#ifndef _MAP_H_
#define _MAP_H_

#include <SDL.h>
#include <vector>

enum SpriteType { Decoration, Enemy, Key, Temporary, Death, Fire_ };

struct Sprite
{
	float x, y = 0;
	unsigned short size = 600;
	unsigned short itex = 0;
	SpriteType type = Decoration;
	//squared distance from the player, it's used to sort sprites so no need to calculate the square root
	float sqr_dist = 0;
	Uint32 start_time = 0;

	bool operator < (const Sprite& s) const
	{
		//sprites further away are sorted before near sprites
		return sqr_dist > s.sqr_dist;
	}

	Sprite() : x(0), y(0), size(600), itex(0), type(Decoration), sqr_dist(0), start_time(SDL_GetTicks()) {}
};

struct Door
{
	unsigned short x, y = 0;
	float animationState = 1; //0 = fully opened, 0.5 = half opened, 1 = fully closed

	Door() : x(0), y(0), animationState(1) {}
};

class Map
{
public:
	Map();
	unsigned short w = 0;
	unsigned short h = 0;

	float speed = 0.03; //enemy's speed, changes based on difficulty
	int damage = 0; //damage inflicted by each enemy, changes based on difficulty
	int enemy_count = 0;

	char get_tile(int x, int y);
	// char get_tile(unsigned short x, unsigned short y);
	void set_tile(int x, int y, char tile);
	// void set_tile(unsigned short x, unsigned short y, char tile);
	void sort_sprites(float player_x, float player_y); //sorts sprites in the vector based on the distance from the player
	std::vector<Sprite> const& get_sprites();
	void delete_sprite(int id); //deletes sprite at specified index in the vector
	// void delete_sprite(unsigned short id);
	void update_sprites(float player_x, float player_y, float dt); //moves enemies so they follow the player and changes the size of temp sprites
	Door get_door(int x, int y); //tries to retrive a door at specified coordinates, return 0 if not found
	//Door get_door(unsigned short x, unsigned short y);
	bool update_doors(float player_x, float player_y, float dt); //updates the animation state if the player is close enough
	void animate_sprites(); //swaps sprites for enemies animation
	int damage_player(); //returns the amount of damage the player should receive based on difficulty and nearby enemies
	bool pickup_keys(); //returns true and remove the key from the map if the player is close enough, returns false if no key is picked up
	void add_temp_sprite(int itex, float x, float y, int size, int type); //adds a sprite to the map that is delete 500ms later (for explosions)
	void update_dist_map(int px, int py);
	// void update_dist_map(unsigned short px, unsigned short py);
	~Map();

	//forbids copy constructor to avoid warning in c++11
	Map(const Map& m) = delete;
	Map& operator=(const Map& m) = delete;

private:
	char* map;
	unsigned short* dist;
	std::vector<Sprite> sprites;
	std::vector<Door> doors;
	Uint32 get_pixel(SDL_Surface* source, int x, int y);
	// Uint32 get_pixel(SDL_Surface* source, unsigned short x, unsigned short y);
};

#endif