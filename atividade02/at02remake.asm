.include "m328pdef.inc"
.org 0x0000
rjmp setup

.equ centena, r3
.equ dezena, r4
.equ unidade, r5
.equ step, r6
.equ vholder, r7
.equ carry, r8

.equ opi, r16
.equ aux1, r17
.equ aux2, r18
.equ output, r19
.equ controle, r20 
.equ addrmaxcent, 0x0120
.equ addrmaxdez, 0x0121
.equ addrmaxuni, 0x0122
.equ addrmincent, 0x0123
.equ addrmindez, 0x0124
.equ addrminuni, 0x0125
.equ addrcurcent, 0x0110
.equ addrcurdez, 0x0111
.equ addrcuruni, 0x0112
setup:
;;configurar pinos

;;configurar ADC

;;configurar interrupts 1,2,3


;;configurar user_config
ldi controle, 0x01

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


rjmp loop


contarup:
clr carry
lds opi, addrcurcent
mov centena, opi
lds opi, addrcurdez
mov dezena,opi
lds opi, addrcuruni
mov uni, opi
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
mov uni, opi
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


loop:

lds opi, addrcurcent
mov centena, opi
lds opi, addrcurdez
mov dezena, opi
lds addrcuruni
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
sbrs vholder, 3
sbr output, 0b00010000
sbrs vholder, 2
sbr output, 0b00100000
sbrs vholder, 1
sbr output, 0b01000000
sbrs vholder, 0
sbr output, 0b10000000
ret
;;;;----output-converter-end-------
exibircentena:
rcall waiting
mov vholder, centena
rcall oconverter
cbr output, 0b00001100 ;;sbr se inverso
ldi opi, 0b00000001 ;; clr se inverso 
sts portb, opi
sts portd, output
rcall waiting
ret

exibirdezena:
rcall waiting
mov vholder,dezena
rcall oconverter
sbr output, 0b00001000 ;;cbr se inverso
cbr output, 0b00000100 ;; sbr se inverso
clr opi ;; ldi 1 se inverso
sts portb, opi
sts portd, output 
rcall waiting 
ret

exibirunidade:
rcall waiting
mov vholder, unidade
rcall oconverter
cbr output, 0b00001000 ;;sbr se inverso
sbr output, 0b00000100 ;; cbr se inverso
clr opi ;; ldi 1 se inverso
sts portb, opi
sts portd, output 
rcall waiting 
ret


int_config_cont:
;;acende o led
;;controle: bit 1, ajuste minimo; bit 2, ajuste máximo, bit 3, ajuste step
ldi opi, 0x02
cp controle, opi
breq setup_lower_bound
ldi opi, 0x04
cp controle, opi
breq setup_upper_bound
ldi opi, 0x08
cp controle, opi
breq setup_step
ldi opi, 0x01
cp controle, opi
breq end_interrupt_setup
ldi controle, 0x01

setup_lower_bound:
;;muda cor do led
;;pega valor do adc
;;chama binbcd
;;carrega o valor nos regs
rcall exibircentena
rcall exibirdezena
rcall exibirunidade
;;checa se botão foi apertado
;;se sim pula para end_setup_lower_bound
rjmp setup_lower_bound

end_setup_lower_bound:
mov opi, centena
sts addrmincent, opi
mov opi, dezena
sts addrmindez, opi
mov opi, unidade
sts addrminuni, opi
rjmp end_interrupt_setup



end_interrupt_setup:
lsl controle 
reti

setup_upper_bound:
;;muda cor do led
;;pega valor do adc
;;chama binbcd
;;carrega o valor nos regs
rcall exibircentena
rcall exibirdezena
rcall exibirunidade
;;checa se botão foi apertado
;;se sim pula para end_setup_upper_bound
rjmp setup_upper_bound

end_setup_upper_bound:
mov opi, centena
sts addrmaxcent, opi
mov opi, dezena
sts addrmaxdez, opi
mov opi, unidade
sts addrmaxuni, opi
rjmp end_interrupt_setup

setup_step:
;;muda cor do led
;;pega valor do adc
;;chama binbcd
;;carrega o valor nos regs
rcall exibircentena
rcall exibirdezena
rcall exibirunidade
;;checa se botão foi apertado
;;se sim pula para end_setup_step
rjmp setup_step

end_setup_step_p1:
clr opi
add opi, dezena
add opi, unidade
cpi opi, 16
brlt end_setup_step_p2
ldi opi, 15
end_setup_step_p2:
mov step, opi
rjmp end_interrupt_setup