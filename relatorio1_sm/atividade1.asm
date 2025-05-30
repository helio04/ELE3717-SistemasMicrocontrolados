.include "m328pdef.inc"
.def opi =r16
.def centena = r21
.def dezena= r22
.def unidade= r23
.def mempointerH= r27
.def mempointerL= r26
.def vholder= r5
.def aux= r19
.def iter1= r6
.def iter2=r7
.def iter3= r8
.def output= r18
.org 0x0000
rjmp setup

loopteste1:
ser opi
out ddrd, opi
sts ddrb, opi
loopteste2:
ldi output, 0x80
ldi opi, 0x01
out portb, opi
clr opi
out portd, output
rjmp loopteste2

setup:
ser opi
out DDRD,opi
out DDRB, opi
ldi mempointerH, 0x01
ldi mempointerL, 0x01
rcall loadvalormemoria
rjmp loop

loadvalormemoria:
ldi opi, 120
sts 0x0101, opi
ldi opi, 9
sts 0x0102, opi
ldi opi, 22
sts 0x0103, opi
ldi opi, 33
sts 0x0104, opi
ldi opi, 0
sts 0x0105, opi
ldi opi,55
sts 0x0106, opi
ldi opi, 66
sts 0x0107, opi
ldi opi, 92
sts 0x0108, opi
ldi opi, 88
sts 0x0109, opi
ldi opi, 99
sts 0x010A, opi
clr opi
ret

;;----binbcd-----
binbcd:
mov aux, vholder
clr centena
clr dezena
clr unidade

bcdcentena:
cpi aux, 100
brlt bcddezena
subi aux, 100
inc centena
rjmp bcdcentena
bcddezena:
cpi aux, 10
brlt bcdunidade
subi aux, 10
inc dezena
rjmp bcddezena
bcdunidade:
mov unidade, aux
clr aux
clr vholder
ret
;;----binbcd-end-------

;;----output-converter----
oconverter:
;;conversao com logica inversa e ordem oposta
;; vholder[3:0] -> -aux[4:7]
;; volder[3] -> -aux[4]
;; vholder[2] -> -aux[5]
;; vholder [1] -> -aux[6]
;; vholder [0] -> -aux[7]
clr aux
;; se não for logica inversa, usar sbrc
sbrc vholder, 3
sbr aux, 0b00010000
sbrc vholder, 2
sbr aux, 0b00100000
sbrc vholder, 1
sbr aux, 0b01000000
sbrc vholder, 0
sbr aux, 0b10000000
mov output,aux 
ret
;;;;----output-converter-end-------

;;;----MAIN-LOOP-------
loop:
ld opi, X+
mov vholder, opi
rcall binbcd
ldi opi, 10
mov iter1, opi
clr opi 
switchloop1:
ldi opi, 100
mov iter2, opi
clr opi 
switchloop2:
ldi opi, 100
mov iter3, opi
clr opi 
switchloop3:

;;;exibir centena
exibircentena:
cpi centena, 0
breq exibirdezena
mov vholder, centena
rcall oconverter
cbr output, 0b00001100 ;;sbr se inverso
ldi opi, 0x01 ;; clr se inverso 
out portb, opi
out portd, output
rcall waiting

;;exibir dezena
exibirdezena:
clr r30
add r30, centena
add r30, dezena
cpi r30, 0
breq exibirunidade
mov vholder, dezena
rcall oconverter
sbr output, 0b00001000 ;;cbr se inverso
cbr output, 0b00000100 ;; sbr se inverso
clr opi ;; ldi 1 se inverso
out portb, opi
out portd, output 
rcall waiting 

;;exibir 
exibirunidade:
mov vholder, unidade
rcall oconverter
cbr output, 0b00001000 ;;sbr se inverso
sbr output, 0b00000100 ;; cbr se inverso
clr opi ;; ldi 1 se inverso
out portb, opi
out portd, output 
rcall waiting 

dec iter3
brne switchloop3
dec iter2
brne switchloop2
dec iter1
brne switchloop1

cpi mempointerL, 0x0B
brne loop
ldi mempointerL, 0x01
rjmp loop

;;------MAIN-LOOP-END----------

;;;----estado de espera--------
waiting:
ldi r17, 10
waitloop:
dec r17
breq endwait
rjmp waitloop
endwait: 
ret
;;------fim estado de espera-----

