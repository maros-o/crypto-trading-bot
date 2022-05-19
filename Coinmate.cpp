#include "Coinmate.h"

Coinmate::Coinmate() 
{
	UpdateTradingPairs();
}

void Coinmate::UpdateTradingPairs()
{
	std::string source = GetData("https://coinmate.io/api/tradingPairs");
	std::vector<std::string> firsts, seconds;

	GetMentions(source, "firstCurrency", firsts, true, "_none_", "_none_", ",");
	GetMentions(source, "secondCurrency", seconds, true, "_none_", "_none_", ",");

	m_tradingPairs.clear();
	for (int i = 0; i < firsts.size(); i++)
	{
		m_tradingPairs.push_back(TradingPair(firsts[i], seconds[i]));
	}
}

void Coinmate::UpdateOrderBook(const size_t& index)
{
	std::string str = "https://coinmate.io/api/orderBook?currencyPair=";
	str += m_orderBooks[index].GetFirstCurrency() + "_" + m_orderBooks[index].GetSecondCurrency() + "&groupByPriceLimit=True";
	const char* url = str.c_str();

	std::string source = GetData(url);
	std::vector<Order> asks, bids;
	std::vector<std::string> prices, amounts;

	GetMentions(source, "price", prices, false, "asks", "bids", ",");
	GetMentions(source, "amount", amounts, false, "asks", "bids", ",");

	m_orderBooks[index].GetAsksVector().clear();
	for (int j = 0; j < prices.size(); j++)
	{
		m_orderBooks[index].GetAsksVector().push_back(Order(std::stod(prices[j]), std::stod(amounts[j])));
	}

	prices.clear();
	amounts.clear();
	GetMentions(source, "price", prices, false, "bids", "_none_", ",");
	GetMentions(source, "amount", amounts, false, "bids", "_none_", ",");

	m_orderBooks[index].GetBidsVector().clear();
	for (int j = 0; j < prices.size(); j++)
	{
		m_orderBooks[index].GetBidsVector().push_back(Order(std::stod(prices[j]), std::stod(amounts[j])));
	}
}

void Coinmate::UpdateTransactionHistory(const size_t& index)
{
	std::string str = "https://coinmate.io/api/transactions?minutesIntoHistory=9999999&currencyPair=";
	str += m_transactionHistories[index].GetFirstCurrency() + "_" + m_transactionHistories[index].GetSecondCurrency();
	const char* url = str.c_str();

	std::string source = GetData(url);
	std::vector<std::string> prices, amounts, tradeTypes;

	GetMentions(source, "price", prices, false, "_none_", "_none_", ",");
	GetMentions(source, "amount", amounts, false, "_none_", "_none_", ",");
	GetMentions(source, "tradeType", tradeTypes, true, "_none_", "_none_", "}");

	if (prices.size() == 0)
		return;

	m_transactionHistories[index].GetTransactionVector().clear();
	for (int j = 0; j < prices.size(); j++)
	{
		m_transactionHistories[index].GetTransactionVector().push_back(Transaction(std::stod(prices[j]), std::stod(amounts[j]), tradeTypes[j]));
	}
}