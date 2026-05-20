# RPN-Calculator　 (Keyboard Version)
RPN（逆ポーランド記法）の関数電卓が欲しくなって、マイコン(ESP32)で作り始めた電卓です。
まずは整数演算からということで、最初は整数用16桁の電卓を作りました。
内部データはBCD（２進化10進数）で持っており、RPNの特徴であるスタックは連結リストで実現しているため、メモリが不足するまで追加できます。四則演算は筆算のアルゴリズムで完全スクラッチしています。
BCDにしたのは丸め誤差が出ないことと、有効桁数を自由に設計出来るので選択しました。

次に整数電卓に実数モードを追加しました。
有効桁数16桁の浮動小数点演算です。整数演算のロジックを実数用に拡張して実数の四則演算が出来るようにしました。
続いて三角関数の実装ですが、三角関数ははCORDICアルゴリズムで実装しました。自作の実数の四則演算を使って計算しています。三角関数はラジアンではなく度の入力にしました。

逆三角関数ですが、テーラー展開で実装しています。
この計算機のRPN電卓の特徴であるスタックをフル活用しようと、逆三角関数を計算するときにスレッドを生成し、そのスレッドでキー操作されたように見せかけて計算しています。具体的には、逆三角関数計算スレッドが”＊”や”＋”等の演算子、”Enter"を押下したように見せかけて、内部メモリ（スタック）を使った計算をするスクリプトのようにプログラムしています。
逆三角関数計算スレッドが電卓を操作しているようになるので、計算途中には表示画面がチラチラ書き変わって行くのも面白いなと思っています。

## ハードウェア構成
[ESP32 DevKitC 1個](https://www.amazon.co.jp/dp/B06XWP81GP?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_4)

[0.96インチ OLEDディスプレイ 2個](https://www.amazon.co.jp/dp/B08MZGX7YR?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1)

[16 ボタンキーパッドキーボード 1個](https://www.amazon.co.jp/dp/B0DFPJ27BX?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1&th=1)

タクトスイッチ 2〜4個 （使っているものは２個。残りは今後の拡張用の予備）

電源周りはご自由に。
USB給電であればそのまま。持ち運びのためにバッテリー駆動するならそれらの部品と回路

![RPN-Calc.jpeg](https://github.com/g8jmgn778p-gif/RPN-Calculator-Keyboard-Version-/blob/main/RPN-Calc.jpeg?raw=true)

## 配線
OLED i2c
 SDApin -- GPIO 21
 SCLpin -- GPIO 22
 
 OLED_I2C_ADDRESS 1 0x3d　（裏面のチップ抵抗の位置をずらしてください）
 OLED_I2C_ADDRESS 2 0x3c

OLED上部SW　（タクトスイッチ）
 F_SW4 -- GPIO 23    // 右端
 F_SW3 -- GPIO 19    // 右から２番目 
 F_SW2 -- GPIO 18    // 左から２番目
 F_SW1 -- GPIO 13    // 左端

テンキー
 y軸 : GPIO 12, 14, 27, 26
 x軸 : GPIO 25, 33, 32, 35
