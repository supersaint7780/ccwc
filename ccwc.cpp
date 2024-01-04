#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace fs = std::filesystem;

int count_words(const char *filename) {
    std::ifstream file(filename);

    if(!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    std::string word;
    int words = 0;
    while(file >> word) {
        ++words;
    }
    file.close();
    return words;
}

int count_characters(const char *filename) {
    std::ifstream file(filename);

    if(!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    char currentChar;
    int characters = 0;
    while(file.get(currentChar)) {
        ++characters;
    }
    file.close();
    return characters;
}

int count_lines(const char *filename) {
    std::ifstream file(filename);

    if(!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1; 
    }

    std::string tempLine;
    int lines = 0;
    while(std::getline(file, tempLine)) {
        ++lines;
    }
    file.close();
    return lines;

}

std::streampos count_bytes(const char* filename) {

    std::ifstream file(filename, std::ios::binary);

    if(!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.close();

    return fileSize;
}

void count_lines_words_bytes(
    std::istream& input, int& lines, int& words, int& bytes_count, int &chars_count) {

    std::string line;
    while (getline(input, line)) {
        lines++;
        words += std::count_if(line.begin(), line.end(), ::isspace) + 1;
        bytes_count += line.size() + 1; // Including newline character
        chars_count += line.size();
    }
}


int main(int argc, char* argv[]) {
    if(argc < 1) {
        std::cerr << "Error: No valid arguments found" << std::endl;
        return 1;
    }

    std::vector<char *> filenames;
    std::vector<char *> flags;

    bool flag_c = false;
    bool flag_w = false;
    bool flag_m = false;
    bool flag_l = false;

    // separate the arguments into filenames and flags
    // detect if flags are valid and prints first invalid flag
    // detect if the file is a file or directory and stops if directory
    for(int i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            char t = argv[i][1];
            if(strlen(argv[i]) != 2 || (t != 'c' && t != 'w' && t != 'm' && t != 'l')) {
                std::cerr << "Error: Invalid flag option: " << argv[i] << std::endl;
                return 1;
            } else {
                flags.push_back(argv[i]);
            }
        } else {
            if(fs::is_directory(argv[i])) {
                std::cerr << argv[i] << ": It is a directory" << std::endl;
                return 1;
            }
            filenames.push_back(argv[i]);
        }
    }

    // set flags
    if(flags.size() == 0) {
        flag_c = flag_l = flag_w = true;
    } else {
        for(auto it:flags) {
            char ch = it[1];
            if(ch == 'c') flag_c = true;
            if(ch == 'w') flag_w = true;
            if(ch == 'l') flag_l = true;
            if(ch == 'm') flag_m = true;
        }
    }

    // check if user provided a filename
    if(filenames.size() == 0) {
        int lines = 0, words = 0, characters = 0, bytes = 0;
        count_lines_words_bytes(std::cin, lines, words, bytes, characters);

        if(flag_l) {
            std::cout << std::setw(8) << lines;
        }
        if(flag_w) {
            std::cout << std::setw(8) << words;
        }
        if(flag_m) {
            std::cout << std::setw(8) << characters;
        }
        if(flag_c) {
            std::cout << std::setw(8) << bytes;
        }
        return 0;
    }


    long long totalLines = 0;
    long long totalWords = 0;
    long long totalCharacters = 0;
    long long totalBytes = 0;

    int filesPrinted = 0;
    for(auto filename: filenames) {
        if(fs::exists(filename)) {
            int lines = count_lines(filename);
            int words = count_words(filename);
            int characters = count_characters(filename);
            std::streampos bytes = count_bytes(filename);

            totalLines += lines;
            totalCharacters += characters;
            totalWords += words;
            totalBytes += bytes;

            if(flag_l) {
                std::cout << std::setw(8) << lines;
            }
            if(flag_w) {
                std::cout << std::setw(8) << words;
            }
            if(flag_m) {
                std::cout << std::setw(8) << characters;
            }
            if(flag_c) {
                std::cout << std::setw(8) << bytes;
            }
            std::cout << " " << filename << std::endl;
            
            ++filesPrinted;
        } else {
            std::cout << std::setw(16) << "Invalid file: " << filename << std::endl; 
        }
    }

    if(filesPrinted > 1) {
        if(flag_l) {
            std::cout << std::setw(8) << totalLines;
        }
        if(flag_w) {
            std::cout << std::setw(8) << totalWords;
        }
        if(flag_m) {
            std::cout << std::setw(8) << totalCharacters;
        }
        if(flag_c) {
            std::cout << std::setw(8) << totalBytes;
        }
        std::cout << " " << "Total" << std::endl;
    }
}