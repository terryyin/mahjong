/*
 * project_euler.h
 *
 *  Created on: Sep 29, 2011
 *      Author: yinterry
 */

#ifndef PROJECT_EULER_H_
#define PROJECT_EULER_H_

int is_prime(int num, long previous_primes[], int count);
int factorial(int n);
int expo(int n, int e);

class prime {
public:
	prime() {memset(previous_primes, 0, sizeof(previous_primes));count = 0;}
	int is_prime(int num){
		if (num < 2)
			return 0;
		int max = sqrt(num);
		if (num != 2 && num % 2 == 0)
			return 0;
		for (int i = 3; i <= max;i+=2)
			if (num % i == 0)
				return 0;

		//for (int i = 0; i < count && previous_primes[i] <= max; i++)
			//if (num % previous_primes[i] == 0)
				//return 0;
		return 1;
	}
private:
	static const int buf_size = 30000;
	int previous_primes[buf_size];
	int count;
};

class triangle
{
public:
	triangle(int * d, int dimension)
	{
		dim = dimension;
		for (int i = 0; i < dim; i++)
			for (int j = 0; j <= i; j++)
				data[i][j] = d[i * dim + j];
	}
	int get_max_sum()
	{
		return _get_max_sum(0, 0);
	}
private:
	int _get_max_sum(int level, int pos)
	{
		if (level == dim - 1)
			return data[level][pos];
		int left = _get_max_sum(level + 1, pos);
		int right = _get_max_sum(level + 1, pos + 1);
		if (right > left)
			left = right;
		return data[level][pos] + left;
	}
	int dim;
	int data[100][100];
};

class dec
{
public:
	dec(int i)
	{
		memset(digits, 0, sizeof(digits));
		digits[0] = i;
		count = 1;
	}
	dec& operator *=(int n)
	{
		for (int i = 0; i < count; i++)
			digits[i] *= n;
		regulate();
		return *this;
	}
	dec& operator +=(dec& other){
		if( count < other.count)
			count = other.count;
		for (int i = 0; i < count; i++)
			digits[i] += other.digits[i];
		regulate();
		return *this;
	}
	void append(int n) {
		digits[count++] = n;
	}
	int operator %(int n) {
		int mod = 0;
		for (int i = count - 1; i >=0; i--) {
			mod = (mod * 10 + digits[i]) % n;
		}
		return mod;
	}
	int digits_sum()
	{
		int sum = 0;
		for (int i = 0; i < count; i++)
			sum += digits[i];
		return sum;
	}
	int get_digit(int i)
	{
		return digits[i];
	}
	int get_count()
	{
		return count;
	}
private:
	int digits[2000];
	int count;
	void regulate()
	{
		for (int i = 0; i < count; i++)
			if (digits[i] > 9)
			{
				digits[i + 1] += digits[i] / 10;
				digits[i] %= 10;
				if (i == count - 1)
					count++;
			}
	}
};

class number_set{
public:
	number_set() {count = 0;}
	int get_count(){return count;}
	void add(int n){
		if (!find(n))
			data[count++] = n;
	}
	int get_sum() {int sum = 0; for(int i = 0; i < count; i++)sum+=data[i];return sum;}
protected:
	int find(int n){
		for (int i =0; i < count ; i++)
			if (data[i]==n)
				return 1;
		return 0;
	}
	int count;
	int data[20000];

};


#endif /* PROJECT_EULER_H_ */
