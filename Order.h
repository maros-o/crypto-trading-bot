#pragma once

struct Order
{
	const double price;
	const double amount;

	Order(double p, double a)
		: price(p), amount(a) {}
};