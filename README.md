# Ble Keyboard dongle

Bluetooth Low Energy 対応のキーボードをUSBキーボードとして認識させるためのドングルのソースコードです。

## 幸せになれる人

* Bluetoothキーボードを複数のPCで使い回したい人
* BIOS画面で別のキーボードを接続するのが面倒な人

## 動作確認済みボード

* [Adafruit Feather nRF52840 Express](https://www.adafruit.com/product/4062)

## ボードへの書き込み方

Arduino IDEでソースを開き、ボードにプログラムを書き込むだけで動作します。

## 使い方

PCに接続すれば、標準キーボードとして認識します。

Bluetoothキーボードの電源を入れたら、自動的にペアリングします。
ペアリングしているキーボードを切り替えたい場合は、ボード備え付けのUserSwを3秒以上押し続けてください。
