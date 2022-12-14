# FNCT-基礎教育用サッカーロボット　サンプルプログラム
    このプログラムは福井高専で使用しているサッカーロボット向けのサンプルプログラムです。
    書籍「Arduinoで始めるロボット製作」(工学社)に収録されている回路図に準拠したピンアサインを使用しています。
    本ファイルとプログラムは機能性よりも読みやすさを重視しています。

## 使用について
      このプロジェクトはMozilla Public License Version 2.0のもとに公開しています。
      学生でレポート等に使用する場合はライセンスの規定に従った引用をお願いします。

## 関数について
このプログラムでは以下の関数を併記する仕様のもとに実装しています。改変する際には参考にしていただけると幸いです。
### Setup
#### 関連する関数: err()
    この関数ではそれぞれのピンアサインの初期化を行っています。
    また、使用している液晶パネルの初期化も同時に行っています。


## m_mod(int)
### 関連する関数: m_spd

    この関数ではモーターの回転方向を制御しています。引数の値によりモード変更を行っており、以下の仕様で動作します
    0: 停止
    1: 前進
    2: 右方向
    3: 左方向
    4: 後退
    5: 右回転
    6: 左回転
    値なし: エラー(液晶パネル上にエラー分を表示)
    処理が完了した段階で液晶パネル上に完了メッセージが表示されます。

### m_spd(int, int, bool)
#### 関連する関数: m_mod
  この関数ではモータの回転速度、回転時間を制御しています。
  最初の引数でモーターの回転速度を指定し、二番目の引数で時間を決定しています。
  ただし、3番めの引数として"False"が渡された場合には時間の指定を無効とし、再度この関数を呼ぶまで指定した回転数を維持します。

### primrun
#### 関連する関数: なし
    この関数は電源投入後に優先的に実行されます。デフォルトではタクトスイッチ1, 3番の状況に応じてそれぞれ前進と停止、後退をする処理を記載しています。


### atk(int)
#### 関連する関数: なし
    この関数ではオフェンス側で使用する基本動作を記載しています。
    1: 一定時間前進
    2: 右方向に移動した後に前進、その後左回転
    3: 左方向に移動した後に前進、その後右回転


### dig
#### 関連する関数: atk
    この関数では搭載されたコンパスから方位データを受信し、起動時との角度の差に応じたモータ値を返す処理を記載しています。型はintです。
    ただし、以下の場合は処理を行わず、コンパスからの応答を待機します。
      データの返答がない場合
      NULLデータが送信されている場合
        * 差が-60~60度の場合
            0を返す。同時に液晶パネル上にFWと表示
        * 差が-120~-60度の場合
            1を返す。同時に液晶パネル上にRと表示
        * 差が120~180度の場合
          2を返す。同時に液晶パネル上にRVと表示
        * 差が60~120度の場合
          3を返す。同時に液晶パネル上にLと表示
### md_det
#### 関連する関数: atk, dig
    この関数では現在の動作方法を液晶パネル上に表示しています。
    特段の操作は必要ありません。


### loop
#### 関連する関数: すべて
    この関数ではこれまでに記載した関数の呼び出しや各種事前処理を記載しています。
