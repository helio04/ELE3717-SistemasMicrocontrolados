.include "m328pdef.inc"
;;;;defs
.def centena= r3
.def dezena= r4
.def unidade= r5
.def step= r6
.def vholder= r7
.def carry= r8

.def opi= r16
.def aux1= r17
.def aux2= r18
.def output= r19
.def controle= r20 
.def loopbcd=r21
.def aux=r22
.def vholderbcd=r23
.def estado=r31
.equ addrmaxcent= 0x0120
.equ addrmaxdez=0x0121
.equ addrmaxuni= 0x0122
.equ addrmincent= 0x0123
.equ addrmindez= 0x0124
.equ addrminuni= 0x0125
.equ addrcurcent= 0x0110
.equ addrcurdez= 0x0111
.equ addrcuruni= 0x0112
.equ red=1
.equ green=2
.equ blue=3
.equ botaosetup=2
.equ botaoup=1
.equ botaodown=3    
.equ PCINT1_vect=0x0008
.equ estadocontarup=1
.equ estadocontardown=2
.equ estadosetup=3
;;;;end_defs

.org 0x0000
rjmp setup

.org PCINT1_vect
rjmp interrupt_botoes


setup:
;;configurar pinos
ldi opi, 0xFF
out DDRD, opi
out ddrb, opi
ldi opi, 0x00
out DDRC, opi

;;configurar ADC
ldi opi, (1<<REFS0)
sts ADMUX, opi
ldi opi, (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)
sts ADCSRA, opi


;;configurar interrupts 1,2,3
ldi opi, (1<<PCIE1)
sts PCICR, opi
ldi opi, (1<<PCINT9) | (1<<PCINT10) | (1<<PCINT11)
sts PCMSK1, opi
sei

;;configurar user_config
ldi controle, 0x01
ldi estado, 0x01

;;configurar contador modo padrão
ldi opi, 0
mov centena, opi
mov dezena, opi
mov unidade, opi
sts addrminuni, opi
sts addrmindez, opi
sts addrmincent, opi
sts addrcurcent, opi
sts addrcurdez, opi
sts addrcuruni, opi
inc opi
mov step, opi
ldi opi, 9
sts addrmaxcent, opi
sts addrmaxdez, opi
sts addrmaxuni, opi


rjmp supreme_loop

interrupt_botoes:
in opi, PINC 
sbrs opi, botaodown
ldi estado, (1<<estadocontardown)
sbrs opi, botaoup
ldi estado, (1<<estadocontarup)
sbrs opi, botaosetup
ldi estado, (1<<estadosetup)
;;impossivel chegar aqui mas vai que né
clr opi
reti

contarup:
ldi r29, 15
clr carry
lds opi, addrcurcent
mov centena, opi
lds opi, addrcurdez
mov dezena,opi
lds opi, addrcuruni
mov unidade, opi
ldi opi, 10
add unidade, step
compareunidade:
cp unidade, opi
brlt somardezena
inc carry
sub unidade, opi
rjmp compareunidade
somardezena:
add dezena, carry
clr carry
comparedezena:
cp dezena, opi
brlt somarcentena
inc carry
sub dezena, opi
rjmp comparedezena
somarcentena:
add centena, carry
cp centena, opi
brlt comparecommax
clr centena
comparecommax:
lds opi, addrmaxcent
cp centena, opi
brlt salvenovovalor
brne overflowcountup
lds opi, addrmaxdez
cp dezena, opi
brlt salvenovovalor
brne overflowcountup
lds opi, addrmaxuni
cp unidade, opi
brlt salvenovovalor 
;;se chegou aqui estourou o valor do contador
;; vai para valor máximo
overflowcountup:
lds opi, addrmaxcent
mov centena, opi
lds opi, addrmaxdez
mov dezena, opi
lds opi, addrmaxuni
mov unidade, opi
clr opi
salvenovovalor:
mov opi, centena
sts addrcurcent, opi
mov opi, dezena
sts addrcurdez, opi
mov opi, unidade
sts addrcuruni, opi
clr opi
ret


contardown:
clr carry
lds opi, addrcurcent
mov centena, opi
lds opi, addrcurdez
mov dezena,opi
lds opi, addrcuruni
mov unidade, opi
ldi opi, 10
compareunidadesubtracao:
cp unidade, step
brge subunidade
inc carry
add unidade, opi
rjmp compareunidadesubtracao
subunidade:
sub unidade,step
mov aux1, carry
clr carry
comparedezenasubtracao:
cp dezena, aux1
brge subdezena
inc carry
add dezena, opi
rjmp comparedezenasubtracao
subdezena:
sub dezena, aux1
clr aux1
comparecentenasubtracao:
cp centena,carry
brge subcentena
mov centena, carry
subcentena:
sub centena, carry

comparecommin:
lds opi, addrmincent
cp centena, opi
breq comparemindez
brge salvenovovalor
comparemindez:
lds opi, addrmindez
cp dezena, opi
breq compareminuni
brge salvenovovalor
compareminuni:
lds opi, addrminuni
cp unidade, opi
brge salvenovovalor
;;underflow sub
mov unidade, opi
lds opi,addrmindez
mov dezena, opi
lds opi,addrmincent
mov centena, opi
clr opi
rjmp salvenovovalor

;;supreme_loop
supreme_loop:
cpi estado, (1<<estadocontarup)
breq estado_contar_up
cpi estado, (1<<estadocontardown)
breq estado_contar_down
cpi estado, (1<<estadosetup)
breq estado_setup
rjmp end_supreme_loop

estado_contar_up:
rcall contarup
rcall superwaiting
rjmp end_supreme_loop

estado_contar_down:
rcall contardown
rcall superwaiting
rjmp end_supreme_loop

estado_setup:
rcall int_config_cont

end_supreme_loop:
rcall exibe_valor_atual
rjmp supreme_loop
;;;supreme loop

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

waiting:
ldi aux2, 10
wait_loop:
dec aux2
breq end_wait
rjmp wait_loop
end_wait:
ret


oconverter:
;;conversao com logica inversa e ordem oposta
;; vholder[3:0] -> -aux[4:7]
;; volder[3] -> -aux[4]
;; vholder[2] -> -aux[5]
;; vholder [1] -> -aux[6]
;; vholder [0] -> -aux[7]
clr output
;; se não for logica inversa, usar sbrc
sbrc vholder, 3
sbr output, 0b00010000
sbrc vholder, 2
sbr output, 0b00100000
sbrc vholder, 1
sbr output, 0b01000000
sbrc vholder, 0
sbr output, 0b10000000
ret
;;;;----output-converter-end-------
exibircentena:
mov vholder, centena
rcall oconverter
cbr output, 0b00001100 ;;sbr se inverso
in opi, pinb
sbr opi, (1<<0);; cbr se inverso
out portb, opi
out portd, output
rcall waiting
rcall waiting
ret

exibirdezena:
mov vholder,dezena
rcall oconverter
sbr output, 0b00001000 ;;cbr se inverso
cbr output, 0b00000100 ;; sbr se inverso
in opi, pinb
cbr opi, (1<<0);; sbr se inverso
out portb, opi
out portd, output 
rcall waiting
rcall waiting
ret

exibirunidade:
mov vholder, unidade
rcall oconverter
cbr output, 0b00001000 ;;sbr se inverso
sbr output, 0b00000100 ;; cbr se inverso
in opi, pinb
cbr opi, (1<<0);; sbr se inverso
out portb, opi
out portd, output 
rcall waiting
rcall waiting
ret


int_config_cont:
;;acende o led
;;controle: bit 1, ajuste minimo; bit 2, ajuste máximo, bit 3, ajuste step

cpi controle, (1<<1)
rcall setup_lower_bound
cpi controle, (1<<2)
rcall setup_upper_bound
cpi controle, (1<<3)
rcall setup_step_intermediario
cpi controle, (1<<0)
rcall end_interrupt_setup
ldi controle, 0x01
rjmp end_interrupt_setup

setup_step_intermediario:
rjmp setup_step
setup_lower_bound:
;;muda cor do led
in opi, pinb
cbr opi, (1<<blue)
sbr opi, (1<<red)
cbr opi, (1<<green)
out portb, opi
;;pega valor do adc
rcall ler_adc
;;chama binbcd
rcall bin_bcd_10bits
;;carrega o valor nos regs
rcall exibircentena
rcall exibirdezena
rcall exibirunidade
;;checa se botão foi apertado
;;se sim pula para end_setup_lower_bound
in aux1, PINC
sbrc aux1, botaosetup ;; ou sbrs
rjmp end_setup_lower_bound
rjmp setup_lower_bound

end_setup_lower_bound:
clr aux1
mov opi, centena
sts addrmincent, opi
mov opi, dezena
sts addrmindez, opi
mov opi, unidade
sts addrminuni, opi
rjmp end_interrupt_setup



end_interrupt_setup:
lsl controle 
ret

setup_upper_bound:
;;muda cor do led
in opi, pinb
cbr opi, (1<<blue)
cbr opi, (1<<red)
sbr opi, (1<<green)
out portb, opi
;;pega valor do adc
rcall ler_adc
;;chama binbcd
rcall bin_bcd_10bits
;;carrega o valor nos regs
rcall exibircentena
rcall exibirdezena
rcall exibirunidade
;;checa se botão foi apertado
;;se sim pula para end_setup_upper_bound
in aux1, PINC
sbrc aux1, botaosetup ;; ou sbrs
rjmp end_setup_upper_bound
rjmp setup_upper_bound

end_setup_upper_bound:
clr aux1
mov opi, centena
sts addrmaxcent, opi
mov opi, dezena
sts addrmaxdez, opi
mov opi, unidade
sts addrmaxuni, opi
rjmp end_interrupt_setup

setup_step:
;;muda cor do led
in opi, pinb
cbr opi, (1<<blue)
cbr opi, (1<<red)
sbr opi, (1<<green)
out portb, opi
;;pega valor do adc
rcall ler_adc
;;chama binbcd
rcall bin_bcd_10bits
;;carrega o valor nos regs
rcall exibircentena
rcall exibirdezena
rcall exibirunidade
;;checa se botão foi apertado
;;se sim pula para end_setup_step
in aux1, PINC
sbrc aux1, botaosetup ;; ou sbrs
rjmp end_setup_step_p1
rjmp setup_step

end_setup_step_p1:
mov opi, dezena
cpi opi, 0
breq skip_add_10_setup
ldi opi, 10
skip_add_10_setup:
add opi, unidade
cpi opi, 16
brlo end_setup_step_p2
ldi opi, 15
end_setup_step_p2:
mov step, opi
rjmp end_interrupt_setup

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


ler_adc:
lds aux2, ADCSRA
sbr aux2, (1<<ADSC)
sts ADCSRA, aux2
wait_conversao_ADC:
lds aux2, ADCSRA
sbrs aux2, ADIF
rjmp wait_conversao_ADC
lds XL, ADCL
lds XH, ADCH
lds aux2, ADCSRA
sbr aux2, (1<<ADIF)
sts ADCSRA, aux2
ret


superwaiting:
ldi r28, 0xFF
waitinglp1:
rcall waiting
dec r28
breq end_super_waiting
rjmp waitinglp1
end_super_waiting:
ret

exibe_valor_atual:
lds opi, addrcurcent
mov centena, opi
lds opi, addrcurdez
mov dezena, opi
lds opi, addrcuruni
mov unidade, opi


rcall exibircentena
rcall exibirdezena
rcall exibirunidade
ret