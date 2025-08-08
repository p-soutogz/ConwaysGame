#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

int obtenerAleatorio() { return rand() % 2; }

class cell
{
private:
    bool status;

public:
    cell(bool s = false) : status(s) {}
    int get_status() const { return status; }
    void change_status() { status = !status; }
    char imprimir_cell() const { return status ? '*' : ' '; }
};

class tablero
{
private:
    int TAMANOX = 40;         // columnas (x)
    int TAMANOY = 20;         // filas (y)
    vector<vector<cell>> tab; // tab[y][x]

    // ---- helpers de ampliación/reducción por lado ----
    void ampliar_top()
    {
        int oldX = TAMANOX, oldY = TAMANOY;
        TAMANOY += 1;
        vector<vector<cell>> copia(TAMANOY, vector<cell>(TAMANOX, cell(false)));
        for (int i = 0; i < oldY; ++i)
            for (int j = 0; j < oldX; ++j)
                copia[i + 1][j] = tab[i][j];
        tab = copia;
    }
    void ampliar_bottom()
    {
        int oldX = TAMANOX, oldY = TAMANOY;
        TAMANOY += 1;
        vector<vector<cell>> copia(TAMANOY, vector<cell>(TAMANOX, cell(false)));
        for (int i = 0; i < oldY; ++i)
            for (int j = 0; j < oldX; ++j)
                copia[i][j] = tab[i][j];
        tab = copia;
    }
    void ampliar_right()
    {
        int oldX = TAMANOX, oldY = TAMANOY;
        TAMANOX += 1;
        vector<vector<cell>> copia(TAMANOY, vector<cell>(TAMANOX, cell(false)));
        for (int i = 0; i < oldY; ++i)
            for (int j = 0; j < oldX; ++j)
                copia[i][j] = tab[i][j];
        tab = copia;
    }
    void ampliar_left()
    {
        int oldX = TAMANOX, oldY = TAMANOY;
        TAMANOX += 1;
        vector<vector<cell>> copia(TAMANOY, vector<cell>(TAMANOX, cell(false)));
        for (int i = 0; i < oldY; ++i)
            for (int j = 0; j < oldX; ++j)
                copia[i][j + 1] = tab[i][j];
        tab = copia;
    }
    void reducir_top()
    {
        int oldX = TAMANOX, oldY = TAMANOY;
        TAMANOY -= 1;
        vector<vector<cell>> copia(TAMANOY, vector<cell>(TAMANOX, cell(false)));
        for (int i = 1; i < oldY; ++i)
            for (int j = 0; j < oldX; ++j)
                copia[i - 1][j] = tab[i][j];
        tab = copia;
    }
    void reducir_bottom()
    {
        int oldX = TAMANOX, oldY = TAMANOY;
        TAMANOY -= 1;
        vector<vector<cell>> copia(TAMANOY, vector<cell>(TAMANOX, cell(false)));
        for (int i = 0; i < oldY - 1; ++i)
            for (int j = 0; j < oldX; ++j)
                copia[i][j] = tab[i][j];
        tab = copia;
    }
    void reducir_right()
    {
        int oldX = TAMANOX, oldY = TAMANOY;
        TAMANOX -= 1;
        vector<vector<cell>> copia(TAMANOY, vector<cell>(TAMANOX, cell(false)));
        for (int i = 0; i < oldY; ++i)
            for (int j = 0; j < oldX - 1; ++j)
                copia[i][j] = tab[i][j];
        tab = copia;
    }
    void reducir_left()
    {
        int oldX = TAMANOX, oldY = TAMANOY;
        TAMANOX -= 1;
        vector<vector<cell>> copia(TAMANOY, vector<cell>(TAMANOX, cell(false)));
        for (int i = 0; i < oldY; ++i)
            for (int j = 1; j < oldX; ++j) // FIX: hasta oldX, no oldX-1
                copia[i][j - 1] = tab[i][j];
        tab = copia;
    }

public:
    tablero()
    {
        
        tab = vector<vector<cell>>(TAMANOY, vector<cell>(TAMANOX, cell(false)));

        
        for (int i = 1; i < TAMANOY - 1; i++)
            for (int j = 1; j < TAMANOX - 1; j++)
                if (obtenerAleatorio())
                    tab[i][j].change_status();
    }

    void putcell(int i, int j)
    {
        if (i >= 0 && i < TAMANOY && j >= 0 && j < TAMANOX)
            tab[i][j].change_status();
    }

    void imprimir_tablero() const
    {
        string prtTablero;
        prtTablero.reserve(TAMANOY * (TAMANOX + 1)); 
        for (int i = 0; i < TAMANOY; i++)
        {
            for (int j = 0; j < TAMANOX; j++)
            {
                if (i == 0 || j == 0 || i == TAMANOY - 1 || j == TAMANOX - 1)
                {
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

        for (int i = 1; i < TAMANOY - 1; i++)
        {
            for (int j = 1; j < TAMANOX - 1; j++)
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

    vector<int> borde_activo() const
    {
        vector<int> v(4, 0); 

        // ---- IZQUIERDA (columna 1) ----
        {
            int consecutivas = 0, max_consec = 0, total = 0;
            for (int y = 0; y < TAMANOY; ++y)
            {
                if (tab[y][1].get_status())
                {
                    ++total;
                    ++consecutivas;
                    if (consecutivas > max_consec)
                        max_consec = consecutivas;
                }
                else
                {
                    consecutivas = 0;
                }
            }
            if (total == 0)
                v[0] = -1;
            else if (max_consec >= 3)
                v[0] = 1;
            else
                v[0] = 0;
        }

        // ---- ARRIBA (fila 1) ----
        {
            int consecutivas = 0, max_consec = 0, total = 0;
            for (int x = 0; x < TAMANOX; ++x)
            {
                if (tab[1][x].get_status())
                {
                    ++total;
                    ++consecutivas;
                    if (consecutivas > max_consec)
                        max_consec = consecutivas;
                }
                else
                {
                    consecutivas = 0;
                }
            }
            if (total == 0)
                v[1] = -1;
            else if (max_consec >= 3)
                v[1] = 1;
            else
                v[1] = 0;
        }

        // ---- DERECHA (columna TAMANOX-2) ----
        {
            int consecutivas = 0, max_consec = 0, total = 0;
            for (int y = 0; y < TAMANOY; ++y)
            {
                if (tab[y][TAMANOX - 2].get_status())
                {
                    ++total;
                    ++consecutivas;
                    if (consecutivas > max_consec)
                        max_consec = consecutivas;
                }
                else
                {
                    consecutivas = 0;
                }
            }
            if (total == 0)
                v[2] = -1;
            else if (max_consec >= 3)
                v[2] = 1;
            else
                v[2] = 0;
        }

        // ---- ABAJO (fila TAMANOY-2) ----
        {
            int consecutivas = 0, max_consec = 0, total = 0;
            for (int x = 0; x < TAMANOX; ++x)
            {
                if (tab[TAMANOY - 2][x].get_status())
                {
                    ++total;
                    ++consecutivas;
                    if (consecutivas > max_consec)
                        max_consec = consecutivas;
                }
                else
                {
                    consecutivas = 0;
                }
            }
            if (total == 0)
                v[3] = -1;
            else if (max_consec >= 3)
                v[3] = 1;
            else
                v[3] = 0;
        }

        return v;
    }
    void ampliar_tablero(const vector<int> &v)
    {
        const int MIN_X = 3;
        const int MIN_Y = 3;

       
        if (v[0] == -1 && TAMANOX > MIN_X)
            reducir_left(); 
        if (v[1] == -1 && TAMANOY > MIN_Y)
            reducir_top(); 
        if (v[2] == -1 && TAMANOX > MIN_X)
            reducir_right();
        if (v[3] == -1 && TAMANOY > MIN_Y)
            reducir_bottom(); 

      
        if (v[0] == 1)
            ampliar_left(); 
        if (v[1] == 1)
            ampliar_top(); 
        if (v[2] == 1)
            ampliar_right(); 
        if (v[3] == 1)
            ampliar_bottom(); 
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

        vector<int> v = T.borde_activo();
        T.ampliar_tablero(v);

        cin.get();
    }
    return 0;
}
