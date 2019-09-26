#ifndef FIFTEEN_GAME_HPP
#define FIFTEEN_GAME_HPP

#include "Position.hpp"
#include <map>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

class Game {
    Position* start_position;

    bool is_solvable() const {
        string line = start_position->get_field();
        int sum = (int)line.find('0') / 4 + 1;
        for (int i = 0; i < 16; ++i) {
            for (int j = i + 1; j < 16; ++j) {
                if (line[j] < line[i] && line[j] != '0') {
                    ++sum;
                }
            }
        }
        return sum % 2 == 0;
    }

    Position* move_left(Position* position) {
        string new_field = position->get_field();
        int zero_position = (int)new_field.find('0');
        if (zero_position % 4 != 3) {
            swap(new_field[zero_position], new_field[zero_position + 1]);
        } else {
            return nullptr;
        }
        return new Position(new_field, position->get_distance() + 1, position);
    }

    Position* move_right(Position* position) {
        string new_field = position->get_field();
        int zero_position = (int)new_field.find('0');
        if (zero_position % 4 != 0) {
            swap(new_field[zero_position], new_field[zero_position - 1]);
        } else {
            return nullptr;
        }
        return new Position(new_field, position->get_distance() + 1, position);
    }

    Position* move_up(Position* position) {
        string new_field = position->get_field();
        int zero_position = (int)new_field.find('0');
        if (zero_position / 4 != 3) {
            swap(new_field[zero_position], new_field[zero_position + 4]);
        } else {
            return nullptr;
        }
        return new Position(new_field, position->get_distance() + 1, position);
    }

    Position* move_down(Position* position) {
        string new_field = position->get_field();
        int zero_position = (int)new_field.find('0');
        if (zero_position / 4 != 0) {
            swap(new_field[zero_position], new_field[zero_position - 4]);
        } else {
            return nullptr;
        }
        return new Position(new_field, position->get_distance() + 1, position);
    }

public:
    explicit Game(Position* start): start_position(start) {}

     void a_star() {
        if (!is_solvable()) {
            throw 2;
        }
         clock_t start_clock = clock();
         start_position->set_forecast(start_position->get_heuristic());
         map<int, map<int, set<Position*, Position::comparator>>> open_set;
         open_set[start_position->get_forecast()][start_position->get_heuristic()].insert(start_position);
         set<Position*, Position::comparator> closed_set;
         Position* final_position = nullptr;
         while (!open_set.empty()) {
             Position* current_position = *open_set.begin()->second.begin()->second.begin();
             if (current_position->get_field() == "123456789ABCDEF0") {
                 final_position = current_position;
                 break;
             }
             open_set[current_position->get_forecast()][current_position->get_heuristic()].erase(current_position);
             if (open_set[current_position->get_forecast()][current_position->get_heuristic()].empty()) {
                 open_set[current_position->get_forecast()].erase(current_position->get_heuristic());
                 if (open_set[current_position->get_forecast()].empty()) {
                     open_set.erase(current_position->get_forecast());
                 }
             }
             closed_set.insert(current_position);
             for (Position* new_position: {move_left(current_position), move_up(current_position), move_right(current_position), move_down(current_position)}) {
                 if (new_position == nullptr) {
                     continue;
                 }
                 if (closed_set.find(new_position) != closed_set.end()) {
                     continue;
                 }
                 int tent_score = new_position->get_distance();
                 auto exist_position = open_set[new_position->get_forecast()][new_position->get_heuristic()].find(new_position);
                 if (exist_position == open_set[new_position->get_forecast()][new_position->get_heuristic()].end()) {
                     open_set[new_position->get_forecast()][new_position->get_heuristic()].insert(new_position);
                     continue;
                 } else if (tent_score >= (*exist_position)->get_distance()) {
                     continue;
                 }
                 new_position = *exist_position;
                 open_set[new_position->get_forecast()][new_position->get_heuristic()].erase(new_position);
                 if (open_set[new_position->get_forecast()][new_position->get_heuristic()].empty()) {
                     open_set[new_position->get_forecast()].erase(new_position->get_heuristic());
                     if (open_set[new_position->get_forecast()].empty()) {
                         open_set.erase(new_position->get_forecast());
                     }
                 }
                 new_position->set_parent(current_position);
                 new_position->set_dist(tent_score);
                 new_position->set_forecast(tent_score + new_position->get_heuristic());
                 open_set[new_position->get_forecast()][new_position->get_heuristic()].insert(new_position);
             }
         }
         double spent_time = double(clock() - start_clock) / CLOCKS_PER_SEC;
         Position* current = final_position;
         vector<Position *> positions;
         while (current != nullptr) {
             positions.push_back(current);
             current = current->get_parent();
         }
         reverse(positions.begin(), positions.end());
         for (const auto position: positions) {
             cout << *position;
         }
         cout << "\n=====================\n" << spent_time << endl;
         if (final_position) {
             cout << final_position->get_distance() << endl;
         }
    }
};

#endif //FIFTEEN_GAME_HPP
