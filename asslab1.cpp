#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <intrin.h>
using namespace std;
unsigned long int rdtsc_zn;

//получение количества тактов
unsigned __int64 rdtsc(void)
{
	return __rdtsc();
}
void FullTheMatrix(unsigned short int M[8][8])
{
	for (int i = 0; i < 8; i++)
	{
		for (int k = 0; k < 8; k++)
		{
			M[i][k] = (i + 1) + (k + 5) + rand() % 40;
		}
	}
}
void PrintTheMatrix(unsigned short int M[8][8])
{
	cout << "________________________________________" << endl;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cout << M[i][j] << " ";
		}
		cout << endl;
	}
	cout << "________________________________________" << endl;
}

int main()
{
	unsigned short int M1[8][8];
	unsigned short int M2[8][8];
	unsigned short int var1, var2;
	unsigned int temp;

	unsigned __int64 TimeValue2 = 0;
	cout << "With SSE: " << endl << "____________" << endl;
	TimeValue2 = rdtsc();


	FullTheMatrix(M1);
	FullTheMatrix(M2);

	unsigned short int x = 2; 
	cout << "x =  " << x << endl; 
	cout << endl;
	cout << "First Matrix:  " << endl;
	PrintTheMatrix(M1);
	cout << "Second Matrix:  " << endl;
	PrintTheMatrix(M2);


	for (int i = 0; i < 8; i++) // Складывание двух матриц
	{
		float HalfM11[4] = { M1[i][0], M1[i][1], M1[i][2], M1[i][3] };
		float HalfM12[4] = { M1[i][4], M1[i][5], M1[i][6], M1[i][7] };
		float HalfM21[4] = { M2[i][0], M2[i][1], M2[i][2], M2[i][3] };
		float HalfM22[4] = { M2[i][4], M2[i][5], M2[i][6], M2[i][7] };
		_asm
		{
			movups xmm0, HalfM11
			movups xmm1, HalfM21
			addps xmm0, xmm1
			movups HalfM11, xmm0 

			movups xmm0, HalfM12
			movups xmm1, HalfM22
			addps xmm0, xmm1
			movups HalfM12, xmm0

			
		}
		M1[i][0] = HalfM11[0];
		M1[i][1] = HalfM11[1];
		M1[i][2] = HalfM11[2];
		M1[i][3] = HalfM11[3];
		M1[i][4] = HalfM12[0];
		M1[i][5] = HalfM12[1];
		M1[i][6] = HalfM12[2];
		M1[i][7] = HalfM12[3];

	}

	unsigned short int ResultMatrix[8][8];
	for (int i = 0; i < 8; i++) // Умножение матрицы M1 на M2, результат в ResultMatrix
	{
		for (int j = 0; j < 8; j++)
		{
			float Matrix1Half1[4] = { M1[i][0], M1[i][1], M1[i][2], M1[i][3] };
			float Matrix1Half2[4] = { M1[i][4], M1[i][5], M1[i][6], M1[i][7] };
			float Matrix2Half1[4] = { M2[0][j], M2[1][j], M2[2][j], M2[3][j] };
			float Matrix2Half2[4] = { M2[4][j], M2[5][j], M2[6][j], M2[7][j] };
			_asm
			{
				movups xmm0, Matrix1Half1
				movups xmm1, Matrix1Half2
				movups xmm2, Matrix2Half1
				movups xmm3, Matrix2Half2

				mulps  xmm0, xmm2
				mulps  xmm1, xmm3

				addps xmm0, xmm1
				movups Matrix1Half1, xmm0
			}
			ResultMatrix[i][j] = Matrix1Half1[0] + Matrix1Half1[1] + Matrix1Half1[2] + Matrix1Half1[3];

		}
	}


	for (int i = 0; i < 8; i++) // Умножение матрицы MatrixResult на x
	{
		float HalfOfStr1[4] = { ResultMatrix[i][0], ResultMatrix[i][1], ResultMatrix[i][2], ResultMatrix[i][3] };
		float HalfOfStr2[4] = { ResultMatrix[i][4], ResultMatrix[i][5], ResultMatrix[i][6], ResultMatrix[i][7] };
		float mnozhitel[4] = { x, x, x, x };
		_asm
		{
			movups xmm0, HalfOfStr1
			movups xmm1, HalfOfStr2

			movups xmm2, mnozhitel

			mulps  xmm0, xmm2
			movups xmm2, mnozhitel
			mulps  xmm1, xmm2

			movups HalfOfStr1, xmm0
			movups HalfOfStr2, xmm1
		}
		ResultMatrix[i][0] = HalfOfStr1[0];
		ResultMatrix[i][1] = HalfOfStr1[1];
		ResultMatrix[i][2] = HalfOfStr1[2];
		ResultMatrix[i][3] = HalfOfStr1[3];
		ResultMatrix[i][4] = HalfOfStr2[0];
		ResultMatrix[i][5] = HalfOfStr2[1];
		ResultMatrix[i][6] = HalfOfStr2[2];
		ResultMatrix[i][7] = HalfOfStr2[3];

	}

	cout << "Result Matrix:  " << endl;
	PrintTheMatrix(ResultMatrix);

	cout << endl;
	// высчитываем время через число тактов за время выполнения программы и мощность процессора
	rdtsc_zn = (rdtsc() - TimeValue2) / (1000000 * 1000 * 2.3) * 10000; 
	// высчитываем время через число тактов за время выполнения программы и мощность процессора
	cout << "Time (in milliseconds) (with SSE) = " << rdtsc_zn << endl; // выводим время выполнения программы

	cout << "Without SSE: " << endl << "____________" << endl;
	unsigned __int64 TimeValue = 0;
	TimeValue = rdtsc();



	FullTheMatrix(M1);
	FullTheMatrix(M2);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			ResultMatrix[i][j] = 0; // чистим итоговую матрицу заранее
		}
	}

	cout << "x = " << x << endl; // выводим заданное x
	cout << endl;
	cout << "First Matrix:  " << endl;
	PrintTheMatrix(M1);
	cout << endl;
	cout << "Second Matrix:  " << endl;
	PrintTheMatrix(M2);

	//Сложение двуз матриц по элементно
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++) {
			var1 = ResultMatrix[i][j];
			var2 = ResultMatrix[i][j];
			_asm
			{
				mov ax, var1
				mov bx, var2
				add ax, bx
				mov var1, ax
			}
			ResultMatrix[i][j] = var1;
		}
	}
	//Умножение матриц M1 и M2
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			for (int r = 0; r < 8; r++)
			{
				var1 = M1[i][r];
				var2 = M2[r][j];
				temp = ResultMatrix[i][j];
				_asm
				{
					mov ax, var1
					mov bx, var2
					mul bx
					add temp, eax
				}
				ResultMatrix[i][j] = temp;
			}
		}
	}
	//Умножение матрицы на x
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			var1 = ResultMatrix[i][j];
			_asm
			{
				mov ax, x
				mov bx, var1
				mul bx
				mov temp, eax
			}
			ResultMatrix[i][j] = temp;
		}
	}

	cout << endl;
	cout << "Result Matrix:  " << endl;
	PrintTheMatrix(ResultMatrix);
	rdtsc_zn = (rdtsc() - TimeValue) / (1000000 * 1000 * 2.3) * 10000; // высчитываем время через число тактов за время выполнения программы и мощность процессора
	cout << "Time (in milliseconds) (without SSE) = " << rdtsc_zn << endl; // выводим время выполнения программы
	return 0;
}