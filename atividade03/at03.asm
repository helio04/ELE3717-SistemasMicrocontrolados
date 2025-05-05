.include "m328pdef.inc"
.org 0x0000
rjmp setup

.org 0x0006
rjmp interrupt1
.org 0x0008
rjmp interrupt2
.org 0x000A
rjmp interrupt3

;; definições
;;vsom/2 = 172
;;calculo dist: (vsom/2)*tempo[em segundos] = dist em m
.equ ptrig,4 ;;pc4 pino do trigger
.equ pecho,5 ;;pc5 pino do echo
.equ pulso1, ;;pino do pulso1
.equ pulso2, ;;pino do pulso2
.equ pulso3, ;;pino do pulso3
.equ opi, r16 ;;reg de operacao
.equ counter0, tcnt0
.equ counter2, TCNT2
.equ controle , r19
.equ valorh, yh ;; valor da leitura do sensor high
.equ valorl, yl ;; valor da leitura do sensor low
.equ centena, r21 ;; valor da centena do binbcd
.equ dezena , r22 ;; valor da dezena do binbcd
.equ unidade, r23 ;; valor da unidade do binbcd


;; interrupção dos 3 botões s1,s2,s3
setup: 
;;configurar as interrupções dos botoes
ldi opi, 0x07
sts pcicr, opi
sei
;;configurar os timers
;;timer0,prescale de 8 (2 pulsos de clock = 1 us), modo comparacao A = 20
ldi opi, 0xC0
sts TCCR0A, opi
ldi opi, 0x02
sts TCCR0B, opi
ldi opi, 20
sts ocr0A, opi
;;TIFR0 bit 1 - set quando contador chega a 20 (10 us)
;;timer2, prescale 8, (2 pulsos de clock = 1 us), modo normal
ldi opi, 0x00
sts tccr2A, opi 
ldi opi, 0x02
sts tccr2B, opi
;;TCNT2 onde é armazenado a contagem
;;configurar a eeprom

;;configurar os regs binbcd
ldi centena, 0
ldi dezena, 0
ldi unidade, 0
;;configurar regs io
ldi opi, 0xFD
sts ddrc, opi
ldi opi, 0xFF
sts ddrc, opi

;; fim do setup

leiturasensor:
;;leitura do sensor
;;pulso de 10us em ptrig
;;mede o tempo em que pecho permanece alto
;;armazena o valor de tempo no registrador de valor
;;fim leitura sensor
;;prossegue para converter leitura
cbi portc, ptrig
clr opi
sts counter0, opi
sbi portc, ptrig
waitloopc0:
sbis tifr0, 1
rjmp waitloopc0;; espera contagem
cbi portc, ptrig;;clear no pino trigger
waiting_pulse_echo:
sbis pinc, pecho
rjmp waiting_pulse_echo
clr opi
sts counter2, opi
waiting_pulse_echo_end:
lds valorl, counter2
sbic pinc,pecho
rjmp waiting_pulse_echo_end
converterleitura:
mov opi, valorl

;;converter leitura do sensor para segundos
;; ((vsom/2)/10000)*valor = distancia em cm 
;;armazenar no registrador de valor
ret
;;retorna

interrupt1:
;;chama leitura sensor
call leiturasensor
;;chama binbcd
call binbcd
;;exdisplay no loop exibe os valores desses 3 regs 
interrupt2:
;;copiar o valor do registrador de dados para o reg op(high depois low)
;;e escrever na eeprom

interrupt3:
;;buscar na eeprom o valor do sensor
;;armazenar no reg de op e escrever em reg de 16 bits
;;chamar binbcd, que ira quebrar o valor em 3 regs
;;exdisplay no loop exibe os valores desses 3 regs
binbcd:
mov zh, valorh
mov zl, valorl
cpi ZH,3
breq ZHIG3
cpi ZH, 2
breq ZHIG2
cpi ZH, 1
breq ZHIG1

zhig3:
adiw centena, 7
adiw dezena, 6
adiw unidade, 8
zhig2:
adiw centena, 5
adiw dezena, 1
adiw unidade, 2
zhig1:
adiw centena, 2
adiw dezena, 5
adiw unidade, 6
dcentenas:
cpi ZL, 100
brlt ddezenas
inc centena
subi ZL, 100
rjmp dcentenas

ddezenas:
cpi ZL, 10
brlt dunidades
inc dezena
subi ZL, 10
rjmp ddezenas

dunidades:
mov ZL, unidade
ret ;; fimbinbcd

loop:
;;exibe o valor dos regs centena,dezena,unidade no display
;;interrupções ativas para os botoes

