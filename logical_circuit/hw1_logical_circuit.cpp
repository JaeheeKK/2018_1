#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

void main() {
	int var, minterm, temp1, temp2;
	int position, memory; // �ٸ� �ڸ����� �ڸ��� ����� ���, ���� �� ���.
	cin >> var >> minterm;

	vector <string> result;
	deque<int> combineminterm; // combine�ɶ� minterm�� �־���� temp
	deque<deque<int>> epiminterm(minterm);//minterm ��.
	deque<deque<int>> realminterm;//real minterm ��.
	deque<deque<int>> realepi; // ��¥ epi
	deque<deque<int>> nepi(minterm, deque<int>(var));//minterm�� 2������ ǥ��
	deque<deque<int>>::iterator overlap; // ��ġ�� epi�� ������ üũ.
	deque<deque<int>> Isepi;//epi���� �ĺ�.
	deque<int> CountOnes; // �� variable�� 1�� ����
	deque<int> ForCompares; // compare�ϱ� ���� ���� ������ �����ֱ� ���� 1�� ���� ����

	char *tempchar = new char[var + 1];
	int howmanyminterm[100] = { 0, };
	int *vars = new int[minterm]; // variable
	int *temp = new int[var + 1]; // 1�� ���� ��� 2���� ���� �ϱ� ���� �ӽ� �迭.
	int *ForCompare = new int[var + 1]; // compare�ϱ� ���� ���� ������ �����ֱ� ���� 1�� ���� ����
	int *CountOne = new int[minterm];// 1�� ����

	memset(ForCompare, 0, sizeof(int)*(var + 1));
	memset(CountOne, 0, sizeof(int)*(minterm));
	tempchar[var] = '\0';

	for (int i = 0; i < minterm; i++) {
		cin >> vars[i];
	}

	for (int i = 0; i < minterm; i++) { // minterm�� 2������ ǥ��
		int variable = vars[i];
		for (int j = var - 1; j >= 0; j--) {
			nepi[i][j] = variable % 2;
			if (nepi[i][j] == 1) { // 2������ ǥ������ �� 1�� ���� count.
				CountOne[i]++;
			}
			variable /= 2;
			if (variable == 0) { // ���̻� ���� ���� ���� ��� ������Ʈ ��� 0����.
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
				temp1 = CountOne[i]; // 1�� ������� ����
				CountOne[i] = CountOne[j];
				CountOne[j] = temp1;
				temp2 = vars[i]; // 1�� ������� variable ����
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

	for (int i = 0; i < minterm; i++) { // 1�� ������ ������ ���� ���� ���� ������ ����.
		ForCompare[CountOne[i]]++;
	}
	for (int i = 0; i <= var; i++) {
		if (ForCompare[i] != 0) {
			ForCompares.push_back(ForCompare[i]);
		}
	}
	while (nepi.size() != 0) {
		bool *combine = new bool[nepi.size()]; //combine�� �Ǿ����� check�ϴ� ���
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

			int compares = 0;//1�� ������ ���� �ͳ��� ����� ���(���� ���� ���)
			for (int j = 0; j < ForCompares[0]; j++) {
				int combinecount3 = 0;
				combinecount++;
				for (int k = ForCompares[0]; k < ForCompares[0] + ForCompares[1]; k++) {
					int check = 0; // �ٸ� �ڸ����� ���� check
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
			if (lastcompare == RememberCompareSize) {// �񱳸� ����ġ�� ������ epi�˻�.
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
			howmanyminterm[realminterm[i][j]]++; //minterm�� ���� ��ŭ �����.
		}
	}

	deque<int> Sortepi;//epi ũ�� ����� ����.
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
				temp2 = Sortepi[i]; // 1�� ������� variable ����
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