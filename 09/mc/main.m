function main()
  debug = true;
  compute_max = false;

  in = [
  1 1 1 1 1;
  1 5 4 6 3;
  1 4 5 7 2;
  1 6 4 2 7;
  1 9 2 9 5;
  ];

  c = in;

  n = size(c)(1);

  if compute_max
    m = max(c(:));
    for i = 1:1:n
      for j = 1:1:n
        c(i, j) = m - c(i, j);
      endfor
    endfor
  endif

  info_v('Матрица стоимостей.', in);

  if compute_max
    info_v('Матрица стоимостей после преобразования.', c);
  endif

  for j = 1:1:n
    m = Inf;
    for i = 1:1:n
      m = min(c(i, j), m);
    endfor

    for i = 1:1:n
      c(i, j) = c(i, j) - m;
    endfor
  endfor

  for i = 1:1:n
    m = Inf;
    for j = 1:1:n
      m = min(c(i, j), m);
    endfor

    for j = 1:1:n
      c(i, j) = c(i, j) - m;
    endfor
  endfor

  debug_v(debug, 'Матрица стоимостей после вычитания минимумов из столбцов и строк.', c);

  z = zeros(n, n);
  starred = 0;

  for j = 1:1:n
    for i = 1:1:n
      if c(i, j) == 0
        to_star = true;
        for k = 1:1:j
          if z(i, k) == 1
            to_star = false;
          endif
        endfor

        if to_star
          z(i, j) = 1;
          starred = starred + 1;
          break;
        endif
      endif
    endfor
  endfor

  debug_z(debug, 'Начальная система независимых нулей.', n, c, z, zeros(n, 1), zeros(n, 1));

  iteration = 0;
  while (starred != n)
    iteration = iteration + 1;
    debug_v(debug, 'Итерация улучшения системы независимых нулей.', iteration);

    [c, z] = improve_z(debug, n, c, z);

    starred = starred + 1;
  endwhile

  x = zeros(n, n);
  for i = 1:1:n
    for j = 1:1:n
      if z(i, j) == 1
        x(i, j) = 1;
      endif
    endfor
  endfor

  info_v('Матрица назначений.', x);

  opt = 0;
  for i = 1:1:n
    for j = 1:1:n
      opt = opt + in(i, j) * x(i, j);
    endfor
  endfor

  info_v('Оптимальное значение функции.', opt);
end

function [c, z] = improve_z(debug, n, c, z)
  plus_cols = zeros(n, 1);
  plus_rows = zeros(n, 1);

  for j = 1:1:n
    for i = 1:1:n
      if z(i, j) == 1
        plus_cols(j) = 1;
        break;
      endif
    endfor
  endfor

  stop_finding_colon = false;
  colon_row = -1;
  colon_column = -1;
  while !stop_finding_colon
    debug_z(debug, 'Текущая система независимых нулей.', n, c, z, plus_rows, plus_cols);

    found_colon_zero = false;

    for j = 1:1:n
      if found_colon_zero
        break;
      endif

      if plus_cols(j) == 1
        continue;
      endif

      for i = 1:1:n
        if plus_rows(i) == 1
          continue;
        endif

        if c(i, j) == 0
          z(i, j) = 2;
          found_colon_zero = true;
          colon_row = i;
          colon_column = j;

          for k = 1:1:n
            if z(i, k) == 1
              plus_cols(k) = 0;
              plus_rows(i) = 1;
            endif
          endfor

          if plus_rows(i) == 0
            stop_finding_colon = true;
          endif
          break;
        endif
      endfor
    endfor

    if !found_colon_zero
      debug_z(debug, 'Текущая система независимых нулей. В невыделенных областях нет нулей.', n, c, z, plus_rows, plus_cols);

      min = -1;

      for j = 1:1:n
        if plus_cols(j) == 1
          continue;
        endif

        for i = 1:1:n
          if plus_rows(i) == 1
            continue;
          endif

          if min == -1 || c(i, j) < min
            min = c(i, j);
          endif
        endfor
      endfor

      for j = 1:1:n
        if plus_cols(j) == 1
          continue;
        endif

        for i = 1:1:n
          c(i, j) = c(i, j) - min;
        endfor
      endfor

      for i = 1:1:n
        if plus_rows(i) == 0
          continue;
        endif

        for j = 1:1:n
          c(i, j) = c(i, j) + min;
        endfor
      endfor

      debug_z(debug, 'Текущая система независимых нулей.', n, c, z, plus_rows, plus_cols);
    endif
  endwhile

  debug_z(debug, 'Текущая система независимых нулей.', n, c, z, plus_rows, plus_cols);

  l_chain = [
    colon_row colon_column;
  ];

  stop_build_l_chain = false;
  step = 1;
  while !stop_build_l_chain
    current_l_element = [];
    if mod(step,2) == 1
      for i = 1:1:n
        if z(i, l_chain(step,2)) == 1
          current_l_element = [i l_chain(step,2)];
          break;
        endif
      endfor
    else
      for j = 1:1:n
        element = z(l_chain(step,1), j);
        if z(l_chain(step,1), j) == 2
          current_l_element = [l_chain(step,1) j];
          break;
        endif
      endfor
    endif

    if length(current_l_element) == 0
      stop_build_l_chain = true;
    else
      l_chain = [l_chain;current_l_element];
      step = step + 1;
    endif
  endwhile

  debug_l(debug, 'Текущая L-цепочка.', n, c, z, l_chain);

  for i = 1:1:step
    if z(l_chain(i,1), l_chain(i,2)) == 1
      z(l_chain(i,1), l_chain(i,2)) = 0;
    elseif z(l_chain(i,1), l_chain(i,2)) == 2
      z(l_chain(i,1), l_chain(i,2)) = 1;
    endif
  endfor

  for i = 1:1:n
    for j = 1:1:n
      if z(i, j) == 2
        z(i, j) = 0;
      endif
    endfor
  endfor

  debug_z(debug, 'Результат замены в L-цепочке.', n, c, z, zeros(n, 1), zeros(n, 1));
end

function debug_z(debug, comment, n, c, z, plus_rows, plus_cols)
  if debug
    disp('')
    disp(comment);
    for i = 1:1:n
      fprintf('   ');
      for j = 1:1:n
        item = int2str(c(i, j));

        if z(i, j) == 1
          item = strcat(item, '*');
        elseif z(i, j) == 2
          item = strcat(item, '`');
        endif

        fprintf('%-4s', item);
      endfor

      if plus_rows(i)
        fprintf('+');
      endif

      fprintf('\n');
    endfor

    fprintf('   ');
    for j = 1:1:n
      if plus_cols(j)
        fprintf('%-4s', '+');
      else
        fprintf('%-4s', '');
      endif
    endfor
    fprintf('\n');
  endif
end

function debug_l(debug, comment, n, c, z, l_chain)
  if debug
    disp('');
    disp(comment);
    for i = 1:1:n
      fprintf('  ');
      for j = 1:1:n
        item = int2str(c(i, j));

        if z(i, j) == 1
          toPrint = strcat(item, '*');
        elseif z(i, j) == 2
          toPrint = strcat(item, '`');
        endif

        in_chain = false;
        for k = 1:1:size(l_chain)(1)
          if i == l_chain(k,1) && j == l_chain(k,2)
            in_chain = true;
            break;
          endif
        endfor

        if in_chain
          fprintf('%-2s', sprintf('(%s)', item));
        else
          fprintf(' %-3s', item);
        endif
      endfor
      disp('');
    endfor
  endif
end

function debug_v(debug, comment, value)
  if debug
    disp('');
    disp(comment);
    disp(value);
  endif
end

function info_v(comment, value)
  disp('');
  disp(comment);
  disp(value);
end






