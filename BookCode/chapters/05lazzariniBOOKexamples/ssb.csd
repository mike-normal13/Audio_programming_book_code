<CsoundSynthesizer>
<CsOptions>
-odac 
</CsOptions>
<CsInstruments>

; an example of the direct use of
; complex sinusoids in
; single-sideband (SSB) modulation
; here we have one sideband on the left
; and another on the right
; using a 440Hz sinewave as one input
; and a modulating sine of ifsht Hz
; if an arbitrary input is used, the
; 'hilbert' opcode can be used to
; split the signal into real/imag
; components

sr = 44100
kr = 4410
ksmps = 10
nchnls = 2
  
instr 1

  idur = p3
  ifsht = p4   ; freq shift amount in Hz 
  
 
  ; complex sinusoid a+ib
  ; a real-valued signal can be got by adding the
  ; positive and negative spectra
  ; positive spectra: a+ib
  ; negative spectra: a-ib
  aa  oscili 1000, 440,  1  
  ab  oscili 1000, 440,  2

  ; another complex sinusoid (c + id)
  ac oscili 1, ifsht, 1
  ad oscili 1, ifsht, 2
 
  ; the higher sideband is: 
  ; the pos spec of input * pos spec of sinusoid 
  ; plus
  ; the neg spec of input * neg spec of sinusoid
  ; the lower sideband is the other way round:
  ; (a+ib)*(c+id) + (a-ib)*(c-id) = 2*(ac - bd) 
  ; (a-ib)*(c+id) + (a+ib)*(c-id) = 2*(ac + bd)
  ashft1 = (aa*ac - ab*ad)
  ashft2 = (aa*ac + ab*ad)

  ; output both sidebands, one on each channel
  ; 440+ifsht, 440-isfht
  outs ashft1,ashft2

endin


</CsInstruments>
<CsScore>

; sine
f1 0 16384 9 1 1 90
f2 0 16384 9 1 1 0

i1 0 10 220
e


</CsScore>
</CsoundSynthesizer>
