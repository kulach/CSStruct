#pragma once 

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <charconv>

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
        if (!has_data()) {
            std::cerr << "Error reading next line" << std::endl;
            exit(1);
        }

        std::getline(fstream, line);
        read_line(format);
    }

    bool has_data() {
        return open && fstream.is_open();
    }

    void close_file() {
        fstream.close();
        open = false;
    }

    private:
    void read_line(Format& format) {
        std::string_view view(line);
        std::string_view::iterator begin = view.begin();
        std::apply([&format, &view, &begin] (auto&... args) {
            ((CsvReader::read_value(format.*(args.second), view, begin)), ...);
            }, Format::properties);
    }

    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true> 
    static void set_value(T& val, std::string_view::iterator& begin, std::string_view::iterator& end) noexcept {
        auto [_, ec] = std::from_chars(begin, end, val);
        if (ec != std::errc()) {
            std::cerr << "Error setting value with given data: " << std::string(begin, end) << std::endl;
            exit(1);
        }
    }

    template <typename T> 
    static void read_value(T& val, const std::string_view& line, std::string_view::iterator& begin) {
        std::string_view::iterator end = std::find(begin, line.end(), ',');
        CsvReader::set_value(val, begin, end);
        if (end != line.end()) {
            begin = end + 1;
        }
    }
};

}

