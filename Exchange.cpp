#include "Exchange.h"

size_t Exchange::cb(void* data, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	struct Memory* mem = (struct Memory*)userp;

	char* ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
	if (ptr == NULL)
		return 0;       // out of memory

	mem->response = ptr;
	memcpy(&(mem->response[mem->size]), data, realsize);
	mem->size += realsize;
	mem->response[mem->size] = 0;

	return realsize;
}

std::string Exchange::GetData(const char url[])
{
	Memory chunk = {0};
	CURL* req = curl_easy_init();
	CURLcode res;
	if (req)
	{
		curl_easy_setopt(req, CURLOPT_URL, url);
		curl_easy_setopt(req, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(req, CURLOPT_FOLLOWLOCATION, 1L);

		// for value storage
		curl_easy_setopt(req, CURLOPT_WRITEFUNCTION, cb);

		// we pass our 'chunk' struct to the callback function
		curl_easy_setopt(req, CURLOPT_WRITEDATA, (void*)&chunk);
		res = curl_easy_perform(req);
		if (res != CURLE_OK)
			std::cerr << "curl_easy_operation() failed : " << curl_easy_strerror(res) << std::endl;
	}
	curl_easy_cleanup(req);
	if (chunk.response != 0)
		return chunk.response;
	return "error";
}

void Exchange::GetMentions(const std::string& source, const std::string& search, std::vector<std::string>& results, 
								bool quotationMarks, const std::string& startingWord, const std::string& endingWord, const std::string& divider)
{
	size_t start, end;

	if (startingWord == "_none_")
		start = 0;
	else
		start = source.find(startingWord);

	if (endingWord == "_none_")
		end = source.length();
	else
		end = source.find(endingWord);

	size_t pointer = source.find(search, start) + search.length() + 2, oldPointer = 0;
	while (oldPointer < pointer && pointer < end)
	{
		std::string found = source.substr(pointer, source.find(divider, pointer) - pointer);

		if (quotationMarks)
			results.push_back(found.substr(1, found.length() - 2));
		else
			results.push_back(found);

		oldPointer = pointer;
		pointer = source.find(search, pointer + 1) + search.length() + 2;
	}
}

void Exchange::PrintWithColor(const std::string& str, const unsigned int& consoleColor, const unsigned int& frontSpaces, const unsigned int& backSpaces)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), consoleColor);

	for (unsigned int i = 0; i < frontSpaces; i++)
		std::cout << std::endl;

	std::cout << str;

	for (unsigned int i = 0; i < backSpaces; i++)
		std::cout << std::endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), consoleColorDefault);
}

bool Exchange::TradingPairExists(const std::string& firCurr, const std::string& secCurr)
{
	for (int i = 0; i < m_tradingPairs.size(); i++)
	{
		if (firCurr == m_tradingPairs[i].GetFirstCurrency() && secCurr == m_tradingPairs[i].GetSecondCurrency())
			return true;
	}
	return false;
}

void Exchange::CreateOrderBook(const std::string& firCurr, const std::string& secCurr)
{
	m_orderBooks.push_back(OrderBook(firCurr, secCurr));
}

void Exchange::CreateTransactionHistory(const std::string& firCurr, const std::string& secCurr)
{
	m_transactionHistories.push_back(TransactionHistory(firCurr, secCurr));
}

int Exchange::GetOrderBookIndex(const std::string& firCurr, const std::string& secCurr)
{
	if (!TradingPairExists(firCurr, secCurr))
		return -1;

	for (int i = 0; i < m_orderBooks.size(); i++)
	{
		if (firCurr == m_orderBooks[i].GetFirstCurrency() && secCurr == m_orderBooks[i].GetSecondCurrency())
			return i;
	}

	CreateOrderBook(firCurr, secCurr);
	return (int)m_orderBooks.size() - 1;
}

std::vector<Order> Exchange::GetAsks(const std::string& firCurr, const std::string& secCurr)
{
	int index = GetOrderBookIndex(firCurr, secCurr);
	UpdateOrderBook(index);
	return m_orderBooks[index].GetAsksVector();;
}

std::vector<Order> Exchange::GetBids(const std::string& firCurr, const std::string& secCurr)
{
	int index = GetOrderBookIndex(firCurr, secCurr);
	UpdateOrderBook(index);
	return m_orderBooks[index].GetBidsVector();;
}

int Exchange::GetTransactionHistoryIndex(const std::string& firCurr, const std::string& secCurr) 
{
	if (!TradingPairExists(firCurr, secCurr))
		return -1;

	for (int i = 0; i < m_transactionHistories.size(); i++)
	{
		if (firCurr == m_transactionHistories[i].GetFirstCurrency() && secCurr == m_transactionHistories[i].GetSecondCurrency())
			return i;
	}

	CreateTransactionHistory(firCurr, secCurr);
	return (int)m_transactionHistories.size() - 1;
}

void Exchange::PrintTradingPairs()
{
	UpdateTradingPairs();

	PrintWithColor("Available Trading Pairs:", consoleColorHeader, 1, 2);
	for (int i = 0; i < m_tradingPairs.size(); i++)
	{
		std::cout << " " << m_tradingPairs[i].GetFirstCurrency() << " - " << m_tradingPairs[i].GetSecondCurrency() << std::endl;
	}
}

bool Exchange::PrintOrderBook(const std::string& firCurr, const std::string& secCurr)
{
	size_t index = (size_t)GetOrderBookIndex(firCurr, secCurr);
	if (index == -1)
		return false;

	UpdateOrderBook(index);

	PrintWithColor("Order Book of " + m_orderBooks[index].GetFirstCurrency() + "_" + m_orderBooks[index].GetSecondCurrency() + ":", consoleColorHeader, 1, 1);

	PrintWithColor(" Asks:", consoleColorRed, 1, 2);

	for (int j = 0; j < m_orderBooks[index].GetAsksVector().size(); j++)
	{
		std::cout << "  price: ";

		PrintWithColor(std::to_string(m_orderBooks[index].GetAsksVector()[j].price), consoleColorRed, 0, 0);

		std::cout << " " << m_orderBooks[index].GetSecondCurrency() << std::endl << "  amount: " << m_orderBooks[index].GetAsksVector()[j].amount << " " << m_orderBooks[index].GetFirstCurrency() << std::endl << std::endl;
	}

	PrintWithColor("Bids:", consoleColorGreen, 0, 2);

	for (int j = 0; j < m_orderBooks[index].GetBidsVector().size(); j++)
	{
		std::cout << "  price: ";

		PrintWithColor(std::to_string(m_orderBooks[index].GetBidsVector()[j].price), consoleColorGreen, 0, 0);

		std::cout << " " << m_orderBooks[index].GetSecondCurrency() << std::endl << "  amount: " << m_orderBooks[index].GetBidsVector()[j].amount << " " << m_orderBooks[index].GetFirstCurrency() << std::endl << std::endl;
	}
	return true;
}

bool Exchange::PrintTransactionHistory(const std::string& firCurr, const std::string& secCurr)
{
	size_t index = (size_t)GetTransactionHistoryIndex(firCurr, secCurr);
	if (index == -1)
		return false;

	UpdateTransactionHistory(index);

	PrintWithColor("Transaction History of " + m_transactionHistories[index].GetFirstCurrency(), consoleColorHeader, 1, 1);

	std::cout.precision(10);

	for (int j = 0; j < m_transactionHistories[index].GetTransactionVector().size(); j++)
	{
		std::cout << std::endl << " Transaction [" << j << "]" << std::endl;
		std::cout << "  price: " << m_transactionHistories[index].GetTransactionVector()[j].price << " " << m_transactionHistories[index].GetSecondCurrency() << std::endl;
		std::cout << "  amount: " << m_transactionHistories[index].GetTransactionVector()[j].amount << " " << m_transactionHistories[index].GetFirstCurrency() << std::endl;
		std::cout << "  tradeType: ";

		if (m_transactionHistories[index].GetTransactionVector()[j].tradeType == "BUY")
			PrintWithColor(m_transactionHistories[index].GetTransactionVector()[j].tradeType, consoleColorGreen, 0, 1);
		else
			PrintWithColor(m_transactionHistories[index].GetTransactionVector()[j].tradeType, consoleColorRed, 0, 1);
	}
	return true;
}