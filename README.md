

# 🌍 Nasa_ui – NASA Events Visualizer

**Nasa_ui** to aplikacja desktopowa napisana w C++ z wykorzystaniem Qt i QML, umożliwiająca pobieranie, analizę i wizualizację danych o zjawiskach naturalnych z API NASA (EONET v3). Celem projektu jest stworzenie intuicyjnego interfejsu graficznego, w którym użytkownik może przeglądać zdarzenia według kategorii oraz czasu wystąpienia.

---

## 📁 Struktura projektu

```
Nasa_ui/
├── asset_imports/              # Wymagane pliki zasobów dla Qt
├── content/                    # Interfejs graficzny aplikacji (QML)
│   ├── App.qml
│   ├── MyMapItem.qml
│   └── Screen01.qml
├── data/                       # Lokalne kopie danych API NASA (JSON)
├── i18n/                       # Pliki tłumaczeń
├── imports/                   # Komponenty QML i logika aplikacji
│   └── Nasa_ui/
│       ├── Constants.qml
│       ├── DataStore.qml
│       ├── EventListModel.qml
│       ├── JsonData.qml
├── insight/                    # (Zarezerwowane dla przyszłych analiz)
├── qmlcomponents/, qmlmodules/ # Dodatkowe komponenty (placeholder)
├── src/                        # Kod źródłowy w C++
│   ├── core/                   # Implementacja logiki aplikacji
│   ├── include/                # Pliki nagłówkowe
│   └── main.cpp
├── tests/                      # Testy jednostkowe (Catch2)
├── translations.db             # Baza tłumaczeń (SQLite)
├── main.qml, *.qmlproject      # Główne pliki uruchomieniowe
├── qtquickcontrols2.conf       # Konfiguracja QtQuick
└── CMakeLists.txt              # Konfiguracja CMake
```

---

## 🧩 Funkcjonalności

- Pobieranie danych z API NASA (EONET v3)
- Wyświetlanie listy zdarzeń z podziałem na kategorie (burze, pożary, trzęsienia ziemi itd.)
- Prezentacja lokalizacji zdarzenia na mapie (komponent `MyMapItem`)
- Testy jednostkowe dla najważniejszych klas

---

## 🛠️ Technologie

- **C++20**
- **Qt 6 / QML**
- **CPR** (C++ Requests) – obsługa HTTP
- **nlohmann/json** – serializacja/deserializacja JSON
- **Catch2** – framework testowy
- **CMake** – system budowania


---

## 🚀 Budowanie projektu

### 1. Wymagania

- CMake ≥ 3.21.1
- Kompilator wspierający C++20
- Qt 6.2 lub wyższy z komponentami:
  - Qt6::Core
  - Qt6::Gui
  - Qt6::Qml
  - Qt6::Quick
  - Qt6::Positioning
- Biblioteki:
  - cpr (`brew install cpr`)
  - nlohmann_json (`brew install nlohmann-json`)
  - Catch2 v3 (`brew install catch2` lub FetchContent)

### 2. Kompilacja

```bash
cmake -S . -B build
cmake --build build
./build/Nasa_uiApp
```

### 3. Testy

```bash
cd build
ctest
```

---

## 🧪 Testy

Zawarte w katalogu `tests/` testy pokrywają:
- Parsowanie obiektów `Event`, `Category`, `FileHandler`
- Komunikację z API
- Obsługę błędów
Framework: **Catch2 v3**

---

## 🌐 API NASA

Aplikacja korzysta z NASA EONET v3:
- Endpoint: `https://eonet.gsfc.nasa.gov/api/v3/events`
- Kategorie: `floods`, `earthquakes`, `wildfires`, `storms`, `volcanoes`, itd.
- Dane zawierają tytuł, lokalizację (lat/lon), kategorię i czas

---


## 🧠 Przykładowe klasy

### `Event` – Reprezentacja pojedynczego zdarzenia naturalnego

Klasa `Event` reprezentuje pojedyncze zdarzenie pobrane z API NASA. Przechowuje wszystkie kluczowe informacje:
- `id`, `title`, `description`, `link`, `closed`
- Lista `categories` (typy zjawisk, np. `wildfires`)
- Lista `sources` – źródła danych (id + opcjonalny URL)
- Lista `geometry` – dane lokalizacyjne z datą i ewentualną magnitudą

Obsługuje:
- Parsowanie z JSON (różne formaty `geometry`)
- Ekstrahowanie pozycji do postaci `std::pair<double, double>`
- Eksport do JSON
- Obsługę `std::optional` dla pól opcjonalnych

### `Category` – Kategoria zjawisk

Klasa `Category` zawiera dane o typie zjawisk (np. `Floods`, `Volcanoes`). Pola:
- `id`, `title`, `description`, `link`, `layers`

Możliwości:
- Parsowanie z JSON (`Category::fromJson`)
- Eksport do JSON
- Funkcja `print()` wypisuje dane w formacie przyjaznym użytkownikowi

### `MapController` – Integracja logiki C++ z mapą w QML

Główna klasa kontrolera logiki aplikacji:
- Łączy komponenty: `Api`, `FileHandler`, `Event`, `Category`
- Emituje sygnały do QML (`addMarker`, `clearMarkers`, `recentEventsReady`)
- Odczytuje dane z plików JSON, filtruje je i przekazuje do warstwy GUI

Funkcje:
- `addEvent()` – przetwarza obiekt `Event` i tworzy znacznik na mapie
- `generateCategories()` – filtruje aktywne kategorie i zapisuje je
- `generateEvents()`, `showEvents()` – tworzenie i wizualizacja zdarzeń
- `generateRecentEvents()` – uproszczona lista ostatnich zdarzeń
- `centerMapOn()` – lokalizuje zdarzenie o podanym tytule
- `clearEvents()`, `selectEvents()` – filtracja zdarzeń wg kategorii

### `FileHandler` – Obsługa plików JSON

Klasa odpowiedzialna za:
- Odczyt/zapis plików JSON (`readFromJson()`, `writeToJson()`)
- Parsowanie danych do obiektów `Event` i `Category`
- Tworzenie struktury katalogów (`data/`, `output/`)
- Czyszczenie danych (`clearDataDir()`)

Obsługuje różne formaty JSON (array, obiekty) i waliduje strukturę danych.

### `Api` – Komunikacja z API NASA

Abstrakcja zapytań HTTP z użyciem biblioteki **CPR**:
- Buduje zapytania (`Url`, `Params`, `Headers`)
- Obsługuje różne endpointy: `events`, `categories`, `sources`, `magnitudes`
- Parsuje odpowiedzi do formatu `nlohmann::json`
- Obsługuje token API przez nagłówki (`Authorization: Bearer ...`)

---

## 📌 Autorzy

- Artur Sułowski
- Paweł Lisowski
- Rok: 2025
- Przedmiot: PROI

