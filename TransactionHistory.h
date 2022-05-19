#pragma once

#include <iostream>
#include <vector>

#include "TradingPair.h"

struct Transaction 
{
	const double price;
	const double amount;
	const std::string tradeType;

	Transaction(double p, double a, std::string t)
		: price(p), amount(a), tradeType(t) {}
};

class TransactionHistory : public TradingPair
{
private:
	std::vector<Transaction> m_transactions;

public:
	TransactionHistory(std::string f, std::string s) 
		: TradingPair(f, s) {}

	std::vector<Transaction>& GetTransactionVector();
};