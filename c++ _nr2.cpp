//POP_2017_01_25_projekt_2_Getek_Filip_EIT_2_165720.cpp
//Visual Studio 2015

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <fstream>
#include <vector>
#include <iterator>

template<class T>
char* as_bytes(T& i)
{
	void* addr = &i;
	return static_cast<char*>(addr);
}
void kopiuj(char* pwej)
{
	std::ifstream input(pwej, std::ios::binary);

	if (!input.is_open())
	{
		std::cout << "Blad otwarcia pliku wejsciowego" << std::endl;
		return;
	}

	std::vector<char> bufor;

	char n;
	while ((input.read(as_bytes(n), 1)) && (bufor.size() < 54))
		bufor.push_back(n);
	
	// odczytanie wielkosci pliku, przesuniecia, szerokosci, wysokosci buforu
	int bfSize = *reinterpret_cast<int*>((char*)bufor.data() + 2);
	int bfOffBits = *reinterpret_cast<int*>((char*)bufor.data() + 10);
	int biWidth = *reinterpret_cast<int*>((char*)bufor.data() + 18);
	int biHeight = *reinterpret_cast<int*>((char*)bufor.data() + 22);

	int ilbobr = bfSize - bfOffBits;		//ilosc bajtow bitmapy
	int ilbwyr = 4 - ((3 * biWidth) % 4);	//ilosc bajtow wyrownujacych
	int ilbwier = 3 * biWidth + ilbwyr;		//ilosc bajtow w wierszu

	std::cout << "Rozmiar naglowka: " << bufor.size() << " bajtow " << std::endl;
	std::cout << "Rozmiar bitmapy:" << ilbobr << "  " << std::endl;
	std::cout << "Szerokosc bitmapy:" << biWidth << std::endl;
	std::cout << "Wysokosc bitmapy:" << biHeight << std::endl;
	std::cout << "Ilosc bajtow wyrownujacych w wierszu:" << ilbwyr << std::endl;
	std::cout << "Ilosc bajtow w wierszu: " << ilbwier << std::endl;
	
	std::ofstream pwyj("sonar2.bmp"); // Utworzenie pliku wyjœciowego 2

	std::ostream_iterator<char> output_iterator(pwyj);
	std::copy(bufor.begin(), bufor.end(), output_iterator); // zapis naglowka do pliku 2

	for (int w = 0; w <= biHeight - 1; w++) {
		bufor.resize(0);
		input.seekg(54 + w*ilbwier);
		while ((bufor.size() < ilbwier) && (input.read(as_bytes(n), 1)))
			bufor.push_back(n);
		int korekta = 3;
		int przesuniecie = 0;
		for (long i = 3 * biWidth - 1; i >= 3 * przesuniecie; i--) {
			switch (i % 3)
			{
			case 0:		//  B piksel	
				if (bufor[i] >= 172) {
					if (bufor[i] < (255 - korekta)) { bufor[i] = bufor[i - 3 * przesuniecie] + korekta; }
					else { bufor[i] = 255; } break;
				}
				else { bufor[i] = 0; break; }
			case 1:		// G piksel
				if (bufor[i] >= 171) { bufor[i] = 255; break; }
				if (bufor[i] >= 87 && bufor[i] <= 170) { bufor[i] = (bufor[i - 3 * przesuniecie] + korekta); break; }
				if (bufor[i] < 87) { bufor[i] = 0; break; }
			case 2:		// R piksel
				if (bufor[i] >= 86) { bufor[i] = 255; break; }
				if (bufor[i] >= 1 && bufor[i] <= 85) { bufor[i] = (bufor[i - 3 * przesuniecie] + korekta); break; }
			}
		}

		bufor.resize(ilbwier);
		pwyj.seekp(54 + w*ilbwier);
		std::copy(bufor.begin(), bufor.end(), output_iterator); // zapis wiersza do pliku 2
	}

}

int main(int argc, char**argv)
{
	kopiuj(argv[1]);
}