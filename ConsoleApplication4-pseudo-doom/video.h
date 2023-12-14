#ifndef VIDEO_H
#define VIDEO_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <SDL.h>
#include <SDL_mixer.h>

class Video {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Chunk* sound;

public:
    Video(SDL_Window* window, SDL_Renderer* renderer);
    void playVideo(const std::string& videoPath);
    void backToMenu();
};

#endif // VIDEO_H
