#pragma once 

#include <stdint.h>
#include <fstream>
#include <exception>

namespace cssv {

class CSSException : std::exception {
    // TODO -- Implement C++ approved exception handling
};

template<typename Format> 
class CsvReader {
    bool open = false;
    std::ifstream fstream;
    std::string line;

    public:
    void open_file(const std::string& filename) {
        fstream.open(filename);
        if (!fstream.is_open()) {
            
        }
    }
};

}

