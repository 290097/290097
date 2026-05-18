#include <iostream>
#include <string>

using namespace std;

class IDrukowalny {
public:
    virtual string przedstawSie() = 0; 
    virtual ~IDrukowalny() {}          
};

void drukuj(IDrukowalny* obj) {
    cout << obj->przedstawSie();
}

class Osoba : public IDrukowalny {
protected:
    string imie;
    string nazwisko;

public:
    Osoba() : imie(""), nazwisko("") {}
    virtual ~Osoba() {}

    virtual bool ustawDane(string i, string n) {
        if (i.length() < 3 || n.length() < 3) {
            return false;
        }
        imie = i;
        nazwisko = n;
        return true;
    }

    string getNazwisko() { return nazwisko; }

    
    string przedstawSie() override {
        return imie + " " + nazwisko;
    }

    virtual void drukuj() {
        cout << przedstawSie();
    }
};

class Student : public Osoba {
private:
    int indeks;
public:
    Student() : Osoba(), indeks(0) {}

    void ustawIndeks(int ind) { indeks = ind; }

    
    string przedstawSie() override {
        return Osoba::przedstawSie() + " [Student, Indeks: " + to_string(indeks) + "]";
    }

    void drukuj() override {
        cout << przedstawSie();
    }
};


class Pracownik : public Osoba {
private:
    int id_pracownika;
public:
    Pracownik() : Osoba(), id_pracownika(0) {}

    void ustawID(int id) { id_pracownika = id; }

    
    string przedstawSie() override {
        return Osoba::przedstawSie() + " [Pracownik, ID: " + to_string(id_pracownika) + "]";
    }

    void drukuj() override {
        cout << przedstawSie();
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

    
    ~ListaObecnosci() {
        delete[] tabOsob;
        delete[] obecnosc;
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
            ::drukuj(tabOsob[i]);
            cout << " -> " << (obecnosc[i] ? "OBECNY" : "NIEOBECNY") << endl;
        }
    }
};

class InterfejsUzytkownika {
private:
    Osoba** tablicaOsob;
    int aktualnaIloscOsob;
    int maxOsob;

    ListaObecnosci* tablicaList;
    int aktualnaIloscList;
    int maxList;

public:
    InterfejsUzytkownika(Osoba** tO, int nO, ListaObecnosci* tL, int nL)
        : tablicaOsob(tO), maxOsob(nO), aktualnaIloscOsob(0),
        tablicaList(tL), maxList(nL), aktualnaIloscList(0) {
    }

    void loop() {
        int wybor;
        do {
            cout << "\n1. Dodaj osobe" << endl;
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
                    int typ;
                    cout << "Kogo chcesz dodac? (1 - Student, 2 - Pracownik): ";
                    cin >> typ;

                    if (typ != 1 && typ != 2) {
                        cout << "Niepoprawny wybor typu!" << endl;
                        break;
                    }

                    string im, naz;
                    bool sukces = false;
                    Osoba* nowaOsoba = nullptr;

                    do {
                        cout << "Imie: "; cin >> im;
                        cout << "Nazwisko: "; cin >> naz;

                        if (typ == 1) {
                            int ind;
                            cout << "Indeks: "; cin >> ind;
                            Student* s = new Student();
                            if (s->ustawDane(im, naz)) {
                                s->ustawIndeks(ind);
                                nowaOsoba = s;
                                sukces = true;
                            }
                            else {
                                delete s;
                            }
                        }
                        else if (typ == 2) {
                            int id;
                            cout << "ID pracownika: "; cin >> id;
                            Pracownik* p = new Pracownik();
                            if (p->ustawDane(im, naz)) {
                                p->ustawID(id);
                                nowaOsoba = p;
                                sukces = true;
                            }
                            else {
                                delete p;
                            }
                        }

                        if (!sukces) {
                            cout << "Blad: imie lub nazwisko za krotkie (min. 3 znaki)" << endl;
                        }
                    } while (!sukces);

                    tablicaOsob[aktualnaIloscOsob] = nowaOsoba;
                    aktualnaIloscOsob++;
                    cout << "Pomyslnie dodano osobe do bazy!" << endl;
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
                    tablicaList[nrL - 1].dodajDoListy(tablicaOsob[nrO - 1]);
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
                    if (tablicaOsob[i]->getNazwisko() == naz) {
                        delete tablicaOsob[i]; 
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
                    if (tablicaOsob[i]->getNazwisko() == klucz) {
                        string ni, nn;
                        bool sukces = false;
                        do {
                            cout << "Nowe imie (min. 3 znaki): "; cin >> ni;
                            cout << "Nowe nazwisko (min. 3 znaki): "; cin >> nn;

                            sukces = tablicaOsob[i]->ustawDane(ni, nn);

                            if (sukces) {
                                Student* s = dynamic_cast<Student*>(tablicaOsob[i]);
                                Pracownik* p = dynamic_cast<Pracownik*>(tablicaOsob[i]);

                                if (s) {
                                    int nind;
                                    cout << "Nowy indeks: "; cin >> nind;
                                    s->ustawIndeks(nind);
                                }
                                else if (p) {
                                    int nid;
                                    cout << "Nowe ID pracownika: "; cin >> nid;
                                    p->ustawID(nid);
                                }

                                cout << "Dane zaktualizowane!" << endl;
                            }
                            else {
                                cout << "Blad: Za krotkie dane!" << endl;
                            }
                        } while (!sukces);
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
    Osoba** tabO = new Osoba * [10];
    ListaObecnosci tabL[2];
    InterfejsUzytkownika ui(tabO, 10, tabL, 2);
    ui.loop();
    delete[] tabO;
    return 0;
}