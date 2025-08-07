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

struct Order {
    uint64_t order_id;
    std::string symbol;
    OrderSide side;
    OrderType type;
    double price;
    uint64_t quantity;
    uint64_t visible_quantity;  // For iceberg orders
    uint64_t filled_quantity;
    std::string participant_id;
    uint64_t timestamp_ns;
    uint32_t priority;  // Time/price priority
    bool is_hidden;
    
    // For advanced order types
    double stop_price;
    double limit_price;
    uint64_t minimum_quantity;

    Order() : order_id(0), side(OrderSide::BUY), type(OrderType::LIMIT), price(0.0), quantity(0), visible_quantity(0), filled_quantity(0), timestamp_ns(0), priority(0), is_hidden(false), stop_price(0.0), limit_price(0.0), minimum_quantity(0) {}
    
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
    uint64_t trade_id;
    string symbol;
    double price;
    uint64_t quantity;
    uint64_t timeStamp_ns;
    uint64_t buy_order_id;
    uint64_t sell_order_id; // Time of the trade in nanoseconds
    string aggressor_side;
    TradeCondition condition;
    // Order ID of the maker

    double price_impact;
    double volume_impact;
    bool is_block_trade;
    bool is_sweep;

    Trade() : trade_id(0), price(0.0), quantity(0), timeStamp_ns(0), buy_order_id(0), sell_order_id(0), aggressor_side(""), condition(TradeCondition::NORMAL), price_impact(0.0), volume_impact(0.0), is_block_trade(false), is_sweep(false) {}
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
        vector<Trade> recent_trades_;
        static constexpr size_t MAX_RECENT_TRADES = 1000;

    public:
        // Constructor
        Orderbook(const string& symbol);

        // Method to add an order
        bool add_order(const string& order) ;
        bool modify_order(const string& order_id,uint64_t new_quantity, double new_price = 0.0) ;

        // Method to get the current state of the order book
        bool cancelOrder(uint64_t order_id);

        void process_trade(const Trade& trade);
        vector<Trade> match_orders();


        vector<PriceLevel> &get_bid_levels() {
            return bid_levels_;
        }
        vector<PriceLevel> &get_ask_levels() {
            return ask_levels_;
        }
        vector<Trade> &get_recent_trades() {
            return recent_trades_;
        }
        double get_mid_price() const {
            return mid_price_;
        }
        double get_spread() const {
            return spread_;
        }
        double get_spread_bps() const {
            return spread_bps_;
        }
        double get_order_flow_imbalance() const {
            return order_flow_imbalance_;
        }
        uint64_t get_sequence_number() const {
            return sequence_number_;
        }

        double calculate_volume_weighted_mid() const;
        double calculate_microprice() const;
        double estimate_queue_position(uint64_t order_id) const;
        double calculate_market_impact(double quantity, OrderSide side) const;
        double get_price_impact_model() const{return price_impact_model_;};


        double calculate_available_liquidity(OrderSide side, double price_range) const;
        double calculate_depth_at_price(double price, OrderSide side) const;
        vector<double> get_liquidity_profile(double range_pct = 0.01) const;

        struct OrderFlowMetrics {
            double aggressive_buy_ratio;
            double aggressive_sell_ratio;
            double average_trade_size;
            double price_trend;
            double momentum;
            uint32_t trade_frequency;
            double block_trade_ratio;
            double sweep_ratio;
        };

        OrderFlowMetrics calculate_order_flow_metrics(uint64_t lookback_ns = 60000000000) const;

        struct MarketQuality{
            double effective_spread;
            double realized_spread;
            double price_impact;
            double quote_stability;
            double depth_resilience;
            double adverse_selection_cost;
        };
        MarketQuality calculate_market_quality() const;

        void update_from_snapshot(const vector<PriceLevel>& bid_levels, const vector<PriceLevel>& ask_levels);
        void clear();

        void print_book(size_t levels = 10 ) const;

        private:
        void insert_price_level(std::vector<PriceLevel>& levels, const PriceLevel& level, bool ascending);
        void update_book_metrics();
        void maintain_book_depth();
        double calculate_vwap(const std::vector<Trade>& trades, uint64_t lookback_ns) const;
        void update_volatility_estimate();
};