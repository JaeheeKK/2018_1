#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

void main() {
	int var, minterm, temp1, temp2;
	int position, memory; // 다른 자리수의 자리가 어딘지 기억, 원래 값 기억.
	cin >> var >> minterm;

	vector <string> result;
	deque<int> combineminterm; // combine될때 minterm을 넣어놓을 temp
	deque<deque<int>> epiminterm(minterm);//minterm 수.
	deque<deque<int>> realminterm;//real minterm 수.
	deque<deque<int>> realepi; // 진짜 epi
	deque<deque<int>> nepi(minterm, deque<int>(var));//minterm을 2진수로 표현
	deque<deque<int>>::iterator overlap; // 겹치는 epi가 없는지 체크.
	deque<deque<int>> Isepi;//epi최종 후보.
	deque<int> CountOnes; // 각 variable의 1의 개수
	deque<int> ForCompares; // compare하기 위한 식의 개수를 구해주기 위해 1의 개수 세기

	char *tempchar = new char[var + 1];
	int howmanyminterm[100] = { 0, };
	int *vars = new int[minterm]; // variable
	int *temp = new int[var + 1]; // 1의 개수 대로 2진수 정렬 하기 위한 임시 배열.
	int *ForCompare = new int[var + 1]; // compare하기 위한 식의 개수를 구해주기 위해 1의 개수 세기
	int *CountOne = new int[minterm];// 1의 개수

	memset(ForCompare, 0, sizeof(int)*(var + 1));
	memset(CountOne, 0, sizeof(int)*(minterm));
	tempchar[var] = '\0';

	for (int i = 0; i < minterm; i++) {
		cin >> vars[i];
	}

	for (int i = 0; i < minterm; i++) { // minterm을 2진수로 표현
		int variable = vars[i];
		for (int j = var - 1; j >= 0; j--) {
			nepi[i][j] = variable % 2;
			if (nepi[i][j] == 1) { // 2진수로 표현했을 때 1의 개수 count.
				CountOne[i]++;
			}
			variable /= 2;
			if (variable == 0) { // 더이상 나눌 것이 없을 경우 상위비트 모두 0으로.
				while (j > 0) {
					j--;
					nepi[i][j] = 0;
				}
			}
		}
	}

	for (int i = 0; i<minterm; i++) {
		for (int j = i + 1; j < minterm; j++) {
			if (CountOne[i] > CountOne[j]) {
				temp1 = CountOne[i]; // 1의 개수대로 정렬
				CountOne[i] = CountOne[j];
				CountOne[j] = temp1;
				temp2 = vars[i]; // 1의 개수대로 variable 정렬
				vars[i] = vars[j];
				vars[j] = temp2;
				nepi[i].swap(nepi[j]);
			}
		}
	}
	for (int i = 0; i < minterm; i++) {
		CountOnes.push_back(CountOne[i]);
		epiminterm[i].push_back(vars[i]);
	}

	for (int i = 0; i < minterm; i++) { // 1의 개수의 개수를 세서 비교할 식의 개수를 구함.
		ForCompare[CountOne[i]]++;
	}
	for (int i = 0; i <= var; i++) {
		if (ForCompare[i] != 0) {
			ForCompares.push_back(ForCompare[i]);
		}
	}
	while (nepi.size() != 0) {
		bool *combine = new bool[nepi.size()]; //combine이 되었는지 check하는 행렬
		memset(combine, 0, sizeof(bool)*(nepi.size()));

		int lastcompare = 0;
		int RememberCompareSize = ForCompares.size() - 1;
		int RemembernepiSize = nepi.size();
		int combinecount = -1;
		int combinecount2 = 0;

		for (int i = 0; i < ForCompares.size(); i++) {
			lastcompare++;
			if (ForCompares.size() == 1) {
				for (int n = 0; n < nepi.size(); n++) {
					Isepi.push_back(nepi[n]);
					realminterm.push_back(epiminterm[n]);
					nepi.pop_front();
				}
				break;
			}
			combinecount2 += ForCompares[0];

			int compares = 0;//1의 개수가 같은 것끼리 몇개인지 계산(식의 개수 계산)
			for (int j = 0; j < ForCompares[0]; j++) {
				int combinecount3 = 0;
				combinecount++;
				for (int k = ForCompares[0]; k < ForCompares[0] + ForCompares[1]; k++) {
					int check = 0; // 다른 자리수의 개수 check
					for (int m = 0; m < var; m++) {
						if (nepi[j][m] != nepi[k][m]) {
							check++;
							position = m;
							memory = nepi[j][m];
						}
					}
					if (check == 1) {
						nepi[j][position] = 2;
						compares++;
						overlap = find(nepi.end() - compares, nepi.end(), nepi[j]);
						if (overlap != nepi.end()) {
							compares -= 1;
						}
						else {
							nepi.push_back(nepi[j]);
						}
						nepi[j][position] = memory;
						combine[combinecount] = true;
						combine[combinecount2 + combinecount3] = true;
						combineminterm = epiminterm[combinecount];
						combineminterm.insert(combineminterm.end(), epiminterm[combinecount2 + combinecount3].begin(), epiminterm[combinecount2 + combinecount3].end());
						epiminterm.push_back(combineminterm);
					}
					combinecount3++;
				}
				if (combine[combinecount] == false) {
					Isepi.push_back(nepi[j]);
					realminterm.push_back(epiminterm[combinecount]);
				}
			}
			for (int h = 0; h < ForCompares[0]; h++) {
				nepi.pop_front();
			}
			ForCompares.pop_front();

			if (compares > 0) {
				ForCompares.push_back(compares);
			}
			if (lastcompare == RememberCompareSize) {// 비교를 끝마치고 마지막 epi검사.
				for (int f = ForCompares[0]; f >= 1; f--) {
					if (combine[RemembernepiSize - f] == false) {
						Isepi.push_back(nepi[0]);
						realminterm.push_back(epiminterm[epiminterm.size() - f]);
					}
					nepi.pop_front();
				}
				ForCompares.pop_front();
			}
		}
		for (int i = 0; i < RemembernepiSize; i++) {
			epiminterm.pop_front();
		}
	}
	for (int i = 0; i < realminterm.size(); i++) {
		for (int j = 0; j < realminterm[i].size(); j++) {
			howmanyminterm[realminterm[i][j]]++; //minterm의 개수 만큼 찍어줌.
		}
	}

	deque<int> Sortepi;//epi 크기 순대로 정렬.
	for (int i = 0; i < Isepi.size(); i++) {
		int ten = 1;
		int numb = 0;
		for (int j = var - 1; j >= 0; j--) {
			numb += (ten * Isepi[i][j]);
			ten *= 10;
		}
		Sortepi.push_back(numb);
	}

	for (int i = 0; i < Sortepi.size() - 1; i++) {
		for (int j = i + 1; j < Sortepi.size(); j++) {
			if (Sortepi[i] > Sortepi[j]) {
				temp2 = Sortepi[i]; // 1의 개수대로 variable 정렬
				Sortepi[i] = Sortepi[j];
				Sortepi[j] = temp2;
				Isepi[i].swap(Isepi[j]);
			}
		}
	}

	result.push_back("EPI");
	for (int i = 0; i < minterm; i++) {
		if (howmanyminterm[vars[i]] == 1) {
			for (int m = 0; m < realminterm.size(); m++) {
				for (int n = 0; n < realminterm[m].size(); n++) {
					if (realminterm[m][n] == vars[i]) {
						realepi.push_back(Isepi[m]);
						for (int k = 0; k < realminterm[m].size(); k++) {
							howmanyminterm[realminterm[m][k]] = 0;
						}
						Isepi.erase(Isepi.begin() + m);
						realminterm.pop_front();
						m = realminterm.size();
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < realepi.size(); i++) {
		for (int j = 0; j < var; j++) {
			if (realepi[i][j] == 2) {
				tempchar[j] = '-';
			}
			else {
				tempchar[j] = realepi[i][j] + '0';
			}
		}
		result.push_back(tempchar);
	}
	result.push_back("NEPI");

	for (int i = 0; i < Isepi.size(); i++) {
		for (int j = 0; j < var; j++) {
			if (Isepi[i][j] == 2) {
				tempchar[j] = '-';
			}
			else {
				tempchar[j] = Isepi[i][j] + '0';
			}
		}
		result.push_back(tempchar);
	}

	for (int i = 0; i < result.size(); i++) {
		cout << result[i] << " ";
	}
	cout << endl;
}