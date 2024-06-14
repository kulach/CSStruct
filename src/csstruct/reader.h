#pragma once 

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string_view>
#include <tuple>
#include <charconv>
#include <array>

namespace cssv {

template <size_t N> 
struct Separator {
    size_t next = 0;
    std::array<std::string_view::iterator, N> begins;
    std::array<std::string_view::iterator, N> ends;

    void parse_view(const std::string_view& view) {
        partition(view);
        next = 0;
    }

    std::pair<std::string_view::iterator, std::string_view::iterator> next_range() {
        size_t curr = next;
        next++;
        return {begins[curr], ends[curr]};
    }

    private:

    void partition(const std::string_view& view) {
        std::string_view::iterator begin = view.begin();
        std::string_view::iterator end = view.end();
        for (size_t i = 0; i < N; i++) {
            std::string_view::iterator last = fill_next_value(i, begin, end);
            if (last != view.end()) begin = last + 1;
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

    void append_bounds(size_t index, std::string_view::iterator first, std::string_view::iterator last) noexcept {
        begins[index] = first;
        ends[index] = last;
    }

    std::string_view::iterator fill_next_value(size_t index, std::string_view::iterator begin, std::string_view::iterator end) {
        bool double_quote = (*begin == '\"');
        std::string_view::iterator first = (double_quote) ? begin + 1 : begin;
        std::string_view::iterator last;
        if (double_quote) {
            std::string_view::iterator quote = begin;
            while (true) {
                quote = std::find(quote, end, '\"');
                if (quote + 1 == end || *(quote + 1) == ',') {
                    // End found
                    last = quote + 1;
                    break;
                }
            }
        } else {
            last = std::find(begin, end, ',');
        }
        append_bounds(index, first, last);
        return last;
    }

};

template<typename Format> 
class CsvReader {
    Separator<std::tuple_size_v<decltype(Format::properties)>> separator;
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
        separator.parse_view(view);
        std::apply([this, &format, &view] (auto&... args) {
            ((this->read_value(format.*(args.second), view)), ...);
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
    void read_value(T& val, std::string_view& view) {
        auto range = separator.next_range();
        auto begin = range.first;
        auto end = range.second;
        CsvReader::set_value(val, begin, end);
        if (end != view.end()) {
            begin = end + 1;
        }
        view = std::string_view(begin, end);
    }
};

}

