## Лабораторная работа №9. Задача 1. Вариант 3
### Входные данные
Аргументы: имя файла с данными о товарах (строка) и цена (целое).
В файле: данные о товарах вида:
```
Товар 1
<Цена 1>
Товар 2
<Цена 2>
...
```

### Выходные данные
Товары в формате, аналогичном входным, цена которых меньше, чем указано в аргументе.

### Позитивные тесты
- 01-03 - введенным условиям удовлетворяет один товар;
- 04 - обычный тест;
- 05-06 - введенным условиям удовлетворяют все товары;
- 07-10 - обычные тесты;
- 11-12 - ни один продукт не удовлетворяет указанному значению цены.

### Негативные тесты
- 01 - пустой файл;
- 02 - в файле 0 продуктов;
- 03-04 - есть продукты с отрицательной или нулевой ценой;
- 05-07 - вместо цены написано невалидное число;
- 08 - обычный тест;
- 09 - файл не существует;
- 10 - невалидное число в аргемунте;
- 11-13 - неверное число аргументов;
- 14-15 - файл содержит не целое число структур;
- 16 - количество структур не соответствует указанному.

