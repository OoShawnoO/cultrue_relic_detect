#include <jinja2cpp/template.h>
#include <sqlite3.h>
#include "conv_event/http/http_conn.h"

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

bool select_items(sqlite3* sql,jinja2::ValuesList& value_list)
{
    const char* sql_str = "select * from item";
    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(sql,sql_str,-1,&stmt,nullptr);
    if(result == SQLITE_OK)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            jinja2::ValuesList values{
                    {sqlite3_column_int(stmt,0)},
                    {(const char*)sqlite3_column_text(stmt,1)},
                    {(const char*)sqlite3_column_text(stmt,2)},
                    {(const char*)sqlite3_column_text(stmt,3)},
                    {(const char*)sqlite3_column_text(stmt,4)},

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



class index_router : public router
{
public:
    index_router():router("/index",{GET,})
    {

    }
    bool method_get(http_conn* con) override
    {
        std::ifstream in("resource/templates/item.html");
        std::stringstream buffer;
        buffer << in.rdbuf();
        in.close();

        jinja2::ValuesList values;
        sqlite3* sql = connect_sqlite();
        select_items(sql,values);
        sqlite3_close(sql);

        jinja2::Template temp;
        temp.Load(buffer.str());
        jinja2::ValuesMap params{
                {"item",values[2]}
        };

        return con->send_str(temp.RenderAsString(params).value());
    }
};
ROUTER(index_router)

int main()
{
    http_multi base;
    base.wait();
}