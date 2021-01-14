#include "stdafx.h"
#include "Silnik.h"

using namespace sf;
using namespace std;

/** @fn emiteryDzwieku
* 
* Funkcja emiteryDzwieku skanuje wszystkie elementy poziomu, tablicaPoziomu, i decyduje
* gdzie umie�ci� emitery. Wynik analizy b�dzie przechowywany w m_EmiteryOgnia
* @param vEmiteryDzwieku wektor z obiekt�w Vector2f zawieraj�cy lokalizacj� emiter�w d�wi�ku w danym poziomie
* @param tablicaPoziomu wska�nik do dwuwymiarowej tablicy warto�ci int, kt�ra zawiera uk�ad naszego poziomu
*/
void Silnik::emiteryDzwieku(
	vector <Vector2f>& vEmiteryDzwieku, int** tablicaPoziomu)
{
	vEmiteryDzwieku.empty(); // Opr�nienie wektora vEmiteryDzwieku

	FloatRect poprzedniEmiter; // Obiekt typu FloatRect, b�dzie nas upewnia� �e w danym czworok�cie (300 an 300 pikseli = ROZMIAR_PLYTKI * 6) znajduje si� tylko jeden emiter, �eby nie by�o ich za wiele

	for (int x = 0; x < (int)m_MP.getRozmiarPoziomu().x; x++) // Zagnie�dzone p�tle for skanuj� poziom w poszukiwaniu �r�de� ognia
	{
		for (int y = 0; y < (int)m_MP.getRozmiarPoziomu().y; y++)
		{
			if (tablicaPoziomu[y][x] == 2) // Je�li znaleziono blok ognia
			{
				if (!FloatRect(x * ROZMIAR_PLYTKI, // Sprawdzenie, �eby odkryty blok ognia nie by� ju� w kontakcie z utworzonym emiterem
					y * ROZMIAR_PLYTKI,
					ROZMIAR_PLYTKI,
					ROZMIAR_PLYTKI).intersects(poprzedniEmiter))
				{
					vEmiteryDzwieku.push_back( // Je�li w pobli�u danego �r�d�a nie ma innych emiter�w, tworzymy nowy
						Vector2f(x * ROZMIAR_PLYTKI, y * ROZMIAR_PLYTKI));

					
					// Zaktualizowanie obiektu FloatRect poprzedniEmiter �eby unikn�� skupisk emiter�w. 
					poprzedniEmiter.left = x * ROZMIAR_PLYTKI;
					poprzedniEmiter.top = y * ROZMIAR_PLYTKI;
					poprzedniEmiter.width = ROZMIAR_PLYTKI * 6; // 300 pikseli
					poprzedniEmiter.height = ROZMIAR_PLYTKI * 6; // na 300 pikseli
				}

			}

		}

	}
	return;

}