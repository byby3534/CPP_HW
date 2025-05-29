#include <iostream>
using namespace std;

class Phone
{
public:
    virtual void displayBrand() = 0;
    virtual void showFeature() = 0;
    virtual ~Phone() {}
};

class Samsung : public Phone
{
public:
    void displayBrand()
    {
        cout << "Samsung" << endl;
    }
    
    void showFeature()
    {
        cout << "Galaxy S 시리즈" << endl;
    }
};

class Apple : public Phone
{
public:
    void displayBrand()
    {
        cout << "Apple" << endl;
    }

    void showFeature()
    {
        cout << "iPhone Pro 시리즈" << endl;
    }
};

// Xiaomi 클래스 작성 (Phone 클래스를 상속받음)
class Xiaomi : public Phone {
public:
    void displayBrand() 
    {
        cout << "Xiaomi" << endl;
    }

    void showFeature() 
    {
        cout << "Redmi Note 시리즈" << endl;
    }
};

int main()
{
    Phone* phones[3];
    phones[0] = new Samsung();
    phones[1] = new Apple();
    phones[2] = new Xiaomi();

    for (int i = 0; i < 3; i++)
    {
        phones[i]->displayBrand();
        phones[i]->showFeature();
    }

    for (int i = 0; i < 3; i++)
    {
        delete phones[i];
    }
    return 0;
}