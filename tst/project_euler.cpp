/*
 * project_euler.cpp
 *
 *  Created on: Sep 29, 2011
 *      Author: yinterry
 */
#include "math.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "project_euler.h"
int is_prime(int num, long previous_primes[], int count)
{
	int max = sqrt(num);
	for (int i = 0; i < count && previous_primes[i] <= max; i++)
		if (num % previous_primes[i] == 0)
			return 0;
	return 1;
}
int factorial(int n) {
	int result = 1;
	for (;n>0; n--) result *= n;
	return result;
}
int expo(int n, int e) {
	int result = 1;
	for (int i = 0; i < e; i ++) result*= n;
	return result;
}
