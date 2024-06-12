#pragma once 

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string_view>
#include <tuple>
#include <charconv>

namespace cssv {

template <size_t N> 
struct Separator {
    const std::string_view& view;
    std::array<std::string_view::iterator, N> begins;
    std::array<std::string_view::iterator, N> ends;

    Separator(const std::string_view& view) 
        : view(view) {
            partition();
        }

    private:

    void partition() {
        std::string_view::iterator begin = view.begin();
        for (size_t i = 0; i < N; i++) {
            fill_next_value(i, begin);
        }
    }

    std::string_view::iterator find_end_quote(std::string_view::iterator begin, std::string_view::iterator end) {
        std::string_view::iterator end_value = begin;

        while (true) {
            std::string_view::iterator quote = std::find(end_value, end, '\"');
            if (quote + 1 == end || *(quote+1) == ',') {
                // We found our string!
                end_value = quote+1;
                break;
            } else if (quote == end) {
                end_value = end; // Technically an error
                break;
            }
        }

        return end_value;
    }

    void fill_next_value(size_t index, std::string_view::iterator begin) {
        bool double_quote = (*begin == '\"');
        std::string_view::iterator first = (double_quote) ? begin + 1 : begin;
        if (double_quote) {
            while (true) {

            }
        }
    }

}

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
        std::apply([&format, &view] (auto&... args) {
                // std::cout << "Reaching here in function" << std::endl;
            ((CsvReader::read_value(format.*(args.second), view)), ...);
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
    static void read_value(T& val, std::string_view& view) {
        std::cout << "Reaching here in read" << std::endl;
        std::string_view::iterator begin = view.begin();
        std::string_view::iterator end = std::find(begin, view.end(), ',');
        CsvReader::set_value(val, begin, end);
        if (end != view.end()) {
            begin = end + 1;
        }
        view = std::string_view(begin, end);
    }
};

}

