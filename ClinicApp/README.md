# Інструкція із запуску **ClinicApp**

## Windows

### Варіант 1: Qt Creator (рекомендовано)

1. Встановіть **Qt 6** (MSVC або MinGW).
2. Відкрийте файл `CMakeLists.txt` через **Qt Creator**.
3. Натисніть **Configure Project**.
4. Натисніть кнопку **Run** (зелений трикутник).

---

### Варіант 2: Командний рядок

```powershell
mkdir build
cd build
cmake ..
cmake --build . --config Release
./Release/appClinicApp.exe
```

---

## Linux (Ubuntu / Debian / Fedora)

### 1. Встановлення залежностей

```bash
# Для Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-declarative-dev
```

---

### 2. Збірка та запуск

```bash
mkdir build
cd build
cmake ..
make
./appClinicApp
```

---

## Вимоги

* **C++ Standard:** 17
* **Framework:** Qt 6
* **Build System:** CMake 3.16+
