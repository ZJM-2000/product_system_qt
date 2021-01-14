#include"infer.h"
infer::infer(){
    QFile file("animal.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"不能打开规则库"<<endl;
    }
    while(!file.atEnd()){
         QByteArray line = file.readLine();
         QString str(line);
         data.push_back(str.trimmed());//去掉换行符
    }
    //通过data中数据构建事实库与规则库
    extractFact();
    extractRules();
}
void infer::extractFact(){
    //从data里提取事实存入QVector<fact>事实库
    QVector<QString>::iterator iter;
    for(iter=data.begin();iter!=data.end();iter++){
        QVector<QString>* processed = splitStr(*iter,"->");
        QVector<QString>::iterator ite;
        for(ite=processed->begin();ite!=processed->end();ite++){
            QVector<QString>* processed = splitStr(*ite,"^"); //文件中条件已‘’分开
            QVector<QString>::iterator it;
            for(it=processed->begin();it!=processed->end();it++){
                if(!exist(*it)){
                    fact a={*it,false};
                    facts.push_back(a);
                    statements.push_back(*it);
                }
            }
        }
    }
}
void infer::extractRules(){
    //从data中提取推理规则存入QVector<rules>
    QVector<QString>::iterator iter;
    for(iter=data.begin();iter!=data.end();iter++){
        QVector<QString>* processed = splitStr(*iter,"->");
        QVector<QString>::iterator ite;
        ite=processed->begin();
        QVector<QString>* processed1 = splitStr(*ite,"^");
        rule* tmp;
        tmp = new rule();
        tmp->conclusion=*(processed->end()-1);
        QVector<QString>::iterator it;
        for (it=processed1->begin();it!=processed1->end();it++){
            tmp->condition.push_back(*it);
        }
        rules.push_back(*tmp);
}
}
void infer::reasoning(){ //核心推理过程，遍历QVector<Rules> rules中所有规则进行推理
    QVector<QString>* res = new QVector<QString>;
    bool change=true;//通过推理得出新事实后继续
    while (change){
        change=false;

        QVector<rule>::iterator ite;
        for (ite=rules.begin();ite!=rules.end();ite++) //逐个检查每个规则是否满足
        {
            rule tmp = *ite;
            bool flag=true;//标志是否符合规则，false表示该条规则里面的某些条件没有
            QVector<QString>::iterator ite1;
            for (ite1=tmp.condition.begin();ite1!=tmp.condition.end();ite1++) //逐个检查每个规则的所有condition是否满足
            {
                if(flag){
                    QVector<fact>::iterator ite2;
                    for (ite2=facts.begin();ite2!=facts.end();ite2++)
                    {
                        if((*ite2).word == *ite1 && (*ite2).state == false){ //state值为false表示该事实不是已知事实，则该条规则匹配失败，break
                            flag = false;
                            break;
                        }
                    }
                }
                else{break;}
            }
            if(flag){
                /*推理出某个结论后，放入事实库，当事实库发生了更新后重复该过程(待修改为：页面输出推理过程，将结论加入事实库，如果事实库没变化不用改变change事实库改变了change)*/
                bool flag1=true; //判断被推理出的规则是否产生的新的右部，有则添加进result，没有则不添加
                QVector<QString>::iterator it;
                for (it=res->begin();it!=res->end();it++){
                    if(*it == tmp.conclusion){
                        flag1=false;
                        break;
                    }
                }
                if(flag1) res->push_back(tmp.conclusion);

                if(flag1){ //若有新的右部，则将新的右部加入已知条件，并将change置为true准备下一次循环
                    change = true;
                    QVector<fact>::iterator ite3;
                    for (ite3=facts.begin();ite3!=facts.end();ite3++)
                    {
                        if((*ite3).word == tmp.conclusion){
                            (*ite3).state = true;
                        }
                    }
                }
            }
        }
    }
    QVector<QString>::iterator it; //将推理出的所有结论连接成语句存入QString result
    for (it=res->begin();it!=res->end();it++){
        result += *it + "\n";
    }
    delete res;
}

bool infer::exist(QString s){
    ////判断一条语句是否已经被加入QVector<QString> statements中
    QVector<QString>::iterator iter;
    for(iter=statements.begin();iter!=statements.end();iter++){
        if((*iter)==s){
            return true;
        }
    }
    return false;
}

QVector<QString>* infer::splitStr(QString s,QString b){ //把字符串s按照子串b进行分隔
    QVector<QString>* t = new(QVector<QString>);
    std::string str = s.toStdString();
    std::string b1 = b.toStdString();
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    char *p = strtok(cstr, b1.c_str()); //c_str() string转char
    while (p != 0)
    {
        std::string a = p;
        t->push_back(QString::fromStdString(a));
        p = strtok(NULL, b1.c_str());
    }
    return t;
}
