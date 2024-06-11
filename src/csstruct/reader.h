#pragma once 

#include <stdint.h>
#include <fstream>
#include <exception>

namespace cssv {

template<typename Format> 
class CsvReader {
    bool open = false;
    std::ifstream fstream;
    std::string line;

    public:
    void open_file(const std::string& filename) {
        fstream.open(filename);
        if (!fstream.is_open()) {
            // TODO -- Add acceptable error handling
            std::cerr << "Error opening file " << filename;
            exit(1);
        }

        if (fstream.peek() != EOF) open = true;     
        else close_file();
        
        std::getline(fstream, line);
        if (!fstream.is_open()) {
            std::cout << "Fstream closed after reading line" << std::endl;
        }
    }

    void get_next_line(Format& format) {
        if !(has_data() || !fstream.is_open()) {
            std::cerr << "Error reading next line" << std::endl;
            exit(1);
        }

        std::getline(fstream, line);
        read_line(format);
    }

    void close_file() {
        fstream.close();
        open = false;
    }

    private:
    void read_line(Format& format) {
        std::string_view view(line);
        std::string_view::iterator begin = view.begin();
        std::apply([&f, &view, &begin], (auto&... args) {
                ((CsvReader::set_value_line(f.*args, view, begin)), ...);
                });
    }
};

}

