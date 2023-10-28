SYMBOL TABLE:
80000000 l    d  .text  00000000 .text
80000034 l    d  .data  00000000 .data
00000000 l    df *ABS*  00000000 test.o
00000008 l       *ABS*  00000000 len
00000002 l       *ABS*  00000000 enroll
00000004 l       *ABS*  00000000 elem_sz
80000034 l       .data  00000000 _x
80000010 l       .text  00000000 lp
80000030 l       .text  00000000 lp2
80000000 g       .text  00000000 _start
80000054 g       .data  00000000 _end

Disassembly of section .text:

80000000 <_start>:
80000000:       00400a13                addi    x20,x0,4
80000004:       00000097                auipc   x1,0x0
80000008:       03008093                addi    x1,x1,48 # 80000034 <_x>
8000000c:       00000fb3                add     x31,x0,x0

80000010 <lp>:
80000010:       0000a103                lw      x2,0(x1)
80000014:       0040a183                lw      x3,4(x1)
80000018:       00808093                addi    x1,x1,8
8000001c:       fffa0a13                addi    x20,x20,-1
80000020:       002f8fb3                add     x31,x31,x2
80000024:       003f8fb3                add     x31,x31,x3
80000028:       fe0a14e3                bne     x20,x0,80000010 <lp>
8000002c:       001f8f93                addi    x31,x31,1


80000030 <lp2>:
80000030:       0000006f                jal     x0,80000030 <lp2>

Disassembly of section .data:

80000034 <_x>:
80000034:       0001                    c.addi  x0,0
80000036:       0000                    c.unimp
80000038:       0002                    c.slli64        x0
8000003a:       0000                    c.unimp
8000003c:       00000003                lb      x0,0(x0) # 0 <enroll-0x2>
80000040:       0004                    0x4
80000042:       0000                    c.unimp
80000044:       0005                    c.addi  x0,1
80000046:       0000                    c.unimp
80000048:       0006                    c.slli  x0,0x1
8000004a:       0000                    c.unimp
8000004c:       00000007                0x7
80000050:       0008                    0x8
        ...