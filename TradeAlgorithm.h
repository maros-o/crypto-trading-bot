#pragma once

#include <chrono>
#include <thread>

#include <iostream>
#include <vector>

#include "TradingPair.h"
#include "Exchange.h"
#include "Order.h"

enum TradeType : bool { Buy = true, Sell = false };

struct TradeOrder : public TradingPair
{
	double price;
	double amount;
	TradeType tradeType;
	bool filled;
	Exchange* exchange;

	TradeOrder(double p, double a, TradeType t, Exchange* ex, std::string fir, std::string sec)
		: price(p), amount(a), tradeType(t), filled(false), exchange(ex), TradingPair(fir, sec) {}
};

class TradeAlgorithm
{
private:
	std::vector<TradeOrder> m_tradeOrders;
	int m_printCounter;

public:
	TradeAlgorithm() : m_printCounter(20) {}

	void AddTradeOrder(double p, double a, TradeType t, Exchange* ex, std::string fir, std::string sec);
	void HandleRequests();
};
