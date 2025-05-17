/* prism.js
 * Inicjalizacja Prism.js do kolorowania składni kodu na stronie
 */

document.addEventListener('DOMContentLoaded', function() {
    // Jeśli Prism jest załadowany, wyróżnij wszystkie bloki kodu
    if (window.Prism && typeof Prism.highlightAll === 'function') {
      Prism.highlightAll();
    }
  });
  