<CsoundSynthesizer>
<CsOptions> 
</CsOptions>
<CsInstruments>

sr= 44100
ksmps = 64
nchnls= 1

instr 1

aosc   oscili 3000, p4, 1
fs1 pvsanal aosc, 1024,256,1024,1
fs2 pvstransp fs1, 1.25, 1
asig pvsynth fs2
    outs asig+aosc

endin



</CsInstruments>
<CsScore>
f1 0 16384 10 1
i1 0 100 440  
</CsScore>
</CsoundSynthesizer>