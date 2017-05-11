<CsoundSynthesizer>
<CsOptions> 
</CsOptions>
<CsInstruments>

; beating the dead FM/PM horse
; instr 1,2,3 should have similar
; spectra safe some rounding errors
; instr 4, 5 should also be similar
; FM is somewhat less stable/predictable because
; of how oscillators integrate the
; phase

instr 1
; FM with foscili
indx = 3
irat = 1
ifc = 500
iamp = 10000
isine = 1

a1 foscili iamp, ifc, irat, 1/irat, indx, isine
   out a1

endin

instr 2
; FM with 2 oscilis
; identical to instr 1
indx = 3
irat = 1
ifc = 500
iamp = 10000
isine = 1

ifm = ifc/irat
a1 oscili indx*ifm, ifm, isine
a2 oscili iamp,ifc*irat+a1, isine

    out a2

endin

instr 3
; PM with cosine modulator
; and sine carrier is
; identical to instr 1
; when both indexes = 3
; other indexes might influence
; the phase integration in FM
; resulting in different spectra

indx = 3
irat = 1
ifc = 500
iamp = 10000
isine = 1
icos = 2
i2pi = 8*taninv(1) 

amod oscili indx/i2pi, ifc/irat, icos 
aph  phasor  ifc*irat
acar tablei  aph+amod, isine, 1, 0, 1
     out acar*iamp

endin

instr 4
; sinewave PM
; this is what Chowning's formula
; is supposed to describe
; so this spectrum should match
; his fourier expansion

indx = 3
irat = 1
ifc = 500
iamp = 10000
isine = 1
i2pi = 8*taninv(1) 

amod oscili indx/i2pi, ifc/irat, isine
aph  phasor  ifc*irat
acar tablei  aph+amod, isine, 1, 0, 1
     out acar*iamp

endin

instr 5
; FM version corresponding to instr 4
; Note differences between this
; and instr 2
indx = 3
irat = 1
ifc = 500
iamp = 10000
isine = 1
icos = 2
ifm = ifc/irat
a1 oscili indx*ifm, ifm, icos
a2 oscili iamp,ifc*irat-a1, isine

    out a2

endin


</CsInstruments>
<CsScore>
f2 0 32768 9 1 1 90
f1 0 32768 10 1 ;0.5 0.3 0.25 0.2

i1 0 2
i2 2 2
i3 4 2
i4 6 2
i5 8 2 

e

</CsScore>
</CsoundSynthesizer>