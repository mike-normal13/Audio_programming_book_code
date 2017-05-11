sr=44100
ksmps=1
nchnls=2

	opcode getFrequency,i,i

ipch 	xin

iout	= (ipch < 15 ? cpspch(ipch) : ipch)
	
	xout	iout


	endop
	opcode yiEnvelope,k,ioooooo

ienvType, iattack, idecay, isus, irel, iOldAmp, iNewAmp	xin

itie	tival
idur = p3

iEndAmp = iNewAmp / iOldAmp


if (ienvType == 0) then
	kenv	adsr	iattack, idecay, isus, irel
elseif (ienvType == 1) then	
	kenv 	linseg	0, p3 * .5, 1, p3 * .5, 0
	kenv logcurve kenv, 1.5
elseif (ienvType == 2) then	
	kenv	linseg 	0, p3 - .1, 1, .1, 0	

elseif (ienvType == 3) then

    if (itie == 0 && p3 < 0) then
        ; this is an initial note within a group of tied notes
        kenv	linseg	0, .2, 1,  .2, 1
       
    elseif (p3 > 0 && itie == 1) then
        ; this is an end note out of a group of tied notes
        kenv linseg	1, p3 - 1, 1, 1, 0
 
    elseif (p3 > 0 && itie == 0) then
        ; this is a stand alone note
        kenv adsr iattack, idecay, isus, irel
    else
        ; this is a middle note within a group of tied notes (p3 < 0 && itie == 1)
        kenv line 1, abs(p3), iEndAmp
    endif  

endif

	xout 	kenv


	endop
	opcode pan_gm2,aa,ak

ain, kspace xin

klast init -2
kleft init 0
kright init 0

;from MIDI GM-2 Default Pan Curve (RP-036)
;Left Channel Gain [dB] = 20*log (cos (Pi/2* max(0,CC#10 ? 1)/126)
;Right Channel Gain [dB] = 20*log (sin (Pi /2* max(0,CC#10 ? 1)/126)

if (kspace != klast) then
 kpercent = (kspace + 1) / 2
 kleft = ampdb(20 * log(cos($M_PI_2 * kpercent)))
 kright = (kpercent == 0) ? 0 : ampdb(20 * log(sin($M_PI_2 * kpercent)))
endif

aleft = ain * kleft
aright = ain * kright

xout aleft, aright

	endop


	instr 1	;Reson 6 - Orch
ipch 	= p4
ipch2	= p5
ipch 	= (ipch < 15 ? cpspch(ipch) : ipch)
ipch2 	= (ipch2 < 15 ? cpspch(ipch2) : ipch2)
kpchline 	line ipch, p3, ipch2
iamp 	= ampdb(p6)
iSpace	= p7
ienvType	= p8
iQ    = 29.881298
kenv 	yiEnvelope ienvType, 0.099999994, 0.014572331, 0.89667374, 0.1461223
imaxdel = (1/ipch > 1/ipch2) ? 1/ipch : 1/ipch2
;INSERT SOUND GENERATION CODE HERE
adel init 0
aexc1	vco2 1, kpchline
aexc = aexc1 + adel
aout = 0
if (1 == 1) then
	aout0 reson aexc * 1.0, kpchline * 1.0, iQ * 0.70333993
	aout sum aout, aout0
endif
if (1 == 1) then
	aout1 reson aexc  * 1.0, kpchline * 2.0, iQ * 0.6287569
	aout sum aout, aout1
endif
if (1 == 1) then
	aout2 reson aexc  * 1.0, kpchline * 3.0, iQ * 0.77589726
	aout sum aout, aout2
endif
if (1 == 1) then
	aout3 reson aexc  * 1.0, kpchline * 4.0, iQ * 0.70936865
	aout sum aout, aout3
endif
if (1 == 1) then
	aout4 reson aexc  * 1.0, kpchline * 6.0, iQ * 0.6634556
	aout sum aout, aout4
endif
if (0 == 1) then
	aout5 reson aexc  * 1.0, kpchline * 8.0, iQ * 0.6587586
	aout sum aout, aout5
endif
aout	balance aout, aexc1
adel vdelay3 aout * 0.12512003, 1/kpchline, imaxdel
adel = -adel
aout = aout * iamp * kenv
;END SOUND GENERATION CODE
aLeft, aRight pan_gm2 aout, iSpace
outc aLeft, aRight

	endin

	instr 4	;untitled
ktime	times
printks "blueTimePointer=%f\\n", 0.05, ktime
	endin


