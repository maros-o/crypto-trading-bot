#pragma once

#include <iostream>

class TradingPair
{
private:
	const std::string m_firstCurrency;
	const std::string m_secondCurrency;

public:
	TradingPair(std::string f, std::string s)
		: m_firstCurrency(f), m_secondCurrency(s) {}

	std::string GetFirstCurrency();
	std::string GetSecondCurrency();
};