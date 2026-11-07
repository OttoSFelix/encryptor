#include <iostream>
#include <vector>
#include <random>
#include <chrono>

std::vector<std::vector<std::string>> key_map() {
    std::vector<std::vector<std::string>> map {
                {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "å"},
                {"a", "s", "d", "f", "g", "h", "j", "k", "l", "ö", "ä"},
                {"z", "x", "c", "v", "b", "n", "m", ",", ".", "-", "/"},
                {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "?"},
                {"!", "=", "#", "%", "@", "(", ")", "+", "<", ">", ":"} 
    };
    return map;
}

int randint(int min, int max) {
    std::random_device rd {};
    std::seed_seq ss{
		static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
		rd(), rd(), rd(), rd(), rd(), rd(), rd() };

    std::mt19937 generator {ss};

    return std::uniform_int_distribution{min, max}(generator);
}

std::string get_encrypted_character(std::string_view string, const std::vector<std::vector<std::string>>& map) {
    int random_direction {randint(1, 4)};
    int random_distance {randint(1, 9)};

    std::pair<int, int> pointer {0, 0};

    bool found {false};
    for (auto row {0uz}; row < map.size(); ++row) {
        for (auto col {0uz}; col < map[0].size(); ++col) {
            if (map[static_cast<int>(row)][static_cast<int>(col)] == string) {
                pointer = {static_cast<int>(row), static_cast<int>(col)};
                found = true;
                break;
            }
            if (found) {
                break;
            }
        }
    }

    if (!found) {
        return "|";
    }

    
    std::pair<int, int> increment {0, 0};
    int final_direction;

    if (random_direction == 1) {
        final_direction = 3;
        increment = {0, 1};
    }
    else if (random_direction == 2) {
        final_direction = 4;
        increment = {1, 0};
    }
    else if (random_direction == 3) {
        final_direction = 1;
        increment = {0, -1};
    }
    else if (random_direction == 4) {
        final_direction = 2;
        increment = {-1, 0};
    }

    int num_amount {random_distance};

    while (num_amount > 0) {
        pointer.first += increment.first;
        pointer.second += increment.second;

        if (pointer.first < 0) {
            pointer.first = static_cast<int>(map.size()) - 1;
        }
        if (pointer.first >= map.size()) {
            pointer.first = 0;
        }
        if (pointer.second < 0) {
            pointer.second = static_cast<int>(map[0].size()) - 1;
        }
        if (pointer.second >= map[0].size()) {
            pointer.second = 0;
        }


        --num_amount;
    }

    char result_direction {static_cast<char>(final_direction + '0')};
    char result_distance {static_cast<char>(random_distance + '0')};
    std::string result_key {map[pointer.first][pointer.second]};
    std::string result {std::string(1, result_direction) + std::string(1, result_distance) + result_key};

    return result;
        
}


int main() {
    const std::vector<std::string> utf8_characters {"å", "ä", "ö"};
    const std::vector<std::vector<std::string>> map {key_map()};

    std::string input;
    std::getline(std::cin, input);
    if (input.size() == 0) {
        std::cout << "Empty input!\n";
        return 1;
    }
 
    std::string result {""};

    auto pointer {0uz};
    std::string current {""};
    while (pointer < input.size()) {
        if (input[static_cast<int>(pointer)] == ' ') {
            result += ' ';
            current = "";
            ++pointer;
            continue;
        }
        
        current += input[static_cast<int>(pointer)];

        if (pointer + 1 < input.size()) {
            for (const auto& c : utf8_characters) {
                if (current + input[static_cast<int>(pointer) + 1] == c) {
                    ++pointer;
                    current += input[static_cast<int>(pointer)];
                    break;
                }
            }
        }

        std::string encrypted_character {get_encrypted_character(current, map)};
        if (encrypted_character == "|") {
            std::cout << "Invalid input\n";
            return 1;
        }
        result += encrypted_character;

        ++pointer;
        current = "";
    }

    std::cout << result << "\n";

    return 0;
}
