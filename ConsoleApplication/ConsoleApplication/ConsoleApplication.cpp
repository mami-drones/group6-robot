// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Windows.h"
#include <iostream>
using namespace std;

int row_h;
int row_v;
int shelf_1 = 1;
int shelf_2 = 2;
int shelf_3 = 3;
int start = 1111;
int finish = 9999;
int robot_x = 0; //��������� ���������� "�" ��� ������
int robot_y = 4; //��������� ���������� "�" ��� ������
int shelf_x = 4; //���������� "�" ������ �����
int shelf_y = 4; //���������� "�" ������ �����
int shelf = 3; //����� ������ �����
int n = 0; //����������, ����������� ��� ���������� ����������� ������� � ������
float m = 0.1;
int h_up;
int h_down;
int v_left;
int v_right;
float need;
float need_1 = 0;
float need_2 = 0;
int x;
int y;
int x_1 = 0;
int y_1 = 0;
int x_2 = 0;
int y_2 = 0;
float stock[5][5];
int way;
int lengtharray;
int i;

void setup() {
	//	Serial.begin(9600);
	for (row_h = 0; row_h<5; ++row_h) {
		for (row_v = 0; row_v<5; ++row_v) {
			stock[row_h][row_v] = 0;

		}
	}
	stock[0][1] = shelf_1;
	stock[2][0] = shelf_2;
	stock[4][4] = shelf_3;
	stock[0][4] = start;
	stock[4][0] = finish;
}


void loop() {
	Sleep(3000);
	h_up = robot_x - 1;
	h_down = robot_x + 1;
	v_left = robot_y - 1;
	v_right = robot_y + 1;

	if (h_up >= 0) {
		if (stock[h_up][robot_y] == shelf) {
			n = 1;
		}
	}
	if (h_down <= 4) {
		if (stock[h_down][robot_y] == shelf) {
			n = 1;
		}
	}
	if (v_left >= 0) {
		if (stock[robot_x][v_left] == shelf) {
			n = 1;
		}
	}
	if (v_right <= 4) {
		if (stock[robot_x][v_right] == shelf) {
			n = 1;
		}
	}

	if (n == 0) {
		if (h_up >= 0 && stock[h_up][robot_y] == 0) {
			stock[h_up][robot_y] = m;
		}
		if (h_down <= 4 && stock[h_down][robot_y] == 0) {
			stock[h_down][robot_y] = m;

		}
		if (v_left >= 0 && stock[robot_x][v_left] == 0) {
			stock[robot_x][v_left] = m;
		}
		if (v_right <= 4 && stock[robot_x][v_right] == 0) {
			stock[robot_x][v_right] = m;
		}
	}
	while (n == 0) {
		for (row_h = 0; row_h<5; ++row_h) {
			for (row_v = 0; row_v<5; ++row_v) {
				if (stock[row_h][row_v] == m) {
					robot_x = row_h;
					robot_y = row_v;
					h_up = robot_x - 1;
					h_down = robot_x + 1;
					v_left = robot_y - 1;
					v_right = robot_y + 1;
					if (h_up >= 0) {
						if (stock[h_up][robot_y] == shelf) {
							n = 1;
						}
					}
					if (h_down <= 4) {
						if (stock[h_down][robot_y] == shelf) {
							n = 1;
						}
					}
					if (v_left >= 0) {
						if (stock[robot_x][v_left] == shelf) {
							n = 1;
						}
					}
					if (v_right <= 4) {
						if (stock[robot_x][v_right] == shelf) {
							n = 1;
						}
					}
					if (n == 0) {

						if (h_up >= 0 && stock[h_up][robot_y] == 0) {
							stock[h_up][robot_y] = m + 0.1;
						}
						if (h_down <= 4 && stock[h_down][robot_y] == 0) {
							stock[h_down][robot_y] = m + 0.1;
						}
						if (v_left >= 0 && stock[robot_x][v_left] == 0) {
							stock[robot_x][v_left] = m + 0.1;
						}
						if (v_right <= 4 && stock[robot_x][v_right] == 0) {
							stock[robot_x][v_right] = m + 0.1;
						}
					}
				}
			}
		}
		m = m + 0.1;
	}
	h_up = shelf_x - 1;
	h_down = shelf_x + 1;
	v_left = shelf_y - 1;
	v_right = shelf_y + 1;
	if (v_left >= 0 && v_right <= 4) {
		if (stock[shelf_x][v_left] != 0 && stock[shelf_x][v_right] != 0) {
			if (stock[shelf_x][v_right] < stock[shelf_x][v_left]) {
				need_1 = stock[shelf_x][v_right];
				x_1 = shelf_x;
				y_1 = v_right;
			}
			else {
				need_1 = stock[shelf_x][v_left];
				x_1 = shelf_x;
				y_1 = v_left;
			}
		}

		else {
			if (stock[shelf_x][v_left] == 0) {
				if (stock[shelf_x][v_right] != 0) {
					need_1 = stock[shelf_x][v_right];
					x_1 = shelf_x;
					y_1 = v_right;
				}
			}
			else {
				need_1 = stock[shelf_x][v_left];
				x_1 = shelf_x;
				y_1 = v_left;
			}
		}
	}
	else if (v_left < 0 && v_right <= 4) {
		need_1 = stock[shelf_x][v_right];
		x_1 = shelf_x;
		y_1 = v_right;
	}
	else if (v_left >= 0 && v_right > 4) {
		need_1 = stock[shelf_x][v_left];
		x_1 = shelf_x;
		y_1 = v_left;
	}


	if (h_up >= 0 && h_down <= 4) {
		if (stock[h_up][shelf_y] != 0 && stock[h_down][shelf_y] != 0) {
			if (stock[h_up][shelf_y] < stock[h_down][shelf_y]) {
				need_2 = stock[h_up][shelf_y];
				x_2 = h_up;
				y_2 = shelf_y;
			}
			else {
				need_2 = stock[h_down][shelf_y];
				x_2 = h_down;
				y_2 = shelf_y;
			}
		}

		else {
			if (stock[h_up][shelf_y] == 0) {
				if (stock[h_down][shelf_y] != 0) {
					need_1 = stock[h_down][shelf_y];
					x_2 = h_down;
					y_2 = shelf_y;
				}
			}
			else {
				need_2 = stock[h_up][shelf_y];
				x_2 = h_up;
				y_2 = shelf_y;
			}
		}
	}
	else if (h_up < 0 && h_down <= 4) {
		need_2 = stock[h_down][shelf_y];
		x_2 = h_down;
		y_2 = shelf_y;
	}
	else if (h_up >= 0 && h_down > 4) {
		need_2 = stock[h_up][shelf_y];
		x_2 = h_up;
		y_2 = shelf_y;
	}
	if (need_1 > 0 && need_2 > 0) {
		if (need_1 < need_2) {
			need = need_1;
			x = x_1;
			y = y_1;
		}
		else {
			need = need_2;
			x = x_2;
			y = y_2;
		}
	}
	if (need_1 == 0) {
		need = need_2;
		x = x_2;
		y = y_2;
	}
	if (need_2 == 0) {
		need = need_1;
		x = x_1;
		y = y_1;
	}
	lengtharray = need * 10 + 1;

	int* way = new int[lengtharray];

	if (x < shelf_x) {
		way[lengtharray - 1] = 1;
	}
	if (x > shelf_x) {
		way[lengtharray - 1] = 2;
	}
	if (y < shelf_y) {
		way[lengtharray - 1] = 3;
	}
	if (y > shelf_y) {
		way[lengtharray - 1] = 4;
	}
	lengtharray = lengtharray - 1;
	m = need - 0.1;
	while (m > 0) {

		h_up = x - 1;
		h_down = x + 1;
		v_left = y - 1;
		v_right = y + 1;

		if (h_up >= 0) {

			if (m == stock[h_up][y]) {  //��� ��� ��� ������-�� �� ����������� �������
				way[lengtharray - 1] = 1;
				x = h_up;
				y = y;
			}
		}
		if (h_down <= 4) {
			if (stock[h_down][y] == m) {
				way[lengtharray - 1] = 2;
				x = h_down;
				y = y;
			}
		}
		if (v_left >= 0) {
			if (stock[x][v_left] == m) {
				way[lengtharray - 1] = 3;
				x = x;
				y = v_left;
			}
		}
		if (v_right <= 4) {
			if (stock[x][v_right] == m) {

				way[lengtharray - 1] = 4;
				x = x;
				y = v_right;
			}
		}
		cout << (way[lengtharray - 1]);
		m = m - 0.1;
		lengtharray = lengtharray - 1;
	}
	h_up = x - 1;
	h_down = x + 1;
	v_left = y - 1;
	v_right = y + 1;
	if (h_up >= 0) {
		if (stock[h_up][y] == 1111) {
			way[lengtharray - 1] = 1;
		}
	}
	if (h_down <= 4) {
		if (stock[h_down][y] == 1111) {
			way[lengtharray - 1] = 2;
		}
	}
	if (v_left >= 0) {
		if (stock[x][v_left] == 1111) {
			way[lengtharray - 1] = 3;
		}
	}
	if (v_right <= 4) {
		if (stock[x][v_right] == 1111) {
			way[lengtharray - 1] = 4;
		}
	}
	lengtharray = need * 10 + 1;
	for (i = 0; i < lengtharray; ++i) {
		cout << way[i];
	}
	delete[] way;
	while (true) {};
}

int _tmain(int argc, _TCHAR* argv[])
{
	setup();
	while (true)
		loop();


	return 0;
}

