index EQU 2
shift EQU 2
newLine EQU 10
asciiShift EQU 20h

intReadStrCode EQU 0Ah
intPrintSymbol EQU 02h

StackSegment SEGMENT WORD STACK 'STACK'
    DB 200h DUP(0)
StackSegment ENDS

DataSegment SEGMENT WORD 'DATA'
    MaxLen BYTE 20h
    Len BYTE 0
    Str DB 20h DUP(0)
DataSegment ENDS

CodeSegment SEGMENT WORD 'CODE'
    ASSUME CS:CodeSegment, DS:DataSegment

readStr:
    MOV BP, SP

    MOV AX, DataSegment
    MOV DS, AX
    MOV DX, SS:[BP+shift]

    MOV AH, intReadStrCode

    INT 21h
    RET shift

printNewLine:
    MOV AH, intPrintSymbol
    MOV DL, newLine
    INT 21h

    RET

printSymbol:
    MOV BP, SP

    CALL printNewLine

    MOV AH, intPrintSymbol
    MOV DL, SS:[BP+shift]
    INT 21h
    RET shift

toUpper:
    MOV BP, SP

    MOV AL, SS:[BP+shift]
    SUB AL, asciiShift

    RET shift

main:
    MOV AX, DataSegment
    MOV DS, AX

    MOV DX, OFFSET MaxLen
    PUSH DX
    CALL readStr

    MOV SI, index
    MOV BL, Str[SI]
    PUSH BX
    CALL toUpper

    PUSH AX
    CALL printSymbol

    MOV AH, 4Ch
    INT 21h
CodeSegment ENDS

END main
