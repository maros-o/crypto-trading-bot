#include "Kraken.h"

Kraken::Kraken()
{
	UpdateTradingPairs();
}

void Kraken::UpdateTradingPairs()
{
	std::string source = GetData("https://api.kraken.com/0/public/AssetPairs");
	std::vector<std::string> pairs;

	GetMentions(source, "wsname", pairs, true, "_none_", "_none_", ",");

	m_tradingPairs.clear();
	for (int i = 0; i < pairs.size(); i++)
	{
		const size_t split = pairs[i].find('/');
		m_tradingPairs.push_back( TradingPair( pairs[i].substr(0, split), pairs[i].substr(split + 1, pairs[i].size()) ));
	}
}

void Kraken::UpdateOrderBook(const size_t& index)
{
	std::string str = "https://api.kraken.com/0/public/Depth?pair=";
	str += m_orderBooks[index].GetFirstCurrency() + m_orderBooks[index].GetSecondCurrency();
	const char* url = str.c_str();

	std::string source = GetData(url);
	std::vector<Order> asks, bids;
	std::vector<std::string> prices, amounts;

	GetMentionsInList(source, prices, "asks", 1);
	GetMentionsInList(source, amounts, "asks", 2);

	m_orderBooks[index].GetAsksVector().clear();
	for (int j = 0; j < prices.size(); j++)
	{
		m_orderBooks[index].GetAsksVector().push_back(Order(std::stod(prices[j]), std::stod(amounts[j])));
	}

	prices.clear();
	amounts.clear();
	GetMentionsInList(source, prices, "bids", 1);
	GetMentionsInList(source, amounts, "bids", 2);

	m_orderBooks[index].GetBidsVector().clear();
	for (int j = 0; j < prices.size(); j++)
	{
		m_orderBooks[index].GetBidsVector().push_back(Order(std::stod(prices[j]), std::stod(amounts[j])));
	}
}

void Kraken::UpdateTransactionHistory(const size_t& index)
{
	std::string str = "https://api.kraken.com/0/public/Trades?pair=";
	str += m_transactionHistories[index].GetFirstCurrency() + m_transactionHistories[index].GetSecondCurrency();
	const char* url = str.c_str();

	std::string source = GetData(url);
	std::vector<std::string> prices, amounts, tradeTypes;

	GetMentionsInList(source, prices, "[[", 1);
	GetMentionsInList(source, amounts, "[[", 2);
	GetMentionsInList(source, tradeTypes, "[[", 4);

	m_transactionHistories[index].GetTransactionVector().clear();
	for (int j = 0; j < prices.size(); j++)
	{
		std::string tradeType = "BUY";
		if (tradeTypes[j] == "s")
			tradeType = "SELL";

		m_transactionHistories[index].GetTransactionVector().push_back(Transaction(std::stod(prices[j]), std::stod(amounts[j]), tradeType));
	}
}

void Kraken::GetMentionsInList(const std::string& source, std::vector<std::string>& results,
							   const std::string& listName, const int placement)
{
	size_t start = source.find(listName);

	bool stop = false;
	size_t pointer = source.find("[[", start) - 1, oldPointer = 0;
	while (oldPointer < pointer && !stop)
	{
		std::string line = source.substr(pointer + 3, source.find("],[", pointer + 1) - (pointer + 3));

		if (source.find("]]", pointer) < source.find("],[", pointer + 1)) 
		{
			line = source.substr(pointer + 3, source.find("]],", pointer + 1) - (pointer + 3));
			stop = true;
		}

		size_t placementPointer = 0;
		for (int i = 0; i < placement - 1; i++) 
			placementPointer = line.find(",", placementPointer + 1) + 1;

		if (line.find(",", placementPointer + 1) != std::string::npos)
			line = line.substr(placementPointer, line.find(",", placementPointer + 1) - placementPointer);
		else
			line = line.substr(placementPointer, line.size());

		if (line[0] == '"')
			line = line.substr(1, line.size() - 2);

		results.push_back(line);

		oldPointer = pointer;
		pointer = source.find("],[", pointer + 1);
	}
}
