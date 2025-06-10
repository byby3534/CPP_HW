#pragma once
#include <iostream>
#include <algorithm>

using namespace std;

template <typename T>
class SimpleVector
{
private:
	T* data;
	int currentSize;
	int currentCapacity;

	// 함수 내부에서만 사용할 수 있도록 private에 생성
	void resize(int newCapacity)
	{
		currentCapacity = newCapacity; // 새로운 용량으로 초기화

		// 깊은 복사로 tempData 동적할당 메모리 생성
		T* tempData = new T[currentSize]; // 새로운 메모리 공간 생성 후 깊은 복사 (생성 시 현재 데이터 크기만큼 생성 낭비 줄임)

		for (int i = 0; i < currentSize; i++)
		{
			tempData[i] = data[i]; // tempData에 이전 데이터를 저장
		}

		delete[] data; // 이전 데이터를 저장한뒤 메모리에서 제거


		data = new T[currentCapacity]; // 새로운 용량의 메모리공간 생성

		for (int i = 0; i < currentSize; i++) // 이전 데이터의 크기만큼만 반복하여 새로운 메모리 공간에 대입
		{
			data[i] = tempData[i];
		}

		delete[] tempData; // 임시저장공간 메모리 해제
	}

public:
	SimpleVector() : currentCapacity(10), currentSize(0)
	{
		data = new T[currentCapacity];
	}

	SimpleVector(int capacity) : currentCapacity(capacity), currentSize(0)
	{
		data = new T[currentCapacity];
	}

	~SimpleVector()
	{
		delete[] data;
		cout << "벡터 메모리 해제 성공!";
	}

	void push_back(const T& value)
	{
		if (currentCapacity > currentSize) // FULL이 아닐때만 푸쉬
		{
			data[currentSize++] = value;
		}
		else // 이미 가득 찼다면 resize멤버 함수로 벡터크기 재설정
		{
			resize(currentCapacity + 5); // 5씩 커짐
			data[currentSize++] = value;
		}
	}

	void pop_back()
	{
		if (currentSize > 0)
		{
			data[--currentSize] = T();
		}
	}

	void sortData()
	{
		sort(data, data + currentSize); // 정렬 알고리즘 사용 기본값 내림차순
	}

	void print()
	{
		for (int i = 0; i < currentSize; i++)
		{
			cout << data[i] << " ";
		}
	}

	int size() { return currentSize; }

	int capacity() { return currentCapacity; }
};