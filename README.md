# README - Juego de Trenes (Ticket to Ride Clone)
Juego de Trenes <!-- Puedes reemplazar esto con una imagen real de tu juego -->

# Descripción
Este es un clon simplificado del popular juego de mesa "Ticket to Ride" implementado en C++. El objetivo del juego es conectar ciudades construyendo rutas de trenes usando cartas de colores, acumulando puntos por cada ruta completada.

# Características Principales
✅ Tablero visual con 14×18 celdas

✅ Sistema de colores ANSI para mejor visualización

✅ 19 ciudades y 20 rutas disponibles

✅ Soporte para 2-4 jugadores

✅ Rutas con diferentes longitudes y colores

✅ Sistema de cartas y turnos

# Requisitos del Sistema
Compilador de C++ compatible con C++11 (g++, clang, MSVC)

Terminal que soporte colores ANSI (en Windows 10+ funciona nativamente)

Linux/macOS: Funciona en terminales estándar

Windows: Requiere Windows 10+ para colores ANSI

# Instalación y Ejecución
Clona el repositorio:

bash
git clone https://github.com/tu-usuario/juego-trenes.git
cd juego-trenes
Compila el programa:

bash
g++ -std=c++11 main.cpp -o juego-trenes
Ejecuta el juego:

bash
./juego-trenes
Cómo Jugar
Inicio: Selecciona el número de jugadores (2-4)

Turnos: Cada jugador puede:

Robar cartas de la zona de cartas disponibles

Construir rutas entre ciudades

Objetivo: Completa rutas para ganar puntos

Fin del juego: Cuando un jugador tiene menos de 5 vagones

Controles
Selecciona opciones usando números (1 o 2)

Para robar cartas, ingresa dos índices (ej. 0 2)

Para construir rutas, ingresa dos ciudades (ej. A D)

# Estructura del Código
juego-trenes/
├── main.cpp          # Código fuente principal
├── README.md         # Este archivo
└── (posibles futuras extensiones)
Personalización
Puedes modificar:

Las rutas y ciudades en inicializarTrayectos()

Los colores disponibles en el enum Color

El número de cartas iniciales en repartirCartasIniciales()

# Capturas de Pantalla
![Captura de pantalla 2025-05-28 164349](https://github.com/user-attachments/assets/6a7697ec-d70b-4c36-8049-faac95f57759)
![Captura de pantalla 2025-05-28 165212](https://github.com/user-attachments/assets/28a0633c-207d-4f6b-91c7-787b7de696fe)
![Captura de pantalla 2025-05-28 165334](https://github.com/user-attachments/assets/ae2def8e-9d4d-47f6-99ea-958603dd766e)
![Captura de pantalla 2025-05-27 173306](https://github.com/user-attachments/assets/5b7cad92-8bd5-4b31-a6f2-36893a66747b)


# Contribuciones
Las contribuciones son bienvenidas. Por favor abre un issue o envía un pull request para:

Reportar bugs

Sugerir mejoras

Agregar nuevas características

# Licencia
Este proyecto está bajo la licencia MIT. Ver el archivo LICENSE para más detalles.

¡Diviértete construyendo rutas y convirtiéndote en el magnate de los trenes! 🚂

# Diagrama UML
![DiagramaUML](https://github.com/user-attachments/assets/a6b07614-9712-466a-a7c0-74219f0e98be)
