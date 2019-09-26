#include "Game.hpp"

unordered_map<char, pair<int, int>> base = {{'1', {0, 0}}, {'2', {0, 1}}, {'3', {0, 2}}, {'4', {0, 3}},
                                            {'5', {1, 0}}, {'6', {1, 1}}, {'7', {1, 2}}, {'8', {1, 3}},
                                            {'9', {2, 0}}, {'A', {2, 1}}, {'B', {2, 2}}, {'C', {2, 3}},
                                            {'D', {3, 0}}, {'E', {3, 1}}, {'F', {3, 2}}, {'0', {3, 3}}};

vector<string> fields = {"52086A43D917ECBF", "450827CB936F1DEA",
                         "123456789ABCDFE0", "1853AC4F6B0D92E7"};

int main() {
    try {
        auto start_pos = new Position(fields[3]);
        auto new_game = new Game(start_pos);
        new_game->a_star();
    } catch(int i) {
        switch (i) {
            case 1:
                cout << "Bad start_position field!";
                break;
            case 2:
                cout << "Start field is not solvable!";
                break;
            default:
                break;
        }
    }
    return 0;
}
