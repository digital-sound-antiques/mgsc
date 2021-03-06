## mgsc - MGSC.COM emulator

mgsc はコンソール上で動作する MGSコンパイラ です。MML を記述したテキストファイルから、[MGSDRV](http://www.gigamix.jp/mgsdrv/)形式の音楽データ(.mgs)を生成します。

mgsc は C++ で実装されていますが、スクラッチで作られたコンパイラではなく、CP/M エミュレータ上で MSX 用の`MGSC.COM`バイナリを直接動作させています。
このため、MSX 版`MGSC.COM`の振る舞いがほぼ完全に再現されています。

MML の文法についてはMSX 版`MGSC.COM`の[MMLドキュメント](http://www.gigamix.jp/mgsdrv/MGSC111.TXT)を参照してください。

## ビルド方法

### OSX や Linux の場合
`git`と`cmake`が必要です。

以下の手順を実行すると mgsc バイナリをビルドできます。

```
$ git clone --recursive https://github.com/digital-sound-antiques/mgsc.git
$ cd mgsc
$ cmake .
$ make
```

### Windows の場合

Visual Studio 2015と`git`と`cmake`がインストールされている環境が必要です。

Visual Studio は、一度も C++ プロジェクトを作ったことがない場合、C++ がインストールされていません。
その場合は C++ プロジェクトを新規作成して C++ をインストールしておいてください。

gitのbashコンソールから以下の手順を実行します。

```
$ git clone --recursive https://github.com/digital-sound-antiques/mgsc.git
$ cd mgsc
$ cmake .
```

以上で Project.sln が生成されるので、Visual Studio で開いてビルドします。

## mgsc の実行方法

```
$ mgsc MMLファイル名 [出力ファイル名] [オプション]
```

出力ファイル名、オプションは省略可能です。出力ファイル名が省略された場合、出力ファイル名は MML ファイルの拡張子をmgsに変更したものになります。
出力ファイル名、入力ファイル名共にパスを含む事が可能です。パス指定が無い場合は、カレントフォルダに対して入出力が行われます。

`mgsc`
の起動後、入力ファイルにエラーが無ければ、*MMLファイルと同じディレクトリに*MGSファイルが生成されます。エラーがあった場合は，エラー情報を表示して終了します。

オプションは `-T`のみ有効です。このオプションがセットされると、コンパイル後に詳細情報が表示されます。ただし、MML ファイル内に`#track_status`命令
が記述されている場合は、`-T`オプション指定の有無にかかわらず、詳細情報が表示されます。

## コードの利用

`main.cpp`がMGSCクラスの利用サンプルを兼ねているので参照してください。
`mgsc.h`をインクルードすると，クラスMGSCが利用可能になるので、mml入力用と，ファイル出力用のストリームを用意して、

```
int MGSC::Compile(std::iostream *mml, std::iostream *mgs);
```

メソッドを呼び出すだけコンパイルができます。

- `mml`は入力用にオープンしておく必要があります。
- `mml`から読み込めるデータは、改行コードがCR+LFのテキストの必要があります。
- `mgs`は入出力用にオープンしておく必要があります。
- `mgs`は`std::ios::binary`フラグ付きでオープンしてください。

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

