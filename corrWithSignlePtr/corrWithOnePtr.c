#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"

int** initMask(int k);
float distance(int a, int b, int c, int d);
int** zeroPad(int* A, int n, int m, int xpad, int ypad);
int eleWiseMult(int* A, int* B, int n_a, int m_a, int n_b, int m_b, int xMult, int yMult);
int** corr2D(int* A, int* B, int n_a, int m_a, int n_b, int m_b);
void freeMat(int* A, int n, int m);
void occupiedSpots(int* A, int* corr, int n, int m);
int findMax(int* A, int n, int m);
void printMat(int** A, int n, int m); // personal use

void main()
{
	int A[3][4] = { {1,0,0,0},{0,1,1,0},{0,0,0,1} };
	int** mask, **paddedMat, **corr;
	int i, j, max,k,n_a,m_a,n_mask,m_mask;

	k = 2;
	n_a = 3;
	m_a = 4;
	n_mask = 2 * k + 1;
	m_mask = 2 * k + 1;

	mask = initMask(k);
	corr = corr2D(*A, *mask, n_a, m_a, n_mask, m_mask);
	occupiedSpots(*A, *corr, n_a, m_a);
	max = findMax(*corr, n_a, m_a);

	printf("max value in mat = %d\n", max);
}
int** initMask(int k)
{
	int x, y, i, j;
	int** mask;
	int* memSpaceHolder;

	x = 2 * k + 1;
	y = 2 * k + 1;

	memSpaceHolder = malloc(sizeof(int) * (x * y));
	mask = malloc(sizeof(int*) * y);

	for (i = 0; i < y; i++)
	{
		*(mask + i) = (memSpaceHolder + x * i);
		for (j = 0; j < x; j++)
		{
			if (distance(i, j, k, k) <= k)
			{
				*(memSpaceHolder + x * i + j) = 1;
			}
			else
			{
				*(memSpaceHolder + x * i + j) = 0;
			}
		}
	}
	return mask;
}
float distance(int a, int b, int c, int d)
{
	return (sqrt(pow((c - a), 2) + pow((d - b), 2)));
}
int** zeroPad(int* A, int n, int m, int xpad, int ypad)
{
	int i, j, xdim, ydim;
	int** newMat;
	int* memSpaceHolder;

	xdim = m + 2 * xpad;
	ydim = n + 2 * ypad;

	memSpaceHolder = malloc(sizeof(int) * (xdim * ydim));
	newMat = malloc(sizeof(int*) * (ydim));
	
	for (i = 0; i < ydim; i++)
	{
		*(newMat + i) = (memSpaceHolder + xdim * i);
		for (j = 0; j < xdim; j++)
		{
			if ( (i < ypad) || (j < xpad) || (i > (n - 1 + ypad)) || (j > (m - 1 + xpad)) )
			{
				*(memSpaceHolder + xdim * i + j) = 0;
			}
			else
			{
				*(memSpaceHolder + xdim * i + j) = *(A + m * (i - ypad) + (j - xpad));
			}
		}
	}
	return newMat;
}
int eleWiseMult(int* A, int* B, int n_a, int m_a, int n_b, int m_b, int xMult, int yMult)
{
	int sum, i, j;
	sum = 0;

	for (i = 0; i < yMult; i++)
	{
		for (j = 0; j < xMult; j++)
		{
			sum += ((*(A + m_a * i + j)) * (*(B + m_b * i + j)));
		}
	}
	return sum;
}
int** corr2D(int* A, int* B, int n_a, int m_a, int n_b, int m_b)
{
	int i, j, xpad, ypad;
	int** corr, **paddedA;
	int* memSpaceHolder;

	xpad = (m_b - 1) / 2;
	ypad = (n_b - 1) / 2;

	paddedA = zeroPad(A, n_a, m_a, xpad, ypad);

	memSpaceHolder = (int*)malloc(sizeof(int) * (n_a * m_a));
	corr = (int**)malloc(sizeof(int*) * n_a);

	for (i = 0; i < n_a; i++)
	{
		*(corr + i) = memSpaceHolder + m_a * i;
		for (j = 0; j < m_a; j++)
		{
			*(memSpaceHolder + m_a * i + j) = eleWiseMult( ( &paddedA[i][j]), B, (n_a + 2 * ypad), (m_a + 2 * xpad), n_b, m_b, m_b, n_b);
		}
	}
	freeMat(*paddedA, (n_a + 2 * ypad), (m_a + 2 * xpad));
	return corr;
}
void freeMat(int* A, int n, int m)
{
	int i;
	for (i = 0; i < n; i++)
	{
		free(*(A + m * i));
	}
	free(A);
}
void occupiedSpots(int* A, int* corr, int n, int m)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			if ( (*(A + m*i + j)) == 1)
			{
				*(corr + m*i + j) = 0;
			}
		}
	}
}
int findMax(int* A, int n, int m)
{
	int i, j, max;
	max = *(A);

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			if ((*(A + m * i + j)) > max)
			{
				max = *(A + m * i + j);
			}
		}
	}
	return max;
}
void printMat(int** A, int n, int m) // personal use
{
	printf("\nPRINT MAT:\n");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			printf("| %d |", A[i][j]);
		}
		printf("\n");
	}
}