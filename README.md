
# libudero 

テクノツール社製ロボットアーム動作用ライブラリ

## コードからのビルド方法

CMakeを使います。CMakeはバージョン3.10以降を使って下さい。

### Windows 10での注意点
#### PeakSystem PCAN-USBドライバのインストール
Uderoに付属しているPCAN-USBのドライバをインストールして、Windowsシステムが変換ケーブルを認識していることを確認してください。

#### PCAN-BASIC APIのインストール
PCAN-BASIC APIに依存しています。PCAN BASIC APIはPeakSystem社が提供するCAN-USB変換アダプタ用のライブラリです。
Peak Systems社のウェブサイトからダウンロードしてください。
https://www.peak-system.com/
ダウンロードしたzipを展開して好きな場所に配置してください。
この位置を後でCMakeの設定に反映させる必要があります。
また、展開したフォルダにあるReadMe.txtを読んで、DLLファイルをSystem32ディレクトリなどに配置してください。

#### CMakeでの設定
CMakeでプロジェクトを生成するときに上記のようにPCAN BASIC APIのディレクトリを設定する必要があります。
CMake-guiでConfigureしたのちに、PCANBASIC_ROOTの値を、PCAN-BASIC APIフォルダに設定してから、Generateを行ってください。


#### Visual Studioでのビルド
CMakeで設定したバージョンのVisual Studioでビルドしてください。開発者はVisual Studio 2019 (x64) での動作確認を行っています。

## テスト用アプリ
appsディレクトリには、Uderoを利用するためのコンパクトなコマンド群を作成するためのソースコードがあります。
これらのコードは、libuderoライブラリを使うためのサンプルにもなっています。コードを確認して使ってください。
主なものは以下の通りです。

- close_gripper グリッパーを閉じます
- digital_input 関節のデジタル入力を画面に表示
- foldin 折りたたみ状態に移動
- foldout 折りたたみ状態から展開状態、もしくは任意の姿勢から展開姿勢に移動
- free 全関節をサーボOFF
- get_angles 関節角度を取得
- homing ホーミングして原点を割り出し
- is_homed ホーミングが終わっているかを確認
- joint_get_state 現在の関節の状態を出力
- joint_set_state 関節の状態を変更
- joy_joint_ctrl ゲームパッドで各関節を動かします。
- lock 全関節をサーボONします。
- move_gripper グリッパーを任意の位置まで動かします
- open_gripper グリッパーを全開します。
- set_angles 各関節を引数の角度まで移動させます。
- socketserver ソケット通信用のサーバーです。
- waitstopped ロボットの関節が目標値に到達して停止するまで待ちます。

これらのアプリはudero_{ディレクトリ名} というexeファイルを生成します。

## ビルド済みパッケージ
以下のアドレスからWindows用のビルド済みバイナリが入手できます。
Visual Studioをインストールしていない環境で動作させる場合は、必要に応じてVisual Studioの再頒布用ライブラリをインストールしてください。

https://github.com/udero/libudero_dist

また、PCAN-USBのドライバ、およびPCAN-BASICのDLLファイルが含まれていません。
各々をダウンロードしてインストールを行ってください。

## ディレクトリの構成

- libuder
  - apps テスト用のアプリ群のソースコードが入ったディレクトリ
  - include uderoライブラリのヘッダーファイル
  - libaqua ThreadやSleepなどのプラットフォーム差を埋めるためのライブラリ
  - libepos モータードライバを使うためのライブラリ
  - libpcanbasic PCAN-USBケーブルを使うためのライブラリ。Windows専用
  - libsocketcan PCAN-USBケーブルをLinuxで使う場合のライブラリ。
  - src Uderoのライブラリ
  - thirdpary サードパーティー製のライブラリの入ったディレクトリ
  - CMakeLists.txt
  - README.md このファイルです。



## Authors

* **Yuki Suga** - *SUGAR SWEET ROBOTICS* -

## License

This project is Proprietary license.
This code is allowed to use with Udero (built by Technotools Corpolation.).
This code is NOT allowed to use not with Udero.

本コードの一部およびすべてをテクノツール社製Udero以外のものと使うことを禁じる。

## 免責事項
本ソースコードを用いた結果、ユーザが被ったいかなる損害についても、作者およびTechnotoolsは一切の責任を負いません。自己の責任の上で使用して下さい。

## 著作権
本ソースコードの著作権は、作者であるTechnotoolsに帰属します。