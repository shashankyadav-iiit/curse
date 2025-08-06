#include<bits/stdc++.h>
using namespace std;


enum class OrderSide : uint8_t{
    BUY = 1,
    SELL = 2
};

enum class OrderType : uint8_t {
    LIMIT = 1,
    MARKET = 2,
    STOP = 3,
    STOP_LIMIT = 4,
    ICEBERG = 5
};



struct Order {
    uint64_t orderId;
    string symbol;
    double price;
    uint64_t quantity;
    uint64_t timeStamp_ns;
    OrderSide side;
    OrderType type;

    Order() : orderId(0), price(0.0), quantity(0), timeStamp_ns(0), side(OrderSide::BUY), type(OrderType::LIMIT) {} 
};

enum class TradeCondition : uint32_t {
    NORMAL = 0x00000001,
    OPENING = 0x00000002,
    CLOSING = 0x00000004,
    CROSS_TRADE = 0x00000008,
    HIDDEN_ORDER = 0x00000010,
    ICEBERG_FILL = 0x00000020,
    BLOCK_TRADE = 0x00000040,
    SWEEP = 0x00000080,
    AGGRESSIVE = 0x00000100
};

struct PriceLevel {
    double price;
    uint64_t total_quantity;
    uint32_t order_count;
    uint64_t last_update_time_ns;
    vector<Order> orders;
    double average_order_size;
    uint32_t iceberg_count;
    uint64_t hidden_quantity;
    double turnover_rate;

    PriceLevel() : price(0.0), total_quantity(0), order_count(0), last_update_time_ns(0), average_order_size(0.0), iceberg_count(0), hidden_quantity(0), turnover_rate(0.0) {}

    void add_order(const Order& order);
    void remove_order(uint64_t orderId);
    void modify_order(uint64_t orderId, uint64_t new_quantity);
};


struct Trade {
    uint64_t tradeId;
    string symbol;
    double price;
    uint64_t quantity;
    uint64_t timeStamp_ns;
    uint64_t buy_order_id;
    uint64_t sell_order_id; // Time of the trade in nanoseconds
    string aggressor_side;
    TradeCondition condition;
    // Order ID of the maker

    Trade() : tradeId(0), price(0.0), quantity(0), timeStamp_ns(0), buy_order_id(0), sell_order_id(0), aggressor_side(""), condition(TradeCondition::NORMAL) {}
};

class Orderbook {
    private:
        string symbol;

        vector<PriceLevel> bid_levels_;   // decending order of price
        vector<PriceLevel> ask_levels_;   // ascending order of price 

        //order tracking
        unordered_map<uint64_t, pair<double, OrderSide>> order_locations_; // <orderId, <price, side>>

        double mid_price_;
        double spread_;
        double spread_bps_;
        uint64_t total_bid_quantity_;
        uint64_t total_ask_quantity_;
        uint64_t sequence_number_;
        uint64_t last_update_time_;


        double order_flow_imbalance_;
        double price_impact_model_;
        double volatility_estimate_;
        uint32_t update_frequency_;

        // trade flow tracking
        vector<Trade> recent_trades_;;

    public:
        // Constructor
        Orderbook() {
            // Initialize order book
        }

        // Method to add an order
        bool addOrder(const string& order) {
            // Logic to add order
        }

        // Method to modify an order
        bool modifyOrder(const string& orderId) {
            // Logic to modify order
        }

        // Method to get the current state of the order book
        bool cancelOrder(){

        }
};