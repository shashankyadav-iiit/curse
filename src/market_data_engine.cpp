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

