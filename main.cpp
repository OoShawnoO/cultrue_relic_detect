#include <jinja2cpp/template.h>
#include <sqlite3.h>
#include "conv_event/detect/detector.h"
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

class index_router : public router
{
public:
    index_router():router("/index",{GET,})
    {

    }
    bool method_get(http_conn* con) override
    {
        std::ifstream in("resource/templates/index.html");
        std::stringstream buffer;
        buffer << in.rdbuf();
        in.close();

        jinja2::ValuesList comments_values;
        sqlite3* sql = connect_sqlite();
        select_comments(sql,comments_values);
        jinja2::ValuesList items_values;
        select_items(sql,items_values);
        close_sqlite(sql);

        jinja2::ValuesMap items_map{
                {"玉石",jinja2::ValuesList()},
                {"绘画",jinja2::ValuesList()},
                {"陶瓷",jinja2::ValuesList()},
                {"青铜器",jinja2::ValuesList()},
                {"雕塑",jinja2::ValuesList()},
        };
        for(const auto& v : items_values)
        {
            items_map.at(v.asList()[4].asString()).asList().emplace_back(v);
        }
        jinja2::Template temp;
        temp.Load(buffer.str());
        jinja2::ValuesMap params{
                {"comments",comments_values},
                {"items",items_map},
                {"cur_user",jinja2::ValuesList {"custom"}}
        };

        return con->send_str(temp.RenderAsString(params).value());
    }
};
ROUTER(index_router)

class detect_router : public router
{
    static detector _detector;
public:
    detect_router() : router("/detect",{POST})
    {

    }
    bool method_post(http_conn* con) override
    {
        std::ofstream out("resource/static/tmp/img.jpg");
        out << con->req_body.files["file"].second;
        out.close();
        auto ret = _detector.predict("resource/static/tmp/img.jpg");
        json _predict{
                {"url","static/tmp/img.jpg"},
                {"result",ret.first},
        };
        return con->send_str(_predict.dump(),"application/json");
    }
};
detector detect_router::_detector("../model/culture_relic");
ROUTER(detect_router)

class login_router : public router{
public:
    login_router() : router("/login",{GET,POST})
    {

    }
    bool method_get(http_conn* con) override
    {
        return con->render("/templates/login.html");
    }
    bool method_post(http_conn* con) override
    {
        return con->redirect("/index");
    }
};
ROUTER(login_router);

class tmp_jpg_filter : public filter {
public:
    tmp_jpg_filter() : filter("/static/tmp/*",{GET})
    {

    }
    bool allow(http_conn* con) override
    {
        con->res_header.response_headers["Cache-Control"] = "no-cache, no-store, must-revalidate";
        con->res_header.response_headers["Expires"] = "0";
        return true;
    }
};
FILTER(tmp_jpg_filter)

class templates_filter : public filter{
public:
    templates_filter() : filter("/templates/*",{GET,POST})
    {
    }
};
FILTER(templates_filter)

int main()
{
    http_multi base;
    base.wait();
}