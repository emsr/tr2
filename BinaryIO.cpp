//  http://www.cplusplus.com/forum/general/103479/
//  This has a cute screen clearing thing too!

// /home/ed/bin/bin/g++ -o BinaryIO BinaryIO.cpp

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

void clearScreen();
std::string validateEntry();
std::string decimalToBin(int number);
int binaryToDec(std::string number);

int main()
{
    clearScreen();
    std::cout << "Enter First Binary Number: ";
    std::string binaryString1 = validateEntry();
    int binaryNum1 = binaryToDec(binaryString1);
    std::cout << "Enter Second Binary Number: ";
    std::string binaryString2 = validateEntry();
    int binaryNum2 = binaryToDec(binaryString2);
    
    std::vector<int> partials(binaryString2.length(), 0);
    
    int bitShift = 0;
    int total = 0;
    for (int i = (binaryString2.length() - 1); i >= 0; i--)
    {
        if (binaryString2.compare(i, 1, "1") == 0)
        {
            partials[bitShift] = binaryNum1<<bitShift;
        }
        else
            partials[bitShift] = 0;
        
        total += partials[bitShift];
        bitShift++;
    }
    
    std::string totalStr = decimalToBin(total);
    total = totalStr.length();
    
    clearScreen();
    
    std::cout << std::fixed
              << std::setw(total) << binaryString1 << "\n"
              << std::setw(total) << binaryString2 << "\n";
    
    for (int i = 0; i < total; i++)
        std::cout << "-";
    std::cout << "\n";
    
    int zeroBuf = binaryString1.length() - 1;
    for (int i = 0; i < (binaryString2.length()); i++)
    {
        std::string tempStr = decimalToBin(partials[i]);
        if (tempStr.length() == 1)
        {
            for (int j = 0; j < zeroBuf; j++)
                tempStr += "0";
        }
        
        zeroBuf++;
        
        std::cout << std::fixed << std::setw(total)
                  << tempStr << "\n";
    }
    for (int i = 0; i < total; i++)
        std::cout << "=";
    std::cout << std::fixed << "\n"
              << std::setw(total) << totalStr << "\n\n";
}

void clearScreen()
{
    std::cout << "\033[2J\033[1;1H";
}

std::string validateEntry()
{
    bool valid = true;
    std::string tempStr;
    
    do
    {
        valid = true;
        getline(std::cin, tempStr);
        for (int i = 0; i < tempStr.length(); i++)
        {
            if ((tempStr.compare(i, 1, "0") != 0) && (tempStr.compare(i, 1, "1") != 0))
            {
                valid = false;
                clearScreen();
                std::cout << "Enter Valid Binary Number: ";
                break;
            }
        }
    } while (valid == false);
    
    clearScreen();
    return tempStr;
}

std::string decimalToBin(int number)
{
    if ( number == 0 ) return "0";
    if ( number == 1 ) return "1";
    
    if ( number % 2 == 0 )
        return decimalToBin(number / 2) + "0";
    else
        return decimalToBin(number / 2) + "1";
}

int binaryToDec(std::string number)
{
    int result = 0, pow = 1;
    for ( int i = number.length() - 1; i >= 0; --i, pow <<= 1 )
        result += (number[i] - '0') * pow;
    
    return result;
}
