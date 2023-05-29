#ifndef CULTURE_RELIC_DETECT_ITEM_ROUTER_H
#define CULTURE_RELIC_DETECT_ITEM_ROUTER_H

#include "../dbutils.h"
#include "http/http_conn.h"
#include <jinja2cpp/template.h>

class item_router : public router{
public:
    item_router() : router("/item",{GET})
    {

    }

    bool method_get(http_conn* con) override
    {
        std::ifstream in("resource/templates/item.html");
        std::stringstream buffer;
        buffer << in.rdbuf();
        in.close();

        int id = atoi(con->req_header.parameters["id"].c_str());
        std::cout << id << std::endl;
        auto connect = connect_sqlite();
        jinja2::ValuesList items;
        select_items(connect,items,id);
        close_sqlite(connect);
        jinja2::Template temp;
        temp.Load(buffer.str());
        jinja2::ValuesMap params{
                {"item",items}
        };
        return con->send_str(temp.RenderAsString(params).value());
    }
};

#endif
