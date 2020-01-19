MGS Data format
========

## Structure

- TEXT Block
- HEADER Block
- DATA Block (Voice / Track)

## TEXT Block

```
0000 STRING "MGS"
0003 STRING "313" 0x0D 0x0A
0008 STRING 任意長のタイトル文字列 0x0D 0x0A 0x1A
```

# HEADER Block

```
0000 BYTE 0x00
0001 BYTE &Bxd?mmmlo 
     o: #opll_mode 0|1
     l: #lfo_mode 0|1
     mmm: #machine_id 0-7
     d: #disenable_mgsrc

0002 WORD #tempo (0-2047)
0004 WORD Voice Track offset 
0006 WORD trk.1 offset
0008 WORD trk.2 offset 
000A WORD trk.3 offset 
000C WORD trk.4 offset 
000E WORD trk.5 offset 
0010 WORD trk.6 offset 
0012 WORD trk.7 offset 
0014 WORD trk.8 offset 
0016 WORD trk.9 offset 
0018 WORD trk.A offset 
001A WORD trk.B offset 
001C WORD trk.C offset 
001E WORD trk.D offset 
0020 WORD trk.E offset 
0022 WORD trk.F offset 
0024 WORD trk.G offset 
0026 WORD trk.H offset 

offset はすべてHeader Block先頭からのオフセット値
```

## Voice Block

```
00 kk [nn] x 8 : @vの定義

02 kk &B1nnzzzzz AL AR DR SL SR RR : @rの定義

02 kk &B0nnzzzzz ll ... : @eの定義
   kk : 音色番号
   nn :　モード (0-3)
   zzzzz : ノイズ周波数 (0-31)
   ll : エンベロープデータの長さ

03 kk [nn] x 32 : @sの定義
   kk : 音色番号

04 [nnnn] x 12 : #psg_tune
05 [nnnn] x 12 : #opll_tune

FF : トラック終端
```

## 一般コマンド

```
2n nn    : 音階 n=0H〜BHが CからBに対応 音長はnn

3n       : 音階 n=0H〜BHが CからBに対応 音長は42Hでの指定値

2C nn    : 休符 nn=音長

3C       : 休符

40       : タイもしくはスラー

41 ll hh : テンポ設定 hhll = テンポ 後ろにワークが48バイト続く

42 nn    : 音長設定 nn = カウント数

44 0n    : Qコマンド n=Qの値

46 nn    : 相対音量変化 nn = 変化値(2の補数)

48 nn    : kコマンド nn = リリース値

49 nn    : @eコマンド nn = 音色番号

4A       : FM音源強制キーオフ

4B nn    : sコマンド nn = ハードエンベロープ番号

4C ll hh : mコマンド hhll = ハードエンベロープ周期

4D nn    : nコマンド nn = ノイズ周波数

4E       : オクターブアップ >

4F       : オクターブダウン <

50 nn    : デチューン PSG, SCC は nn = 0 - デチューン値
                          FM は nn = デチューン値

51 ll hh : 高精度デチューン(PSG, SCCのみ) hhll = デチューン値

52 nn    : オートベンド(p) nn = ベンド速度

53 nn    : ポルタメント(_) nn = 開始音程(00H〜0BHがCからBに対応)
           この直後にもしあればオクターブコマンド，さらに
           ポルタメント終了位置の音階コマンドがくる．

54 n1 n2 n3 n4 : LFO (h)
            n1 = Hコマンドの 第一パラメタ+1 
            n2 = 第二パラメタ
            n3 = 第三パラメタ + 1
            n4 = 256-第四パラメタ (PSG,SCC)
            n4 = 第四パラメタ (FM)

55 ll hh : LFO粗さ詳細指定(@p) hhll = LFO粗さ

57 nn ll hh : ループ開始 nn = ループ回数 hhll = ????

58 ll hh : ループ脱出 hhll = 脱出先

59 00 ll hh : ループ終了 hhll = ジャンプ先(相対値)

5A nn    : LFO開始(ho,hf) nn = 01 が hoに相当 nn = 00 が hfに相当
           リズムトラックではko, kfになる．

5B nn    : サスティン(so,sf) nn = 01 が soに相当 nn = 00 が sfに相当

5C n1 n2 : y n1, n2

5D       : デバッグマーカー

5F nn    : 高精度デチューン(FMのみ) nn = デチューン値

60 nn    : @mコマンド

61 nn    : @oコマンド

63       : @fコマンド

64       : hiコマンド

8n       : 音色設定 音色番号00から15まで． n = 音色番号

9n       : 音色設定 音色番号16から31まで． n = 音色番号の下位4ビット

Cn       : 音量 n = 音量(0-F)

Dn       : オクターブ n = OCT-1

FF       : 終端マーカ
```

## リズムトラック専用コマンド

```
&B001bsmch nn  : 楽器発音 nn=音長

&B101bsmch     : 楽器発音 音長はlコマンドの値

45 &Bxxx0nnnn  : &Bxxx = 楽器ID(0:b 1:s 2:m 3:c 4:h), nnnn = 音量

47 &Bxxxnnnnn  : &Bxxx = 楽器ID, &Bnnnnn = 音量増減値(２の補数)
```
