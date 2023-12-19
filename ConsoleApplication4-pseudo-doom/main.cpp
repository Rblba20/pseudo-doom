#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include "renderer.h"
#include "player.h"
#include "sound.h"
#include <cstdlib>
#include <vector>
#include <list>
#include <sstream>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

int main(int argc, char* argv[])
{
    double lastTime = 0;
    double lastFPSDisplay = 0;
    double fps = 60;
    Uint32 time = SDL_GetTicks();

    Menu menu;
    Map map;
    if(map.w == 0) return 0;
	Player player(&map, &menu);
    Renderer renderer(&player, &map, &menu);
    Sound sound(&menu, &player);
    sound.init_sounds();

    string bullets_filename("bullets.txt");
    int bullets_number;

    ifstream bullets_input_file(bullets_filename);
    if (!bullets_input_file.is_open()) {
        cerr << "Could not open the file - '" << bullets_filename << "'" << endl;
        //  return EXIT_FAILURE;
    }

    while (bullets_input_file >> bullets_number) {
        player.ammo = bullets_number;
    }
    bullets_input_file.close();


    string mode_filename("mode.txt");
    int mode_number;

    ifstream mode_input_file(mode_filename);
    if (!mode_input_file.is_open()) {
        cerr << "Could not open the file - '" << mode_filename << "'" << endl;
        //  return EXIT_FAILURE;
    }

    while (mode_input_file >> mode_number) {
    }
    mode_input_file.close();
    std::ofstream outfile(mode_filename); // использовать значение mode_filename
    if (mode_number == 7734) {
        outfile << 666 << std::endl;
        outfile.close();
    }
    else if (mode_number == 666) {
        outfile << 071117 << std::endl;
        outfile.close();
    }
    else {

    }



    if(!renderer.init_sdl("DOOM_Like", 1280, 720))
        return 0;
    
    //game loop
    while (!menu.wants_to_quit)
    {
      //  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        if (20 > (SDL_GetTicks() - time))
        {
            SDL_Delay(20 - (SDL_GetTicks() - time)); //SDL_Delay pauses the execution.
        }
        float dt = (SDL_GetTicks() - lastTime) / 1000.0;
        lastTime = SDL_GetTicks();
        if(dt == 0) dt = 1; //prevent division by zero

        //called five times per second
        if(lastFPSDisplay + 200 < lastTime)
        {
            lastFPSDisplay = lastTime;
            fps = 1 / dt;

            //sort sprites only 5 times per seconds
            map.sort_sprites(player.get_x(), player.get_y());
            map.animate_sprites();
       //     player.ammo = 10000;

            if(map.pickup_keys())
            {
                player.key_count++;
                player.key_sound = true;
            }

            if(menu.current == None)
            {
                int damage = map.damage_player();
                player.health -= damage;
                if(damage != 0)
                    player.hurt_sound = true;
            }
        }
        
        if(menu.current == None) //no menu is displayed, the player is currently playing
        {
            SDL_ShowCursor(SDL_DISABLE);
            if(player.key_count > 0 && map.update_doors(player.get_x(), player.get_y(), dt))
                player.key_count--;

            map.update_sprites(player.get_x(), player.get_y(), dt);
        }
        else if(menu.current == Main)
        {
            SDL_ShowCursor(SDL_ENABLE);
            SDL_SetRelativeMouseMode(SDL_FALSE);
            map.damage = menu.difficulty == 0 ? 1 : (menu.difficulty == 1 ? 3 : 6);
            map.speed = menu.difficulty == 0 ? 0.8 : (menu.difficulty == 1 ? 1.2 : 2.5);
            sound.set_volume(menu.sound == 0 ? 0 : (menu.sound == 1 ? 20 : 100));
        }
        else {
            SDL_ShowCursor(SDL_ENABLE);
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }

        player.handle_events(dt);
        renderer.draw(fps);
        sound.play_sounds();
    }
    SDL_ShowCursor(SDL_ENABLE);
    return 0;
}