#include "TradeAlgorithm.h"

void TradeAlgorithm::AddTradeOrder(double p, double a, TradeType t, Exchange* ex, std::string fir, std::string sec)
{
	m_tradeOrders.push_back(TradeOrder(p, a, t, ex, fir, sec));
}
void TradeAlgorithm::HandleRequests()
{
	bool print = false;

	m_printCounter++;
	if (m_printCounter > 5)
	{
		system("CLS");
		print = true;
		m_printCounter = 0;
	}

	for (int i = 0; i < m_tradeOrders.size(); i++)
	{
		if (print)
		{
			std::cout << i + 1 << ". limit price: " << m_tradeOrders[i].price << " " << m_tradeOrders[i].GetSecondCurrency() << ", amount: " << m_tradeOrders[i].amount << " " << m_tradeOrders[i].GetFirstCurrency() << ", tradeType: ";
			if (m_tradeOrders[i].tradeType == Buy)
				std::cout << "buy";
			else
				std::cout << "sell";

			std::cout << ", status: ";
			if (m_tradeOrders[i].filled)
				std::cout << "filled";
			else
				std::cout << "open";
		}

		if (m_tradeOrders[i].tradeType == Buy)
		{
			if (print)
				std::cout << ", (current top buy price: " << m_tradeOrders[i].exchange->GetBids(m_tradeOrders[i].GetFirstCurrency(), m_tradeOrders[i].GetSecondCurrency())[0].price << ")" << std::endl;
			if (m_tradeOrders[i].price >= m_tradeOrders[i].exchange->GetBids(m_tradeOrders[i].GetFirstCurrency(), m_tradeOrders[i].GetSecondCurrency())[0].price)
			{
				m_tradeOrders[i].filled = true;
			}
		}
		else
		{
			if (print)
				std::cout << ", (current top sell price: " << m_tradeOrders[i].exchange->GetAsks(m_tradeOrders[i].GetFirstCurrency(), m_tradeOrders[i].GetSecondCurrency())[0].price << ")" << std::endl;
			if (m_tradeOrders[i].price <= m_tradeOrders[i].exchange->GetAsks(m_tradeOrders[i].GetFirstCurrency(), m_tradeOrders[i].GetSecondCurrency())[0].price)
			{
				m_tradeOrders[i].filled = true;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	HandleRequests();
}