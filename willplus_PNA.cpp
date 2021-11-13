#include <bits/stdc++.h>
#include <windows.h>
#include <direct.h>

using namespace std;
struct pnahead
{
    char name[4];
    long undefine1;
    long width;
    long height;
    long count;
};
struct pnginpna
{
    long kind=-999;
    long check=-999;
    long remark1=-999;
    long remark2=-999;
    long width=-999;
    long height=-999;
    long add1=-999;
    long add2=-999;
    long remark3=-999;
    long size=-999;
    long index = -1;
    void print()
    {
        if (remark3 == 1072693248)
        {
            if (add1 == 0 && add2 == 0)
            {
                printf_s("PNG:序号[INDEX]:%d 标记1[REMARK1]:%d 标记2[REMARK2]:%d 宽[WIDTH]：%d 高[HEIGHT]：%d 大小（字节）[SIZE (BYTE)]：%d\n", index, remark1, remark2, width, height, size);
            }
            
        }
        
    }
};

int main()
{
    while (true)
    {
        int ct = 1;
        cout << "1为解包[1 UNPACK]，2为封包[2 REPACK]，其余退出[ELSE EXIT]"<<endl;
        int kind;
        cin >> kind;
        switch (kind)
        {
        case 1:
        {
            cout << "输入名称(不含.pna)[INPUT FILE NAME DO NOT INCLUDE .pna]:";
            fstream v;
            string name;
            cin >> name;
            v.open(name + ".pna", ios::in | ios::binary);
            pnahead pna{};
            v.read((char*)&pna, sizeof(pna));
            pnginpna* png = new pnginpna[pna.count];
            for (int i = 0; i < pna.count; i++)
            {
                v.read((char*)&png[i], 40);
                png[i].index = ct;
                ct++;

            }
            string now= _getcwd(0,0);
            now += "\\" + name;
            CreateDirectoryA(const_cast<char*>(now.c_str()), 0);

            for (int i = 0; i < pna.count; i++)
            {
                if (png[i].check == -1)
                {
                    cout << "非正常[SPECIAL]" << png[i].index << endl;
                    fstream wt;
                    string filename = name + "/" + to_string(png[i].index) + "." + to_string(png[i].kind);
                    wt.open(filename, ios::out | ios::binary);
                    wt.close();
                }
                else
                {
                    cout << "正常[PNG]:";
                    png[i].print();
                    fstream wt;
                    string filename = name + "/" + to_string(png[i].index) + ".png";
                    wt.open(filename, ios::out | ios::binary);
                    char* buf = new char[png[i].size];
                    v.read(buf, png[i].size);
                    wt.write(buf, png[i].size);
                    wt.close();
                }
            }
            cout << "执行完毕[FINISH]"<<endl;
            break;
        }
        case 2:
        {
            vector <char*>mytext;
            vector <long>mysize;
            cout << "请确保程序运行目录下有该pna文件且存在该pna文件的文件夹(意味着你要用本程序解包一次该pna)[USE REPACK AFTER UNPACKED]" << endl;
            cout << "输入名称(不含.pna):";
            fstream v;
            string name;
            cin >> name;
            v.open(name + ".pna", ios::in | ios::binary);
            pnahead pna{};
            v.read((char*)&pna, sizeof(pna));
            pnginpna* png = new pnginpna[pna.count];
            for (int i = 0; i < pna.count; i++)
            {
                v.read((char*)&png[i], 40);
                png[i].index = ct;
                ct++;
            }
            fstream wt;
            string filename = "new_"+name+ ".pna";
            wt.open(filename, ios::out | ios::binary);
            pnahead new_pna = pna;
            pnginpna* new_png = png;
            for (int i = 1; i <= new_pna.count; i++)
            {
                fstream tmp;
                string filename = name + "/" + to_string(i) + +".png";
                tmp.open(filename, ios::out|ios::in | ios::binary);
                if (tmp)
                {
                    cout << "PNG" + to_string(i) << endl;
                    tmp.seekg(0, ios::end);
                    long tmpsize = tmp.tellg();
                    cout << "size:" << tmpsize<<endl;
                    new_png[i - 1].size = tmpsize;
                    tmp.seekg(0,ios::beg);
                    //cout << "现在指针" << tmp.tellg()<<endl;
                    char* pp = new char[tmpsize];
                    tmp.read(pp, tmpsize);
                    //cout << "后指针" << tmp.tellg() << endl;
                    mytext.push_back(pp);
                    mysize.push_back(tmpsize);
                }
                filename= name + "/" + to_string(i) + +".1";
                tmp.close();
                tmp.open(filename, ios::in | ios::binary);
                if (tmp)
                {
                    cout << "BLOCK" + to_string(i) << endl;
                    tmp.seekg(0, ios::end);
                    long tmpsize = tmp.tellg();                    
                    cout << "size:" << tmpsize << endl;
                    new_png[i - 1].size = tmpsize;
                }
                filename = name + "/" + to_string(i) + +".2";
                tmp.close();
                tmp.open(filename, ios::in | ios::binary);
                if (tmp)
                {
                    cout << "BLOCK" + to_string(i) << endl;
                    tmp.seekg(0, ios::end);
                    long tmpsize = tmp.tellg();
                    cout << "size:" << tmpsize << endl;
                    new_png[i - 1].size = tmpsize;

                    
                }
            }
            wt.write((char*)&new_pna, sizeof(new_pna));
            for (int i = 0; i < new_pna.count; i++)
            {
                wt.write((char*)&new_png[i], 40);
            }
            cout <<"写入数量(COUNT):" << mytext.size() << endl;
            for (int i = 0; i != mytext.size(); i++)
            {
                cout << "写入(BYTE)"<<mysize[i] << endl;
                wt.write(mytext[i], mysize[i]);
            }
            break;
        }
        default:
            exit(0);
            break;
        }
    }
    system("pause");
    return 0;
}
