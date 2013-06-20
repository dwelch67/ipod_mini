
    .code 32

.globl _start
_start:
    b reset
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang

reset:
    ;@ ldr sp,=0x10001000
    bl main
hang: b hang

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl dummy
dummy:
    bx lr
