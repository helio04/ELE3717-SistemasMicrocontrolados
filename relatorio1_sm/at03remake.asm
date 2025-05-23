.include "m328pdef.inc"
;;defs
.def centena= r3
.def dezena= r4
.def unidade= r5
.def step= r6
.def vholder= r7
.def carry= r8
.def valor_lido= r9
.def opi= r16
.def aux1= r17
.def aux2= r18
.def output= r19
.def controle= r20 

.equ pulsos1cm=50 ;; quantidade de pulsos de clock equivalentes a 1cm
.equ PRE_SCALE_=2
.equ pinotrigger=4
.equ pinoecho=5 ;;PC5
.equ botaosalvar=2 ;;s2
.equ botaomedir=1 ;;s3
.equ botaoler=3  ;;s1
.equ PCINT1_vect=0x0008
.equ contadorH=TCNT1H
.equ contadorL=TCNT1L
.equ addrEEPROMH=0x00
.equ addrEEPROML=0x35
.equ addrcurcent = 0x0101
.equ addrcurdez = 0x0102
.equ addrcuruni = 0x0103

;defs

.org 0x0000
rjmp setup

.org PCINT1_vect
rjmp interrupt_botoes



setup:

;;configurar pinos
ldi opi, 0xFF
out DDRD, opi
out ddrb, opi
ldi opi, (1<<pinotrigger)
out DDRC, opi


;;configurar interrupts
ldi opi, (1<<PCIE1)
sts PCICR, opi
ldi opi, (1<<PCINT9) | (1<<PCINT10) | (1<<PCINT11) | (1<<PCINT12)
sts PCMSK1, opi
sei

;;configurar contador
ldi opi, 0x00
sts TCCR1A, opi
sts TIMSK1, opi
sts TCCR1C, opi
ldi opi,(1<<PRE_SCALE_)
sts TCCR1B, opi

;;;configura EEPROM
ldi opi, addrEEPROMH
sts EEARH, opi
ldi opi, addrEEPROML
sts EEARL, opi
ldi opi, 0x04
sts EECR, opi

rjmp loop

interrupt_botoes:
in opi, PINC
sbrs opi, botaoler
rcall ler_eeprom
sbrs opi, botaosalvar
rcall escrever_eeprom
sbrs opi, botaomedir
rcall medir_valor
sbrs opi, botaoler
rjmp end_interrupt 
sbrs opi, botaosalvar
rjmp end_interrupt
sbrs opi, botaomedeir
rjmp end_interrupt
rcall contar_xs
end_interrupt:
reti

escrever_eeprom:
mov opi, valor_lido
sts EEDR, opi
ldi opi, (1<<2)
sts EECR, opi
nop
ori opi, (1<<1)
sts EECR, opi
nop
clr opi
ret
ler_eeprom:
ldi opi, (1<<0)
sts EECR, opi
nop
lds opi, EEDR
mov valor_lido, opi
clr opi
ret

medir_valor:
ldi opi, (1<<pinotrigger)
out portc, opi
rcall wait_10
andi opi, ~(1<<pinotrigger)
ret

contar_xs:
;;primeira interrupção (borda de subida) controle está 0
;; reseta o contador e termina
clr opi
sbrc controle, 0
rjmp skip_reset_counter
sts contadorH, opi
sts contadorL, opi
ldi controle, 1
rjmp end_contar_xs
skip_reset_counter: ;;segunda interrupção (borda de descida) controle 1
;; armazena o valor do contador em X e reseta controle
lds XH, contadorH
lds XL, contadorL
ldi controle, 0
rcall cvdist

end_contar_xs:
ret

cvdist:
clr valor_lido
cp valor_lido, 0
loop_cvdist:
brlt end_cvdist
sbiw XH:XL, pulsos1cm
inc valor_lido
rjmp loop_cvdist
end_cvdist: ;;valor lido ja convertido para centimetros
ret

wait_10:
ldi r30, 10
loop_wait_10:
dec r30
breq end_wait_10
rjmp loop_wait_10
end_wait_10:
ret

loop:
lds opi, addrcurcent
mov centena, opi
lds opi, addrcurdez
mov dezena, opi
lds opi, addrcuruni
mov unidade, opi


rcall exibircentena
rcall exibirdezena
rcall exibirunidade
rjmp loop

;;----binbcd-----
bin_bcd_10bits:
clr centena
clr dezena
clr unidade
bin_bcd_reverse:
cpi XH, 0x00
breq binbcd_old
sbiw XH:XL, 50
sbiw XH:XL, 50
inc centena
rjmp bin_bcd_reverse

binbcd_old:
mov aux, XL


bcdcentena:
cpi aux, 100
brlo bcddezena
subi aux, 100
inc centena
rjmp bcdcentena
bcddezena:
cpi aux, 10
brlo bcdunidade
subi aux, 10
inc dezena
rjmp bcddezena
bcdunidade:
mov unidade, aux

ret
;;----binbcd-end-------