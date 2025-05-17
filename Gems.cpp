#include <SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <string>
#include "Bonus.h"

using namespace std;

const int WIDTH = 8;     
const int HEIGHT = 8;    
const int COLORS = 5;  
const int TILE_SIZE = 64;     // размер одного кубика в пикселях

int score = 0;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL не инициализировалс! Ошибка: " << SDL_GetError() << endl;
        return false;
    }
    window = SDL_CreateWindow("gems game - Score: 0",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE,
        SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Окно не создалось! Ошибка: " << SDL_GetError() << endl;
        SDL_Quit();
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        cerr << "Рендерер не создался! Ошибка: " << SDL_GetError() << endl;
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

// заполняем игровое поле случайными цветами
void initializeBoard(vector<vector<int>>& board) {
    srand(static_cast<unsigned int>(time(nullptr)));
    board.resize(HEIGHT, vector<int>(WIDTH));
    for (int i = 0; i < HEIGHT; ++i)
        for (int j = 0; j < WIDTH; ++j)
            board[i][j] = rand() % COLORS;
}

void drawBoard(const vector<vector<int>>& board, pair<int, int> selected = { -1, -1 }) {
    SDL_Color colors[COLORS] = {
        {255, 0, 0, 255},    // красный
        {0, 255, 0, 255},    // зелёный
        {0, 0, 255, 255},    // синий
        {255, 255, 0, 255},  // жёлтый
        {255, 165, 0, 255}   // оранжевый
    };

    int gap = 4;  // небольшой отступ между кубиками
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (board[i][j] == -1) continue; // пустая клетка, ничего не рисуем
            SDL_Rect tile{ j * TILE_SIZE + gap / 2, i * TILE_SIZE + gap / 2, TILE_SIZE - gap, TILE_SIZE - gap };
            SDL_Color c = colors[board[i][j]];
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_RenderFillRect(renderer, &tile);

            // рисуем чёрную рамку вокруг кубика
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &tile);
        }
    }

    //  выделяем выбранный кубик белой рамкой
    if (selected.first != -1 && selected.second != -1) {
        SDL_Rect selRect{ selected.second * TILE_SIZE + gap / 2, selected.first * TILE_SIZE + gap / 2, TILE_SIZE - gap, TILE_SIZE - gap };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int thickness = 0; thickness < 3; ++thickness) {
            SDL_Rect r = { selRect.x - thickness, selRect.y - thickness, selRect.w + 2 * thickness, selRect.h + 2 * thickness };
            SDL_RenderDrawRect(renderer, &r);
        }
    }
}

// проверяем, соседствуют ли два кубика (дигональ - не считается соседством)
bool isAdjacent(pair<int, int> a, pair<int, int> b) {
    int dr = abs(a.first - b.first);
    int dc = abs(a.second - b.second);
    return ((dr == 1 && dc == 0) || (dr == 0 && dc == 1));
}

// плавное исчезновение кубиков
void animateDisappearance(const vector<vector<int>>& board, const vector<pair<int, int>>& cells) {
    for (int alpha = 255; alpha >= 0; alpha -= 15) {
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);

        // Рисуем всю доску
        SDL_Color colors[COLORS] = {
            {255, 0, 0, 255},
            {0, 255, 0, 255},
            {0, 0, 255, 255},
            {255, 255, 0, 255},
            {255, 165, 0, 255}
        };
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if (board[i][j] == -1) continue;
                SDL_Rect tile{ j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_Color c = colors[board[i][j]];
                SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
                SDL_RenderFillRect(renderer, &tile);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &tile);
            }
        }

        // Рисуем "исчезающие" кубики белым с изменяемой прозрачностью
        for (auto& cell : cells) {
            SDL_Rect tile{ cell.second * TILE_SIZE, cell.first * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
            SDL_RenderFillRect(renderer, &tile);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
}

// плавный сдвиг кубиков вниз(падение)
void animateFall(vector<vector<int>>& board) {
    bool falling = true;
    while (falling) {
        falling = false;
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);

        for (int row = HEIGHT - 2; row >= 0; --row) {
            for (int col = 0; col < WIDTH; ++col) {
                if (board[row][col] != -1 && board[row + 1][col] == -1) {
                    swap(board[row][col], board[row + 1][col]);
                    falling = true;
                }
            }
        }

        drawBoard(board);
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }
}

// ищем цепочки из 3 и больше одинаковых кубиков и удаляем их
// возвращаем сколько кубиков удалили (для обновления счёта игрока)
// сколько кубиков разбили (именно за 1 перестановку (то, что кубики падают сами
// и разбиваются - не учитывается в счёт)) - столько + в счёт
// если ничего не разбили -1 очко
int removeChains(vector<vector<int>>& board, bool animate =true, vector<pair<int, int>>* removedCellsOut = nullptr, vector<int>* removedColorsOut = nullptr) {
    vector<vector<bool>> visited(HEIGHT, vector<bool>(WIDTH, false));
    vector<vector<bool>> toRemove(HEIGHT, vector<bool>(WIDTH, false));
    int removedCount = 0;

    auto bfs = [&](int startR, int startC) {
        int color = board[startR][startC];
        vector<pair<int, int>> group;
        queue<pair<int, int>> q;
        q.push({ startR, startC });
        visited[startR][startC] = true;

        while (!q.empty()) {
            auto curr = q.front(); q.pop();
            group.push_back(curr);
            int r = curr.first;
            int c = curr.second;

            const int dr[4] = { -1, 1, 0, 0 };
            const int dc[4] = { 0, 0, -1, 1 };
            for (int i = 0; i < 4; ++i) {
                int nr = r + dr[i];
                int nc = c + dc[i];
                if (nr >= 0 && nr < HEIGHT && nc >= 0 && nc < WIDTH
                    && !visited[nr][nc]
                    && board[nr][nc] == color) {
                    visited[nr][nc] = true;
                    q.push({ nr, nc });
                }
            }
        }
        if ((int)group.size() >= 3) {
            for (auto& p : group) {
                toRemove[p.first][p.second] = true;
            }
        }
        };

    // проверяем всё поле
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (board[i][j] != -1 && !visited[i][j]) {
                bfs(i, j);
            }
        }
    }

    // удаляем отмеченные кубики, считаем сколько, позиции, цвет
    vector<pair<int, int>> removedCells;
    vector<int> removedColors;
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (toRemove[i][j]) {
                removedCells.push_back({ i, j });
                removedColors.push_back(board[i][j]);
                board[i][j] = -1;
                removedCount++;
            }
        }
    }
    if (animate) {
        animateDisappearance(board, removedCells);
    }
    
    if (removedCellsOut)   *removedCellsOut = removedCells;
    if (removedColorsOut)  *removedColorsOut = removedColors;

    return removedCount;
}

// сдвигаем кубики вниз, чтобы заполнить пустоты (без анимации) 
// (чтобы вначале при отрисовке поля убрать готовые комбинации)
void collapseBoard(vector<vector<int>>& board) {
    for (int col = 0; col < WIDTH; ++col) {
        int emptySpot = HEIGHT - 1;
        for (int row = HEIGHT - 1; row >= 0; --row) {
            if (board[row][col] != -1) {
                if (row != emptySpot) {
                    board[emptySpot][col] = board[row][col];
                    board[row][col] = -1;
                }
                emptySpot--;
            }
        }
    }
}

// заполняем пустые места новыми случайными кубиками
void refillBoard(vector<vector<int>>& board) {
    for (int col = 0; col < WIDTH; ++col) {
        for (int row = 0; row < HEIGHT; ++row) {
            if (board[row][col] == -1) {
                board[row][col] = rand() % COLORS;
            }
        }
    }
}

// обновляем заголовок окна, чтобы показывать текущий счёт
void updateWindowTitle(int score) {
    string title = "gems game - Score: " + to_string(score);
    SDL_SetWindowTitle(window, title.c_str());
}

int main(int argc, char* argv[]) {
    if (!initSDL()) return -1;

    vector<vector<int>> board;
    initializeBoard(board);

    int moveCount = 0;
    int bonusesGiven = 0;
    const int maxMovesForBonus = 10; // пусть на 10 ходов выпадает хотя-бы 2 бонуса
    const int maxBonusesPerPeriod = 2;

    // с самого начала убираем все цепочки, чтобы поле было без готовых комбинаций (без очков)
    while (removeChains(board, false) > 0) {
        collapseBoard(board);
        refillBoard(board);
    }

    cout << "Score: " << score << endl;
    updateWindowTitle(score);

    bool quit = false;
    SDL_Event e;

    pair<int, int> selected = { -1, -1 }; // выбранный кубик

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
                        // если еще ничего не выбрано, выбираем кубик
                        selected = { row, col };
                    }
                    else {
                        // уже выбран первый кубик, выбираем второй и проверяем ход
                        pair<int, int> second = { row, col };
                        if (isAdjacent(selected, second)) {
                            // меняем кубики местами
                            swap(board[selected.first][selected.second], board[second.first][second.second]);

                            // смотрим, сколько удалится кубиков сразу после хода
                            vector<pair<int, int>> removedCells;
                            vector<int> removedColors;
                            int removedFirst = removeChains(board, true, &removedCells, &removedColors);

                            if (removedFirst > 0) {
                                // если удалили хотя бы 3 кубика (цепочка) — даём очки за эти кубики
                                score += removedFirst;
                                updateWindowTitle(score);

                                moveCount++;

                                // генерируем бонус с вероятностью и ограничением по их кол-ву
                                if (moveCount <= maxMovesForBonus && bonusesGiven < maxBonusesPerPeriod) {
                                    Bonus bonus = generateRandomBonus(removedCells, removedColors, board);
                                    if (bonus.type != NONE) {
                                        bonusesGiven++;
                                        // применяем бонус
                                        if (bonus.type == COLOR_CHANGE) {
                                            applyColorChangeBonus(board, bonus);
                                        }
                                        else if (bonus.type == BOMB) {
                                            applyBombBonus(board, bonus);
                                        }
                                        animateFall(board);
                                    }
                                }

                                // теперь делаем каскадные удаления (сдвиги и дозаполнение), но очки уже не даём за них
                                while (true) {
                                    animateFall(board);
                                    refillBoard(board);
                                    int removedCascade = removeChains(board, true);
                                    if (removedCascade == 0) break;
                                }
                                // сброс счётчика
                                if (moveCount >= maxMovesForBonus) {
                                    moveCount = 0;
                                    bonusesGiven = 0;
                                }

                            }
                            else {
                                // если цепочек не появилось — минус 1 очко, но ход не отменяем (кубики остаются на новых местах)
                                score--;
                                updateWindowTitle(score);
                            }
                            selected = { -1, -1 };
                        }
                        else {
                            // если выбрали не соседние кубики, просто меняем выбор
                            // последний выбранный кубик(2) станет первым и нужно выбрать второй кубик для перемены
                            selected = { row, col };
                        }
                    }
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // фон 
        SDL_RenderClear(renderer);

        drawBoard(board, selected);

        SDL_RenderPresent(renderer);
    }

    closeSDL();
    return 0;
}
