#include "Bonus.h"
#include <algorithm>
#include <cstdlib>

Bonus generateRandomBonus(
    const vector<pair<int, int>>& removedPositions,
    const vector<int>& removedColors,
    const vector<vector<int>>& board
) {
    Bonus bonus;
    bonus.type = NONE;
    if (removedPositions.empty()) return bonus;

    int chance = rand() % 100;
    if (chance < 15) {
        int idx = 0;
        bonus.row = removedPositions[idx].first;
        bonus.col = removedPositions[idx].second;
        // берём цвет именно того удалённого кубика, куда выпадет бонус (первого)
        bonus.color = removedColors[idx];

        int bonusTypeRoll = rand() % 100;
        bonus.type = (bonusTypeRoll < 50 ? COLOR_CHANGE : BOMB);
    }
    return bonus;
}

void applyColorChangeBonus(vector<vector<int>>& board, const Bonus& bonus) {
    int r = bonus.row;
    int c = bonus.col;
    int originalColor = bonus.color;

    vector<pair<int, int>> candidates;
    for (int i = max(0, r - 3); i <= min((int)board.size() - 1, r + 3); ++i) {
        for (int j = max(0, c - 3); j <= min((int)board[0].size() - 1, c + 3); ++j) {
            if (board[i][j] != -1)
                candidates.push_back({ i, j });
        }
    }

    board[r][c] = originalColor;

    vector<pair<int, int>> nonNeighbors;
    for (auto& p : candidates) {
        int dr = abs(p.first - r);
        int dc = abs(p.second - c);
        if (!((dr == 1 && dc == 0) || (dr == 0 && dc == 1)) && !(p.first == r && p.second == c)) {
            nonNeighbors.push_back(p);
        }
    }

    std::random_shuffle(nonNeighbors.begin(), nonNeighbors.end());
    int count = min(2, (int)nonNeighbors.size());
    for (int i = 0; i < count; ++i) {
        board[nonNeighbors[i].first][nonNeighbors[i].second] = originalColor;
    }
}

void applyBombBonus(vector<vector<int>>& board, const Bonus& bonus) {
    vector<pair<int, int>> filledCells;
    for (int i = 0; i < (int)board.size(); ++i) {
        for (int j = 0; j < (int)board[0].size(); ++j) {
            if (board[i][j] != -1) {
                filledCells.push_back({ i, j });
            }
        }
    }

    if (board[bonus.row][bonus.col] != -1) {
        bool found = false;
        for (auto& p : filledCells) {
            if (p.first == bonus.row && p.second == bonus.col) {
                found = true;
                break;
            }
        }
        if (!found) filledCells.push_back({ bonus.row, bonus.col });
    }

    std::random_shuffle(filledCells.begin(), filledCells.end());

    int toRemove = min(5, (int)filledCells.size());
    for (int i = 0; i < toRemove; ++i) {
        board[filledCells[i].first][filledCells[i].second] = -1;
    }
}
