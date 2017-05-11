;==============================================================================; 
;==============================================================================; 
;==============================================================================;
;                            == TRAPPED IN CONVERT ==                          ;
;                                Richard Boulanger                             ;
;==============================================================================; 
;==============================================================================; 
;                                   ORCHESTRA                                  ;
;==============================================================================; 
;====================================================================; 
;====================================================================;
;                  written July 1979 in music11                      ;
;                M.I.T. Experimental Music Studio                    ; 
;                  revised June 1986 in Csound                       ;
;                      M.I.T. Media Lab                              ;
;                revised July 1996 in SHARCsound                     ;
;                      Analog Devices Inc.                           ;
;              Stolen by John ffitch for D &DC #4                    ;
;                        Sept 2002                                   ;
;====================================================================; 
;====================================================================; 
;========================= HEADER ===================================;
;====================================================================; 
  sr        =           44100
  kr        =           441
  ksmps     =           100
  nchnls    =           2
;====================================================================; 
;===================== INITIALIZATION ===============================;
;====================================================================; 
  garvb     init        0
  gadel     init        0
  garvbq    init        0
  gadelq    init        0
  giscale   init        0.7

;====================================================================; 
;======================== INSTRUMENTS ===============================;
;====================================================================; 
;========================== IVORY ===================================;
;====================================================================; 
instr 1                            ; p6 = AMP 
  p3        =           p3+p3
  ifreq     =           p5                      ; p7 = VIB RATE
                                                ; p8 = GLIS. DEL TIME (DEFAULT < 1)
  aglis     expseg      1, p8, 1, p3-p8, p9     ; p9 = FREQ DROP FACTOR

  k1        line        0, p3, 5
  k2        oscil       k1, p7, 1
  k3        linseg      0, p3*.7, p6, p3*.3, 0
  a1        oscil3      k3, (ifreq+k2)*aglis, 1

  k4        linseg      0, p3*.6, 6, p3*.4, 0
  k5        oscil       k4, p7*.9, 1, 1.4
  k6        linseg      0, p3*.9, p6, p3*.1, 0
  a3        oscil3      k6, ((ifreq+.009)+k5)*aglis, 9, .2

  k7        linseg      9, p3*.7, 1, p3*.3, 1
  k8        oscil       k7, p7*1.2, 1, .7
  k9        linen       p6, p3*.5, p3, p3*.333
  a5        oscil3      k9, ((ifreq+.007)+k8)*aglis, 10, .3

  k10       expseg      1, p3*.99, 3.1, p3*.01, 3.1
  k11       oscil       k10, p7*.97, 1, .6
  k12       expseg      .001, p3*.8, p6, p3*.2, .001
  a7        oscil3      k12, ((ifreq+.005)+k11)*aglis, 11, .5

  k13       expseg      1, p3*.4, 3, p3*.6, .02
  k14       oscil       k13, p7*.99, 1, .4
  k15       expseg      .001, p3*.5, p6, p3*.1, p6*.6, p3*.2, p6*.97, p3*.2, .001
  a9        oscil3      k15, ((ifreq+.003)+k14)*aglis, 12, .8

  k16       expseg      4, p3*.91, 1, p3*.09, 1
  k17       oscil       k16, p7*1.4, 1, .2
  k18       expseg      .001, p3*.6, p6, p3*.2, p6*.8, p3*.1, p6*.98, p3*.1, .001
  a11       oscil3      k18, ((ifreq+.001)+k17)*aglis, 13, 1.3

  al        dcblock     a1+a3+a5
  ar        dcblock     a7+a9+a11
            outs        giscale*al, giscale*ar
endin 
;====================================================================; 
;========================== BLUE ====================================;
;====================================================================; 
instr 2                               ; p6 = AMP   
  ifreq     =           p5                      ; p7 = REVERB SEND FACTOR
                                                ; p8 = LFOFREQ
  k1        randi       1, 30                   ; p9 = NUMBER OF HARMONIC      
  k2        linseg      0, p3*.5, 1, p3*.5, 0   ; p10 = SWEEP RATE   
  k3        linseg      .005, p3*.71, .015, p3*.29, .01
  k4        oscil       k2, p8, 1, .2
  k5        =           k4+2

  ksweep    linseg      p9, p3*p10, 1, p3*(p3-(p3*p10)), 1

  kenv      expseg      .001, p3*.01, p6, p3*.99, .001
  asig      gbuzz       kenv, ifreq+k3, k5, ksweep, k1, 15

  asig      dcblock     asig
            outs        giscale*asig, giscale*asig
  garvb     =           garvb+(asig*p7)
endin 
;====================================================================; 
;========================== VIOLET ==================================;
;====================================================================; 
instr 3, 103                                    ; p6 = AMP
  ifreq     =           p5                      ; p7 = REVERB SEND FACTOR
                                                ; p8 = RANDFREQ
  k3        expseg      1, p3*.5, 30, p3*.5, 2
  k4        expseg      10, p3*.7, p8, p3*.3, 6
  k8        linen       p6, p3*.333, p3, p3*.333
  k13       line        0, p3, -1

  k14       randh       k3, k4, .5
  a1        oscil3      k8, ifreq+(p5*.05)+k14+k13, 1, .1

  k1        expseg      1, p3*.8, 6, p3*.2, 1
  k6        linseg      .4, p3*.9, p8*.96, p3*.1, 0
  k7        linseg      8, p3*.2, 10, p3*.76, 2

  kenv2     expseg      .001, p3*.4, p6*.99, p3*.6, .0001
  k15       randh       k6, k7
  a2        buzz        kenv2, ifreq+(p5*.009)+k15+k13, k1, 1, .2

  kenv1     linen       p6, p3*.25, p3, p3*.4
  k16       randh       k4*1.4, k7*2.1, .2
  a3        oscil3      kenv1, ifreq+(p5*.1)+k16+k13, 16, .3

  amix      =           a1+a2+a3
            outs        giscale*(a1+a3), giscale*(a2+a3)
if p1 == 103 goto qu
  garvb     =           garvb+(amix*p7)
            goto        end
qu:
  garvbq    =           garvbq+(amix*p7)
end:
endin 
;====================================================================; 
;========================== BLACK ===================================;
;====================================================================; 
instr 4                             ; p6 = AMP 
  ifreq     =           p5                      ; p7 = FILTERSWEEP STRTFREQ
                                                ; p8 = FILTERSWEEP ENDFREQ
  k1        expon       p7, p3, p8              ; p9 = BANDWIDTH
  anoise    rand        8000                    ; p10 = REVERB SEND FACTOR 
  a1        reson       anoise, k1, k1/p9, 1
  k2        oscil       0.6, 11.3, 1, 0.1
  k3        expseg      0.001, p3*0.001, p6, p3*0.999, 0.001
  a2        oscil3      k3, ifreq+k2, 15

            outs        giscale*((a1*0.8)+a2), giscale*((a1*0.6)+(a2*0.7))
  garvb     =           garvb+(a2*p10)
endin 
;====================================================================; 
;========================== GREEN ===================================;
;====================================================================; 
instr 5                                         ; p6 = AMP    
  ifreq     =           p5                      ; p7 = REVERB ;SEND FACTOR
                                                ; p8 = PAN DIRECTION 
  k1        line        p9, p3, 1               ; ... (1.0 = L -> R, 0.1 = R -> L)
  k2        line        1, p3, p10              ; p9 = CARRIER FREQ  
  k4        expon       2, p3, p12              ; p10 = MODULATOR FREQ
  k5        linseg      0, p3*.8, 8, p3*.2, 8   ; p11 = MODULATION INDEX
  k7        randh       p11, k4                 ; p12 = RAND FREQ                  
  k6        oscil       k4, k5, 1, .3

  kenv1     linen       p6, .03, p3, .2
  a1        foscil      kenv1, ifreq+k6, k1, k2, k7, 1

  kenv2     linen       p6, .1, p3, .1
  a2        oscil3      kenv2, ifreq*1.001, 1

  amix      =           a1+a2
  kpan      linseg      int(p8), p3*.7, frac(p8), p3*.3, int(p8)
            outs       giscale*amix*kpan, giscale*amix*(1-kpan)
  garvb     =           garvb+(amix*p7)
endin 
;====================================================================; 
;======================== COPPER ====================================;
;====================================================================; 
instr 6, 106                                    ; p5 = FILTERSWEEP STARTFREQ
  ifuncl    =           8                       ; p6 = FILTERSWEEP ENDFREQ
                                                ; p7 = BANDWIDTH
  k1        phasor      p4                      ; p8 = REVERB SEND FACTOR
  k2        table       k1*ifuncl, 19           ; p9 = AMP       
  anoise    rand        8000
  k3        expon       p5, p3, p6
  a1        reson       anoise, k3*k2, k3/p7, 1

  kenv      linen       p9, .01, p3, .05
  asig      =           a1*kenv

            outs        giscale*asig, giscale*asig
if p1==106 goto qu
  garvb     =           garvb+(asig*p8)
goto end
qu:
  garvbq    =           garvbq+(asig*p8)
end:
endin 
;====================================================================; 
;========================== PEWTER ==================================;
;====================================================================; 
instr 7, 107                                    ; p4 = AMP
  ifuncl    =           512                     ; p5 = FREQ
  ifreq     =           p5                      ; p6 = BEGIN PHASE POINT        
                                                ; p7 = END PHASE POINT
  a1        oscil3      1, ifreq, p10           ; p8 = CTRL OSC AMP (.1 -> 1)   
  k1        linseg      p6, p3*.5, p7, p3*.5, p6 ; p9 = CTRL OSC FUNC      
  a3        oscili      p8, ifreq+k1, p9        ; p10 = MAIN OSC FUNC (f2 OR f3)
  a4        phasor      ifreq                   ; ...(FUNCTION LENGTH MUST BE 512!) 
  a5        table       (a4+a3)*ifuncl, p10     ; p11 = REVERB SEND FACTOR    

  kenv      linen       p4, p3*.4, p3, p3*.5
  asig      =           kenv*((a1+a5)*.2)

            outs        giscale*asig, giscale*asig
if p1==107 goto qu
  garvb     =           garvb+(asig*p11)
            goto        end
qu:
  garvbq    =           garvbq+(asig*p11)
end:
endin 
;====================================================================; 
;========================== RED =====================================;
;====================================================================; 
instr 8, 108                                    ; p4 = AMP
  ifuncl    =           16                      ; p5 = FILTERSWEEP STARTFREQ
                                                ; p6 = FILTERSWEEP ENDFREQ
  k1        expon       p5, p3, p6              ; p7 = BANDWIDTH 
  k2        line        p8, p3, p8*.93          ; p8 = CPS OF rand1
  k3        phasor      k2                      ; p9 = CPS OF rand2
  k4        table       k3*ifuncl, 20           ; p10 = REVERB SEND FACTOR     
  anoise    rand        8000
  aflt1     reson       anoise, k1, 20+(k4*k1/p7), 1

  k5        linseg      p6*.9, p3*.8, p5*1.4, p3*.2, p5*1.4
  k6        expon       p9*.97, p3, p9
  k7        phasor      k6
  k8        tablei      k7*ifuncl, 21
  aflt2     reson       anoise, k5, 30+(k8*k5/p7*.9), 1

  abal      oscil3      1000, 1000, 1
  a3        balance     aflt1, abal
  a5        balance     aflt2, abal

  k11       linen       p4, .15, p3, .5
  a3        =           a3*k11
  a5        =           a5*k11

  k9        randh       1, k2
  aleft     =           ((a3*k9)*.7)+((a5*k9)*.3)
  k10       randh       1, k6
  aright    =           ((a3*k10)*.3)+((a5*k10)*.7)
            outs        giscale*aleft, giscale*aright
if p1==108 goto qu
  garvb     =           garvb+(a3*p10)
            goto        end
qu:
  garvbq    =           garvbq+(a3*p10)
end:
endin 
;====================================================================; 
;========================== SAND ====================================;
;====================================================================; 
instr 9, 109                                    ; p4 = DELAY SEND FACTOR    
  ifreq     =           p5                      ; p5 = FREQ     
                                                ; p6 = AMP
  k2        randh       p8, p9, .1              ; p7 = REVERB SEND FACTOR 
  k3        randh       p8*.98, p9*.91, .2      ; p8 = RAND AMP     
  k4        randh       p8*1.2, p9*.96, .3      ; p9 = RAND FREQ    
  k5        randh       p8*.9, p9*1.3

  kenv      linen       p6, p3*.1, p3, p3*.8

  a1        oscil3      kenv, ifreq+k2, 1, .2
  a2        oscil3      kenv*.91, (ifreq+.004)+k3, 2, .3
  a3        oscil3      kenv*.85, (ifreq+.006)+k4, 3, .5
  a4        oscil3      kenv*.95, (ifreq+.009)+k5, 4, .8

  amix      =           a1+a2+a3+a4

            outs        giscale*(a1+a3), giscale*(a2+a4)
if p1 == 109 goto qu
  garvb     =           garvb+(amix*p7)
  gadel     =           gadel+(amix*p4)
            goto        end
qu:
  garvbq    =           garvbq+(amix*p7)
  gadelq    =           gadelq+(amix*p4)
end:
endin 
;====================================================================; 
;========================== TAUPE ===================================;
;====================================================================; 
instr 10                            
  ifreq     =           p5                      ; p5 = FREQ     
                                                ; p6 = AMP
  k2        randh       p8, p9, 0.1             ; p7 = REVERB SEND FACTOR 
  k3        randh       p8*0.98, p9*0.91, 0.2   ; p8 = RAND AMP     
  k4        randh       p8*1.2, p9*.96, 0.3     ; p9 = RAND FREQ    
  k5        randh       p8*.9, p9*1.3

  kenv      linen       p6, p3*0.1, p3, p3*0.8

  a1        oscil3      kenv, ifreq+k2, 1, 0.2
  a2        oscil3      kenv*.91, (ifreq+.004)+k3, 2, 0.3
  a3        oscil3      kenv*.85, (ifreq+.006)+k4, 3, 0.5
  a4        oscil3      kenv*.95, (ifreq+.009)+k5, 4, 0.8

  amix      =           a1+a2+a3+a4

            outs        giscale*(a1+a3), giscale*(a2+a4)
  garvb     =           garvb+(amix*p7)
endin 
;====================================================================; 
;========================== RUST ====================================;
;====================================================================; 
instr 11                                        ; p4 = DELAY SEND FACTOR
  ifreq     =           p5                      ; p5 = FREQ             
                                                ; p6 = AMP
  k1        expseg      1, p3*.5, 40, p3*.5, 2  ; p7 = REVERB SEND FACTOR
  k2        expseg      10, p3*.72, 35, p3*.28, 6
  k3        linen       p6, p3*.333, p3, p3*.333
  k4        randh       k1, k2, .5
  a4        oscil3      k3, ifreq+(p5*.05)+k4, 1, .1

  k5        linseg      .4, p3*.9, 26, p3*.1, 0
  k6        linseg      8, p3*.24, 20, p3*.76, 2
  k7        linen       p6, p3*.5, p3, p3*.46
  k8        randh       k5, k6, .4
  a3        oscil3      k7, ifreq+(p5*.03)+k8, 14, .3

  k9        expseg      1, p3*.7, 50, p3*.3, 2
  k10       expseg      10, p3*.3, 45, p3*.7, 6
  k11       linen       p6, p3*.25, p3, p3*.25
  k12       randh       k9, k10, .5
  a2        oscil3      k11, ifreq+(p5*.02)+k12, 1, .1
  k13       linseg      .4, p3*.6, 46, p3*.4, 0
  k14       linseg      18, p3*.1, 50, p3*.9, 2
  k15       linen       p6, p3*.2, p3, p3*.3
  k16       randh       k13, k14, .8
  a1        oscil3      k15, ifreq+(p5*.01)+k16, 14, .3

  amix      =           a1+a2+a3+a4
            outs        giscale*(a1+a3), giscale*(a2+a4)
  garvb     =           garvb+(amix*p7)
  gadel     =           gadel+(amix*p4)
endin 
;====================================================================; 
;========================== TEAL ====================================;
;====================================================================; 
instr 12                                        ; p6 = AMP
  ifreq     =           octpch(p5)              ; p7 = FILTERSWEEP STARTFREQ
  ifuncl    =           8                       ; p8 = FILTERSWEEP PEAKFREQ
                                                ; p9 = BANDWDTH
  k1        linseg      0, p3*0.8, 9, p3*0.2, 1 ; p10 = REVERB SEND FACTOR
  k2        phasor      k1
  k3        table       k2*ifuncl, 22
  k4        expseg      p7, p3*0.7, p8, p3*0.3, p7*0.9

  anoise    rand        8000

  aflt      reson       anoise, k4, k4/p9, 1
  kenv1     expseg      .001, p3*0.1, p6, p3*0.1, p6*0.5, p3*0.3, p6*0.8, p3*0.5, .001
  a3        oscil3      kenv1, cpsoct(ifreq+k3)+aflt*0.8, 1

            outs        giscale*a3, giscale*((a3*0.98)+(aflt*0.3))
  garvb     =           garvb+(anoise*p10)
endin 
;====================================================================; 
;========================== FOAM ====================================;
;====================================================================; 
instr 13                                        ; p6 = AMP
  ifreq     =           octpch(p5)              ; p7 = VIBRATO RATE
                                                ; p8 = GLIS.FACTOR
  k1        line        0, p3, p8
  k2        oscil       k1, p7, 1
  k3        linseg      0, p3*.7, p6, p3*.3, 1
  a1        oscil3      k3, cpsoct(ifreq+k2), 1

  k4        linseg      0, p3*.6, p8*.995, p3*.4, 0
  k5        oscil       k4, p7*.9, 1, .1
  k6        linseg      0, p3*.9, p6, p3*.1, 3
  a2        oscil3      k6, cpsoct((ifreq+.009)+k5), 4, .2

  k7        linseg      p8*.985, p3*.7, 0, p3*.3, 0
  k8        oscil       k7, p7*1.2, 1, .7
  k9        linen       p6, p3*.5, p3, p3*.333
  a3        oscil3      k6, cpsoct((ifreq+.007)+k8), 5, .5

  k10       expseg      1, p3*.8, p6, p3*.2, 4
  k11       oscil       k10, p7*.97, 1, .6
  k12       expseg      .001, p3*.99, p8*.97, p3*.01, p8*.97
  a4        oscil3      k12, cpsoct((ifreq+.005)+k11), 6, .8

  k13       expseg      .002, p3*.91, p8*.99, p3*.09, p8*.99
  k14       oscil       k13, p7*.99, 1, .4
  k15       expseg      .001, p3*.5, p6, p3*.1, p6*.6, p3*.2, p6*.97, p3*.2, .001
  a5        oscil3      k15, cpsoct((ifreq+.003)+k14), 7, .9

  k16       expseg      p8*.98, p3*.81, .003, p3*.19, .003
  k17       oscil       k16, p7*1.4, 1, .2
  k18       expseg      .001, p3*.6, p6, p3*.2, p6*.8, p3*.1, p6*.98, p3*.1, .001
  a6        oscil3      k18, cpsoct((ifreq+.001)+k17), 8, .1

            outs        giscale*(a1+a3+a5), giscale*(a2+a4+a6)
endin 
;====================================================================; 
;========================== SMEAR ===================================;
;====================================================================; 
instr 98
  aenv      transeg     1, 387, 0, 1, p3-387, -1, 0.0000001
  asigl     delay       gadel, .18
  asigr     delay       gadel, .37
            outs        giscale*aenv*asigl, giscale*aenv*asigr
  gadel     =           0
endin 
instr 198
  asigl     delay       gadelq, .18
  asigr     delay       gadelq, .37
            outs        giscale*asigl, giscale*asigr
  gadelq    =           0
endin 
;====================================================================; 
;========================== SWIRL ===================================;
;====================================================================; 
instr 99                            ; p4 = PANRATE
  aenv      transeg     0.5, 392, 0, 0.5, p3-392, -1, 0.0000001
  ksw       linseg     p4, 200, p4, 31.5, 0.001, p3-231.1, p4
  k1        oscil       .5, ksw, 1
  k2        =           .5+k1
  k3        =           1-k2
  asig      nreverb     garvb, 3.1, .2
;;asig   reverb garvb, 3.1, .2
            outs        giscale*asig*k2*aenv, giscale*(asig*k3)*aenv*(-1)
  garvb     =           0
endin 
instr 199                            ; p4 = PANRATE
  aenv      linseg      0, 0.005, 1, p3-0.01, 1, 0.005, 0  ; Stop clicks
  k1        oscil       .5, p4, 1
  k2        =           .5+k1
  k3        =           1-k2
  asig      nreverb     garvbq, 3.1, .2
;;asig   reverb garvb, 3.1, .2
            outs        giscale*aenv*asig*k2, giscale*aenv*(asig*k3)*(-1)
  garvbq    =           0
endin 
;====================================================================; 
;====================================================================; 
;====================================================================; 
;====================================================================;

