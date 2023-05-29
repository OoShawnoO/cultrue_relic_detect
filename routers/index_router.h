#ifndef CULTURE_RELIC_DETECT_INDEX_ROUTER_H
#define CULTURE_RELIC_DETECT_INDEX_ROUTER_H
#include "../dbutils.h"
#include "http/http_conn.h"
#include <jinja2cpp/template.h>

class index_router : public router
{
public:
    index_router():router("/",{GET,})
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

#endif
