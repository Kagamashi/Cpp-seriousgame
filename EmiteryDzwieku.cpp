#include "stdafx.h"
#include "Silnik.h"

using namespace sf;
using namespace std;

/** @fn emiteryDzwieku
* 
* Funkcja emiteryDzwieku skanuje wszystkie elementy poziomu, tablicaPoziomu, i decyduje
* gdzie umieścić emitery. Wynik analizy będzie przechowywany w m_EmiteryOgnia
* @param vEmiteryDzwieku wektor z obiektów Vector2f zawierający lokalizację emiterów dźwięku w danym poziomie
* @param tablicaPoziomu wskaźnik do dwuwymiarowej tablicy wartości int, która zawiera układ naszego poziomu
*/
