#include <iostream>
#include <vector>
#include <cassert>

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

std::string get_character(const std::string_view string, const std::vector<std::vector<std::string>>& map) {
    assert((string.size() == 3 || string.size() == 4) && "Invalid encryption input");

    std::vector<char> valid_directions {'1', '2', '3', '4'};

    char direction {string[0]};
    char amount {string[1]};
    std::string character {string};
    character.erase(0, 2);

    bool valid_direction = false;
    bool valid_amount = false;

    for (const auto& n : valid_directions) {
        if (n == direction) {
            valid_direction = true;
        }
    }

    for (char c {'0'}; c <= '9'; ++c) {
        if (c == amount) {
            valid_amount = true;
            break;
        }
    }

    if (not valid_direction || not valid_amount) {
        return "|";
    }

    std::pair<int, int> pointer {0, 0};

    bool found {false};
    for (auto row {0uz}; row <  map.size(); ++row) {
        for (auto col {0uz}; col < map[0].size(); ++col) {
            if (map[static_cast<int>(row)][static_cast<int>(col)] == character) {
                pointer = {static_cast<int>(row), static_cast<int>(col)};
                found = true;
                break;
            }
        }
        if (found) {
            break;
        }
    }

    if (!found) {
        return "|";
    }

    std::pair<int, int> increment {0, 0};

    switch (direction) {
        case '1': {
            increment = {0, 1};
            break;
        }
        case '2': {
            increment = {1, 0};
            break;
        }
        case '3': {
            increment = {0, -1};
            break;
        }
        case '4': {
            increment = {-1, 0};
            break;
        }
        default: increment = {0, 0};
    }

    int num_amount = amount - '0';

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
    
    std::string result {map[pointer.first][pointer.second]};
    return result;
}

int main() {
    const std::vector<std::vector<std::string>> map {key_map()};
    const std::vector<std::string> utf8_characters {"å", "ä", "ö"};
    std::string input;
    std::getline(std::cin, input);
    if (input.size() < 3) {
        std::cout << "Too short input";
        return 1;
    }

    std::string result {""};

    std::string current {""};
    auto pointer {0uz};
    bool space {false};
    while (pointer < input.size()) {                
        if (input[static_cast<int>(pointer)] == ' ') {
            if (current.size() > 0 && current.size() < 3) {
                std::cout << "Invalid encryption input\n";
                return 1;
            }
            ++pointer;
            space = true;
        }
        
        current += input[static_cast<int>(pointer)];

        if (current.size() == 3) {
            if (current.size() < 3 || current.size() > 4) {
                std::cout << "Invalid encryption input\n";
                return 1;
            }

            bool utf8 {false};
            std::string encrypted_character {current};
            encrypted_character.erase(0, 2);
            if (pointer + 1 < input.size()) {
                for (const auto& c : utf8_characters) {
                    if (encrypted_character + input[static_cast<int>(pointer) + 1] == c) {
                        utf8 = true;
                    }
                }
            }
            if (utf8) {
                ++pointer;
                current += input[static_cast<int>(pointer)];
            }


            std::string character {get_character(current, map)};
            if (character == "|") {
                std::cout << "Invalid encryption input\n";
                return 1;
            }
            if (space) {
                result += ' ';
            }

            result += character;
            space = false;
            current = "";
        }
        
        ++pointer;

    }

    std::cout << result << "\n";



    return 0;
}
