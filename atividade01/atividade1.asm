.include "m328pdef.inc"
.org 0x0000

setup:
ser ddrd
ser ddrb
rcall loadvm

rjmp mainloop

mainloop:
ld r5, X+
rcall binbcd
ldi r17,100
loop1: ldi r18, 100
loop2: ldi r19, 100
loop3: rcall exibedisplay
dec r19
brne loop3
dec r18
brne loop2
dec r17
brne loop1 ;; fim loop de espera
cpi XL, 0x09
brne mainloop
ldi XL, 0x00
rjmp mainloop






loadvm: 
ldi r16, 0    ; valor 1
sts 0x0100, r16

ldi r16, 200   ; valor 2
sts 0x0101, r16

ldi r16, 33    ; valor 3
sts 0x0102, r16

ldi r16, 45    ; valor 4
sts 0x0103, r16

ldi r16 ,105     ; valor 5
sts 0x0104, r16

ldi r16, 66     ; valor 6
sts 0x0105, r16

ldi r16, 79     ; valor 7
sts 0x0106, r16

ldi r16, 18     ; valor 8
sts 0x0107, r16

ldi r16, 9     ; valor 9
sts 0x0108, r16

ldi r16, 10    ; valor 10
sts 0x0109, r16

clr r16
ldi XH, 0x01
ldi XL, 0x00
ret



binbcd: 
mov r20, r5 ; r20 para operacao de bin bcd, copiado do reg de operacao
clr r21 ; contador de centenas
clr r22 ; contador de dezenas
clr r23 ; contador de unidades

dcentenas: 
cpi r20, 100
brlt ddezenas
inc r21
subi r20, 100
rjmp dcentenas

ddezenas:
cpi r20, 10
brlt dunidades
inc r22
subi r20, 10
rjmp ddezenas

dunidades:
mov r23, r20
ret ;; fim binbcd

exibedisplay:
ldi r16, 16
exibecentenas:
clr r1
clr r0
mul r21, r16 ; leftshift 4bits das centenas
mov r5, r0 ; pega o valor para o registrador de operacao
out pind, r5 ; copia para o registrador de io
cbi pind, 0b00001000; limpa o pino das dezenas
cbi pind, 0b00000100; limpa o pino das unidades
sbi pinb, 0b00000001 ; ativa o pino das centenas
exibe_dezenas:
clr r1
clr r0
mul r22, r16 ;leftshift 4bits das dezenas
mov r5, r0 ; pega o valor para o registrador de operacao
out pind, r5 ; copia para o registrador de io
cbi pinb, 0b00000001 ; limpa o pino das centenas
sbi pind, 0b00001000 ; ativa o pino das dezenas
cbi pind, 0b00000100 ; limpa o pino das unidades
exibe_unidades:
clr r1
clr r0
mul r23, r16 ;leftshift 4bits das dezenas
mov r5, r0 ; pega o valor para o registrador de operacao
out pind, r5 ; copia para o registrador de io
cbi pinb, 0b00000001 ; limpa o pino das centenas
cbi pind, 0b00001000 ; limpa o pino das dezenas
sbi pind, 0b00000100 ; ativa o pino das unidades

ret ; fim exibedisplay