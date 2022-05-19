#include <iostream>
#include <string>

#include "Exchange.h"
#include "Coinmate.h"
#include "Kraken.h"
#include "TradeAlgorithm.h"

void StrToUpper(std::string& str) 
{
    for (size_t i = 0; i < str.length(); i++)
        str[i] = std::toupper(str[i]);
}

int PickExchange(bool err)
{
    system("CLS");
    std::cout << "Pick Crypto Exchange: " << std::endl;
    std::cout << " 1. CoinMate " << std::endl;
    std::cout << " 2. Kraken " << std::endl;

    if (err)
        std::cout << "Wrong input, try again: " << std::endl;

    int input;
    std::cin >> input;

    if (input == 1 || input == 2) 
        return input - 1;

    return PickExchange(true);
}

int PickAction(bool err)
{
    system("CLS");
    std::cout << "Pick Action: " << std::endl;
    std::cout << " 1. Show available trading pairs " << std::endl;
    std::cout << " 2. Show order book of trading pair " << std::endl;
    std::cout << " 3. Show transaction history of trading pair " << std::endl;
    std::cout << " 4. Create order " << std::endl;
    std::cout << " 5. Start algorithm " << std::endl;

    if (err)
        std::cout << "Wrong input, try again: " << std::endl;

    int input;
    std::cin >> input;

    if (input > 0 && input < 6)
        return input;

    return PickAction(true);
}

void PairsInput(std::string& firrCurr, std::string& secCurr, Exchange* exchange, bool err)
{
    std::string fir_str, sec_str;

    system("CLS");
    std::cout << "Enter existing trading pair (format: ETH EUR): " << std::endl;
    if (err)
        std::cout << "Wrong input, try again: " << std::endl;

    std::cin >> fir_str;
    std::cin >> sec_str;

    StrToUpper(fir_str);
    StrToUpper(sec_str);

    if (exchange->TradingPairExists(fir_str, sec_str)) 
    {
        firrCurr = fir_str;
        secCurr = sec_str;
        return;
    }
    PairsInput(firrCurr, secCurr, exchange, true);
}

int main()
{
    Exchange** exchanges = new Exchange * [2];
    exchanges[0] = new Coinmate(); 
    exchanges[1] = new Kraken();

    TradeAlgorithm* algo = new TradeAlgorithm;

    int currEx;
    int currAc;
    std::string firCurr, secCurr;

    while (true) 
    {
        currEx = PickExchange(false);
        currAc = PickAction(false);
        if (currAc == 1)
        {
            system("CLS");
            exchanges[currEx]->PrintTradingPairs();
        }
        else if (currAc == 5) 
        {
            algo->HandleRequests();
        }
        else
        {
            PairsInput(firCurr, secCurr, exchanges[currEx], false);

            if (currAc == 2)
                exchanges[currEx]->PrintOrderBook(firCurr, secCurr);
            else if (currAc == 3)
                exchanges[currEx]->PrintTransactionHistory(firCurr, secCurr);

            if (currAc == 4)
            {
                double price;
                double amount;
                std::string tradeTypeStr;
                TradeType tradeType;

                std::cout << "Enter price: ";
                std::cin >> price;

                std::cout << "Enter amount: ";
                std::cin >> amount;

                std::cout << "Enter buy/sell: ";
                std::cin >> tradeTypeStr;

                if (tradeTypeStr == "buy")
                    tradeType = Buy;
                else
                    tradeType = Sell;

                algo->AddTradeOrder(price, amount, tradeType, exchanges[currEx], firCurr, secCurr);
                std::cout << " Order added" << std::endl;
            }
        }
        system("pause");
    }
}