#include "Bonus.h"
#include <algorithm>
#include <cstdlib>

Bonus generateRandomBonus(
    const vector<pair<int, int>>& removedPositions,
    const vector<int>& removedColors,
    vector<vector<int>>& board
) {
    Bonus bonus;
    bonus.type = NONE;
    if (removedPositions.empty())
        return bonus;

    if (rand() % 100 >= 40)
        return bonus;

    int h = board.size();
    int w = board.empty() ? 0 : board[0].size();

    vector<pair<int, int>> candidates;
    for (auto const& pos : removedPositions) {
        for (int i = max(0, pos.first - 3); i <= min(h - 1, pos.first + 3); ++i) {
            for (int j = max(0, pos.second - 3); j <= min(w - 1, pos.second + 3); ++j) {
                if (board[i][j] != -1) {
                    candidates.emplace_back(i, j);
                }
            }
        }
    }
    if (candidates.empty())
        return bonus;

    auto choice = candidates[rand() % candidates.size()];
    bonus.row = choice.first;
    bonus.col = choice.second;

    bonus.type = (rand() % 2 == 0 ? COLOR_CHANGE : BOMB);

    if (bonus.type == COLOR_CHANGE) {
        board[bonus.row][bonus.col] = 100; // Малиновый (цвет смены)
    }
    else {
        board[bonus.row][bonus.col] = 101; // Бирюзовый (цвет бомбы)
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
            int dist = abs(i - r) + abs(j - c);
            if (dist >= 1 && dist <= 3 && board[i][j] != -1)
                candidates.push_back({ i, j });
        }
    }

    board[r][c] = originalColor;

    std::random_shuffle(candidates.begin(), candidates.end());
    int count = min(2, (int)candidates.size());
    for (int i = 0; i < count; ++i) {
        board[candidates[i].first][candidates[i].second] = originalColor;
    }
}

void applyBombBonus(vector<vector<int>>& board, const Bonus& bonus) {
    board[bonus.row][bonus.col] = -1; // уничтожает себя

    vector<pair<int, int>> filledCells;
    for (int i = 0; i < (int)board.size(); ++i) {
        for (int j = 0; j < (int)board[0].size(); ++j) {
            if (board[i][j] != -1) filledCells.push_back({ i, j });
        }
    }

    std::random_shuffle(filledCells.begin(), filledCells.end());

    int toRemove = min(5, (int)filledCells.size());
    for (int i = 0; i < toRemove; ++i) {
        board[filledCells[i].first][filledCells[i].second] = -1;
    }
}
