import React from 'react';

const InfoPage = () => {
    return (
        <div className="container mx-auto p-8">
            <h2 className="text-4xl font-bold mb-6 text-center text-blue-800">Informacje o Projekcie</h2>
            <div className="bg-white p-8 rounded-lg shadow-lg max-w-2xl mx-auto">
                <p className="text-lg text-gray-700 mb-4">
                    Witaj w aplikacji do recenzowania gier! Jest to projekt stworzony w ramach zajęć
                    "Nowoczesne Technologie WWW" na semestrze letnim 2024/2025.
                </p>
                <p className="text-lg text-gray-700 mb-4">
                    Głównym celem aplikacji jest umożliwienie użytkownikom przeglądania, dodawania,
                    edycji i usuwania gier oraz recenzji do nich. System posiada pełną autoryzację i
                    autentykację użytkowników (login, rejestracja) oraz rolę administratora.
                </p>
                <h3 className="text-2xl font-semibold mb-3 text-gray-800">Cechy Projektu:</h3>
                <ul className="list-disc list-inside text-gray-700 mb-4 space-y-2">
                    <li>**SPA (Single Page Application):** Frontend zbudowany w React JS z wykorzystaniem Vite.js.</li>
                    <li>**Komunikacja z REST API:** Pełna komunikacja z przygotowanym backendem Spring Boot.</li>
                    <li>**Autentykacja JWT:** Bezpieczne logowanie i rejestracja z użyciem JSON Web Tokens.</li>
                    <li>**System Ról:** Rozróżnienie ról użytkowników (User, Admin) z ograniczonym dostępem do funkcji.</li>
                    <li>**Zarządzanie Grami:** CRUD (Create, Read, Update, Delete) gier z filtrowaniem i paginacją.</li>
                    <li>**Zarządzanie Recenzjami:** Dodawanie, przeglądanie, edycja i usuwanie recenzji gier.</li>
                    <li>**Responsywny Design:** Interfejs dostosowujący się do różnych rozmiarów ekranu.</li>
                    <li>**Obsługa Błędów:** Wbudowane mechanizmy obsługi błędów API i sieci.</li>
                </ul>
                <p className="text-lg text-gray-700 mt-6 text-right font-semibold">
                    Stworzone przez: Kajetan Plewa
                </p>
            </div>
        </div>
    );
};

export default InfoPage;
