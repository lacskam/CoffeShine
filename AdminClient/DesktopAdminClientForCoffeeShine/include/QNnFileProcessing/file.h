#ifndef FILE_H
#define FILE_H
#include <torch/torch.h>
#include <iostream>
#include <vector>
#include <QFileDialog>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QApplication>
#include <random>
#include <QDate>
#include <algorithm>
#include <unordered_map>



void writeFile(int prod, int day, int mou, float dem); //запись результата в файл

std::vector<std::tuple<int, int, int, float>> getfile(int *prod); //чтение из файла(олд)

std::vector<std::tuple<float,float,float,float,float,float,float,float>> getfile1(int *prod); //чтение из фалов allprod и weater (че нить сделать потом нужно с этим, нах эти файлы)


#endif // FILE_H
