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
Pole znajdzNastepny(double**, Pole, Pole);
Pole znajdzNajlepszy(double**, Pole, Pole);
bool porownanieDwochPol(Pole, Pole);
bool czyOdwiedzony(Pole);
double **przypiszWartości(double**);
double obliczF(Pole, Pole);
double **naprawG(double**);

int main(void)
{

	int wym2 = 20;
	int wym1 = 20;
	int rows = wym2 + 1;
	Pole start = Pole();
	start.x = 19;
	start.y = 0;
	start.wartość = 3;
	Pole cel = Pole();
	cel.x = 0;
	cel.y = 19;
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
	G = naprawG(G);
	while (aktualny.x != cel.x && aktualny.y != cel.y)
	{
		aktualny = znajdzNastepny(G, cel, start);
	}
	G = przypiszWartości(G);


	for (int i = 0; i<wym2; i++)
	{
		for (int j = 0; j<wym1; j++)
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
		pomoc = pomocnicza.back();
		if (porownanieDwochPol(pole, pomoc))
			return 1;
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
Pole znajdzNajlepszy(double** G, Pole start, Pole cel)
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
				if(G[pomoc.x][pomoc.y]!=5)
					najlepszy = pomoc;
			}
			lista_otwarta.pop_front();
		}
		if (najlepszy.x == cel.x && najlepszy.y == cel.y)
			return cel;
		else
		{
			return najlepszy;
		}
	}
	else 
	{
		return start;
	}
}
Pole znajdzNastepny(double** G, Pole cel, Pole start)
{
	Pole aktualny = lista_zamknieta.back();
	Pole nastepny = Pole();
	if (aktualny.y - 1 >= 0)
	{
		Pole nastepny1 = Pole();
		nastepny1.x = aktualny.x;
		nastepny1.y = aktualny.y - 1;
		nastepny1.f = obliczF(nastepny1, cel);
		nastepny1.x_rodzica = aktualny.x;
		nastepny1.y_rodzica = aktualny.y;
		if (!czyOdwiedzony(nastepny1) && G[nastepny1.y][nastepny1.x]!=5)
		{
			lista_otwarta.push_back(nastepny1);
		}

	}
	if (aktualny.y + 1 < 20)
	{
		Pole nastepny2 = Pole();
		nastepny2.x = aktualny.x;
		nastepny2.y = aktualny.y + 1;
		nastepny2.x_rodzica = aktualny.x;
		nastepny2.y_rodzica = aktualny.y;
		nastepny2.f = obliczF(nastepny2, cel);
		if (!czyOdwiedzony(nastepny2) && G[nastepny2.y][nastepny2.x] != 5)
		{
			lista_otwarta.push_back(nastepny2);
		}
	}
	if (aktualny.x - 1 >= 0)
	{
		Pole nastepny3 = Pole();
		nastepny3.x = aktualny.x - 1;
		nastepny3.y = aktualny.y;
		nastepny3.x_rodzica = aktualny.x;
		nastepny3.y_rodzica = aktualny.y;
		nastepny3.f = obliczF(nastepny3, cel);
		if (!czyOdwiedzony(nastepny3) && G[nastepny3.y][nastepny3.x] != 5)
		{
			lista_otwarta.push_back(nastepny3);
		}
	}
	if (aktualny.x + 1 < 20)
	{
		Pole nastepny4 = Pole();
		nastepny4.x = aktualny.x + 1;
		nastepny4.y = aktualny.y;
		nastepny4.x_rodzica = aktualny.x;
		nastepny4.y_rodzica = aktualny.y;
		nastepny4.f = obliczF(nastepny4, cel);
		if (!czyOdwiedzony(nastepny4) && G[nastepny4.y][nastepny4.x] != 5)
		{
			lista_otwarta.push_back(nastepny4);
		}
	}
	if (!lista_otwarta.empty())
	{
		nastepny = znajdzNajlepszy(G, start, cel);
		nastepny.wartość = 3;
		lista_zamknieta.push_back(nastepny);
		return nastepny;
	}
	else if(znajdzNajlepszy(G, start, cel).x == cel.x && znajdzNajlepszy(G, start, cel).y == cel.y)
	{
		cel.wartość = 3;
		lista_zamknieta.push_back(cel);
		return cel;
	}
	else if(znajdzNajlepszy(G, start, cel).x == start.x && znajdzNajlepszy(G, start, cel).y == start.y)
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
		pole = lista_zamknieta.front();
		G[pole.y][pole.x] = 3;
		lista_zamknieta.pop_front();
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
double **naprawG(double** G)
{
	for  (int i = 0; i < 20;  i++)
	{
		for (int j = 0; j <20; j++)
		{
			G[i][j] = G[i + 1][j + 1];
		}
	}
	return G;
}
