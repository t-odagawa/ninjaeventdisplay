# Tools for NINJA eventdisplay auto creation

## 要件

Wagasci MC および Wagasci Event Display が必須である．
したがって，以下のソフトウェア，プログラムを事前に実行可能な状態にしておく必要がある．
 - GEANT4 v10.5.0+
 - ROOT 6.20+
 - BOOST 1.53+
 - Wagasci BabyMIND Monte Carlo v0.1.12+
 - Wagasci BabyMIND event display v0.1.0+
詳しい設定方法は WagasciMC の README.md を参照のこと．

NINJA Sharing file については [NINJA wiki の該当ページ](https://www-he.scphys.kyoto-u.ac.jp/research/Neutrino/ninja/dokuwiki/doku.php?id=software:howto:dataformat:ninjasf) を参照

## 使い方

まずEventDisplayNinjaSf をコンパイルする
これは.ninjasf またはテキストダンプされたSharing file (.txt) をイベントディスプレイ作成に必要なリストに変換するプログラムである．
単純なプログラムなので実行ファイルにする必要もないのかもしれない．

```shell script
mkdir build
cd build
cmake ..
make
```
コンパイルが終わったら実行してリストを作成する．
```shell script
./src/EventDisplayNinjaSf <ninjasf file> <list file>
```

最後にシェルスクリプトをlist file 分回すpython script を実行する．
```shell script
cd ../scipt
python run_shell.py
```