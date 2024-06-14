#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <WinSock.h>
#include "mysql.h"                   //包含MySQL的API函数的头文件
#pragma comment(lib, "libmysql.lib") // 导入libmysql.lib库
using namespace std;

MYSQL mysql;
int insert();     // 数据的插入
int update();     // 数据的修改
int deleteData(); // 数据的删除
int select();     // 数据的选择
int display();    // 查看所有数据

int main()
{
    int num = 0;
    char fu = '0';
    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
    // mysql_real_connect连接到MySQL数据库服务器，其中localhost为服务器机器名，root为连接用户名，123456为密码，
    // test为数据库名，3306为端口号
    if (mysql_real_connect(&mysql, "localhost", "root", "123456", "test", 3306, 0, 0))
    {
        while (1)
        {
            system("cls");
            cout << "C语言嵌入式SQL应用程序开发\n";
            cout << "请选择所需的功能项编号：\n";
            cout << "0--exit.\n";
            cout << "1--添加S表记录	2--修改S表记录 3--删除S表 4--选择S表数据 5--显示S表所有数据\n";
            fflush(stdin);
            cin >> fu;
            if (fu == '0')
                exit(0);
            //	else if(fu=='1') create_student_table();
            else if (fu == '1')
                insert();
            else if (fu == '2')
                update();
            else if (fu == '3')
                deleteData();
            else if (fu == '4')
                select();
            else if (fu == '5')
                display();
            else
                break;
            system("pause");
        }
    }
    else
    {
        printf("数据库不存在！\n");
    }
    mysql_close(&mysql);
    return 0;
}

int insert()
{
    char sno[10] = "S1";
    char sname[10] = "精益";
    char status[10] = "20";
    char city[10] = "天津";
    char strquery[100] = "insert into s values('";
    // char yn[2];
    while (1)
    {
        cout << "请输入供应商代码(sno)(如：S1):";
        cin >> sno;
        strcat_s(strquery, sno);
        strcat_s(strquery, "','");
        cout << "请输入供应商姓名(SNAME)(如：精益):";
        cin >> sname;
        strcat_s(strquery, sname);
        strcat_s(strquery, "','");
        cout << "请输入供应商状态(STATUS)(如：20):";
        cin >> status;
        strcat_s(strquery, status);
        strcat_s(strquery, "','");
        cout << "请输入供应商所在城市(CITY)(如：天津):";
        cin >> city;
        strcat_s(strquery, city);
        strcat_s(strquery, "');");
        if (mysql_query(&mysql, strquery) == 0)
        {
            cout << "插入成功！" << endl;
        }
        else
        {
            string error(mysql_error(&mysql)); // 无错误返回空串
            cout << "Error:" << error << endl;
            // cout << "ERROR:插入失败！" << endl;
        }
        break;
    }
    return 0;
}
int update()
{
    char sno[10];
    char sname[10];
    char status[10];
    char city[10];
    cout << "请输入所需要修改的供应商代码(sno)(如：S1):";
    cin >> sno;
    char strquery[100] = "UPDATE s SET sname='";
    char yn[2];
    cout << "请输入供应商姓名(SNAME)(如：精益):";
    cin >> sname;
    strcat_s(strquery, sname);
    strcat_s(strquery, "', status='");
    cout << "请输入供应商状态(STATUS)(如：20):";
    cin >> status;
    strcat_s(strquery, status);
    strcat_s(strquery, "', city='");
    cout << "请输入供应商所在城市(CITY)(如：天津):";
    cin >> city;
    strcat_s(strquery, city);
    strcat_s(strquery, "' WHERE sno='");
    strcat_s(strquery, sno);
    strcat_s(strquery, "';");
    mysql_query(&mysql, strquery); // 执行sql语句
    int num(mysql_affected_rows(&mysql));
    string error(mysql_error(&mysql));
    if (num > 0 && error == "")
    {
        cout << "修改" << sno << "成功！\n";
    }
    else
    {
        cout << "修改" << sno << "失败！" << endl;
        if (error != "") // 给出修改不成功的原因
        {
            cout << "Error:" << mysql_error(&mysql) << endl;
        }
        else
        {
            cout << "Error:不存在(" << sno << ")代码号" << endl;
        }
    }
    return 0;
}
int deleteData()
{
    char yn[2];
    char sno[10];
    char sname[10];
    char status[10];
    char city[10];

    MYSQL_ROW row;
    cout << "请输入所要删除零件代码(SNO):\n";
    cin >> sno;
    char strquery[100] = "DELETE FROM s WHERE sno='";
    strcat_s(strquery, sno);
    strcat_s(strquery, "';");
    mysql_query(&mysql, strquery); // 执行sql语句
    int num(mysql_affected_rows(&mysql));
    string error(mysql_error(&mysql));
    if (num > 0 && error == "")
    {
        cout << "删除" << sno << "成功！\n";
    }
    else
    {
        cout << "删除" << sno << "失败！" << endl;
        if (error != "") // 给出删除不成功的原因
        {
            cout << "Error:" << mysql_error(&mysql) << endl;
        }
        else
        {
            cout << "Error:不存在(" << sno << ")代码号" << endl;
        }
    }

    return 0;
}
int select()
{
    char sno[10];
    char sname[9];
    char status[9];
    char city[9];
    // mysql_query(&myconnt, "SET NAMES 'GB2312' ");
    MYSQL_ROW row;
    char strquery[100] = "select sno,sname,status,city from s where sno like '%";
    printf("请输入SNO:");
    cin >> sno;
    strcat_s(strquery, sno);
    strcat_s(strquery, "%';"); // 模糊查询
    mysql_query(&mysql, strquery);
    MYSQL_RES *result = mysql_store_result(&mysql);

    int num_fields = mysql_field_count(&mysql);
    int flag = 0;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        if (flag == 0)
        {
            cout << "sno name status city  " << endl;
            flag = 1;
        }
        for (int i = 0; i < num_fields; i++)
        {
            switch (i)
            {
            case 0:
                cout << row[i] << "  ";
                strcpy_s(sno, row[i]);
                break;
            case 1:
                cout << row[i] << "   ";
                strcpy_s(sname, row[i]);
                break;
            case 2:
                cout << row[i] << "   ";
                strcpy_s(status, row[i]);
                break;
            case 3:
                cout << row[i];
                strcpy_s(city, row[i]);
                break;
            }
        }
        cout << endl;
    }
    string error(mysql_error(&mysql)); // 无错误返回空串
    if (error != "")
    {
        cout << "Error:" << error;
    }
    if (flag == 0)
    {
        cout << "数据库无该信息！" << endl;
    }
    return 0;
}
int display()
{
    char sno[10];
    char sname[9];
    char status[9];
    char city[9];
    // mysql_query(&myconnt, "SET NAMES 'GB2312' ");
    MYSQL_ROW row;
    char strquery[100] = "select sno,sname,status,city from s";
    mysql_query(&mysql, strquery);
    MYSQL_RES *result = mysql_store_result(&mysql);
    int num_fields = mysql_field_count(&mysql);
    int flag = 0;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        if (flag == 0)
        {
            cout << "sno name status city  " << endl;
            flag = 1;
        }
        for (int i = 0; i < num_fields; i++)
        {
            switch (i)
            {
            case 0:
                cout << row[i] << "  ";
                strcpy_s(sno, row[i]);
                break;
            case 1:
                cout << row[i] << "   ";
                strcpy_s(sname, row[i]);
                break;
            case 2:
                cout << row[i] << "   ";
                strcpy_s(status, row[i]);
                break;
            case 3:
                cout << row[i];
                strcpy_s(city, row[i]);
                break;
            }
        }
        cout << endl;
    }
    string error(mysql_error(&mysql)); // 无错误返回空串
    if (error != "")
    {
        cout << "Error:" << error;
    }
    return 0;
}