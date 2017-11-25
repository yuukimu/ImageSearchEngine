//
//  main.cpp
//  ImageSearchEngine
//
//  Created by 室川優希 on 2017/11/25.
//  Copyright © 2017年 室川優希. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include<fstream>
#include <cmath>
#include <dirent.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

// 文字列の分割
void split(const string &s, char delim, long *vec);
// 検索ディレクトリ内の画像一覧を取得
vector<string> getImages(const char *path);
// ヒストグラムファイルの読み込み
unordered_map<string, string> readHistgram();
// 入力画像のヒストグラム作成
void getHistgram(std::string input_path, long *input_histgram);
// 類似画像検索
void searchSimilarImage(string input_path, unordered_map<string, string> histgram);

void split(const string &s, char delim, long *vec) {
    vector<string> elems;
    string item;
    int index = 0;
    for (char ch: s) {
        if (ch == delim) {
            if (!item.empty()) {
                vec[index++] = stol(item);
            }
            item.clear();
        }
        else {
            item += ch;
        }
    }
    if (!item.empty()) {
        vec[index] = stol(item);
    }
}

unordered_map<string, string> readHistgram() {
    unordered_map<string, string> histgram;
    string tmp[2];
    
    ifstream ifs("histgram.txt");
    if(!ifs){
        cout<<"入力エラー";
        exit(-1);
    }
    string str;
    while(getline(ifs,str)){
        string token;
        istringstream stream(str);
        
        int index = 0;
        while(getline(stream,token,':')){
            tmp[index++] = token;
        }
        histgram[tmp[1]] = tmp[0];
    }
    return histgram;
}

vector<string> getImages(const char *path) {
    string dir = path;
    vector<string> imgs;
    DIR *dp;
    dirent* entry;
    dp = opendir(path);
    if (dp==NULL) exit(1);
    
    do {
        entry = readdir(dp);
        if (entry != NULL && entry->d_type == DT_REG) {
            string tmp = entry->d_name;
            if (tmp[0] != '.') {
                imgs.push_back(dir + "/" + entry->d_name);
            }
        }
    } while (entry != NULL);
    return imgs;
}

void getHistgram(std::string input_path, long *input_histgram) {
    for (int i = 0; i < 48; i++) {
        input_histgram[i] = 0;
    }
    cv::Mat input_img = cv::imread(input_path);
    if (!input_img.data) {
        return;
    }
    int width = input_img.cols;
    int height = input_img.rows;
    for (int y = 0; y < height; y++) {
        cv::Vec3b* ptr = input_img.ptr<cv::Vec3b>(y);
        for (int x = 0; x < width; x++) {
            cv::Vec3b bgr = ptr[x];
            input_histgram[ bgr[2]/16 ] += 1;
            input_histgram[ bgr[1]/16 + 16 ] += 1;
            input_histgram[ bgr[0]/16 + 32 ] += 1;
        }
    }
}

void searchSimilarImage(string input_path, unordered_map<string, string> histgram) {
    long input_histgram[48];
    getHistgram(input_path, input_histgram);
    for (auto itr = histgram.begin(); itr != histgram.end(); ++itr) {
        long distance = 0;
        long tmp[48];
        split(itr->second, ' ', tmp);
        for (int i = 0; i < 48; i++) {
            distance += sqrt(pow(input_histgram[i]-tmp[i], 2));
        }
        cout << itr->first << ":" << distance << endl;
    }
}

int main(int argc, const char * argv[]) {
    vector<string> imgs = getImages(argv[1]);
    unordered_map<string, string> histgram = readHistgram();
    for(string img : imgs) {
        cout << "##" << img << "##" << endl;
        searchSimilarImage(img, histgram);
        cout << "-----------------------" << endl;
        cout << endl;
    }
    return 0;
}
