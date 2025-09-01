#include "market_data_engine.hpp"
#include<iostream>
#include<algorithm>


void PriceLevel::add_order(const Order& order) {
    orders.push_back(order);
    total_quantity += order.quantity;
    order_count++;
    last_update_time_ns = order.timeStamp_ns;

    if (order.type == OrderType::ICEBERG) {
        iceberg_count++; // Assuming the entire quantity is hidden for iceberg orders
    }
    if(order.is_hidden){
        hidden_quantity += order.quantity; // Add visible quantity for hidden orders
    }
}

void PriceLevel::remove_order(uint64_t orderId) {
    auto it = find_if(orders.begin(), orders.end(), [orderId](const Order& order) {
        return order.order_id == orderId;
    });
    
    if (it != orders.end()) {
        // Update price level metrics before removing the order
        total_quantity -= it->quantity;
        order_count--;
        
        // Update iceberg and hidden order metrics if applicable
        if (it->type == OrderType::ICEBERG) {
            iceberg_count--;
        }
        if(it->is_hidden) {
            hidden_quantity -= it->quantity;
        }
        
        // Remove the order from the vector
        orders.erase(it);
    }
}

void PriceLevel::modify_order(uint64_t orderId, uint64_t new_quantity) {
    auto it = std::find_if(orders.begin(), orders.end(),
        [order_id](const Order& order) { return order.order_id == order_id; });
    
    if (it != orders.end()) {
        total_quantity = total_quantity - it->quantity + new_quantity;
        visible_quantity = visible_quantity - it->visible_quantity + std::min(new_quantity, it->visible_quantity);
        it->quantity = new_quantity;
        if (it->visible_quantity > new_quantity) {
            it->visible_quantity = new_quantity;
        }
    }

}

Orderbook::Orderbook(const string& symbol) : symbol(symbol), mid_price_(0.0), spread_(0.0), spread_bps_(0.0), total_bid_quantity_(0), total_ask_quantity_(0), sequence_number_(0), last_update_time_(0), order_flow_imbalance_(0.0), price_impact_model_(0.0), volatility_estimate_(0.0), update_frequency_(100) {
    // Initialize bid and ask levels
    bid_levels_.clear();
    ask_levels_.clear();

    bool Orderbook::add_order(const string& order) {
      get_sequence_number()++;
    }
}

