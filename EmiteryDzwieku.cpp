#include "stdafx.h"
#include "Silnik.h"

using namespace sf;
using namespace std;

/** @fn emiteryDzwieku
* 
* Funkcja emiteryDzwieku skanuje wszystkie elementy poziomu, tablicaPoziomu, i decyduje
* gdzie umieœciæ emitery. Wynik analizy bêdzie przechowywany w m_EmiteryOgnia
* @param vEmiteryDzwieku wektor z obiektów Vector2f zawieraj¹cy lokalizacjê emiterów dŸwiêku w danym poziomie
* @param tablicaPoziomu wskaŸnik do dwuwymiarowej tablicy wartoœci int, która zawiera uk³ad naszego poziomu
*/
void Silnik::emiteryDzwieku(
	vector <Vector2f>& vEmiteryDzwieku, int** tablicaPoziomu)
{
	vEmiteryDzwieku.empty(); // OpróŸnienie wektora vEmiteryDzwieku

	FloatRect poprzedniEmiter; // Obiekt typu FloatRect, bêdzie nas upewnia³ ¿e w danym czworok¹cie (300 an 300 pikseli = ROZMIAR_PLYTKI * 6) znajduje siê tylko jeden emiter, ¿eby nie by³o ich za wiele

	for (int x = 0; x < (int)m_MP.getRozmiarPoziomu().x; x++) // Zagnie¿dzone pêtle for skanuj¹ poziom w poszukiwaniu Ÿróde³ ognia
	{
		for (int y = 0; y < (int)m_MP.getRozmiarPoziomu().y; y++)
		{
			if (tablicaPoziomu[y][x] == 2) // Jeœli znaleziono blok ognia
			{
				if (!FloatRect(x * ROZMIAR_PLYTKI, // Sprawdzenie, ¿eby odkryty blok ognia nie by³ ju¿ w kontakcie z utworzonym emiterem
					y * ROZMIAR_PLYTKI,
					ROZMIAR_PLYTKI,
					ROZMIAR_PLYTKI).intersects(poprzedniEmiter))
				{
					vEmiteryDzwieku.push_back( // Jeœli w pobli¿u danego Ÿród³a nie ma innych emiterów, tworzymy nowy
						Vector2f(x * ROZMIAR_PLYTKI, y * ROZMIAR_PLYTKI));

					
					// Zaktualizowanie obiektu FloatRect poprzedniEmiter ¿eby unikn¹æ skupisk emiterów. 
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