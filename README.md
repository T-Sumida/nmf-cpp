## NMF_for_cpp

## Overview
単純なNMFをC++で実装したものです．

通常のNMFと，テンプレートデータを辞書行列に登録した場合のテンプレートあり分離処理を実装しています．

コード内の行列演算に問題がある可能性があるので、もしお気づきになられたら修正 or 連絡をお願い致します。

## Usage
srcフォルダ内の「CMakeLists.txt」を用いれば基本OKです．
```
$ cmake CMakeLists
$ make
$ ./NMF_for_cpp
```
これで一応テストプログラムは動作します．

CMakeが入ってない方は，
```
$ c++ main.cpp Array2D.hpp NMF.hpp Separator.h
$ ./a.out
```
などで実行可能です．

実際のNMFの使い方に関しては，「main.cpp」に一通りの使い方と流れを記載．

## Requirement
C++11


## Licence
Copyright © 2016 T_Sumida Distributed under the MIT License.
