# Algorithm Analyzer

Proyecto desarrollado en lenguaje C para generar, almacenar, ordenar, buscar y analizar registros de deportistas mediante algoritmos basados principalmente en el paradigma **Divide y Vencerás**.

El sistema trabaja con un archivo CSV de deportistas y permite ejecutar operaciones prácticas como generación de datos, búsqueda por ID, ranking por puntaje, obtención del k-ésimo mejor deportista y benchmarks experimentales de algoritmos de ordenamiento, búsqueda y selección.

## Autores

- Iván Gallardo
- Pablo Gómez
- Ayrton Morrison

## Características principales

- Generación automática de deportistas en formato CSV.
- Carga dinámica de registros desde archivo.
- Ordenamiento por distintos criterios:
  - ID
  - puntaje
  - competencias
  - nombre
  - equipo
- Búsqueda eficiente por ID, puntaje exacto y rango de puntaje.
- Ranking de deportistas según puntaje.
- Obtención del k-ésimo mejor deportista mediante Quick Select.
- Benchmarks comparativos para medir tiempos de ejecución.
- Generación de gráficos a partir de los resultados experimentales.

## Algoritmos implementados

### Ordenamiento

- Merge Sort clásico.
- Merge Sort optimizado con Insertion Sort para subarreglos pequeños.
- Quick Sort con partición de Lomuto y distintas variantes de pivote:
  - último elemento,
  - primer elemento,
  - pivote aleatorio,
  - mediana de tres.
- Insertion Sort como algoritmo auxiliar y como base de comparación en algunos procesos.

### Búsqueda

- Búsqueda binaria recursiva por ID.
- Búsqueda binaria por puntaje exacto.
- Búsqueda binaria modificada para rangos de puntaje.
- Búsqueda exponencial por ID.
- Búsqueda por interpolación por ID.

### Selección

- Quick Select para obtener el k-ésimo mejor deportista según puntaje sin ordenar completamente el arreglo.

## Estructura del proyecto

```text
.
├── build/              # Ejecutable compilado
├── db/                 # Archivos CSV de datos y resultados de benchmarks
├── docs/               # Informe, documentación y archivos asociados
├── include/            # Archivos de cabecera (.h)
├── obj/                # Archivos objeto generados al compilar
├── plots/              # Script y gráficos generados con gnuplot
├── src/                # Código fuente (.c)
├── Doxyfile            # Configuración de Doxygen
├── Makefile            # Automatización de compilación y tareas principales
└── README.md
```

## Requisitos

Para compilar y ejecutar el proyecto se requiere:

- GCC
- Make

Opcionalmente, para generar gráficos y documentación:

- Gnuplot
- Doxygen
- LaTeX, si se desea recompilar el informe ubicado en `docs/latex/`

En sistemas basados en Debian, Ubuntu o WSL se puede instalar lo principal con:

```bash
sudo apt update
sudo apt install -y build-essential make gnuplot doxygen
```

## Compilación

Desde la raíz del proyecto:

```bash
make
```

Esto genera el ejecutable en:

```text
build/algorithm_analyzer.out
```

También se puede compilar y ejecutar directamente con:

```bash
make run
```

## Uso general

La forma general de uso es:

```bash
./build/algorithm_analyzer.out [opciones]
```

Para ver la ayuda del programa:

```bash
./build/algorithm_analyzer.out -h
```

## Generar datos

Antes de realizar búsquedas, rankings o benchmarks, se recomienda generar un archivo CSV con deportistas:

```bash
./build/algorithm_analyzer.out -g 1000
```

Esto crea el archivo:

```text
db/deportistas.csv
```

El CSV generado utiliza el siguiente formato:

```csv
ID,Nombre,Equipo,Puntaje,Competencias
```

Cada deportista contiene:

- ID único.
- Nombre generado aleatoriamente.
- Equipo.
- Puntaje entre 0 y 100.
- Cantidad de competencias.

## Opciones disponibles

| Opción | Descripción |
|---|---|
| `-h` | Muestra la ayuda del programa. |
| `-g [cantidad]` | Genera una cantidad determinada de deportistas aleatorios. |
| `-t` | Ejecuta el flujo interactivo de ordenamiento y búsqueda. |
| `-id [valor]` | Busca un deportista por ID. |
| `--id [valor]` | Busca un deportista por ID usando opción larga. |
| `-r [cantidad]` | Muestra el top N de deportistas por puntaje. |
| `-k [posición]` | Muestra el k-ésimo mejor deportista por puntaje. |
| `-b` | Ejecuta benchmark de búsqueda. |
| `-s` | Ejecuta benchmark de ordenamiento. |
| `-q` | Ejecuta benchmark de Quick Select. |
| `-x` | Ejecuta benchmark de búsqueda por rango de puntaje. |
| `-m` | Ejecuta benchmark de Merge-Insertion variando el threshold. |


Los resultados de los benchmark se guardan en archivos CSV dentro de la carpeta `db/`, por ejemplo:

```text
db/search_benchmark.csv
db/sort_benchmark.csv
db/select_benchmark.csv
db/range_benchmark.csv
db/merge_threshold_benchmark.csv
```

## Generación de gráficos

Después de ejecutar los benchmarks, se pueden generar los gráficos con:

```bash
make plot
```

El script utilizado se encuentra en:

```text
plots/plot.gp
```

Los gráficos se generan en formato PDF dentro de la carpeta `plots/`.

---
- Iván Gallardo
- Pablo Gómez
- Ayrton Morrison