#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib> // Para usar rand()
#include <ctime>   // Para usar time()

#define TAMANO 40

using namespace std;
using namespace std::chrono;

// Función que devuelve un 0 o 1 aleatorio
int obtenerAleatorio()
{
    return rand() % 2; // rand() genera un número aleatorio, % 2 asegura que sea 0 o 1
}

class cell
{
private:
    bool status;

public:
    cell(bool s = false) : status(s) {} // Constructor con valor por defecto

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
        return status ? '*' : ' '; // Más visual
    }
};

class tablero
{
private:
    cell tab[TAMANO][TAMANO];

public:
    tablero()
    {
        for (int i = 0; i < TAMANO; i++)
            for (int j = 0; j < TAMANO; j++)
                tab[i][j] = cell(false);

        for (int i = 1; i < TAMANO - 1; i++)
            for (int j = 1; j < TAMANO - 1; j++)
                tab[i][j] = obtenerAleatorio();
    }

    void putcell(int i, int j)
    {
        if (i >= 0 && i < TAMANO && j >= 0 && j < TAMANO)
            tab[i][j].change_status();
    }

    void imprimir_tablero() const
    {
        string prtTablero;
        prtTablero.reserve(TAMANO * (TAMANO + 1)); // +1 por cada salto de línea
        for (int i = 0; i < TAMANO; i++)
        {
            for (int j = 0; j < TAMANO; j++)
                prtTablero += tab[i][j].get_status();
            prtTablero += '\n';
        }
        cout << prtTablero;
    }

    void revisar_tablero()
    {
        cell copia[TAMANO][TAMANO];
        for (int i = 0; i < TAMANO; ++i)
            for (int j = 0; j < TAMANO; ++j)
                copia[i][j] = tab[i][j];

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
                    tab[i][j].change_status();
                else if (!copia[i][j].get_status() && vivos == 3)
                    tab[i][j].change_status();
            }
        }
    }
};

int main()
{

    srand(static_cast<unsigned int>(time(0)));

    tablero T;

    // Inicialización: glider
    T.putcell(10, 10);
    T.putcell(11, 11);
    T.putcell(12, 9);
    T.putcell(12, 10);
    T.putcell(12, 11);

    while (true)
    {
        system("cls"); // Usa "clear" si estás en Linux/Mac
        T.imprimir_tablero();
        T.revisar_tablero();
        cin.get();
    }

    return 0;
}
