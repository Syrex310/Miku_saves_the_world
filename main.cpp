#include <SDL2/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Player {
    int x, y;
    int width, height;
    int speed;
};

bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void handleInput(SDL_Event& event, Player& player, bool& running) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_LEFT]) player.x -= player.speed;
    if (keystate[SDL_SCANCODE_RIGHT]) player.x += player.speed;
    if (keystate[SDL_SCANCODE_UP]) player.y -= player.speed;
    if (keystate[SDL_SCANCODE_DOWN]) player.y += player.speed;

    if (event.type == SDL_QUIT) running = false;
}

void renderPlayer(SDL_Renderer* renderer, Player& player) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green player
    SDL_Rect rect = { player.x, player.y, player.width, player.height };
    SDL_RenderFillRect(renderer, &rect);
}

void gameLoop(SDL_Window* window, SDL_Renderer* renderer) {
    bool running = true;
    SDL_Event event;

    Player player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50, 50, 5 };

    while (running) {
        while (SDL_PollEvent(&event)) {
            handleInput(event, player, running);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        renderPlayer(renderer, player);

        SDL_RenderPresent(renderer);
    }
}

void close(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) {
        return -1;
    }

    gameLoop(window, renderer);
    close(window, renderer);

    return 0;
}
