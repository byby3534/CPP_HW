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

	// �Լ� ���ο����� ����� �� �ֵ��� private�� ����
	void resize(int newCapacity)
	{
		currentCapacity = newCapacity; // ���ο� �뷮���� �ʱ�ȭ

		// ���� ����� tempData �����Ҵ� �޸� ����
		T* tempData = new T[currentSize]; // ���ο� �޸� ���� ���� �� ���� ���� (���� �� ���� ������ ũ�⸸ŭ ���� ���� ����)

		for (int i = 0; i < currentSize; i++)
		{
			tempData[i] = data[i]; // tempData�� ���� �����͸� ����
		}

		delete[] data; // ���� �����͸� �����ѵ� �޸𸮿��� ����


		data = new T[currentCapacity]; // ���ο� �뷮�� �޸𸮰��� ����

		for (int i = 0; i < currentSize; i++) // ���� �������� ũ�⸸ŭ�� �ݺ��Ͽ� ���ο� �޸� ������ ����
		{
			data[i] = tempData[i];
		}

		delete[] tempData; // �ӽ�������� �޸� ����
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
		cout << "���� �޸� ���� ����!";
	}

	void push_back(const T& value)
	{
		if (currentCapacity > currentSize) // FULL�� �ƴҶ��� Ǫ��
		{
			data[currentSize++] = value;
		}
		else // �̹� ���� á�ٸ� resize��� �Լ��� ����ũ�� �缳��
		{
			resize(currentCapacity + 5); // 5�� Ŀ��
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
		sort(data, data + currentSize); // ���� �˰��� ��� �⺻�� ��������
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