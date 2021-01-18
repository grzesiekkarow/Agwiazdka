
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <list>
#include <algorithm> 
using namespace std;

struct Pole
{
	int x, y;
	int wartość;
	int x_rodzica, y_rodzica;
	double g;
	double h;
	double f;
};
list<Pole> lista_otwarta = {};
list<Pole> lista_zamknieta;
Pole znajdzNastepny(double**, Pole cel);
bool porownanieDwochPol(Pole, Pole);
bool czyOdwiedzony(Pole);
double **przypiszWartości(double**);
double obliczF(Pole pole, Pole cel);

int main(void)
{

	int wym2 = 20;
	int wym1 = 20;
	int rows = wym2 + 1;
	Pole start = Pole();
	start.x = 0;
	start.y = 19;
	start.wartość = 3;
	Pole cel = Pole();
	cel.x = 19;
	cel.y = 0;
	lista_zamknieta.push_back(start);
	Pole aktualny = start;
	double **G;
	G = new double*[rows];
	while (rows--) G[rows] = new double[wym1 + 1];

	ifstream plik("grid.txt");
	for (unsigned int i = 1; i< wym2 + 1; i++)
	{
		for (unsigned int j = 1; j< wym1 + 1; j++)
		{
			plik >> G[i][j];
		}
	}
	plik.close();
	while (aktualny.x != cel.x &&  aktualny.y != cel.y)
	{
		aktualny = znajdzNastepny(G, cel);
	}
	G = przypiszWartości(G);


	for (int i = 1; i<wym2 + 1; i++)
	{
		for (int j = 1; j<wym1 + 1; j++)
		{
			cout << " " << G[i][j];
		}cout << "\n";
	}
	for (int i = 0; i<wym2 + 1; i++)
	{
		delete[] G[i];
	}
	delete[] G;

	cout << "\n\nNacisnij ENTER aby zakonczyc";
	getchar();
	return 0;
}
bool czyOdwiedzony(Pole pole)
{
	list<Pole> pomocnicza = lista_zamknieta;
	Pole pomoc = Pole();
	int wielkosc = pomocnicza.size();
	for (int i = 0; i < wielkosc; i++)
	{
		if (porownanieDwochPol(pole, pomoc))
			return 1;
		pomoc = pomocnicza.back();
		pomocnicza.pop_back();
	}
	return 0;
}
bool porownanieDwochPol(Pole pole, Pole porownywane)
{
	if (pole.x == porownywane.x && pole.y == porownywane.y)
		return 1;
	return 0;
}
Pole znajdzNajlepszy()
{
	if (!lista_otwarta.empty())
	{
		Pole najlepszy = lista_otwarta.front();
		int wielkosc = lista_otwarta.size();
		for (int i = 0; i < wielkosc; i++)
		{
			Pole pomoc = lista_otwarta.front();
			if (najlepszy.f > pomoc.f)
			{
				najlepszy = pomoc;
			}
			lista_otwarta.pop_front();
		}
		return najlepszy;
	}
	else 
	{
		Pole pomoc = Pole();
		return pomoc;
	}
}
Pole znajdzNastepny(double** G, Pole cel)
{
	Pole aktualny = lista_zamknieta.back();
	Pole nastepny = Pole();
	if (aktualny.y - 1 > 0 && G[aktualny.x][aktualny.y - 1] != 5)
	{
		Pole nastepny1 = Pole();
		nastepny1.x = aktualny.x;
		nastepny1.y = aktualny.y - 1;
		nastepny1.f = obliczF(nastepny1, cel);
		nastepny1.x_rodzica = aktualny.x;
		nastepny1.y_rodzica = aktualny.y;
		if (!czyOdwiedzony(nastepny1))
		{
			lista_otwarta.push_back(nastepny1);
		}

	}
	if (aktualny.y + 1 > 0 && G[aktualny.x][aktualny.y + 1] != 5)
	{
		Pole nastepny2 = Pole();
		nastepny2.x = aktualny.x;
		nastepny2.y = aktualny.y + 1;
		nastepny2.x_rodzica = aktualny.x;
		nastepny2.y_rodzica = aktualny.y;
		nastepny2.f = obliczF(nastepny2, cel);
		if (!czyOdwiedzony(nastepny2))
		{
			lista_otwarta.push_back(nastepny2);
		}
	}
	if (aktualny.x - 1 > 0 && G[aktualny.x - 1][aktualny.y] != 5)
	{
		Pole nastepny3 = Pole();
		nastepny3.x = aktualny.x - 1;
		nastepny3.y = aktualny.y;
		nastepny3.x_rodzica = aktualny.x;
		nastepny3.y_rodzica = aktualny.y;
		nastepny3.f = obliczF(nastepny3, cel);
		if (!czyOdwiedzony(nastepny3))
		{
			lista_otwarta.push_back(nastepny3);
		}
	}
	if (aktualny.x + 1 > 0 && G[aktualny.x + 1][aktualny.y] != 5)
	{
		Pole nastepny4 = Pole();
		nastepny4.x = aktualny.x + 1;
		nastepny4.y = aktualny.y;
		nastepny4.x_rodzica = aktualny.x;
		nastepny4.y_rodzica = aktualny.y;
		nastepny4.f = obliczF(nastepny4, cel);
		if (!czyOdwiedzony(nastepny4))
		{
			lista_otwarta.push_back(nastepny4);
		}
	}
	if (!lista_otwarta.empty())
	{
		nastepny = znajdzNajlepszy();
		nastepny.wartość = 3;
		lista_zamknieta.push_back(nastepny);
		return nastepny;
	}
	else if(znajdzNajlepszy().x == 20 && znajdzNajlepszy().y == 20)
	{
		cel.wartość = 3;
		lista_zamknieta.push_back(cel);
		return cel;
	}
	else if(znajdzNajlepszy().x == 0 && znajdzNajlepszy().y == 0)
	{
		cout << "Nie mozna dotrzec do celu \n";
		return cel;
	}

}


double **przypiszWartości(double** G)
{
	Pole pole = Pole();
	int wielkosc = lista_zamknieta.size();
	for (int j = 0; j < wielkosc; j++)
	{
		pole = lista_zamknieta.back();
		G[pole.x][pole.y] = 3;
		lista_zamknieta.pop_back();
	}
	return	G;
}
double obliczF(Pole pole, Pole cel)
{
pole.g = 1;
pole.h = sqrt(pow(pole.x - cel.x, 2) + pow(pole.y - cel.y, 2));
double f = pole.g + pole.h;
return f;
}
