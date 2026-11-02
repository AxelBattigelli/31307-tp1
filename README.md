# Compilation

Projet C++20 (CMake), dépendant de SFML 2.5 minimum (graphics, window, system).

L’exécutable généré est nommé `TP1`.
Après la compilation, la police est copiée dans `build/assets/`.

## Linux

### Dépendances

Ubuntu / Debian :

```bash
sudo apt update
sudo apt install cmake g++ libsfml-dev
```

Fedora :

```bash
sudo dnf install cmake gcc-c++ SFML-devel
```

### Compiler et lancer

Depuis la racine du dépôt :

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/TP1
```

Pour recompiler après un changement de source, relancer seulement `cmake --build build`.

## Windows

Prérequis : CMake et Visual Studio 2022 (générateur MSVC 64 bits), plus [SFML 2.6.2 pour Visual C++ 17 64 bits](https://github.com/SFML/SFML/releases/download/2.6.2/SFML-2.6.2-windows-vc17-64-bit.zip).

Dans PowerShell, depuis la racine du dépôt, après avoir extrait l’archive dans `sfml/` :

```powershell
cmake -S . -B build -DSFML_DIR="$PWD/sfml/SFML-2.6.2/lib/cmake/SFML"
cmake --build build --config Release
Copy-Item sfml/SFML-2.6.2/bin/sfml-graphics-2.dll build/Release/
Copy-Item sfml/SFML-2.6.2/bin/sfml-window-2.dll build/Release/
Copy-Item sfml/SFML-2.6.2/bin/sfml-system-2.dll build/Release/
.\build\Release\TP1.exe
```

Les DLL SFML doivent rester à côté de `TP1.exe`.
Le dossier `build\Release\assets` est créé à la compilation.

## clang-format et clang-tidy (Linux)

Installer les outils :

```bash
# Ubuntu / Debian
sudo apt install clang-format clang-tidy

# Fedora
sudo dnf install clang-tools-extra
```

Les règles du dépôt sont dans `.clang-format` et `.clang-tidy`. Les commandes se lancent depuis la racine.

### Formater

Réécrire les sources en place :

```bash
find src -type f \( -name '*.cpp' -o -name '*.hpp' \) -print0 \
  | xargs -0 clang-format -i
```

Vérifier sans modifier les fichiers (code de sortie non nul si un fichier n’est pas conforme) :

```bash
find src -type f \( -name '*.cpp' -o -name '*.hpp' \) -print0 \
  | xargs -0 clang-format --dry-run --Werror
```

### Analyse statique

clang-tidy a besoin de la base de compilation. La régénérer si les sources ou les options de compilation changent :

```bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Analyser tous les `.cpp` :

```bash
find src -name '*.cpp' -print0 \
  | xargs -0 clang-tidy -p build
```

Corriger automatiquement ce qui est corrigeable :

```bash
find src -name '*.cpp' -print0 \
  | xargs -0 clang-tidy -p build --fix
```

Un seul fichier :

```bash
clang-tidy -p build src/main.cpp
```
