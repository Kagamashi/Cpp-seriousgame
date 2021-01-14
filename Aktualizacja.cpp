#include "stdafx.h"
#include "Silnik.h"
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;

/** @fn aktualizacja
*
*Jedna z funkcji prywatnych serca naszego programu czyli Silnika.
*Odpowiedzialna za aktualizacje danych i stanu gry w oparciu o czas w sekundach
*który up³yn¹³ od ostatniej klatki. 
*
* @param czSekundy wartoœæ ile czasu w sekundach trwa³a poprzednia ramka
*/
void Silnik::aktualizacja(float czSekundy) 
{
	if (m_NowyPoziomWymagany)
	{
		zaladujPoziom(); 
	}

	if (m_Gra)
	{
		m_Tom.aktualizacja(czSekundy); // Aktualizacja postaci Tom'a na podstawie czasu który up³yn¹³ od ostatniej ramki

		m_Jerry.aktualizacja(czSekundy); // Aktualizacja postaci Jerry'ego na podstawie czasu który up³yn¹³ od ostatniej ramki

		if (wykryjKolizje(m_Tom) && wykryjKolizje(m_Jerry)) // Sprawdza mo¿liwe kolizje i wykrywa czy postacie osi¹gnê³y metê.
		{
			m_NowyPoziomWymagany = true; // Jeœli postacie osi¹gnê³y metê to trzeba za³adowaæ nowy poziom
			m_MD.odtworzZwyciestwo(); //  Odtwóz dŸwiêk zwyciêstwa
		}
		else // Jeœli Tom trafi³ do mety, a Jerry nie sprawdzamy ponownie tym razem tylko dla Jerry'ego
		{
			wykryjKolizje(m_Jerry); // Wykonuje funkcjê wykryjKolizje dla Jerry'ego
		}

		
		// Umo¿liwienie postaci Tom'a i Jerry'ego na skakanie po swoich g³owach nawzajem.
		if (m_Jerry.getStopy().intersects(m_Tom.getGlowa()))
		{
			m_Jerry.zatrzymajSpadanie(m_Tom.getGlowa().top); // Jeœli stopy Jerryego zderz¹ siê z g³ow¹ Toma, to Jerry przestaje spadaæ (przenikaæ przez Toma)
		}
		else if (m_Tom.getStopy().intersects(m_Jerry.getGlowa()))
		{
			m_Tom.zatrzymajSpadanie(m_Jerry.getGlowa().top); // Jeœli stopy Tom'a zderz¹ siê z g³ow¹ Jerry'ego to Tom przestaje spadaæ 
		}

		m_PozostalyCzas -= czSekundy; // Zliczanie w dó³ czasu, który pozosta³ graczowi na ukoñczenie poziomu

		
		// Jeœli Tom'owi i Jerry'emu skoñczy³ siê czas na ukoñczenie poziomu to trzeba za³adowaæ nowy 
		if (m_PozostalyCzas <= 0)
		{
			m_NowyPoziomWymagany = true;
		}
	}

	vector<Vector2f>::iterator it; // Sprawdzenie czy ma byæ odegrany dŸwiêk ognia

	for (it = m_EmiteryOgnia.begin(); it != m_EmiteryOgnia.end(); it++) // Iteracja przez wektor obiektów Vector2f w poszukiwaniu emiterów
	{
		float pozX = (*it).x; // Zmienna typu float, przechowuje poziom¹ wspó³rzêdne emitera
		float pozY = (*it).y; // Zmienna typu float, przechowuje pionowe wspó³rzêdne emitera

		FloatRect localRect(pozX - 250, pozY - 250, 500, 500); // Obiekt FloatRect, sprawdza czy emiter znajduje siê blisko postaci, sprawdzamy dla Tom'a(utworzenie kwadratu 500pikseli na 500 wokó³ emitera)

		if (m_Tom.getPozycja().intersects(localRect)) // Sprawdzamy czy postaæ znajduje siê wewn¹trz utworzonego kwadratu localRect
		{
			m_MD.odtworzOgien(Vector2f(pozX, pozY), m_Tom.getSrodekPostaci()); // Odtworzenie dŸwiêku ognia i przekazanie lokalizacji
		}
	}

	if (m_SplitScreen)
	{
		m_WidokLewy.setCenter(m_Tom.getSrodekPostaci()); // Ustawienie skupienia na postaci Tom'a w lewej czêœci ekranu
		m_WidokPrawy.setCenter(m_Jerry.getSrodekPostaci()); // Ustawienie skupienia na postaci Jerry'ego w prawej czêœci ekranu
	}
	else //tryb pe³noekranowy
	{
		if (m_Postac1) 
		{
			m_WidokGlowny.setCenter(m_Tom.getSrodekPostaci()); // Jeœli aktualnie sterowana jest postaæ Tom to skupienie na niej
		}
		else
		{
			m_WidokGlowny.setCenter(m_Jerry.getSrodekPostaci()); // Jeœli sterowana jest postaæ Jerry to skupienie na niej
		}
	}

	m_RamkiOdOstatniejAktualizacjiHUD++; // Wartoœæ ramek od ostatniej aktulizacji jest zwiêkszana po ka¿dej ramce

	if (m_RamkiOdOstatniejAktualizacjiHUD > m_DoceloweRamkiNaAktualizacjeHUD) // Aktualizacja HUD po up³ywie m_DoceloweRamkiNaAktualizacje (500 ramek)
	{
		stringstream ssCzas; // Objekt stringstream do aktualizacji czasu
		stringstream ssPoziom; // Obiekt stringstream do aktualizacji numeru poziomu

		ssCzas << (int)m_PozostalyCzas; // Pobranie pozosta³ego czasu na ukoñczenie poziomu
		m_Hud.ustawCzas(ssCzas.str());

		ssPoziom << "Poziom:" << m_MP.getAktualnyPoziom(); // Pobranie aktualnego numeru poziomu
		m_Hud.ustawPoziom(ssPoziom.str());

		m_RamkiOdOstatniejAktualizacjiHUD = 0; // Wyzerowanie liczby ramek od ostatniej aktualizacji. 
	}
}