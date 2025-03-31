#include "common.hpp"


vector<SDL_Texture*> gifFrames;
int currentFrame = 0;
Uint32 lastFrameTime = 0;
const int frameDelay = 100;

void loadGifFrames(SDL_Renderer* renderer) {
    for (int i = 1; i <= 16; i++) {
        string filename = "image/t" + to_string(i) + ".png";
        SDL_Texture* frame = LoadTexture(filename.c_str(), renderer);
        if (frame) {
            gifFrames.push_back(frame);
            //cout << "Loaded frame: " << filename << endl;
        } else {
            cout << "Failed to load: " << filename << endl;
        }
    }
}


void renderGif(SDL_Renderer* renderer) {
    if (gifFrames.empty()) return;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime >= lastFrameTime + frameDelay) {
        currentFrame = (currentFrame + 1) % gifFrames.size(); //change frames
        lastFrameTime = currentTime; //update time
    }
    SDL_Rect pos = {0, 0, 1600, 900};
    SDL_RenderCopy(renderer, gifFrames[currentFrame], NULL, &pos);
}

void freeGifFrames() {
    for (SDL_Texture* frame : gifFrames) {
        SDL_DestroyTexture(frame);
    }
    gifFrames.clear(); //free 16 frame upon exit
}