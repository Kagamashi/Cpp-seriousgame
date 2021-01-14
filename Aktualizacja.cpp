#include "stdafx.h"
#include "Silnik.h"
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;

/** @fn aktualizacja
*
*Jedna z funkcji prywatnych serca naszego programu czyli Silnika.
*Odpowiedzialna za aktualizacje danych i stanu gry w oparciu o czas w sekundach
*kt�ry up�yn�� od ostatniej klatki. 
*
* @param czSekundy warto�� ile czasu w sekundach trwa�a poprzednia ramka
*/
void Silnik::aktualizacja(float czSekundy) 
{
	if (m_NowyPoziomWymagany)
	{
		zaladujPoziom(); 
	}

	if (m_Gra)
	{
		m_Tom.aktualizacja(czSekundy); // Aktualizacja postaci Tom'a na podstawie czasu kt�ry up�yn�� od ostatniej ramki

		m_Jerry.aktualizacja(czSekundy); // Aktualizacja postaci Jerry'ego na podstawie czasu kt�ry up�yn�� od ostatniej ramki

		if (wykryjKolizje(m_Tom) && wykryjKolizje(m_Jerry)) // Sprawdza mo�liwe kolizje i wykrywa czy postacie osi�gn�y met�.
		{
			m_NowyPoziomWymagany = true; // Je�li postacie osi�gn�y met� to trzeba za�adowa� nowy poziom
			m_MD.odtworzZwyciestwo(); //  Odtw�z d�wi�k zwyci�stwa
		}
		else // Je�li Tom trafi� do mety, a Jerry nie sprawdzamy ponownie tym razem tylko dla Jerry'ego
		{
			wykryjKolizje(m_Jerry); // Wykonuje funkcj� wykryjKolizje dla Jerry'ego
		}

		
		// Umo�liwienie postaci Tom'a i Jerry'ego na skakanie po swoich g�owach nawzajem.
		if (m_Jerry.getStopy().intersects(m_Tom.getGlowa()))
		{
			m_Jerry.zatrzymajSpadanie(m_Tom.getGlowa().top); // Je�li stopy Jerryego zderz� si� z g�ow� Toma, to Jerry przestaje spada� (przenika� przez Toma)
		}
		else if (m_Tom.getStopy().intersects(m_Jerry.getGlowa()))
		{
			m_Tom.zatrzymajSpadanie(m_Jerry.getGlowa().top); // Je�li stopy Tom'a zderz� si� z g�ow� Jerry'ego to Tom przestaje spada� 
		}

		m_PozostalyCzas -= czSekundy; // Zliczanie w d� czasu, kt�ry pozosta� graczowi na uko�czenie poziomu

		
		// Je�li Tom'owi i Jerry'emu sko�czy� si� czas na uko�czenie poziomu to trzeba za�adowa� nowy 
		if (m_PozostalyCzas <= 0)
		{
			m_NowyPoziomWymagany = true;
		}
	}

	vector<Vector2f>::iterator it; // Sprawdzenie czy ma by� odegrany d�wi�k ognia

	for (it = m_EmiteryOgnia.begin(); it != m_EmiteryOgnia.end(); it++) // Iteracja przez wektor obiekt�w Vector2f w poszukiwaniu emiter�w
	{
		float pozX = (*it).x; // Zmienna typu float, przechowuje poziom� wsp�rz�dne emitera
		float pozY = (*it).y; // Zmienna typu float, przechowuje pionowe wsp�rz�dne emitera

		FloatRect localRect(pozX - 250, pozY - 250, 500, 500); // Obiekt FloatRect, sprawdza czy emiter znajduje si� blisko postaci, sprawdzamy dla Tom'a(utworzenie kwadratu 500pikseli na 500 wok� emitera)

		if (m_Tom.getPozycja().intersects(localRect)) // Sprawdzamy czy posta� znajduje si� wewn�trz utworzonego kwadratu localRect
		{
			m_MD.odtworzOgien(Vector2f(pozX, pozY), m_Tom.getSrodekPostaci()); // Odtworzenie d�wi�ku ognia i przekazanie lokalizacji
		}
	}

	if (m_SplitScreen)
	{
		m_WidokLewy.setCenter(m_Tom.getSrodekPostaci()); // Ustawienie skupienia na postaci Tom'a w lewej cz�ci ekranu
		m_WidokPrawy.setCenter(m_Jerry.getSrodekPostaci()); // Ustawienie skupienia na postaci Jerry'ego w prawej cz�ci ekranu
	}
	else //tryb pe�noekranowy
	{
		if (m_Postac1) 
		{
			m_WidokGlowny.setCenter(m_Tom.getSrodekPostaci()); // Je�li aktualnie sterowana jest posta� Tom to skupienie na niej
		}
		else
		{
			m_WidokGlowny.setCenter(m_Jerry.getSrodekPostaci()); // Je�li sterowana jest posta� Jerry to skupienie na niej
		}
	}

	m_RamkiOdOstatniejAktualizacjiHUD++; // Warto�� ramek od ostatniej aktulizacji jest zwi�kszana po ka�dej ramce

	if (m_RamkiOdOstatniejAktualizacjiHUD > m_DoceloweRamkiNaAktualizacjeHUD) // Aktualizacja HUD po up�ywie m_DoceloweRamkiNaAktualizacje (500 ramek)
	{
		stringstream ssCzas; // Objekt stringstream do aktualizacji czasu
		stringstream ssPoziom; // Obiekt stringstream do aktualizacji numeru poziomu

		ssCzas << (int)m_PozostalyCzas; // Pobranie pozosta�ego czasu na uko�czenie poziomu
		m_Hud.ustawCzas(ssCzas.str());

		ssPoziom << "Poziom:" << m_MP.getAktualnyPoziom(); // Pobranie aktualnego numeru poziomu
		m_Hud.ustawPoziom(ssPoziom.str());

		m_RamkiOdOstatniejAktualizacjiHUD = 0; // Wyzerowanie liczby ramek od ostatniej aktualizacji. 
	}
}