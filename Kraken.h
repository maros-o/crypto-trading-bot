#pragma once

#include "Exchange.h"

class Kraken : public Exchange
{
private:
	virtual void UpdateTradingPairs() override;
	virtual void UpdateOrderBook(const size_t& index) override;
	virtual void UpdateTransactionHistory(const size_t& index) override;

	static void GetMentionsInList(const std::string& source, std::vector<std::string>& results, 
								  const std::string& startingWord, const int placement);

public:
	Kraken();
};
