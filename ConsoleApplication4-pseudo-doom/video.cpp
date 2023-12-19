#include <iostream>
#include <cmath>
#include "video.h"
#include "menu.h"
#include <ctime>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <list>
#include <sstream>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

Video::Video(SDL_Window* window, SDL_Renderer* renderer) {
    this->window = window;
    this->renderer = renderer;
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    sound = Mix_LoadWAV("sounds/story.wav");
    Mix_PlayChannel(-1, sound, 0);
}

void Video::playVideo(const std::string& videoPath) {
    cv::VideoCapture cap(videoPath);
    cv::Mat frame;
    bool success = true;

    string mode_filename("mode.txt");
    int mode_number;

    std::ofstream outfile(mode_filename); // использовать значение mode_filename
    outfile << 7734 << std::endl;
    outfile.close();

    while (success) {
        SDL_Event event;
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            success = false;
        }

        cap >> frame;

        if (!frame.empty()) {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, frame.cols, frame.rows);
            std::cout << "SOMETING WENT VERY WRONG, PLEASE RESTART THE GAME AND DON'T DO SUCH A THING AGAIN, HE IS WATCHING YOU" << std::endl;
            SDL_UpdateTexture(texture, nullptr, frame.data, frame.cols * 3);
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
            SDL_DestroyTexture(texture);

            SDL_Delay(17);
        }
    }
}

void Video::backToMenu() {
    Mix_FreeChunk(sound);
    Mix_CloseAudio();
    Menu menu;
    menu.current = Help;
    
}
