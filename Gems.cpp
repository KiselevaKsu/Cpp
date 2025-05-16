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

void drawBoard(const vector<vector<int>>& board, pair<int, int> selected = { -1, -1 }) {
    SDL_Color colors[COLORS] = {
        {255, 0, 0, 255},    // Красный
        {0, 255, 0, 255},    // Зелёный
        {0, 0, 255, 255},    // Синий
        {255, 255, 0, 255},  // Желтый
        {255, 165, 0, 255}   // Оранжевый
    };

    int gap = 4;
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            SDL_Rect tile{ j * TILE_SIZE + gap / 2, i * TILE_SIZE + gap / 2, TILE_SIZE - gap, TILE_SIZE - gap };
            SDL_Color c = colors[board[i][j]];
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_RenderFillRect(renderer, &tile);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &tile);
        }
    }

    // выбран 1 кубик — подсвечиваем его белой рамкой
    if (selected.first != -1 && selected.second != -1) {
        SDL_Rect selRect{ selected.second * TILE_SIZE + gap / 2, selected.first * TILE_SIZE + gap / 2, TILE_SIZE - gap, TILE_SIZE - gap };

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int thickness = 0; thickness < 3; ++thickness) {
            SDL_Rect r = { selRect.x - thickness, selRect.y - thickness, selRect.w + 2 * thickness, selRect.h + 2 * thickness };
            SDL_RenderDrawRect(renderer, &r);
        }
    }
}

// проверка соседства (не сосед по горизонтали или вертикали)
bool isAdjacent(pair<int, int> a, pair<int, int> b) {
    int dr = abs(a.first - b.first);
    int dc = abs(a.second - b.second);
    return ((dr == 1 && dc == 0) || (dr == 0 && dc == 1));
}

int main(int argc, char* argv[]) {
    if (!initSDL()) return -1;

    vector<vector<int>> board;
    initializeBoard(board);

    bool quit = false;
    SDL_Event e;

    pair<int, int> selected = { -1, -1 };  // выбранный кубик (row, col)

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                int col = x / TILE_SIZE;
                int row = y / TILE_SIZE;

                if (row >= 0 && row < HEIGHT && col >= 0 && col < WIDTH) {
                    if (selected.first == -1) {
                        // выбираем первый кубик
                        selected = { row, col };
                    }
                    else {
                        // выбрали второй кубик
                        pair<int, int> second = { row, col };
                        if (isAdjacent(selected, second)) {
                            // меняем цвета местами
                            std::swap(board[selected.first][selected.second], board[second.first][second.second]);
                        }
                        selected = { -1, -1 };
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawBoard(board, selected);

        SDL_RenderPresent(renderer);
    }

    closeSDL();
    return 0;
}
