MGSDRV v3.xx data format
========

## Structure

```
Offset                      |Block
----------------------------+--------------------------
0000                        | ASCII Header
0000 + Length of Text Block | Binary Header
*                           | Track 0 (Voice) Commands
*                           | Track 1 Commans
...                         |
*                           | Track 17 Commands
```

## ASCII Header
```
Offset              |# of bytes |Hex            |Description
--------------------+-----------+---------------+-------------------------
0000                |3 STRING   |4D 47 53       |"MGS"
0003                |3 STRING   |** ** **       |Version "304", "313", etc. 
0006                |2 STRING   |0D 0A          |CR LF
0008                |* STRING   |** .. ** 0D 0A |Title. ends with 0x0D 0x0A
0008 + Title Length |1 STRING   |1A 00          |EOF marker
```

### Known Versions
As far as the author know, following version strings exist.
```
300 302 303 304 307 308 309 310 311 313
```

If the MGS file is compressed by MGSARC.COM, the first letter of the version is changed to "A".
The author does not have MGSARC.COM but confirmed that the following version strings exist.
```
A00 A04 A07 A10 A11 A13
```

## Binary Header - Not Compressed
```
Offset |# of bytes |Hex |Description
-------+-----------+------------------------------------------------------------
0000   |1 BYTE     |00  |Marker 00H. MGSDRV recognizes this as top of header. 
0001   |1 BYTE     |**  |%0d-mmmlo
       |           |    | || |  |\- o:   #opll_mode 0|1
       |           |    | || |  \-- l:   #lfo_mode 0|1
       |           |    | || \----- mmm: #machine_id 0-7
       |           |    | |\------- d:   #disenable_mgsrc 0|1
       |           |    | \-------- 0:   compression flag
0002   |2 WORD     |    |#tempo. Valid value is 57 to 2047. 0 if Version < 304.
0004   |2 WORD     |    |Offset to Track 0 (Voice) Commands 
0006   |2 WORD     |    |Offset to Track 1 Commands
0008   |2 WORD     |    |Offset to Track 2 Commands
...    |...        |... |...
0026   |2 WORD     |    |Offset to Track 17 Commands
```

Track Offset is relative from the top of the binary header.

## Binary Header - Compressed
```
Offset |# of bytes |Hex |Description
-------+-----------+------------------------------------------------------------
0000   |1 BYTE     |00    |Marker 00H. MGSDRV recognizes this as top of header. 
0001   |1 BYTE     |**    |%1d-mmmlo
       |           |      | || |  |\- o:   #opll_mode 0|1
       |           |      | || |  \-- l:   #lfo_mode 0|1
       |           |      | || \----- mmm: #machine_id 0-7
       |           |      | |\------- d:   #disenable_mgsrc 0|1
       |           |      | \-------- 1:   compression flag
0002   |2 WORD     |** ** |Length of compressed data size
0004   |*          |*     |Compressed data
```

The compression algorithm is a sort of [dictionay coder](https://en.wikipedia.org/wiki/Dictionary_coder), seems to be specific for MGSDRV. The author will summarize the specification later.

An impelmentation to uncompress MGSDRV data is available: [uncompress.ts](https://github.com/digital-sound-antiques/mgsrc-js/blob/master/src/uncompress.ts).

## Track 0 Commands

```
00 kk dd .. : @v definition

   kk = %---vvvvv
            \------ vvvvv: patch number (0-31)
   dd .. = 8 bytes of OPLL voice parameter.

01 nn mm : @# definition

   nn = from patch number
   mm = to patch number

02 kk nn ll dd .. : @e definition
   
   kk = %---vvvvv
             \------ vvvvv: envelope number (0-31)
   nn = %0mmzzzzz
          || \----- zzzzz: noise freq. (0-31)
          |\------- mm   : mode (0-3)
          \-------- 0    : indicates that this is @e style.
   ll = length of envelope (1-255)
   dd .. = ll bytes of envelope commands

02 kk nn al ar dr sl sr rr : @r definition

   kk = %---vvvvv
            \------ vvvvv: envelope number (0-31)
   nn = %1mmzzzzz
         || \----- zzzzz: noise freq. (0-31)
         |\------- mode : mode (0-3)
         \-------- 1    : indicates that this it @r style.
   al = Attack Level
   ar = Attack Rate
   dr = Decay Rate
   sl = Sustain Level
   sr = Sustain Rate
   rr = Release Rate
          
03 kk dd .. : @s wave definition

   kk    = wave number (0-31)
   dd .. = 32 bytes of wave data.

04 [ll hh] x 12 : #psg_tune

05 [ll hh] x 12 : #opll_tune

06 tt ss .. .. 00  : @m definition. 
      
   ss .. .. 00  = NUL-terminated Shift-JIS text.

FF : Track termination marker.
```

### Envelope Commands
```
0n       : Volume Command; n: volume, corresponds to 0..f
10 nn    : @ command; nn: patch number(0..31)
11 rr dd : y command; rr: register, dd: value
12 nn    : \ command; nn: detune(-128..127)
2n cc    : = command; n: volume(0..f) cc: count
40       : [ command;
60       : ] command;
8n 9n    : n command; n0...n31 
An       : / command; n: mode(0...3)
En cc    : : command; n: volume(0..f), cc: count
```

## Track 1-17 Commands

```
0n ll .. : [Legacy MGS<304] Note with length command. 
 
       n = 0..B corresponds to note C to B.
   ll .. = length of note. 1 tick = 1/60s. If nn is 0FFH, the succeeding byte will be read and added to the length. 
           This procedure will continue until reading byte is not 0FFH.

1n       : [Legacy MGS<304] Note command. n=0H..BH corresponds to note C to B. 
           The length of note follows l (043H) command value.

2n ll    : Note with length command. 

    n = 0..B corresponds to note C to B.
   ll = length of note. 48 for the quarter note.

3n       : Note command. n=0H..BH corresponds to note C to B. 
           The length of note follows l (042H) command value.

2C nn    : Rest with length command. nn=length

3C       : Rest command.

40       : & command. Tie or slur

41 ll hh : tempo command. hhll=tempo. 48 bytes of workarea is followed by this command.

42 nn    : l command: nn=length.

43 ll hh : [Legacy MGS<304] l command. hhll=length. 1 tick = 1/60s.

44 0n    : q command. n=value

46 nn    : v+/v- command. nn=delta(signed, 2's complement)

48 nn    : k command. nn=value

49 nn    : @e command. nn=envelope number

4A       : / command. key off for FM channels.

4B nn    : s command. nn=hardware envelope number

4C ll hh : m command. hhll=hardware envelop speed

4D nn    : n command. nn=noise frequency

4E       : > command.

4F       : < command

50 nn    : \ command. nn=detune (signed, 2's complement)

51 ll hh : @\ command for PSG/SCC. hhll=detune (signed, 2's complement)

52 nn    : p command. nn=bend speed

53 nn    : _ command. nn=base note (similar to note command).
           must be followed with note command, or octave command then note command.

54 n1 n2 n3 n4 : h command.
                 n1 = 1 + (1st parameter of h)
                 n2 = 2nd parameter of h
                 n3 = 1 + (3rd parameter of h)
                 n4 = 4th parameter (signed, 2's complement)

55 ll hh : @p command. hhll=value (signed, 2's complement)

57 nn ll hh : [ command. nn = number of loops. hhll=offset to loop end?(not verified)

58 ll hh : | command. hhll=offset to exit loop.

59 00 ll hh : ] command. hhll=offset to loop start.

5A 00 : hf command in FM channel.
      : ko command in rhtyhm channel.

5A 01 : ho command in FM channel. 
        kf command in rhythm channel.

5B 00 : sf command.
5B 01 : so command.

5C n1 n2 : y command. n1=register, n2=value.

5D       : $ command.

5F nn    : @\ command for FM. nn=detune (unsigned).

60 nn    : @m command.

61 nn    : @o command.

63       : @f command.

64       : hi command.

8n       : @ command for patch  0..15. n=patch number.
9n       : @ command for patch 16..31. n=patch number - 16.

Cn       : v command. n=volume(0..15).

Dn       : o command. n=octave-1.

En       : / command for PSG. n=mode(0..3)

FF       : termination marker.
```

### Rhythm Commands (Track 15)
```
xx ll .. : [Legacy MGS<304] Rhythm note on/off with length (xx = 00...1F)

   xx = %000bsmch  
            ||||\- h: hi-hat
            |||\-- c: top-cym
            ||\--- m: tom-tom
            |\---- s: snare
            \----- b: bass drum
   ll .. = length of note. 1 tick = 1/60s. If nn is 0FFH, the succeeding byte will be read and added to the length. 
           This procedure will continue until the reading byte is not 0FFH.

xx ll : Rhythm note on/off with length (xx = 20...3F)

   xx = %001bsmch  
            ||||\- h: hi-hat
            |||\-- c: top-cym
            ||\--- m: tom-tom
            |\---- s: snare
            \----- b: bass drum
   ll = length of note. 48 for the quarter note.

xx : Rhythm note on/off (xx = A0...BF)

   xx = %101bsmch
            ||||\- h: hi-hat
            |||\-- c: top-cym
            ||\--- m: tom-tom
            |\---- s: snare
            \----- b: bass drum 

45 nn : Rhythm volume

   nn = %rrr-vvvv   
         |   \---- vvvv: volume
         \-------- rrr : target
                         0: bass drum 
                         1: snare drum 
                         2: tom-tom
                         3: top-cym
                         4: hi-hat

47 nn : Rhythm offset volume

   nn = %rrrvvvvv 
         |  \----- vvvvv: offset(signed, 2's complement)
         \-------- rrr  : target
                           0: bass drum 
                           1: snare drum 
                           2: tom-tom
                           3: top-cym
                           4: hi-hat
```

Thanks
======
Thanks to Xyz for http://ylemxyz.ddns.net/mml/mgsdrv/mgsformat_EN.txt
