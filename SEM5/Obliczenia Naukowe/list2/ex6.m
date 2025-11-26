function ex6(c, x0, num_iterations)
% COBWEB_PLOT Generuje wykres pajęczyny (cobweb plot) dla funkcji kwadratowej.
% Funkcja iteracyjna: x(n+1) = x(n)^2 + c
%
% Argumenty:
%   c: stała z równania (np. -1.0)
%   x0: warunek początkowy (np. -1.0)
%   num_iterations: liczba iteracji do narysowania

    % 1. Definicja funkcji iteracyjnej
    f = @(x) x.^2 + c;
    
    % 2. Definicja zakresu wykresu (na podstawie stabilnych punktów i cyklu)
    % Ustawiamy zakres, aby pokrywał punkty cyklu (-1, 0) i obszar wokół
    x_min = -2;
    x_max = 2;
    x_plot = linspace(x_min, x_max, 500);

    % 3. Rysowanie funkcji y = f(x) i prostej identyczności y = x
    figure;
    hold on;
    
    % Wykres funkcji iteracyjnej: y = x^2 + c
    plot(x_plot, f(x_plot), 'r', 'LineWidth', 2, 'DisplayName', 'y = x^2 + c'); 
    
    % Wykres prostej identyczności: y = x
    plot(x_plot, x_plot, 'k--', 'LineWidth', 1, 'DisplayName', 'y = x'); 
    
    % 4. Inicjalizacja i pętla iteracyjna
    x = x0;
    
    % Pętla rysująca "pajęczynę"
    for i = 1:num_iterations
        x_next = f(x); % Obliczenie następnej wartości x(n+1)
        
        % Ochrona przed eksplozją
        if abs(x_next) > 10 % Jeśli wartość ucieka, przerywamy
            disp(['Trajektoria uciekła do nieskończoności po ', num2str(i), ' krokach.']);
            break; 
        end
        
        % Rysowanie kroku pionowego (x(n), x(n+1))
        plot([x, x], [x, x_next], 'b', 'LineWidth', 0.5); 
        
        % Rysowanie kroku poziomego (x(n+1), x(n+1))
        plot([x, x_next], [x_next, x_next], 'b', 'LineWidth', 0.5); 
        
        % Aktualizacja do następnej iteracji
        x = x_next;
    end
    
    % Ustawienia wykresu
    title(sprintf('Iteracja Graficzna dla x(n+1) = x(n)^2 + %.2f, x0 = %.2f', c, x0));
    xlabel('x_n');
    ylabel('x_{n+1}');
    grid on;
    axis equal; % Upewnij się, że osie mają tę samą skalę
    xlim([x_min, x_max]);
    ylim([x_min, x_max]);
    legend('show', 'Location', 'northwest');
    hold off;
end