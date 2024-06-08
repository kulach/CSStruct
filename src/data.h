#pragma once
#include <csv.h>
#include <cmath>

struct TargetPositionData {
    // Csv data structs must have global data to work
    uint32_t unique_id;
    uint64_t target_arrival_time;
    uint64_t target_end_time;
    int64_t target_position; 

    uint64_t t_start; // Arrival time in seconds
    uint64_t t_end; // End time in seconds
    static constexpr auto types = std::make_tuple(
            &TargetPositionData::unique_id,
            &TargetPositionData::target_arrival_time,
            &TargetPositionData::target_end_time,
            &TargetPositionData::target_position
            );

    void clean_data() {
        t_start = std::round(target_arrival_time / 1e9);
        t_end = std::round(target_end_time / 1e9);
    }

};

struct MarketDataRow {
    uint32_t unique_id;
    uint64_t interval_start_time;
    uint64_t interval_end_time;
    double last_trade_price;
    double interval_average_spread;
    uint64_t interval_traded_volume;

    uint64_t t_start; // Start time in seconds
    uint64_t t_end; // End time in seconds
    static constexpr auto types = std::make_tuple(
            &MarketDataRow::unique_id,
            &MarketDataRow::interval_start_time, 
            &MarketDataRow::interval_end_time,
            &MarketDataRow::last_trade_price,
            &MarketDataRow::interval_average_spread,
            &MarketDataRow::interval_traded_volume
            );

    void clean_data() {
        t_start = std::round(interval_start_time / 1e9);
        t_end = std::round(interval_end_time / 1e9);
    }
};

// Output PNL Format
struct PnlDataRow {
    uint64_t time;
    double unrealized;
    double realized;
    double pnl;
    static constexpr auto types = std::make_tuple(
            &PnlDataRow::time,
            &PnlDataRow::unrealized,
            &PnlDataRow::realized,
            &PnlDataRow::pnl
            );
    static constexpr auto names = {"time", "unrealized", "realized", "pnl"};
    PnlDataRow(uint64_t time, double unrealized, double realized, double pnl) 
        : time(time), unrealized(unrealized), realized(realized), pnl(pnl) {}
};

// Output Trade Format
struct TradeRow {
    uint32_t unique_id;
    uint64_t fill_time;
    int32_t qty; // Negative sign implies sells, positive implies 
    double price;
    static constexpr auto types = std::make_tuple(
            &TradeRow::unique_id,
            &TradeRow::fill_time,
            &TradeRow::qty,
            &TradeRow::price
            );

    static constexpr auto names = {"unique_id", "fill_time", "qty", "price"};

    void clean_data() {}

    TradeRow() {}

    TradeRow(uint32_t unique_id, uint64_t fill_time, int32_t qty, double price) 
        : unique_id(unique_id), fill_time(fill_time), qty(qty), price(price) {}
};


