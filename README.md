
# libudero 

テクノツール社製ロボットアーム動作用ライブラリ

## コードからのビルド方法

CMakeを使います。CMakeはバージョン3.10以降を使って下さい。

### Windows 10での注意点
PCAN-BASIC APIに依存しています。PCAN BASIC APIはPeakSystem社が提供するCAN-USB変換アダプタ用のライブラリです。
Peak Systems社のウェブサイトからダウンロードしてください。
https://www.peak-system.com/

CMakeでプロジェクトを生成する場合は、PCAN BASIC APIのディレクトリを設定する必要があります。
CMake-guiでConfigureしたのちに、PCANBASIC_LIBDIRの値を、PCAN-BASIC APIフォルダに設定してから、再度Configure/Generateを行ってください。


## テスト用アプリ
appsディレクトリには、Uderoを利用するためのコンパクトなコマンド群を作成するためのソースコードがあります。
これらのコードは、libuderoライブラリを使うためのサンプルにもなっています。コードを確認して使ってください。

## Authors

* **Yuki Suga** - *SUGAR SWEET ROBOTICS* -

## License

This project is Proprietary license.
This code is allowed to use with Udero (built by Technotools Corpolation.).
This code is NOT allowed to use not with Udero.

本コードの一部およびすべてをテクノツール社製Udero以外のものと使うことを禁じる。