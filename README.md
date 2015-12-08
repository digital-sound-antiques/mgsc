## mgsc - MGSC.COM emulator

mgscはコンソール上で動作するMGSコンパイラです。MMLを記述したテキストファイルから、[MGSDRV](http://www.gigamix.jp/mgsdrv/)形式の音楽データ(.mgs)を生成します。

mgscはC++で実装されていますが、スクラッチで作られたコンパイラではなく、CP/M エミュレータ上でMSX用の`MGSC.COM`バイナリを直接動作させています。
このため、MSX版`MGSC.COM`の振る舞いがほぼ完全に再現されています。

MMLの文法についてはMSX版`MGSC.COM`の[MMLドキュメント](http://www.gigamix.jp/mgsdrv/MGSC111.TXT)を参照してください。

## ビルド方法

### OSX や Linux の場合
`git`と`cmake`が必要です。

以下の手順を実行すると mgsc バイナリをビルドできます。

```
$ git clone https://https://github.com/digital-sound-antiques/mgsc.git
$ cd mgsc
$ git submodule init
$ git submodule update
$ mkdir build; cd build
$ cmake ..
$ make
```

### Windows の場合

ビルド手順は現在準備中です。

## mgsc の実行方法

```
$ mgsc MMLファイル名 [出力ファイル名] [オプション]
```

出力ファイル名、オプションは省略可能です。出力ファイル名が省略された場合、出力ファイル名はMMLファイルの拡張子をmgsに変更したものになります。
出力ファイル名、入力ファイル名共にパスを含む事が可能です。パス指定が無い場合は、カレントフォルダに対して入出力が行われます。

`mgsc`の起動後、入力ファイルにエラーが無ければMGSファイルが生成されます。エラーがあった場合は，エラー情報を表示して終了します。

オプションは `-T`のみ有効です。このオプションがセットされると、コンパイル後に詳細情報が表示されます。ただし、MMLファイル内に`#track_status`命令
が記述されている場合は、`-T`オプション指定の有無にかかわらず、詳細情報が表示されます。

## コードの利用

`main.cpp`がMGSCクラスの利用サンプルを兼ねているので参照してください。
`mgsc.h`をインクルードすると，クラスMGSCが利用可能になるので、mml入力用と，ファイル出力用のストリームを用意して、

```
int MGSC::Compile(std::iostream *mml, std::iostream *mgs);
```

メソッドを呼び出すだけコンパイルができます。

ただし、ファイル入出力ストリームを使用する場合、`mml`は入力専用でオープンしてかまいませんが、
`mgs`は入出力用にオープンしておく必要があります。
また、両者共に`std::ios::binary`フラグ付きでオープンしてください。これを忘れるとMMLの読み込みに失敗します。

Compile関数はCP/Mエミュレータが異常終了した場合`false`を返し、正常終了時は`true`を返します。
この値がコンパイル成功失敗の判定では無いことに注意してください。コンパイル成功失敗の判定が必要な場合、`mgs`
ストリームに書き込まれたサイズを確認します。コンパイルに失敗した場合、`mgs`ストリームには一切書き込みが行われません。

MGSCクラスはコンパイル状況を標準出力に表示しますが、これをユーザーが用意した出力ストリームに置き換える場合には、

```
void MGSC::SetConsole(std::istream *cin, std::ostream *cout);
```

を利用してください。

## 著作権等

- mgsc-com.h は MSX版 MGSC.COM のダンプデータです。著作権はAin、[GIGAMIX](http://www.gigamix.jp/mgsdrv/)の両氏が保有しています。
- Z80エミュレーションにMamiya氏作のKMZ80を利用しています。KMZ80はMamiya 氏によりPublic Domain宣言されています。

## その他　

cppファイルのフォーマットは`clang-format -style=LLVM`を利用しています。

