#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

#pragma region 상수값
#define BROKENBUTTON 10
#define CURRENTNUMBER 100
#define MAXNUMBERLENGTH 6
#pragma endregion

using namespace std;

int GetMinNumber(int minNum, int brokenNum[]);
int GetMaxNumber(int maxNum, int brokenNum[]);
//��ȣ ũ�� ���ϴ� �Լ�
int GetDigitCount(int number);
//+/-��ư���θ� �̵��� �������� �Ǵ�
bool DecideOnlyButton(int targetNum);
//������ �ڸ� �� ����
template <typename T>
vector<T> GetDigits(T number);
//��ȣ�� ���峭 ��ư���� üũ
bool CheckBrokenNumber(int number, int brokenNum[]);
//��ǥ ä�� ���� ���峭 ��ư�� �ִ��� Ȯ��
bool *CheckTargetNumber(int targetNum, int brokenNum[]);
int VariationNumber(int number, int brokenNum[], int index);
int FindNumber(int number, int brokenNum[]);
//
vector<int> FindOptimumNumber(int targetNum, int brokenNum[], bool* brokenNumPos, int minNum, int maxNum);

int main() {
	vector<int> result;

	int targetNum;
	int brokenCount; 
	int brokenNum[BROKENBUTTON];

	cin >> targetNum;
	cin >> brokenCount;

	//�μ��� ���ڹ�ư
	for (int index = 0; index < brokenCount; index++) {
		cin >> brokenNum[index];
	}

	int minAbleNum = 0;
	int maxAbleNum = 9;
	minAbleNum = GetMinNumber(minAbleNum, brokenNum);
	maxAbleNum = GetMaxNumber(maxAbleNum, brokenNum);

	// +/-��ư���θ� �̵��� �������� �Ǵ�
	if (DecideOnlyButton(targetNum) || brokenCount >= 10) {
		cout << abs(targetNum - CURRENTNUMBER) << endl;
	}
	else {
		//�̵��Ϸ��� �ϴ� ä�κ��� ū/���� ���ڸ� �Է��ؾ��ϴ��� �Ǵ�
		bool* brokenNumberPosition = CheckTargetNumber(targetNum, brokenNum);

		//üũ�� ���ڰ� (Ŀ����/�۾�����) �� ���� ���ڴ� ���� (���� ��/ū ��)�� ������ ��
		//특정 위치의 숫자가 (커지면/작아지면) 다음 숫자들은 (가장 작은/가장 큰) 숫자가 되어야 한다.
		result = FindOptimumNumber(targetNum, brokenNum, brokenNumberPosition, minAbleNum, maxAbleNum);
	}

	for(int i = 0;i<result.size();i++){
		cout << result[i];
	}
	cout << "\n";

	return 0;
}

int GetMinNumber(int minNum, int brokenNum[]){
	int index = 0;
	while(true){
		if(minNum != brokenNum[index])
			return minNum;

		minNum += 1;
		index += 1;
	}
}
int GetMaxNumber(int maxNum, int brokenNum[]){
	int index = sizeof(brokenNum) / sizeof(*brokenNum) - 1;
	while(true){
		if(maxNum != brokenNum[index])
			return maxNum;

		maxNum -= 1;
		index -= 1;
	}
}
///<summary> ������ �ڸ����� ���ϴ� �Լ� </summary>
int GetDigitCount(int number) {
	if (number == 0) return 1;
	return static_cast<int>(log10(abs(number)) + 1);
}
/// <summary>+/-��ư���θ� �̵��� �������� �Ǵ�</summary>
bool DecideOnlyButton(int targetNum) {
	int numLen = GetDigitCount(targetNum);
	int subNum = abs(targetNum - CURRENTNUMBER);

	if (subNum <= numLen)
		return true;
	
	return false;
}
///<summary>������ �ڸ� �� ����</summary>
template <typename T>
vector<T> GetDigits(T number) {
	vector<T> digits;

	while (number > 0) {
		digits.push_back(number % 10);
		number /= 10;
	}

	reverse(digits.begin(), digits.end());

	return digits;
}
/// <summary>��ȣ�� ���峭 ��ư���� üũ</summary>
bool CheckBrokenNumber(int number, int brokenNum[]) {
	int brokenNumberLength = sizeof(brokenNum) / sizeof(*brokenNum);

	for (int i = 0; i < brokenNumberLength; i++) {
		if (number == brokenNum[i]) {
			return true;
		}
	}
	return false;
}
/// <summary>��ǥ ä�ο� ���峭 ��ư�� ���ڰ� �����ϴ��� üũ</summary>
bool *CheckTargetNumber(int targetNum, int brokenNum[]) {
	vector<int> targetNumber = GetDigits<int>(targetNum);
	//int targetNumberLength = GetDigitCount(targetNum);
	int targetNumberLength = targetNumber.size();
	int brokenNumberLength = sizeof(brokenNum) / sizeof(*brokenNum);

	bool brokenNumberPosition[MAXNUMBERLENGTH];

	for (int i = 0; i < targetNumberLength; i++) {
		brokenNumberPosition[i] = CheckBrokenNumber(targetNumber[i], brokenNum);
	}

	return brokenNumberPosition;
}
int VariationNumber(int number, int brokenNum[], int index){
	int result = number;

	if(CheckBrokenNumber(result + index, brokenNum)){
		result = FindNumber(result + index, brokenNum);
		return result;
	}

	return result;
}
int FindNumber(int number, int brokenNum[]) {
	int maxNumber = VariationNumber(number, brokenNum, 1);
	int minNumber = VariationNumber(number, brokenNum, -1);

	if (abs(number - maxNumber) > abs(number - minNumber)) {
		return minNumber;
	}
	return maxNumber;
}
vector<int> FindOptimumNumber(int targetNum, int brokenNum[], bool* brokenNumPos, int minNum, int maxNum){
	vector<int> targetNumber = GetDigits<int>(targetNum);
	vector<int> resultNumber;
	int targetNumberLength = targetNumber.size();
	bool isBroken = false;
	int isUpper = 0;

	for(int i = 0; i < targetNumberLength; i++){
		if (isBroken) {
			if(isUpper == 1)
				resultNumber.push_back(minNum);
			else
				resultNumber.push_back(maxNum);
			continue;
		}

		if(!brokenNumPos[i]){
			resultNumber.push_back(targetNumber[i]);
			continue;
		}
		
		int number = FindNumber(targetNumber[i], brokenNum);
		resultNumber.push_back(number);
		isBroken = true;
		
		if(targetNumber[i] > number)
			isUpper = -1;
		else
			isUpper = 1;
	}

	return resultNumber;
}