#include <iostream>
#include "stock20.h"
// constructors
Stock::Stock() // default constructor
{
    company = new char[8];
    company[0] = 'n';
    company[1] = 'o';
    company[2] = ' ';
    company[3] = 'n';
    company[4] = 'a';
    company[5] = 'm';
    company[6] = 'e';
    company[7] = '\0';
    shares = 0;
    share_val = 0.0;
    total_val = 0.0;
}

Stock::Stock(const std::string & co, long n, double pr)
{
    company = new char[co.size() + 1];
    for (int i = 0; i < co.size();i++) {
        company[i] = co[i];
    }
    company[co.size()]='\0';
    if (n < 0)
    {
        std::cout << "Number of shares can't be negative; "
        << company << " shares set to 0.\n";
        shares = 0;
    }
    else
        shares = n;
    
    share_val = pr;
    set_tot();
}

// class destructor
Stock::~Stock() // quiet class destructor
{
    delete[] company;
}

// other methods
void Stock::buy(long num, double price)
{
    if (num < 0)
    {
        std::cout << "Number of shares purchased can't be negative. "
        << "Transaction is aborted.\n";
    }
    else
    {
        shares += num;
        share_val = price;
        set_tot();
    }
}

void Stock::sell(long num, double price)
{
    using std::cout;
    if (num < 0)
    {
        cout << "Number of shares sold can't be negative. "
        << "Transaction is aborted.\n";
    }
    else if (num > shares)
    {
        cout << "You can't sell more than you have!' "
        << "Transaction is aborted.\n";
    }
    else
    {
        shares -= num;
        share_val = price;
        set_tot();
    }
}

void Stock::update(double price)
{
    share_val = price;
    set_tot();
}
std::ostream& operator<<(std::ostream& os, const Stock& t) {
    using std::cout;
    using std::ios_base;
    // set format to #.###
    ios_base::fmtflags orig =
        os.setf(ios_base::fixed, ios_base::floatfield);
    std::streamsize prec = os.precision(3);
    os << "Company: "; //<< t.company;
    for (int i = 0; i < strlen(t.company); i++) {
        os << t.company[i];
    }
    os << " Shares: " << t.shares << '\n';
    os << " Share Price: $" << t.share_val;
    
    // set format to #.##
    os.precision(2);
    os << " Total Worth: $" << t.total_val << '\n';
    // restore original format
    os.setf(orig, ios_base::floatfield);
    os.precision(prec);
    return os;
}

const Stock & Stock::topval(const Stock & s) const
{
    if (s.total_val > total_val)
        return s;
    else
        return *this;
}
