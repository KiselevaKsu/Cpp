#include <SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

const int WIDTH = 8;
const int HEIGHT = 8;
const int COLORS = 5;
const int TILE_SIZE = 64;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("GEMS Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH * TILE_SIZE,
        HEIGHT * TILE_SIZE,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void initializeBoard(vector<vector<int>>& board) {
    srand(static_cast<unsigned int>(time(nullptr)));
    board.resize(HEIGHT, vector<int>(WIDTH));
    for (int i = 0; i < HEIGHT; ++i)
        for (int j = 0; j < WIDTH; ++j)
            board[i][j] = rand() % COLORS;
}

void drawBoard(const vector<vector<int>>& board) {
    SDL_Color colors[COLORS] = {
        {255, 0, 0, 255},    // Красный
        {0, 255, 0, 255},    // Зелёный
        {0, 0, 255, 255},    // Синий
        {255, 255, 0, 255},  // Желтый
        {255, 165, 0, 255}   // Оранжевый
    };

    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            int gap = 4;
            SDL_Rect tile{ j * TILE_SIZE + gap / 2, i * TILE_SIZE + gap / 2, TILE_SIZE - gap, TILE_SIZE - gap };

            SDL_Color c = colors[board[i][j]];

            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_RenderFillRect(renderer, &tile);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &tile);
        }
    }
}


int main(int argc, char* argv[]) {
    if (!initSDL()) return -1;

    vector<vector<int>> board;
    initializeBoard(board);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawBoard(board);

        SDL_RenderPresent(renderer);
    }

    closeSDL();
    return 0;
}
