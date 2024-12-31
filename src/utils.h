#pragma once
#include <string>
#include <vector>
#include <sstream>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

std::string leftPad(std::string str, int length, char padChar = ' ')
{
    if (str.length() >= length)
        return str;
    return std::string(length - str.length(), padChar) + str;
}

std::string rightPad(std::string str, int length, char padChar = ' ')
{
    if (str.length() >= length)
        return str;
    return str + std::string(length - str.length(), padChar);
}

std::vector<std::string> splitLines(const std::string& input) {
    std::vector<std::string> lines;
    std::stringstream ss(input);
    std::string line;
    
    while (std::getline(ss, line)) {
        lines.push_back(line);
    }
    
    return lines;
}