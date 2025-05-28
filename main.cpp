#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <random>
#include <ctime>
#include <iomanip>
#include <limits>
#include <climits>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Colores disponibles para las cartas y trayectos
enum class Color { ROJO, AZUL, VERDE, MORADO, CAFE, NARANJA,NEGRO, BLANCO, GRIS };

// Configuración de colores ANSI
struct AnsiColor {
    string fg;  // Código ANSI para primer plano
    string bg;  // Código ANSI para fondo (opcional)
};

// Mapeo de colores a códigos ANSI
const map<Color, AnsiColor> ANSI_COLORS = {
    {Color::ROJO, {"\033[31m", "\033[41m"}},
    {Color::AZUL, {"\033[34m", "\033[44m"}},
    {Color::VERDE, {"\033[32m", "\033[42m"}},
    {Color::MORADO, {"\033[35m", "\033[45m"}},
    {Color::CAFE, {"\033[38;5;130m", "\033[48;5;130m"}},
    {Color::NARANJA, {"\033[38;5;208m", "\033[48;5;208m"}},
    {Color::GRIS, {"\033[37m", "\033[47m"}},
    {Color::NEGRO, {"\033[30m", "\033[40m"}},
    {Color::BLANCO, {"\033[97m", "\033[107m"}}
};

const string ANSI_RESET = "\033[0m";

// Habilitar colores ANSI en Windows (solo para Windows 10+)
void enableANSIColors() {
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    #endif
}

// Estructura para representar una carta
struct Carta {
    Color color;
    string getColorCode() const {
        return ANSI_COLORS.at(color).fg;
    }
};

// Estructura para representar un trayecto entre ciudades
struct Trayecto {
    string ciudad1;
    string ciudad2;
    int longitud;
    Color color;
    bool ocupado;
    int jugador;
    vector<pair<int, int>> posiciones;
    
    string getColorCode() const {
        return ANSI_COLORS.at(color).fg;
    }
};

// Estructura para representar un jugador
struct Jugador {
    int id;
    int vagones;
    vector<Carta> mano;
    int puntos;
    Color color;
    
    string getColorCode() const {
        return ANSI_COLORS.at(color).fg;
    }
};

class JuegoTrenes {
private:
    // Dimensiones del tablero
    static const int FILAS = 14;
    static const int COLUMNAS = 19;
    
    vector<string> ciudades = {"A", "D", "E", "F", "G", "C", "H", "K", "L", "M", "J", "N", "O", "R", "S","I","B","P","Q"};
    vector<Trayecto> trayectos;
    vector<Carta> baraja;
    vector<Carta> descarte;
    vector<Carta> zonaCartas;
    vector<Jugador> jugadores;
    int turnoActual;
    bool juegoTerminado;
    bool useColor;
    
    // Tablero visual
    char tablero[FILAS][COLUMNAS];
    Color coloresTablero[FILAS][COLUMNAS];

    void inicializarTablero() {
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            tablero[i][j] = ' ';  // Espacio dentro de los corchetes
            coloresTablero[i][j] = Color::BLANCO;
        }
    }
        
        // Posiciones de las ciudades 
        tablero[7][0] = 'A'; coloresTablero[7][0] = Color::NEGRO;
        tablero[4][2] = 'B'; coloresTablero[4][2] = Color::NEGRO;
        tablero[3][5] = 'E'; coloresTablero[3][5] = Color::NEGRO;
        tablero[7][5] = 'F'; coloresTablero[7][5] = Color::NEGRO;
        tablero[0][3] = 'D'; coloresTablero[0][3] = Color::NEGRO;
        tablero[13][2] = 'C'; coloresTablero[13][2] = Color::NEGRO;
        tablero[7][8] = 'H'; coloresTablero[7][8] = Color::NEGRO;
        tablero[7][11] = 'K'; coloresTablero[7][11] = Color::NEGRO;
        tablero[11][11] = 'L'; coloresTablero[11][11] = Color::NEGRO;
        tablero[13][12] = 'M'; coloresTablero[13][12] = Color::NEGRO;
        tablero[1][10] = 'J'; coloresTablero[1][10] = Color::NEGRO;
        tablero[1][13] = 'N'; coloresTablero[1][13] = Color::NEGRO;
        tablero[3][15] = 'O'; coloresTablero[3][15] = Color::NEGRO;
        tablero[5][18] = 'R'; coloresTablero[5][18] = Color::NEGRO;
        tablero[9][18] = 'S'; coloresTablero[9][18] = Color::NEGRO;
        tablero[11][5] = 'G'; coloresTablero[11][5] = Color::NEGRO;
        tablero[4][9] = 'I'; coloresTablero[4][9] = Color::NEGRO;
        tablero[7][15] = 'P'; coloresTablero[7][15] = Color::NEGRO;
        tablero[11][15] = 'Q'; coloresTablero[11][15] = Color::NEGRO;

        
    }

    void inicializarTrayectos() {
        
        // Trayecto A-B (4 rojas)
        Trayecto t1;
        t1.ciudad1 = "A";
        t1.ciudad2 = "B";
        t1.longitud = 4;
        t1.color = Color::ROJO;
        t1.ocupado = false;
        t1.jugador = -1;
        t1.posiciones = {{6,0}, {5,0}, {4,0}, {4,1}};
        trayectos.push_back(t1);
        
        // Trayecto A-F (3 azules)
        Trayecto t2;
        t2.ciudad1 = "A";
        t2.ciudad2 = "F";
        t2.longitud = 4;
        t2.color = Color::AZUL;
        t2.ocupado = false;
        t2.jugador = -1;
        t2.posiciones = {{7,1}, {7,2}, {7,3},{7,4}};
        trayectos.push_back(t2);
        
        // Trayecto F-E (2 verdes)
        Trayecto t3;
        t3.ciudad1 = "F";
        t3.ciudad2 = "E";
        t3.longitud = 3;
        t3.color = Color::VERDE;
        t3.ocupado = false;
        t3.jugador = -1;
        t3.posiciones = {{6,5}, {5,5},{4,5}};
        trayectos.push_back(t3);
        
        // Trayecto E-D (4 cafes)
        Trayecto t4;
        t4.ciudad1 = "E";
        t4.ciudad2 = "D";
        t4.longitud = 4;
        t4.color = Color::CAFE;
        t4.ocupado = false;
        t4.jugador = -1;
        t4.posiciones = {{2,5}, {1,5}, {0,5}, {0,4}};
        trayectos.push_back(t4);
        
        // Trayecto F-G (3 NARANJAS)
        Trayecto t5;
        t5.ciudad1 = 'F';
        t5.ciudad2 = 'G';
        t5.longitud = 3;
        t5.color = Color::NARANJA;
        t5.ocupado = false;
        t5.jugador = -1;
        t5.posiciones = {{10,5},{9,5},{8,5}};
        trayectos.push_back(t5);
        
        // Trayecto G-C (4 CAFES)
        Trayecto t6;
        t6.ciudad1 = 'G';
        t6.ciudad2 = 'C';
        t6.longitud = 4;
        t6.color = Color::CAFE;
        t6.ocupado = false;
        t6.jugador = -1;
        t6.posiciones = {{11,4},{11,3},{11,2},{12,2}};
        trayectos.push_back(t6);
        
        // Trayecto F-H (2 NARANJA)
        Trayecto t7;
        t7.ciudad1 = 'F';
        t7.ciudad2 = 'H';
        t7.longitud = 2;
        t7.color = Color::NARANJA;
        t7.ocupado = false;
        t7.jugador = -1;
        t7.posiciones = {{7,6},{7,7}};
        trayectos.push_back(t7);
        
        // Trayecto H-L (6 ROJO)
        Trayecto t8;
        t8.ciudad1 = 'H';
        t8.ciudad2 = 'L';
        t8.longitud = 6;
        t8.color = Color::ROJO;
        t8.ocupado = false;
        t8.jugador = -1;
        t8.posiciones = {{8,8},{9,8},{10,8},{11,8},{11,9},{11,10}};
        trayectos.push_back(t8);
        
         // Trayecto H-K (6 ROJO)
        Trayecto t9;
        t9.ciudad1 = 'H';
        t9.ciudad2 = 'K';
        t9.longitud = 2;
        t9.color = Color::AZUL;
        t9.ocupado = false;
        t9.jugador = -1;
        t9.posiciones = {{7,9},{7,10}};
        trayectos.push_back(t9);
        
        // Trayecto K-L (3 NARANJA)
        Trayecto t10;
        t10.ciudad1 = 'K';
        t10.ciudad2 = 'L';
        t10.longitud = 3;
        t10.color = Color::NARANJA;
        t10.ocupado = false;
        t10.jugador = -1;
        t10.posiciones = {{8,11},{9,11},{10,11}};
        trayectos.push_back(t10);
        
        // Trayecto K-I (4 MORADO)
        Trayecto t11;
        t11.ciudad1 = 'K';
        t11.ciudad2 = 'I';
        t11.longitud = 4;
        t11.color = Color::MORADO;
        t11.ocupado = false;
        t11.jugador = -1;
        t11.posiciones = {{6,11},{5,11},{5,10},{5,9}};
        trayectos.push_back(t11);
        
        // Trayecto I-O (6 AZUL)
        Trayecto t12;
        t12.ciudad1 = 'I';
        t12.ciudad2 = 'O';
        t12.longitud = 6;
        t12.color = Color::AZUL;
        t12.ocupado = false;
        t12.jugador = -1;
        t12.posiciones = {{3,9},{3,10},{3,11},{3,12},{3,13},{3,14}};
        trayectos.push_back(t12);
        
        // Trayecto O-N (3 VERDE)
        Trayecto t13;
        t13.ciudad1 = 'O';
        t13.ciudad2 = 'N';
        t13.longitud = 3;
        t13.color = Color::VERDE;
        t13.ocupado = false;
        t13.jugador = -1;
        t13.posiciones = {{2,15},{1,15},{1,14}};
        trayectos.push_back(t13);
        
        // Trayecto J-N (2 ROJO)
        Trayecto t14;
        t14.ciudad1 = 'N';
        t14.ciudad2 = 'J';
        t14.longitud = 2;
        t14.color = Color::ROJO;
        t14.ocupado = false;
        t14.jugador = -1;
        t14.posiciones = {{1,12},{1,11}};
        trayectos.push_back(t14);
        
        // Trayecto K-P (3 VERDE)
        Trayecto t15;
        t15.ciudad1 = 'K';
        t15.ciudad2 = 'P';
        t15.longitud = 3;
        t15.color = Color::VERDE;
        t15.ocupado = false;
        t15.jugador = -1;
        t15.posiciones = {{7,12},{7,13},{7,14}};
        trayectos.push_back(t15);
        
        // Trayecto P-Q (3 VERDE)
        Trayecto t16;
        t16.ciudad1 = 'P';
        t16.ciudad2 = 'Q';
        t16.longitud = 3;
        t16.color = Color::VERDE;
        t16.ocupado = false;
        t16.jugador = -1;
        t16.posiciones = {{8,15},{9,15},{10,15}};
        trayectos.push_back(t16);
        
        // Trayecto P-O (3 NARANJA)
        Trayecto t17;
        t17.ciudad1 = 'P';
        t17.ciudad2 = 'O';
        t17.longitud = 3;
        t17.color = Color::NARANJA;
        t17.ocupado = false;
        t17.jugador = -1;
        t17.posiciones = {{6,15},{5,15},{4,15}};
        trayectos.push_back(t17);
        
        // Trayecto Q-S (4 CAFE)
        Trayecto t18;
        t18.ciudad1 = 'Q';
        t18.ciudad2 = 'S';
        t18.longitud = 4;
        t18.color = Color::CAFE;
        t18.ocupado = false;
        t18.jugador = -1;
        t18.posiciones = {{11,16},{11,17},{11,18},{10,18}};
        trayectos.push_back(t18);
        
        // Trayecto O-R (4 CAFE)
        Trayecto t19;
        t19.ciudad1 = 'O';
        t19.ciudad2 = 'R';
        t19.longitud = 4;
        t19.color = Color::MORADO;
        t19.ocupado = false;
        t19.jugador = -1;
        t19.posiciones = {{3,16},{3,17},{3,18},{4,18}};
        trayectos.push_back(t19);
        
        // Trayecto Q-M (4 MORADO)
        Trayecto t20;
        t20.ciudad1 = 'Q';
        t20.ciudad2 = 'M';
        t20.longitud = 4;
        t20.color = Color::MORADO;
        t20.ocupado = false;
        t20.jugador = -1;
        t20.posiciones = {{12,15},{13,15},{13,14},{13,13}};
        trayectos.push_back(t20);
        
        
    }

    void inicializarBaraja() {
        // 12 cartas de cada color
        vector<Color> colores = {Color::ROJO, Color::AZUL, Color::VERDE, 
                                Color::MORADO, Color::CAFE, Color::NARANJA};
        
        for (auto color : colores) {
            for (int i = 0; i < 12; ++i) {
                baraja.push_back({color});
            }
        }
    }

    void barajarCartas() {
        random_device rd;
        mt19937 g(rd());
        shuffle(baraja.begin(), baraja.end(), g);
    }

    void repartirCartasIniciales() {
        for (auto& jugador : jugadores) {
            for (int i = 0; i < 4; ++i) {
                if (!baraja.empty()) {
                    jugador.mano.push_back(baraja.back());
                    baraja.pop_back();
                }
            }
        }
    }

    void actualizarTablero() {
        // Resetear colores del tablero
        for (int i = 0; i < FILAS; ++i) {
            for (int j = 0; j < COLUMNAS; ++j) {
                if (tablero[i][j] != ' ' && !isalpha(tablero[i][j])) {
                    tablero[i][j] = ' ';
                    coloresTablero[i][j] = Color::BLANCO;
                }
            }
        }
        
        // Dibujar trayectos
        for (const auto& trayecto : trayectos) {
            if (trayecto.ocupado) {
                char simbolo = '0' + trayecto.jugador;
                for (auto pos : trayecto.posiciones) {
                    if (pos.first >= 0 && pos.first < FILAS && 
                        pos.second >= 0 && pos.second < COLUMNAS) {
                        tablero[pos.first][pos.second] = simbolo;
                        coloresTablero[pos.first][pos.second] = jugadores[trayecto.jugador-1].color;
                    }
                }
            } else {
                char simbolo = '-'; // Símbolo para trayectos disponibles
                for (auto pos : trayecto.posiciones) {
                    if (tablero[pos.first][pos.second] == ' ') {
                        tablero[pos.first][pos.second] = simbolo;
                        coloresTablero[pos.first][pos.second] = trayecto.color;
                    }
                }
            }
        }
    }

    int contarCartasColor(const vector<Carta>& mano, Color color) const {
        return count_if(mano.begin(), mano.end(), 
            [color](const Carta& c) { return c.color == color; });
    }

    void usarCartas(Jugador& jugador, Color color, int cantidad) {
        vector<Carta> nuevasCartas;
        int usadas = 0;
        
        for (const auto& carta : jugador.mano) {
            if (carta.color == color && usadas < cantidad) {
                descarte.push_back(carta);
                usadas++;
            } else {
                nuevasCartas.push_back(carta);
            }
        }
        
        jugador.mano = nuevasCartas;
    }

void reponerCartasZona() {
    // Limpiar la zona de cartas actual
    zonaCartas.clear();
    
    // Vector con los colores requeridos (ahora solo 4)
    vector<Color> coloresRequeridos = {Color::ROJO, Color::AZUL, Color::NARANJA, 
                                      Color::VERDE, Color::CAFE, Color::MORADO};
    
    // Barajar los colores para orden aleatorio
    random_device rd;
    mt19937 g(rd());
    shuffle(coloresRequeridos.begin(), coloresRequeridos.end(), g);
    
    // Tomar solo los primeros 4 colores
    coloresRequeridos.resize(4);
    
    // Buscar una carta de cada color requerido
    for (Color color : coloresRequeridos) {
        auto it = find_if(baraja.begin(), baraja.end(), 
            [color](const Carta& c) { return c.color == color; });
        
        if (it != baraja.end()) {
            zonaCartas.push_back(*it);
            baraja.erase(it);
        } else {
            // Si no hay carta de ese color, buscar en el descarte
            auto it_descarte = find_if(descarte.begin(), descarte.end(),
                [color](const Carta& c) { return c.color == color; });
            
            if (it_descarte != descarte.end()) {
                zonaCartas.push_back(*it_descarte);
                descarte.erase(it_descarte);
            } else {
                // Si no hay en baraja ni descarte, usar cualquier carta
                if (!baraja.empty()) {
                    zonaCartas.push_back(baraja.back());
                    baraja.pop_back();
                } else if (!descarte.empty()) {
                    zonaCartas.push_back(descarte.back());
                    descarte.pop_back();
                }
            }
        }
    }
    
    // Si no hay suficientes cartas, barajar el descarte
    if (zonaCartas.size() < 4 && !descarte.empty()) {
        baraja = descarte;
        descarte.clear();
        barajarCartas();
        reponerCartasZona();
    }
}



    void mostrarCarta(const Carta& carta) const {
    if (useColor) {
        // Usar bloque de color más grande para mejor visibilidad
        cout << ANSI_COLORS.at(carta.color).bg << "  " << ANSI_RESET;
    } else {
        // Mostrar inicial del color si no hay colores ANSI
        cout << "[" << colorToString(carta.color).substr(0, 1) << "]";
    }
}

public:
JuegoTrenes(int numJugadores, bool usarColor = true) : useColor(usarColor) {
    if (useColor) enableANSIColors();
    
    inicializarTablero();
    inicializarTrayectos();
    inicializarBaraja();
    
    vector<Color> coloresJugador = {Color::ROJO, Color::AZUL, Color::VERDE, Color::MORADO};
    for (int i = 0; i < numJugadores; ++i) {
        Jugador j;
        j.id = i + 1;
        j.vagones = 18;
        j.puntos = 0;
        j.color = coloresJugador[i];
        jugadores.push_back(j);
    }
    
    barajarCartas();
    repartirCartasIniciales();
    
    // Inicializar zona de cartas con 4 colores diferentes
    reponerCartasZona();
    
    turnoActual = 0;
    juegoTerminado = false;
    actualizarTablero();
}

    void mostrarTablero() const {
    // Encabezado superior con números de columna
    cout << "\n=== TABLERO (14×18) ===" << endl;
    cout << "     ";  // Espacio para alinear con los números de fila
    
    // Mostrar números de columnas con espaciado ajustado
    for (int j = 0; j < COLUMNAS; ++j) {
        cout << setw(3) << j << " ";  // setw(2) para números de 2 dígitos
    }
    cout << "\n";
    
    // Mostrar filas del tablero
    for (int i = 0; i < FILAS; ++i) {
        // Número de fila alineado a la derecha
        cout << setw(3) << i << "  ";
        
        for (int j = 0; j < COLUMNAS; ++j) {
            if (useColor) {
                cout << ANSI_COLORS.at(coloresTablero[i][j]).fg 
                     << "[" << tablero[i][j] << "]" 
                     << ANSI_RESET;
            } else {
                cout << "[" << tablero[i][j] << "]";
            }
            cout << " ";  // Espacio entre celdas
        }
        cout << "\n";
    }
}

    string colorToString(Color color) const {
        switch (color) {
            case Color::ROJO: return "Rojo";
            case Color::AZUL: return "Azul";
            case Color::VERDE: return "Verde";
            case Color::MORADO: return "Morado";
            case Color::CAFE: return "Café";
            case Color::NARANJA: return "Naranja";
            case Color::GRIS: return "Gris";
            case Color::NEGRO: return "Negro";
            case Color::BLANCO: return "Blanco";
            default: return "Desconocido";
        }
    }

    void robarCartas(int jugadorId, int carta1, int carta2) {
    if (jugadorId != jugadores[turnoActual].id) {
        cout << "¡No es tu turno! Es el turno del Jugador " << (turnoActual + 1) << endl;
        return;
    }
    
    
    if (carta1 < 0 || carta1 >= zonaCartas.size() || 
        carta2 < 0 || carta2 >= zonaCartas.size()) {
        cout << "Selección de cartas inválida! Debe ser entre 0 y " << (zonaCartas.size()-1) << endl;
        return;
    }
    
    // Mostrar las cartas robadas
    // Mostrar información detallada de lo robado
    cout << "\nJugador " << jugadorId << " ha robado:\n";
    cout << "  Carta " << carta1 << ": ";
    mostrarCarta(zonaCartas[carta1]);
    cout << " (" << colorToString(zonaCartas[carta1].color) << ")\n";
    cout << "  Carta " << carta2 << ": ";
    mostrarCarta(zonaCartas[carta2]);
    cout << " (" << colorToString(zonaCartas[carta2].color) << ")\n";
    
    // Agregar cartas a la mano del jugador
    jugadores[turnoActual].mano.push_back(zonaCartas[carta1]);
    jugadores[turnoActual].mano.push_back(zonaCartas[carta2]);
    
    zonaCartas.erase(zonaCartas.begin() + max(carta1, carta2));
    zonaCartas.erase(zonaCartas.begin() + min(carta1, carta2));
    
    // Reponer cartas en la zona
    reponerCartasZona();
    
    siguienteTurno();
    }
    
    int obtenerJugadorActual() const {
        return turnoActual; // Devuelve 1 para jugador 1, 2 para jugador 2, etc.
    }

    void siguienteTurno() {
    turnoActual = (turnoActual + 1) % jugadores.size();
    cout << "\n--- Es ahora el turno del Jugador " << (turnoActual + 1) << " ---\n";
    }

    bool intentarConstruirTrayecto(int jugadorId, const string& ciudad1, const string& ciudad2) {
        if (jugadorId != jugadores[turnoActual].id) {
        cout << "¡No es tu turno! Es el turno del Jugador " << (turnoActual + 1) << endl;
        return false;
    }
        
        auto it = find_if(trayectos.begin(), trayectos.end(), 
            [&](const Trayecto& t) {
                return ((t.ciudad1 == ciudad1 && t.ciudad2 == ciudad2) || 
                       (t.ciudad1 == ciudad2 && t.ciudad2 == ciudad1)) && 
                       !t.ocupado;
            });
        
        if (it == trayectos.end()) {
            cout << "Trayecto no encontrado o ya ocupado!" << endl;
            return false;
        }
        
        Trayecto& trayecto = *it;
        Jugador& jugador = jugadores[turnoActual];
        
        int cartasNecesarias = trayecto.longitud;
        int cartasDisponibles = contarCartasColor(jugador.mano, trayecto.color);
        
        if (cartasDisponibles < cartasNecesarias) {
            cout << "No tienes suficientes cartas de ese color!" << endl;
            return false;
        }
        
        if (jugador.vagones < trayecto.longitud) {
            cout << "No tienes suficientes vagones!" << endl;
            return false;
        }
        
        usarCartas(jugador, trayecto.color, cartasNecesarias);
        jugador.vagones -= trayecto.longitud;
        trayecto.ocupado = true;
        trayecto.jugador = jugador.id;
        
        int puntos = 0;
        switch (trayecto.longitud) {
            case 2: puntos = 1; break;
            case 3: puntos = 2; break;
            case 4: puntos = 4; break;
            case 5: puntos = 6; break;
            case 6: puntos = 9; break;
        }
        jugador.puntos += puntos;
        
        if (jugador.vagones < 5) {
            juegoTerminado = true;
            cout << "Jugador " << jugador.id << " tiene menos de 5 vagones. Fin del juego!" << endl;
        }
        
        actualizarTablero();
        siguienteTurno();
        return true;
    }
    
    void mostrarRutasDisponibles() const {
    cout << "\n=== RUTAS DISPONIBLES ===" << endl;
    cout << "Ciudad1 - Ciudad2 : Longitud - Color\n";
    
    for (const auto& trayecto : trayectos) {
        if (!trayecto.ocupado) {
            if (useColor) {
                cout << trayecto.ciudad1 << " - " << trayecto.ciudad2 << " : " 
                     << trayecto.longitud << " - "
                     << trayecto.getColorCode() << colorToString(trayecto.color) 
                     << ANSI_RESET << endl;
            } else {
                cout << trayecto.ciudad1 << " - " << trayecto.ciudad2 << " : " 
                     << trayecto.longitud << " - "
                     << colorToString(trayecto.color) << endl;
            }
        }
    }
}

    void mostrarEstado() const {
    cout << "=== ESTADO DEL JUEGO ===" << endl;
    mostrarTablero();
    mostrarRutasDisponibles();
    
    // Mostrar información de cada jugador
    for (const auto& jugador : jugadores) {
        cout << "\nJugador " << jugador.id << " (" << colorToString(jugador.color) << "):\n";
        cout << "  Puntos: " << jugador.puntos << " | Vagones: " << jugador.vagones << "\n";
        cout << "  Cartas en mano (" << jugador.mano.size() << "): ";
        
        if (jugador.mano.empty()) {
            cout << "Sin cartas";
        } else {
            for (const auto& carta : jugador.mano) {
                mostrarCarta(carta);
                cout << " ";
            }
        }
        cout << endl;
    }
    
    // Mostrar zona de cartas con más detalle
    cout << "\nZona de cartas disponibles (" << zonaCartas.size() << "):\n";
    for (int i = 0; i < zonaCartas.size(); ++i) {
        cout << "  " << i << ": ";
        mostrarCarta(zonaCartas[i]);
        cout << " (" << colorToString(zonaCartas[i].color) << ")\n";
    }
}

    bool estaTerminado() const {
        return juegoTerminado;
    }

    void mostrarGanador() const {
        int maxPuntos = -1;
        vector<int> ganadores;
        
        for (const auto& jugador : jugadores) {
            if (jugador.puntos > maxPuntos) {
                maxPuntos = jugador.puntos;
                ganadores.clear();
                ganadores.push_back(jugador.id);
            } else if (jugador.puntos == maxPuntos) {
                ganadores.push_back(jugador.id);
            }
        }
        
        if (ganadores.size() == 1) {
            cout << "El ganador es el Jugador " << ganadores[0] << " con " << maxPuntos << " puntos!" << endl;
        } else {
            cout << "Empate entre los jugadores: ";
            for (int id : ganadores) {
                cout << id << " ";
            }
            cout << "con " << maxPuntos << " puntos cada uno!" << endl;
        }
    }
};

int main() {
    cout << "Bienvenido al Juego de Trenes!" << endl;
    
    int numJugadores;
    do {
        cout << "Ingrese el número de jugadores (2-4): ";
        cin >> numJugadores;
    } while (numJugadores < 2 || numJugadores > 4);
    
    bool usarColores = true;
    #ifdef _WIN32
    // Preguntar si usar colores en Windows
    char respuesta;
    cout << "¿Usar colores ANSI? (s/n): ";
    cin >> respuesta;
    usarColores = (respuesta == 's' || respuesta == 'S');
    #endif
    
    JuegoTrenes juego(numJugadores, usarColores);

    while (!juego.estaTerminado()) {
        juego.mostrarEstado();
        
        // Obtener jugador actual correctamente
        int jugadorActual = juego.obtenerJugadorActual(); // Necesitarás implementar este método
        
        cout << "\nTurno del Jugador " << (jugadorActual + 1) << endl;
        cout << "Opciones:\n";
        cout << "1. Robar cartas\n";
        cout << "2. Construir trayecto\n";
        cout << "Elije una opción: ";
        
        int opcion;
        cin >> opcion;
        
        if (opcion == 1) {
            cout << "Selecciona dos cartas de la zona (ingresa índices 0-3 separados por espacio): ";
            int carta1, carta2;
            cin >> carta1 >> carta2;
            juego.robarCartas(jugadorActual + 1, carta1, carta2); // +1 porque los IDs empiezan en 1
        } 
        else if (opcion == 2) {
            cout << "Ingresa las dos ciudades del trayecto (ejemplo: A D): ";
            string ciudad1, ciudad2;
            cin >> ciudad1 >> ciudad2;
            juego.intentarConstruirTrayecto(jugadorActual + 1, ciudad1, ciudad2);
        }
        else {
            cout << "Opción inválida. Pierdes tu turno.\n";
            juego.siguienteTurno();
        }
        
        // Limpiar pantalla
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    juego.mostrarGanador();
    
    return 0;
}
