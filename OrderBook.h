#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "TradingPair.h"
#include "Order.h"

class OrderBook : public TradingPair
{
private:
	std::vector<Order> m_asks;
	std::vector<Order> m_bids;

public:
	OrderBook(std::string f, std::string s)
		: TradingPair(f, s) {}

	std::vector<Order>& GetAsksVector();
	std::vector<Order>& GetBidsVector();
};