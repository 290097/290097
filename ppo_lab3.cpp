#include <iostream>
#include <string>

using namespace std;

class Osoba {
private:
    string imie;
    string nazwisko;
    int indeks;

public:
    Osoba() : imie(""), nazwisko(""), indeks(0) {}

    bool ustawDane(string i, string n, int ind) {
        if (i.length() < 3 || n.length() < 3) {
            return false;
        }
        imie = i;
        nazwisko = n;
        indeks = ind;
        return true;
    }

    string getNazwisko() { return nazwisko; }

    void wyswietl() {
        cout << imie << " " << nazwisko << " (Indeks: " << indeks << ")";
    }
};

class ListaObecnosci {
private:
    Osoba** tabOsob; 
    bool* obecnosc;
    int ilosc;
    int maxOsob;

public:
    ListaObecnosci() : ilosc(0), maxOsob(10) {
        tabOsob = new Osoba * [maxOsob];
        obecnosc = new bool[maxOsob];
    }

    void dodajDoListy(Osoba* os) {
        if (ilosc < maxOsob) {
            tabOsob[ilosc] = os;
            obecnosc[ilosc] = false;
            ilosc++;
        }
    }

    void zmienStatus(string nazwisko, bool stan) {
        for (int i = 0; i < ilosc; i++) {
            if (tabOsob[i]->getNazwisko() == nazwisko) {
                obecnosc[i] = stan;
                cout << "Zmieniono status obecnosci dla: " << nazwisko << endl;
                return;
            }
        }
        cout << "Nie znaleziono takiej osoby na tej liscie!" << endl;
    }

    void drukuj() {
        if (ilosc == 0) cout << "(lista pusta)" << endl;
        for (int i = 0; i < ilosc; i++) {
            tabOsob[i]->wyswietl();
            cout << " -> " << (obecnosc[i] ? "OBECNY" : "NIEOBECNY") << endl;
        }
    }
};

class InterfejsUzytkownika {
private:
    Osoba* tablicaOsob;
    int aktualnaIloscOsob;
    int maxOsob;

    ListaObecnosci* tablicaList;
    int aktualnaIloscList;
    int maxList;

public:
    InterfejsUzytkownika(Osoba* tO, int nO, ListaObecnosci* tL, int nL)
        : tablicaOsob(tO), maxOsob(nO), aktualnaIloscOsob(0),
        tablicaList(tL), maxList(nL), aktualnaIloscList(0) {
    }

    void loop() {
        int wybor;
        do {
            cout << "1. Dodaj osobe" << endl;
            cout << "2. Dodaj liste obecnosci" << endl;
            cout << "3. Dodaj osobe do listy" << endl;
            cout << "4. Ustaw obecnosc osoby na liscie" << endl;
            cout << "5. Usun osobe (z bazy)" << endl;
            cout << "6. Zmien dane osoby" << endl;
            cout << "7. Podglad bazy i list" << endl;
            cout << "0. Wyjscie" << endl;
            cout << "Wybor: "; cin >> wybor;

            switch (wybor) {
            case 1: {
                if (aktualnaIloscOsob < maxOsob) {
                    string im, naz; int ind;
                    bool sukces = false;
                    do {
                        cout << "Imie: "; cin >> im;
                        cout << "Nazwisko: "; cin >> naz;
                        cout << "Indeks: "; cin >> ind;

                        sukces = tablicaOsob[aktualnaIloscOsob].ustawDane(im, naz, ind);
                        if (!sukces) {
                            cout << "Blad: imie lub nazwisko nie maja 3 znakow" << endl;
                        }
                    } while (!sukces); 

                    aktualnaIloscOsob++;
                } 
                else {
                    cout << "Baza pelna!" << endl;
                } 
                break;
            } 
          
            case 2: { 
                if (aktualnaIloscList < maxList) {
                    aktualnaIloscList++;
                    cout << "Aktywowano liste nr " << aktualnaIloscList << endl;
                }
                else cout << "Limit list wyczerpany!" << endl;
                break;
            }
            case 3: { 
                int nrO, nrL;
                cout << "Nr osoby (1-" << aktualnaIloscOsob << "): "; cin >> nrO;
                cout << "Nr listy (1-" << aktualnaIloscList << "): "; cin >> nrL;
                if (nrO > 0 && nrO <= aktualnaIloscOsob && nrL > 0 && nrL <= aktualnaIloscList) {
                    tablicaList[nrL - 1].dodajDoListy(&tablicaOsob[nrO - 1]);
                }
                break;
            }
            case 4: { 
                int nrL; string naz; int stan;
                cout << "Nr listy: "; cin >> nrL;
                cout << "Nazwisko osoby: "; cin >> naz;
                cout << "Obecny? (1-Tak, 0-Nie): "; cin >> stan;
                if (nrL > 0 && nrL <= aktualnaIloscList) {
                    tablicaList[nrL - 1].zmienStatus(naz, stan == 1);
                }
                break;
            }
            case 5: { 
                string naz;
                cout << "Nazwisko osoby do usuniecia: "; cin >> naz;
                for (int i = 0; i < aktualnaIloscOsob; i++) {
                    if (tablicaOsob[i].getNazwisko() == naz) {
                        for (int j = i; j < aktualnaIloscOsob - 1; j++) {
                            tablicaOsob[j] = tablicaOsob[j + 1];
                        }
                        aktualnaIloscOsob--;
                        cout << "Osoba usunieta" << endl;
                        break;
                    }
                }
                break;
            }
            case 6: { 
                string klucz;
                cout << "Podaj nazwisko osoby do edycji: "; cin >> klucz;
                for (int i = 0; i < aktualnaIloscOsob; i++) {
                    if (tablicaOsob[i].getNazwisko() == klucz) {
                        string ni, nn; int nind;
                        bool sukces = false;
                        do {
                            cout << "Nowe imie (min. 3 znaki): "; cin >> ni;
                            cout << "Nowe nazwisko (min. 3 znaki): "; cin >> nn;
                            cout << "Nowy indeks: "; cin >> nind;
                            sukces = tablicaOsob[i].ustawDane(ni, nn, nind);
                            if (!sukces) cout << "Blad: Za krotkie dane!" << endl;
                        } while (!sukces);
                        cout << "Dane zaktualizowane!" << endl;
                        break;
                    }
                }
                break;
            }
            case 7: { 
                for (int i = 0; i < aktualnaIloscList; i++) {
                    cout << "\nLISTA " << i + 1 << ":" << endl;
                    tablicaList[i].drukuj();
                }
                break;
            }
            }
        } while (wybor != 0);
    }
};

int main() {
    Osoba tabO[10];
    ListaObecnosci tabL[2];
    InterfejsUzytkownika ui(tabO, 10, tabL, 2);
    ui.loop();
    return 0;
}