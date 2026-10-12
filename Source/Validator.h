#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <ios>
#include "HotelException.h"

// Input helpers and validation rules (header-only, no state).
namespace Validator {

inline std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) { return (char)std::tolower(c); });
    return s;
}

inline bool isValidName(const std::string& s) {
    if (s.empty() || s.size() > 40) return false;
    bool hasAlpha = false;
    for (char c : s) {
        if (std::isalpha((unsigned char)c)) hasAlpha = true;
        else if (c != ' ' && c != '.') return false;
    }
    return hasAlpha;
}

inline bool isValidPhone(const std::string& s) {
    if (s.size() != 10) return false;
    for (char c : s) if (!std::isdigit((unsigned char)c)) return false;
    return s[0] >= '6' && s[0] <= '9';
}

inline bool isValidEmail(const std::string& s) {
    if (s.size() < 5 || s.size() > 50) return false;
    for (char c : s) if (c == ' ' || c == '|') return false;
    size_t at = s.find('@');
    if (at == std::string::npos || at == 0 || s.find('@', at + 1) != std::string::npos) return false;
    size_t dot = s.find('.', at);
    return dot != std::string::npos && dot > at + 1 && dot < s.size() - 1;
}

// Format DD-MM-YYYY
inline bool isValidDate(const std::string& s) {
    if (s.size() != 10 || s[2] != '-' || s[5] != '-') return false;
    for (size_t i = 0; i < s.size(); i++)
        if (i != 2 && i != 5 && !std::isdigit((unsigned char)s[i])) return false;
    int d = std::stoi(s.substr(0, 2));
    int m = std::stoi(s.substr(3, 2));
    int y = std::stoi(s.substr(6, 4));
    if (y < 2000 || y > 2100 || m < 1 || m > 12 || d < 1) return false;
    int dim[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    if (leap) dim[1] = 29;
    return d <= dim[m - 1];
}

inline std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    if (!std::getline(std::cin, s)) throw std::ios_base::failure("Input stream closed");
    size_t b = s.find_first_not_of(" \t\r\n");
    size_t e = s.find_last_not_of(" \t\r\n");
    return (b == std::string::npos) ? "" : s.substr(b, e - b + 1);
}

inline int readInt(const std::string& prompt, int lo, int hi) {
    while (true) {
        std::string s = readLine(prompt);
        try {
            size_t pos;
            int v = std::stoi(s, &pos);
            if (pos == s.size() && v >= lo && v <= hi) return v;
        } catch (...) {}
        std::cout << "  Invalid input. Enter a whole number between " << lo << " and " << hi << ".\n";
    }
}

inline double readDouble(const std::string& prompt, double lo, double hi) {
    while (true) {
        std::string s = readLine(prompt);
        try {
            size_t pos;
            double v = std::stod(s, &pos);
            if (pos == s.size() && v >= lo && v <= hi) return v;
        } catch (...) {}
        std::cout << "  Invalid input. Enter a number between " << lo << " and " << hi << ".\n";
    }
}

} // namespace Validator

#endif