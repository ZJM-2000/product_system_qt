#ifndef INFER_H
#define INFER_H
#include <QVector>
#include <QDebug>
#include <cstring>
#include <string>
#include <QFile>
#include <iostream>
struct fact{
    QString word;
    bool state;
};
struct rule{
    QString conclusion;
    QVector<QString> condition;
};
class infer{
public:
    QVector<QString> data;
    QVector<fact> facts; //存放事实及其状态，如：{有毛发,true},{是哺乳动物,false}
    QVector<rule> rules; //存放规则，如：{{有犬齿,有爪,眼盯前方},是食肉动物}
    QVector<QString> statements; //包含全部事实,用于显示的事实库
    QString result; //最终结果

    infer();
    void extractFact();
    void extractRules();

    void reasoning();
    QVector<QString>* splitStr(QString s, QString b);
    bool exist(QString s);
};
#endif // INFER_H
