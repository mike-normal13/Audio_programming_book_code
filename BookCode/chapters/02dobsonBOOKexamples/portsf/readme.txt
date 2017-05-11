The Portsf test programs.

These programs demonstrate and exercise the portsf seek and overwrite functions. These functions have been modified very recently to support large soundfiles (> 2GB), and also to be portable to both 32bit and 64bit platforms.

sfreverse:  reverses a soundfile by the extreme method of seeking backwards from the end one frame at a time.


sfrewind: demonstrates the read/rewind/modify function of portsf.
It generates a  chirp signal (rising frequency sinusoid), with a slow rising amplitude and a short terminating decay ramp. It them rewinds 1/3 of the way, and rescales the data previously written. 
Best used with a duration of 10 seconds or so. For the most extreme test, try a huge file (> 2GB)!

This is not a recommended way of creating or processing soundfiles, but there will always be that one situation where it seems appropriate! 
It does however form a reasonably robust test of the random-access functions of portsf: psf_sndSeek, psf_sndTell, etc.

This program writes a TPDF-dithered 16bit soundfile - it will be easy to observe the effect of dither at the start of the file.


RWD August 2009.


