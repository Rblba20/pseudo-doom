#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_mixer.h>
#include "sound.h"

Sound::Sound(Menu* me, Player* p) : theme(NULL), music_menu(NULL), music_pause(NULL), gameover(NULL), death_sound(NULL), no_ammo_sound(NULL), victory(NULL),
	gunshot(NULL), death_enemy(NULL), key(NULL), enemy(NULL), wall(NULL), hurt(NULL), menu(me), player(p)
{
	
}

void Sound::init_sounds()
{
	std::cout<<"Loading sounds ..."<<std::endl;
	//Initialize SDL_mixer 
	Mix_Init(MIX_INIT_OGG);		
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	egg = load_sound("sounds/story.mp3");
	theme = load_sound("sounds/playlist.mp3");
	music_menu  = load_sound("sounds/menu.mp3");
	music_pause = load_sound("sounds/pause.ogg");
	gameover = load_sound("sounds/gameover.ogg");
	death_sound = load_sound("sounds/dead.wav");
	no_ammo_sound = load_sound("sounds/null_ammo.mp3");
	victory = load_sound("sounds/victory.ogg");
	gunshot = load_sound("sounds/pistol.mp3");
	death_enemy = load_sound("sounds/death_turkey.ogg");
	key = load_sound("sounds/key.ogg");
	enemy = load_sound("sounds/turkey.ogg");
	wall = load_sound("sounds/wall.ogg");
	hurt = load_sound("sounds/do_damage.mp3");
}

void Sound::play_sounds()
{
	//MUSICS
	if (menu->current == None)//plays theme when not in menu
	{
		if (Mix_Playing(2) == 1) Mix_HaltChannel(2);
		else if (Mix_Playing(3) == 1) Mix_HaltChannel(3);

		if (Mix_Playing(1) == 0) play(theme, 1, -1);
		resume_music(1);
	}
	else if (menu->current == Main || menu->current == Help)//plays main menu music
	{
		if (Mix_Playing(2) == 0) play(music_menu, 2, -1);
	}
	else if (menu->current == Pause)//plays pause menu music
	{
		pause_music(1);
		if (Mix_Playing(3) == 0) play(music_pause, 3, -1);
	}
	else if (menu->current == GameOver)//plays game over music
	{
		Mix_HaltChannel(1);
		if (Mix_Playing(2) == 0) play(gameover, 2, -1);
	}
	else if (menu->current == Win)//plays game over music
	{
		Mix_HaltChannel(1);
		if (Mix_Playing(2) == 0) play(victory, 2, -1);
	}
	else if (menu->current == EasterEgg)//plays game over music
	{
		Mix_HaltChannel(2);
		play(egg, 3, -1);
	}
	//SOUND EFFECTS
	if (player->display_flash && player->ammo > 0)
		play(gunshot, 4, 0);

	if (player->display_flash && player->ammo <= 0)
		play(no_ammo_sound, 4, 0);

	//if (player->health <= 10)
	//	play(death_sound, 5, 0);

	if(player->enemy_destruct)
		play(death_enemy, 5, 0);

	if(player->wall_destruct)
		play(wall, 6, 0);

	if(player->hurt_sound)
	{
		play(hurt, 7, 0);
		player->hurt_sound = false;
	}
	
	if(player->key_sound)
	{
		play(key, 6, 0);
		player->key_sound = false;
	}
}

Mix_Chunk* Sound::load_sound(std::string filename)
{
	Mix_Chunk* chunk;
	chunk = Mix_LoadWAV(filename.c_str());
	return chunk;
}

void Sound::set_volume(int volume)
{
	Mix_Volume(-1, volume);
}

void Sound::pause_music(int channel)
{
	if(Mix_Playing(channel) == 1) Mix_Pause(channel);
}

void Sound::resume_music(int channel)
{
	if(Mix_Paused(channel) == 1) Mix_Resume(channel);
}

void Sound::play(Mix_Chunk* sound, int channel, int loops)
{
	Mix_PlayChannel(channel, sound, loops);
}

Sound::~Sound()
{
	//freeMusics
	Mix_FreeChunk(theme);
	Mix_FreeChunk(music_menu);
	Mix_FreeChunk(music_pause);
	Mix_FreeChunk(gameover);
	Mix_FreeChunk(victory);

	//freeSFX
	Mix_FreeChunk(gunshot);
	Mix_FreeChunk(death_enemy);
	Mix_FreeChunk(wall);
	Mix_FreeChunk(key);
	Mix_FreeChunk(enemy);
	Mix_FreeChunk(hurt);

	Mix_CloseAudio();
	Mix_Quit();
}