## Лабораторная работа №8, комплект вариантов 2, вариант 12
### Входные данные
Аргументы командной строки.

### Выходные данные
Зависят от аргументов. Если первый аргумент:
- `a`, то результат сложения матриц;
- `m`, то результат умножения матриц;
- `o`, то согласно варианту. В данном случае определитель матрицы.

### Позитивные тесты
- 01-05 - сложение матриц;
- 06-12 - умножение матриц;
- 13-14 - умножение на единичную матрицу;
- 15-19 - обычные тесты.

### Негативные тесты
- 01-08 - некорретное количество аргументов;
- 09-11 - опция не существует;
- 12-14 - файл не существует;
- 15-18 - пустой файл;
- 19-22 - матрица размером 0x0;
- 23-26 - неожиданный конец файла при чтении матрицы;
- 27-30 - невалидное число в размере матрицы;
- 31-34 - невалидное число в элементе матрицы;
- 35-37 - неподходящие размеры матриц для сложения;
- 38-39 - неподходящие размеры матриц для умножения;
- 40-41 - неподходящий размер матрицы для вычисления определителя.

