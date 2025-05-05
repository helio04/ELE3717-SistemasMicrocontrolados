.include "m328pdef.inc"
.org 0x0000
rjmp setup
;; r19 - controle (bit 0, up//down , up=high, down=low) (bit 1 - conte1) (bit 2 - pare a contagem)
;; (bit 3 - displaymin) (bit 4 - display max) (bit 5 -displaystep) 
;; (bit 6 - start count) (bit 7 - display config)
;; r18 - step
;; r5 - contador-low
;; r4 - contador-high
;; r2 - max-value-low
;; r1 - max-value-high
;; r6 - min-value-high
;; r7 - min-value-low

setup:
;;configurar ADC

;;configurar interrupts 1,2,3

;;configurar contador modo padrão

rjmp loop
;;fimsetup

;;------------interrupts
interruptbot1:
sbrc r19,0b00000100
rjmp fiminterrupt1
sbr r19, 0b00000010 ;;conte1
sbr r19, 0b00000001 ;;conte up
rcall contador
fiminterrupt1:
reti
interruptbot3:
sbrc r19, 0b00000100
rjmp fiminterrupt3
sbr r19, 0b00000010
cbr r19, 0b00000001
rcall contador
fiminterrupt3:
reti

interruptbot2:
rcall lerpot ;; valor do pot do regX
rcall settingsuser ;; chama a config
reti
;;-------interrupts

;;-------settings
settingsuser:
sbrc r19, 3
rcall setmin
sbrc r19, 4
rcall setmax
sbrc r19, 5
rcall setstep
sbrc r19, 6
rcall setcountmode
ret
setmin:
mov r6, XH
mov r7, XL
cbr r19, 0b00001000
sbr r19, 0b00010000
ret
setmax:
mov r1, XH
mov r2, XL
cbr r19, 0b00010000
sbr r19, 0b00100000
ret
setstep:
mov r18, XL
cpi r18, 16
brlt endsetstep
ldi r18, 15
endsetstep:
cbr r19, 0b00100000
sbr r19, 0b01000000
ret
setcountmode:
cbr r19, 0b00000100 ;; bit 2 clear e bit 6 set ativa a contagem
ret
;;---------settings

contador:
sbrs r19, 0
rjmp contardown
contarup:
ldi r16,0 
add r5, r18 ;; somar contador low+step
adc r4,r16
cp r4,r1 
brge stopthecounting
cp r5, r2
brge stopthecounting
rjmp fimcontador
contardown:
ldi r16, 0
sub r5,r18
sbc r4,r16
cp r4, r1
brlt stopthecounting
cp r5, r2
brlt stopthecounting
rjmp fimcontador

stopthecounting:
sbr r19, 0b00000100
rjmp fimcontador

fimcontador:
cbr r19, 0b00000010
ret

;;;----loop
loop:
mov ZH,r4 ;; carrega em Z o valor do contador
mov ZL, r5
sbrc r19, 0b00000100
rcall load_X ;; X contem a leitura do potenciometro
sbrs r19, 0b01000000
rcall load_X
;;se não estiver em modo de contagem, exibe o valor de X
rcall exibirDisplay ;;funcao para exibir o valor de Z no display

rjmp loop
 
load_X:
mov ZH, XH
mov ZL, XL
ret

exibirDisplay:

rcall binbcd ;;; valor de Z em bcd em r20(c),r21(d),r22(u)
rcall displaybcd;; exibe o valor bcd no display 

ret ;; fim exibirDisplay

binbcd:
cpi ZH,3
breq ZHIG3
cpi ZH, 2
breq ZHIG2
cpi ZH, 1
breq ZHIG1

zhig3:
adiw r20, 7
adiw r21, 6
adiw r22, 8
zhig2:
adiw r20, 5
adiw r21, 1
adiw r22, 2
zhig1:
adiw r20, 2
adiw r21, 5
adiw r22, 6
dcentenas:
cpi ZL, 100
brlt ddezenas
inc r20
subi ZL, 100
rjmp dcentenas

ddezenas:
cpi ZL, 10
brlt dunidades
inc r21
subi ZL, 10
rjmp ddezenas

dunidades:
mov ZL, r22
ret ;; fimbinbcd
