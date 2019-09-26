#ifndef FIFTEEN_POSITION_HPP
#define FIFTEEN_POSITION_HPP

#include <unordered_map>
#include <iostream>
#include <string>
#include <set>

extern std::unordered_map<char, std::pair<int, int>> base;

class Position {
    std::string field;
    int heuristic;
    int distance;
    int forecast;
    Position* parent;

    bool is_correct_line(const std::string& field) const {
        if (field.length() != 16) {
            return false;
        }
        std::set<char> temp;
        for (const auto &c: field) {
            temp.insert(c);
        }
        if (temp.size() != 16) {
            return false;
        }
        for (char i = '0'; i <= '9'; ++i) {
            if (temp.find(i) == temp.end()) {
                return false;
            }
        }
        for (char i = 'A'; i <= 'F'; ++i) {
            if (temp.find(i) == temp.end()) {
                return false;
            }
        }
        return true;
    }

    int calc_heuristic() const {
        int sum = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (field[i * 4 + j] == '0') {
                    continue;
                }
                sum += abs(base[field[i * 4 + j]].first - i) + abs(base[field[i * 4 +j]].second - j);
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j1 = 0; j1 < 3; ++j1) {
                for (int j2 = j1 + 1; j2 < 4; ++j2) {
                    if (field[i * 4 + j1] != '0' && field[i + 4 + j2] != '0' &&
                        base[field[i * 4 + j1]].first == i && base[field[i * 4 + j2]].first == i &&
                        base[field[i * 4 + j1]].first == base[field[i * 4 + j2]].first &&
                        base[field[i * 4 + j1]].second > base[field[i * 4 + j2]].second) {
                        sum += 2;
                    }
                    if (field[j1 * 4 + i] != '0' && field[j2 + 4 + i] != '0' &&
                        base[field[j1 * 4 + i]].second == i && base[field[j2 * 4 + i]].second == i &&
                        base[field[j1 * 4 + i]].second == base[field[j2 * 4 + i]].second &&
                        base[field[j1 * 4 + i]].first > base[field[j2 * 4 + i]].first) {
                        sum += 2;
                    }
                }
            }
        }
        return sum;
    }

public:
    explicit Position(const std::string& field): distance(0) {
        if (!is_correct_line(field)) {
            throw 1;
        }
        this->field = field;
        heuristic = calc_heuristic();
        forecast = distance + heuristic;
        parent = nullptr;
    }

    Position(std::string field, int distance, Position* parent): field(std::move(field)), distance(distance), parent(parent) {
        heuristic = calc_heuristic();
        forecast = distance + heuristic;
    }

    ~Position() {
        delete parent;
    }

    std::string get_field() const {
        return field;
    }

    int get_heuristic() const {
        return heuristic;
    }

    int get_distance() const {
        return distance;
    }

    void set_dist(int new_dist) {
        distance = new_dist;
    }

    int get_forecast() const {
        return forecast;
    }

    void set_forecast(int new_forecast) {
        forecast = new_forecast;
    }

    Position* get_parent() const {
        return parent;
    }

    void set_parent(Position* new_parent) {
        parent = new_parent;
    }

    friend std::ostream& operator<<(std::ostream& os, const Position& position) {
        for (int i = 0; i < 4; ++i, os << "\n")
            for (int j = 0; j < 4; ++j, os << " ")
                os << position.field[i * 4 + j];
        os << "-------\n";
        return os;
    }

    struct comparator {
        bool operator()(const Position* first_position, const Position* second_position) const {
            return first_position->get_field() < second_position->get_field();
        }
    };
};

#endif //FIFTEEN_POSITION_HPP
