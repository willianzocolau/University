#include <stdafx.h>
#include <iostream>
#include <stdio.h>

using namespace std;

struct elemento {
	unsigned long long id;
	unsigned long long index;
	unsigned long long v;
	unsigned long long pa;
};

unsigned long long n, l;

unsigned long long calcular(unsigned long long n) {
	if (n == 1)
	{
		return 1;
	}
	else
	{
		return (n * 4) - 4;
	}
}

unsigned long long pa(unsigned long long superior, unsigned long long inferior, unsigned long long termos)
{
	unsigned long long r = 0;
	if (inferior != 1)
	{
		r = (superior + inferior) * termos;
		r = r / 2;
	}
	else
	{
		inferior = calcular(3);
		r = (superior + inferior) * (termos - 1);
		r = r / 2;
		r++;
	}
	return r;
}

elemento termo_meio(unsigned long long indice_s, unsigned long long indice_i)
{
	elemento e;
	e.index = (indice_s + indice_i) / 2;
	if (n % 2 == 0)
	{
		e.id = -2 * (e.index - ((n / 2) + 1));
	}
	else
	{
		e.id = (n + 2) - 2 * e.index;
	}
	e.v = calcular(e.id);
	return e;
}

elemento indice(elemento s, elemento i, unsigned long long tam)
{
	unsigned long long d_superior = pa(tam, s.v, s.index);
	unsigned long long d_inferior = pa(tam, i.v, i.index);
	if (s.index == 0) {
		s.v = 0;
		s.index = 1;
	}
	else
	{
		s.v = d_superior - s.v;
	}
	i.v = d_inferior - i.v;

	unsigned long long ls = l - s.v;
	unsigned long long li = l - i.v;
	if (ls < li || li < 1) {
		s.index--;
		return s;
	}
	else {
		i.index--;
		return i;
	}
}

void solucao(elemento s)
{
	unsigned long long resto = l - s.v;
	unsigned long long x = s.index + 1, y = s.index + 1, counter = 0;
	unsigned long long resultado = 0;
	if (resto == 1)
	{
		cout << x << " " << y << endl;
	}
	else
	{
		long t = resto - s.id;
		if (t <= 0)
		{
			cout << x << " " << y + resto - 1 << endl;
			resultado = 1;
		}
		else
		{
			resto = t;
			y = s.id + y;
		}

		t = resto - (s.id - 1);
		y--;
		if (t <= 0 && resultado == 0)
		{
			cout << x + resto << " " << y << endl;
			resultado = 1;
		}
		else
		{
			resto = t;
			x = s.id + x;
		}

		t = resto - (s.id - 1);
		x--;
		if (t <= 0 && resultado == 0)
		{
			cout << x << " " << y - resto << endl;
			resultado = 1;
		}
		else
		{
			resto = t;
			y = y - s.id;
		}

		y++;
		if (resultado == 0) {
			cout << x - resto << " " << y << endl;
			resultado = 1;
		}
	}
}

void par()
{
	elemento superior, inferior, meio;
	superior.id = n;
	superior.index = 0;
	superior.v = 0;
	superior.pa = 0;

	inferior.id = 2;
	inferior.index = n / 2;
	inferior.v = calcular(inferior.id);
	inferior.pa = pa(calcular(n), inferior.v, inferior.index);

	meio = termo_meio(1, inferior.index);

	unsigned long long tam = calcular(n);
	while ((superior.id - inferior.id) != 2 && (superior.id - inferior.id) != 0)
	{
		unsigned long long r = pa(tam, meio.v, meio.index);
		meio.pa = r;
		if (l < r)
		{
			inferior = meio;
			meio = termo_meio(superior.index, inferior.index);
		}
		else
		{
			superior = meio;
			meio = termo_meio(superior.index, inferior.index);
		}
	}
	elemento s = indice(superior, inferior, tam);
	solucao(s);
}

void impar()
{
	elemento superior, inferior, meio;
	superior.id = n;
	superior.index = 0;
	superior.v = 0;
	superior.pa = 0;

	inferior.id = 1;
	inferior.index = (n + 1) / 2;
	inferior.v = calcular(inferior.id);
	inferior.pa = pa(calcular(n), inferior.v, inferior.index);

	meio = termo_meio(1, inferior.index);

	unsigned long long tam = calcular(n);
	while ((superior.id - inferior.id) != 2 && (superior.id - inferior.id) != 0)
	{
		unsigned long long r = pa(tam, meio.v, meio.index);
		meio.pa = r;
		if (l < r)
		{
			inferior = meio;
			meio = termo_meio(superior.index, inferior.index);
		}
		else
		{
			superior = meio;
			meio = termo_meio(superior.index, inferior.index);
		}
	}

	elemento s = indice(superior, inferior, tam);
	solucao(s);
}

int main()
{
	do {
		cin >> n >> l;
		if (n % 2 == 0) {
			par();
		}
		else {
			impar();
		}
	} while (true);
	return 0;
}