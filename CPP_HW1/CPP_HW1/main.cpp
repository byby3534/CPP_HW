#include <iostream>
#include <vector>

using namespace std;

enum SortOrder{ asc = 1, desc };

int Sum(vector<int> v)
{
	int result = 0;
	for (int i = 0; i < v.size(); i++)
	{
		result += v[i];
	}

	return result;
}

float Avg(vector<int> v)
{
	return (float)Sum(v) / v.size();
}

void Swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

// 버블 정렬 알고리즘 : 두 값을 비교해 큰 값을 뒤로 보내는 방식 / 시간복잡도 O(n^2)
// 1은 올림차순 2는 내림차순
void bSort(vector<int>& arr, SortOrder order = asc)
{
	for (int i = 0; i < arr.size() - 1; i++)
	{
		for (int j = 0; j < arr.size() - i - 1; j++)
		{
			if ((arr[j] > arr[j + 1]) && order == asc)	// x > y -> 오름차순일 경우 Swap
			{
				Swap(arr[j], arr[j + 1]);
			}
			else if(arr[j] < arr[j + 1] && order == desc) // x < y -> 내림차순일 경우 Swap
			{
				Swap(arr[j], arr[j + 1]);
			}
		}
	}
}

int main()
{
	// 1-1 : 배열의 합 배열의 평균 함수
	vector<int> v;
	int n, num;
	cin >> n; // 몇 번 입력할 건지 선택
	for (int i = 0; i < n; i++)
	{
		cin >> num;
		v.push_back(num);
	}

	cout << Avg(v) << endl;

	// 1-2 : 정렬 알고리즘(버블 정렬 알고리즘 : 두 값을 비교해 큰 값을 뒤로 보내는 방식 시간복잡도(O(n^2))
	
	bSort(v, desc);

	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << " ";
	}
	return 0;
}