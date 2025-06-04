#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Animal.h"
using namespace std;

class Zoo {
private:
    Animal* animals[10] = { nullptr }; // 동물 객체를 저장하는 포인터 배열
    int count = 0;

public:
    // 동물을 동물원에 추가하는 함수
    // - Animal 객체의 포인터를 받아 포인터 배열에 저장합니다.
    // - 같은 동물이라도 여러 번 추가될 수 있습니다.
    // - 입력 매개변수: Animal* (추가할 동물 객체)
    // - 반환값: 없음
    void addAnimal(Animal* animal)
    {
        if (count > 9)
        {
            cout << "The zoo is full" << endl;
            return;
        }
        animals[count] = animal;
        count++;
    }

    // 동물원에 있는 모든 동물의 행동을 수행하는 함수
    // - 모든 동물 객체에 대해 순차적으로 소리를 내고 움직이는 동작을 실행합니다.
    // - 입력 매개변수: 없음
    // - 반환값: 없음
    void performActions()
    {
        for (int i = 0; i < count; i++)
        {
            animals[i]->makeSound();
        }
    }

    // Zoo 소멸자
    // - Zoo 객체가 소멸될 때, 동물 벡터에 저장된 모든 동물 객체의 메모리를 해제합니다.
    // - 메모리 누수를 방지하기 위해 동적 할당된 Animal 객체를 `delete` 합니다.
    // - 입력 매개변수: 없음
    // - 반환값: 없음
    ~Zoo()
    {
        for (int i = 0; i < count; i++)
        {
            delete animals[i];
        }
    }
};


// 랜덤 동물을 생성하는 함수
    // - 0, 1, 2 중 하나의 난수를 생성하여 각각 Dog, Cat, Cow 객체 중 하나를 동적으로 생성합니다.
    // - 생성된 객체는 Animal 타입의 포인터로 반환됩니다.
    // - 입력 매개변수: 없음
    // - 반환값: Animal* (생성된 동물 객체의 포인터)
Animal* createRandomAnimal()
{
    int randNum = rand() % 3;
    if (randNum == 0)
    {
        Animal* dog = new Dog();
        return dog;
    }
    else if (randNum == 1)
    {
        Animal* cat = new Cat();
        return cat;
    }
    else if (randNum == 2)
    {
        Animal* cow = new Cow();
        return cow;
    }
}

int main()
{
    Zoo myZoo;
    
    //
    for (int i = 0; i < 10; i++)
    {
        myZoo.addAnimal(createRandomAnimal());
    }
    myZoo.performActions();
}
