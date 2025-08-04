#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

int obtenerAleatorio()
{
    return rand() % 2;
}

class cell
{
private:
    bool status;

public:
    cell(bool s = false) : status(s) {}

    int get_status() const
    {
        return status;
    }

    void change_status()
    {
        status = !status;
    }

    char imprimir_cell() const
    {
        return status ? '*' : ' ';
    }
};

class tablero
{
private:
    int TAMANO = 40;
    vector<vector<cell>> tab;

public:
    tablero()
    {
        tab = vector<vector<cell>>(TAMANO, vector<cell>(TAMANO, cell(false)));

       
        for (int i = 10; i < TAMANO - 10; i++)
            for (int j = 10; j < TAMANO - 10; j++)
                if (obtenerAleatorio())
                    tab[i][j].change_status();
    }

    void putcell(int i, int j)
    {
        if (i >= 0 && i < TAMANO && j >= 0 && j < TAMANO)
            tab[i][j].change_status();
    }

    void imprimir_tablero() const
    {
        string prtTablero;
        prtTablero.reserve(TAMANO * (TAMANO + 1));
        for (int i = 0; i < TAMANO; i++)
        {
            for (int j = 0; j < TAMANO; j++){
                if(i==0 || j==0 || i==TAMANO-1|| j==TAMANO-1 ){
                    prtTablero += '#';
                    continue;
                }
                prtTablero += tab[i][j].imprimir_cell();
            }    
            prtTablero += '\n';
        }
        cout << prtTablero;
    }

    void revisar_tablero()
    {
        vector<vector<cell>> copia = tab;

        for (int i = 1; i < TAMANO - 1; i++)
        {
            for (int j = 1; j < TAMANO - 1; j++)
            {
                int vivos = 0;
                vivos += copia[i + 1][j].get_status();
                vivos += copia[i - 1][j].get_status();
                vivos += copia[i + 1][j + 1].get_status();
                vivos += copia[i - 1][j + 1].get_status();
                vivos += copia[i + 1][j - 1].get_status();
                vivos += copia[i - 1][j - 1].get_status();
                vivos += copia[i][j + 1].get_status();
                vivos += copia[i][j - 1].get_status();

                if (copia[i][j].get_status() && (vivos < 2 || vivos > 3))
                {
                    tab[i][j].change_status(); 
                }
                else if (!copia[i][j].get_status() && vivos == 3)
                {
                    tab[i][j].change_status(); 
                }
            }
        }
    }

    bool borde_activo() const
    {
        for (int i = 0; i < TAMANO; i++)
        {
            if (tab[i][1].get_status() || tab[i][TAMANO - 2].get_status() ||
                tab[1][i].get_status() || tab[TAMANO - 2][i].get_status())
            {
                return true;
            }
        }
        return false;
    }

void ampliar_tablero()
{
    int oldTAMANO = TAMANO;
    TAMANO += 2;
    vector<vector<cell>> copia(TAMANO, vector<cell>(TAMANO, cell(false)));

    for (int i = 0; i < oldTAMANO; ++i)
        for (int j = 0; j < oldTAMANO; ++j)
            copia[i + 1][j + 1] = tab[i][j];

    tab = copia;
}
};

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    tablero T;
    T.imprimir_tablero();

    while (true)
    {
        system("cls"); 
        T.imprimir_tablero();
        T.revisar_tablero();
         if (T.borde_activo())
             T.ampliar_tablero();
        cin.get();
    }

    return 0;
}
