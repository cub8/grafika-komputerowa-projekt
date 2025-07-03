# Projekt Grafika symulacja eksplozji elektrowni jądrowej

## Wymagania

- GCC
- CMake
- Ninja
- Windows
- Pod żadnym pozorem nie trzymać projektu w ścieżce z polskimi znakami

## Kompilacja assimp

Assimpa należy ręcznie skompilować.

### Pobierz repozytorium

Pamiętaj tylko, żeby je pobierać w innym folderze niż tym od projektu.

```bash
$ git clone https://github.com/assimp/assimp.git
```

### Budujemy je

```bash
$ cd assimp
$ cmake -G Ninja .
$ cmake --build .
```

### Przekopiuj .dll

Interesują nas tylko dwa pliki: `lib/libassimp.dll.a` oraz `bin/libassimp-6.dll`. Wybieramy je kompujemy do `external/assimp/lib` oraz `external/assimp/bin`,

## GLFW

Istnieje prawdopodobieństwo, że należy skompilować GLFW. Robi się to podobnie jak assimpa, z tą różnicą, że kopiujemy `libglfw3.a` pod ścieżkę `external/GLFW` 

## Kompilacja Projektu

1. Tworzymy folder `build` i wchodzimy do niego

```bash
$ mkdir build
$ cd build
```

2. Generujemy projekt Ninją

```bash
$ cmake -G Ninja ..
```

3. Kompilujemy projekt

```bash
# jeżeli jesteś w folderze ./build
$ cmake --build .
# jeżeli jesteś w głównym folderze
$ cmake --build ./build
```

4. Uruchamiamy

```bash
$ cd ./build
$ ./main.exe
```
