#ifndef CULTURE_RELIC_DETECT_DBUTILS_H
#define CULTURE_RELIC_DETECT_DBUTILS_H

#include <sqlite3.h>
#include <jinja2cpp/template.h>
const char* database_path = "db/app.db";

sqlite3* connect_sqlite()
{
    sqlite3* sql = nullptr;
    int result = sqlite3_open_v2(database_path,&sql,SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, nullptr);
    if(result == SQLITE_OK){
        return sql;
    }
    else
    {
        std::cout << "数据库链接失败" << std::endl;
        return sql;
    }
}

void close_sqlite(sqlite3* sql)
{
    sqlite3_close_v2(sql);
    sql = nullptr;
}

bool select_items(sqlite3* sql,jinja2::ValuesList& value_list,int id = -1)
{
    std::string sql_str;
    if(id < 0 )
        sql_str = "select * from item";
    else
        sql_str = "select * from item where id =" + std::to_string(id);
    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(sql,sql_str.c_str(),-1,&stmt,nullptr);
    if(result == SQLITE_OK)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            jinja2::ValuesList values;
            values.emplace_back(sqlite3_column_int(stmt,0));
            values.emplace_back((const char*)sqlite3_column_text(stmt,1));
            values.emplace_back((const char*)sqlite3_column_text(stmt,2));
            values.emplace_back((const char*)sqlite3_column_text(stmt,3));
            values.emplace_back((const char*)sqlite3_column_text(stmt,4));
            if(id < 0){
                value_list.emplace_back(std::move(values));
            }
            else
            {
                value_list = std::move(values);
            }
        }
    }
    else
    {
        return false;
    }
    sqlite3_finalize(stmt);
    stmt = nullptr;
    return true;
}

bool select_comments(sqlite3* sql,jinja2::ValuesList& value_list)
{
    std::string sql_str = "select user.username,user.admin,comment.content,comment.star,comment.id from user,comment where comment.uid = user.id";
    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(sql,sql_str.c_str(),-1,&stmt,nullptr);
    if(result == SQLITE_OK)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            jinja2::ValuesList values{
                    {(const char*)sqlite3_column_text(stmt,0)},
                    {sqlite3_column_int(stmt,1)},
                    {(const char*)sqlite3_column_text(stmt,2)},
                    {sqlite3_column_int(stmt,3)},
                    {sqlite3_column_int(stmt,4)},
            };
            value_list.emplace_back(values);
        }
    }
    else
    {
        return false;
    }
    sqlite3_finalize(stmt);
    stmt = nullptr;
    return true;
}

#endif
