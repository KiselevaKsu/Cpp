#ifndef BONUS_H
#define BONUS_H

#include <vector>
using namespace std;

enum BonusType {
    NONE,
    COLOR_CHANGE,
    BOMB
};

struct Bonus {
    BonusType type;
    int row, col;
    int color;
};

Bonus generateRandomBonus(const vector<pair<int, int>>& removedPositions, const vector<int>& removedColors,const vector<vector<int>>& board);

void applyColorChangeBonus(vector<vector<int>>& board, const Bonus& bonus);
void applyBombBonus(vector<vector<int>>& board, const Bonus& bonus);

#endif

