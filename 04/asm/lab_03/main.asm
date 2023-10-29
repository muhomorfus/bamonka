StkSeg  SEGMENT PARA STACK 'STACK' ; сегмент стека
        DB 200h DUP (?)
StkSeg  ENDS

DataS   SEGMENT WORD 'DATA' ; сегмент данных
HelloMessage DB   13 ; начало строки
             DB   10 ; новая строка
             DB   'Hello, world!' ; текст сообщения
             DB '$' ; конец строки
DataS    ENDS

Code    SEGMENT WORD 'CODE' ; сегмент кода
        ASSUME  CS:Code, DS:DataS
DispMsg:
        mov   AX, DataS ; загрузка в AX адреса сегмент данных
        mov   DS, AX ; установка DS
        mov   DX, OFFSET HelloMessage ; установка в DX смещения строки. DS:DX --- адрес строки
        mov   AH, 9 ; АН=09h --- выдать на дисплей строку
        mov   cx, 3
loop1:
        int   21h ; системное прерывание DOS
        loop loop1
        mov   AH, 7 ; AH=07h --- ввести символ без эха
        int   21h
        mov   AH, 4Ch ; AH=4Ch --- выход из программы
        int   21h
Code    ENDS
        END   DispMsg
