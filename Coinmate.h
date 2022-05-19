#pragma once

#include "Exchange.h"

class Coinmate : public Exchange 
{
private:
	virtual void UpdateTradingPairs() override;
	virtual void UpdateOrderBook(const size_t& index) override;
	virtual void UpdateTransactionHistory(const size_t& index) override;

public:
	Coinmate();
};
