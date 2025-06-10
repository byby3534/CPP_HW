#include <iostream>
#include "SimpleVector.h"

using namespace std;

int main()
{
    int num;
    SimpleVector<int> sv;
    
    while (cin >> num)
    {
        sv.push_back(num);
        cout << sv.size() << endl;
        cout << sv.capacity() << endl;
    }
    sv.sortData();
    sv.print();
    return 0;
}