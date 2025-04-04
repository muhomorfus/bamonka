function main()
  debug = false;

  a = 0;
  b = 1;
  eps = 0.000001;

  draw_plot(a, b, eps);

  [x_min, y_min, n] = find_min_1(debug, a, b, eps);

  info_xy('Точка минимума 1.', x_min, y_min);
  info_v('Количество вычислений функции 1.', n);

  [x_min, y_min, n] = find_min_2(debug, a, b, eps);

  info_xy('Точка минимума 2.', x_min, y_min);
  info_v('Количество вычислений функции 2.', n);
end

function [x_min, y_min, n] = find_min_1(debug, a, b, eps)
  # Шаг 1
  sh = (b - a)/4;

  x0 = a;
  y0 = f(x0);

  n = 1;

  while true
    debug_xy(debug, sprintf('Приближающая точка №%d.', n), x0, y0);

    # Шаг 2

    x1 = x0 + sh;
    y1 = f(x1);

    n = n + 1;

    # Шаг 3

    if y0 > y1
      # Шаг 4
      x0 = x1;
      y0 = y1;

      if a <= x0 <= b
        continue;
      endif
    endif

    # Шаг 5

    if abs(sh) < eps
      x_min = x0;
      y_min = y0;
      return;
    endif

    # Шаг 6

    x0 = x1;
    y0 = y1;
    sh = -sh/4;
  endwhile
end

function [x_min, y_min, n] = find_min_2(debug, r, l, eps)
  phi = (1 + sqrt(5)) / 2;
  resphi = 2 - phi;
  x0 = l + resphi * (r - l);
  x1 = r - resphi * (r - l);
  y0 = f(x0);
  y1 = f(x1);

  n = 2;

  while true
    debug_xy(debug, sprintf('Приближающая точка №%d.', n-1), x0, y0);

    if y0 < y1
      r = x1;
      x1 = x0;
      y1 = y0;
      y0 = l + resphi * (r - l);
      y0 = f(x0);
    else
      l = x0;
      x0 = x1;
      y0 = y1;
      x1 = r - resphi * (r - l);
      y1 = f(x1);
    endif

    n = n + 1;

    if abs(r - l) < eps
      x_min = (x0 + x1) / 2;
      y_min = f(x_min);
      return;
    endif
  endwhile
end

function draw_plot(a, b, eps)
  x=a:eps:b;
  y=f(x);
  plot(x,y);
end

function y = f(x)
  y = asin((35 * x.^2 - 30 * x + 9)/20) + cos((10 * x.^3 + 185 * x.^2 + 340 * x + 103) / (50 * x.^2 + 100*x + 30)) + 0.5;
end

function debug_xy(debug, comment, x, y)
  if debug
    disp('');
    disp(comment);
    printf("(%d, %d)\n", x, y);
  endif
end

function info_xy(comment, x, y)
  disp('');
  disp(comment);
  printf("(%d, %d)\n", x, y);
end


function info_v(comment, v)
  disp('');
  disp(comment);
  disp(v);
end






