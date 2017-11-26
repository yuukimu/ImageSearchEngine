//
//  image_search.h
//  ImageSearchEngine
//
//  Created by yuukimurokawa on 2017/11/26.
//  Copyright © 2017年 室川優希. All rights reserved.
//

#ifndef image_search_h
#define image_search_h

#define RANKNUM 10

#include <iostream>
#include <unordered_map>


using namespace std;

// 文字列の分割
extern void split(const string &s, char delim, long *vec);
// 検索ディレクトリ内の画像一覧を取得
extern vector<string> getImages(const char *path);
// ヒストグラムファイルの読み込み
extern unordered_map<string, string> readHistgram();
// 入力画像のヒストグラム作成
extern void getHistgram(std::string input_path, long *input_histgram);
// 類似画像検索
extern void searchSimilarImage(string input_path, unordered_map<string, string> histgram);
// 距離が最大のインデックスを取得
extern int getMaxIndex(const long *ary);
// 結果のソート
extern void sortResultData(string *img_list, long *dist_list);

#endif /* image_search_h */
