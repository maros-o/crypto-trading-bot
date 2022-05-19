#include "OrderBook.h"

std::vector<Order>& OrderBook::GetAsksVector()
{
	return m_asks;
}

std::vector<Order>& OrderBook::GetBidsVector()
{
	return m_bids;
}