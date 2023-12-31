#include <iostream>
#include <cmath>
#include "player.h"
#include "renderer.h" 



Player::Player(Map* ma, Menu* me) : display_flash(false), health(100), ammo(230), key_count(0), enemy_destruct(false), wall_destruct(false),
hurt_sound(false), key_sound(false), x(5), y(5), angle(0), turn(0), walk_x(0), walk_y(0), pressed_keys(NULL), map(ma), menu(me)
{
    pressed_keys = new bool[4];
    for (int i = 0; i < 4; i++)
    {
        pressed_keys[i] = false;
    }
}

void Player::handle_events(float dt)
{
    SDL_Event event;
    if (health < 1)
        menu->current = GameOver;

    menu->mouse_down = false; //the menu should see the mouse button clicked for one frame only
    display_flash = false;
    enemy_destruct = false;
    wall_destruct = false;

    while (SDL_PollEvent(&event))
    {
        //mouse button is clicked so we update the menu
        if ((event.type == SDL_MOUSEBUTTONDOWN) && (menu->current != None)) {
            menu->mouse_down = true;
        }
        //a key is pressed
        else if (event.type == SDL_KEYDOWN) {
          //  std::cout << "I am here" << std::endl;
            update_key(event.key.keysym.sym, true);
        }

        // Mouse moved ?
        else if ((event.type == SDL_MOUSEMOTION) && (menu->current == None)) {
            SDL_SetRelativeMouseMode(SDL_TRUE);
             //Get the relative movement of the mouse.
            int mouseXOffset;
            SDL_GetRelativeMouseState(&mouseXOffset, NULL);


            //Make the unit turn if needed.
            if (mouseXOffset != 0)
                turn = (float)mouseXOffset / 1280;
          //      unitPlayer->setAmountTurn((float)mouseXOffset / windowWidth);

                       // Get the relative movement of the mouse
            //int mouseX = event.motion.xrel;

            //// Adjust turn based on mouse movement
            //turn = mouseX * turn_accel * dt;


         //   std::cout << turn << std::endl;
            angle += turn* turn_accel;
            if (angle > M_PI)
                angle -= 2 * M_PI;
            if (angle < -M_PI)
                angle += 2 * M_PI;
        }
        else if ((event.type == SDL_MOUSEBUTTONDOWN) && (menu->current == None) && (event.button.button == SDL_BUTTON_LEFT))
        {
            if (menu->current == None && ((SDL_GetTicks() - start) >= 600))
            {
                start = SDL_GetTicks();
                Fire();
            }
           
            //std::cout << "I am here" << std::endl;



                 //weapon firing
                 //if (event.button.button == SDL_BUTTON_LEFT)
                 //{
                 //  //  std::cout << "Left button was pressed!" << std::endl;
                 //    if (menu->current == None)
                 //        Fire();
                 //}


        }
        //a key is released
        else if (event.type == SDL_KEYUP)
        {
            // std::cout << "Key_up" << std::endl;
            if (event.type = SDL_KEYUP) {
                update_key(event.key.keysym.sym, false);
            }

            //pauses the game if the player presses Escape
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                if (menu->current == None)
                    menu->current = Pause;
                else if (menu->current == Pause)
                    menu->current = None;
            }

            //quits the game if the player is dead and presses Space or Escape
            if (menu->current == GameOver)
            {
                //  if(event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_ESCAPE)
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    menu->wants_to_quit = true;
            }
        }
        //close the game if the user presses the close window button
        else if (!menu->wants_to_quit)
            menu->wants_to_quit = event.type == SDL_QUIT;
    }

    if (menu->current == Main || menu->current == Help)
    {
        angle += dt * 0.1;
        return;
    }

    //Forward - backward movement
    if (pressed_keys[0]) {
        //if (SDLK_UP)
        walk_y = speed * dt;
    }
    else if (pressed_keys[2])
        //else if (SDLK_DOWN)
        walk_y = -speed * dt;
    else
        walk_y = 0;

    //Left - right movement
    if (pressed_keys[3])
        //if(SDLK_LEFT)
        walk_x = speed * dt * 0.5;
    else if (pressed_keys[1])
        //else if(SDLK_RIGHT)
        walk_x = -speed * dt * 0.5;
    else
        walk_x = 0;

    float nx = x + (walk_y * cos(angle) + walk_x * cos(angle + M_PI / 2.0)) * 0.1;
    float ny = y + (walk_y * sin(angle) + walk_x * sin(angle + M_PI / 2.0)) * 0.1;

    if (int(nx) >= 0 && int(nx) < map->w && int(ny) >= 0 && int(ny) < map->h)
    {
        if (map->get_tile(int(nx), int(y)) == ' ')
            x = nx;
        if (map->get_tile(int(x), int(ny)) == ' ')
            y = ny;
    }
}

void Player::update_key(SDL_Keycode key, bool state)
{
    /* if (key == 'z' && menu->current != Pause) pressed_keys[0] = state;
     else if (key == 'q' && menu->current != Pause) pressed_keys[1] = state;
     else if (key == 's' && menu->current != Pause) pressed_keys[2] = state;
     else if (key == 'd' && menu->current != Pause) pressed_keys[3] = state;
     else if (key == SDLK_LEFT && menu->current != Pause) pressed_keys[4] = state;
     else if (key == SDLK_RIGHT && menu->current != Pause) pressed_keys[5] = state;
     else if (key == SDLK_SPACE && menu->current != Pause) pressed_keys[6] = state;*/
    if (key == 'w' && menu->current != Pause) pressed_keys[0] = state;
    else if (key == 'a' && menu->current != Pause) pressed_keys[1] = state;
    else if (key == 's' && menu->current != Pause) pressed_keys[2] = state;
    else if (key == 'd' && menu->current != Pause) pressed_keys[3] = state;
}

void Player::Fire()
{
    std::vector<Sprite> sprites = map->get_sprites();

    float dist = 0;
    bool hit_wall = false;

    //we don't need to calculate sin and cos for each step
    float x_offset = cos(angle);
    float y_offset = sin(angle);
    if(ammo > 0){
        ammo -= 1;
        while (!hit_wall && dist < 10) //makes the ray move forward step by step
        {
            dist += 0.1;

            //pos of the tip of the ray for this iteration
            float ray_x = x + x_offset * dist;
            float ray_y = y + y_offset * dist;

            if (map->get_tile(int(ray_x), int(ray_y)) != ' ') //the current tile is not empty, we hit a wall
            {
                hit_wall = true;
                if (map->get_tile(int(ray_x), int(ray_y)) == '2')
                {
                    wall_destruct = true;
                    map->set_tile(int(ray_x), int(ray_y), ' ');
                    map->add_temp_sprite(7, int(ray_x) + 0.5, int(ray_y) + 0.5, 1000, 0);
                    map->sort_sprites(x, y);
                    map->update_dist_map(x, y);
                }
            }
            else
            {
                for (unsigned int i = 0; i < sprites.size(); i++)
                {
                    if ((sprites.at(i).type == Enemy) || (sprites.at(i).type == Fire_))
                    {
                        float sqr_dist = pow(ray_x - sprites.at(i).x, 2) + pow(ray_y - sprites.at(i).y, 2);
                        if (sqr_dist < 0.016)
                        {
                            enemy_destruct = true;
                            map->delete_sprite(i);
                            hit_wall = true;
                            //  map->add_temp_sprite(6, sprites.at(i).x, sprites.at(i).y, 400);
                            map->add_temp_sprite(15, sprites.at(i).x, sprites.at(i).y, 400, 1);
                            //  SDL_Delay(50);
                        //      map->add_temp_sprite(16, sprites.at(i).x, sprites.at(i).y, 400, 1);
                        //    //  SDL_Delay(50);
                        //      map->add_temp_sprite(17, sprites.at(i).x, sprites.at(i).y, 400, 1);
                        //   //   SDL_Delay(50);
                        //      map->add_temp_sprite(18, sprites.at(i).x, sprites.at(i).y, 350, 1);
                        ////      SDL_Delay(50);
                        //      map->add_temp_sprite(19, sprites.at(i).x, sprites.at(i).y, 300, 1);
                        ////      SDL_Delay(50);
                        //      map->add_temp_sprite(20, sprites.at(i).x, sprites.at(i).y, 200, 1);
                       //       SDL_Delay(50);
                            map->sort_sprites(x, y);

                            map->enemy_count--;
                            if (map->enemy_count < 1)
                            {
                                menu->timer.stop();
                                menu->current = Win;
                                menu->leaderboard.add_score(menu->timer.get_time());
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    //while (!hit_wall && dist < 10) //makes the ray move forward step by step
    //{
    //    dist += 0.1;

    //    //pos of the tip of the ray for this iteration
    //    float ray_x = x + x_offset * dist;
    //    float ray_y = y + y_offset * dist;

    //    if (map->get_tile(int(ray_x), int(ray_y)) != ' ') //the current tile is not empty, we hit a wall
    //    {
    //        hit_wall = true;
    //        if (map->get_tile(int(ray_x), int(ray_y)) == '2')
    //        {
    //            wall_destruct = true;
    //            map->set_tile(int(ray_x), int(ray_y), ' ');
    //            map->add_temp_sprite(7, int(ray_x) + 0.5, int(ray_y) + 0.5, 1000, 0);
    //            map->sort_sprites(x, y);
    //            map->update_dist_map(x, y);
    //        }
    //    }
    //    else
    //    {
    //        for (unsigned int i = 0; i < sprites.size(); i++)
    //        {
    //            if ((sprites.at(i).type == Enemy) || (sprites.at(i).type == Fire_))
    //            {
    //                float sqr_dist = pow(ray_x - sprites.at(i).x, 2) + pow(ray_y - sprites.at(i).y, 2);
    //                if (sqr_dist < 0.016)
    //                {
    //                    turkey_destruct = true;
    //                    map->delete_sprite(i);
    //                    hit_wall = true;
    //                    //  map->add_temp_sprite(6, sprites.at(i).x, sprites.at(i).y, 400);
    //                    map->add_temp_sprite(15, sprites.at(i).x, sprites.at(i).y, 400, 1);
    //                    //  SDL_Delay(50);
    //                //      map->add_temp_sprite(16, sprites.at(i).x, sprites.at(i).y, 400, 1);
    //                //    //  SDL_Delay(50);
    //                //      map->add_temp_sprite(17, sprites.at(i).x, sprites.at(i).y, 400, 1);
    //                //   //   SDL_Delay(50);
    //                //      map->add_temp_sprite(18, sprites.at(i).x, sprites.at(i).y, 350, 1);
    //                ////      SDL_Delay(50);
    //                //      map->add_temp_sprite(19, sprites.at(i).x, sprites.at(i).y, 300, 1);
    //                ////      SDL_Delay(50);
    //                //      map->add_temp_sprite(20, sprites.at(i).x, sprites.at(i).y, 200, 1);
    //               //       SDL_Delay(50);
    //                    map->sort_sprites(x, y);

    //                    map->enemy_count--;
    //                    if (map->enemy_count < 1)
    //                    {
    //                        menu->timer.stop();
    //                        menu->current = Win;
    //                        menu->leaderboard.add_score(menu->timer.get_time());
    //                    }
    //                    break;
    //                }
    //            }
    //        }
    //    }
    //}

    display_flash = true;
}

float Player::get_x()
{
    return x;
}

float Player::get_y()
{
    return y;
}

float Player::get_angle()
{
    return angle;
}

Player::~Player()
{
    delete pressed_keys;
    std::cout << "Player deleted" << std::endl;
}