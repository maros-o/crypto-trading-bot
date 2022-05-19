#pragma once
#include <iostream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

#define CURL_STATICLIB
#include <curl/curl.h>

#include "OrderBook.h"
#include "TransactionHistory.h"
#include "TradingPair.h"
#include "Order.h"

const unsigned int consoleColorGreen = 10;
const unsigned int consoleColorRed = 12;
const unsigned int consoleColorDefault = 7;
const unsigned int consoleColorHeader = 15;

struct Memory
{
	char* response;
	size_t size;
};

class Exchange 
{
private:
	void CreateOrderBook(const std::string& firCurr, const std::string& secCurr);
	void CreateTransactionHistory(const std::string& firCurr, const std::string& secCurr);

protected:
	std::vector<TradingPair> m_tradingPairs;
	std::vector<OrderBook> m_orderBooks;
	std::vector<TransactionHistory> m_transactionHistories;

	static size_t cb(void* data, size_t size, size_t nmemb, void* userp);

	static std::string GetData(const char url[]);

	static void GetMentions(const std::string& source, const std::string& search, std::vector<std::string>& results,
							bool quotationMarks, const std::string& startingWord, const std::string& endingWord, const std::string& divider);

	static void PrintWithColor(const std::string& str, const unsigned int& consoleColor, const unsigned int& frontSpaces, const unsigned int& backSpaces);

	int GetOrderBookIndex(const std::string& firCurr, const std::string& secCurr);
	int GetTransactionHistoryIndex(const std::string& firCurr, const std::string& secCurr);

	virtual void UpdateTradingPairs() = 0;
	virtual void UpdateOrderBook(const size_t& index) = 0;
	virtual void UpdateTransactionHistory(const size_t& index) = 0;

public:
	void PrintTradingPairs();
	bool PrintOrderBook(const std::string& firCurr, const std::string& secCurr);
	bool PrintTransactionHistory(const std::string& firCurr, const std::string& secCurr);

	bool TradingPairExists(const std::string& firCurr, const std::string& secCurr);
	std::vector<Order> GetAsks(const std::string& firCurr, const std::string& secCurr);
	std::vector<Order> GetBids(const std::string& firCurr, const std::string& secCurr);
};