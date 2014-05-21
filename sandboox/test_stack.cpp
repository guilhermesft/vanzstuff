/*
 * =====================================================================================
 *
 *       Filename:  test_stack.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  05/20/2014 10:19:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  José Guilherme Vanz (vanz), guilherme.sft@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include <iostream>

using namespace std;

int * foo();
void bar();

int main(int arvc, char**argv)
{
	int * b = foo();
	bar();
	cout << *b << endl;;
	*b = 2;
	cout << *b << endl;
}

int * foo()
{
	int a = 1;
	return &a;
}

void bar()
{
	int c = 3;
	int d = 4;
	int e = 5;
}
