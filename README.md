# Projekt Grafika symulacja eksplozji elektrowni jądrowej

## Wymagania

- GCC
- CMake
- Ninja

Plus ja wszystko ogarniałem tutaj na Windowsie, także jak robicie na innych systemach to trzeba będzie coś pokombinować z dokumentacją jak to ogarnąć.

Nie jestem też pewien czy nie będziecie musieli na nowo skompilować glfw.

A i nie trzymajcie tego w ścieżce z polskimi znakami bo się skicha!

## Kompilacja assimp

Assimp jest bardzo niegrzeczną biblioteką i w związku z tym trzeba ją ręcznie skompilować. Pff.

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

## Kompilacja

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

4. Uruchomienie

```bash
$ ./build/main.exe
```
