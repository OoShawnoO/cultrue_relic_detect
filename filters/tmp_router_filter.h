#ifndef CULTURE_RELIC_DETECT_TMP_ROUTER_FILTER_H
#define CULTURE_RELIC_DETECT_TMP_ROUTER_FILTER_H

#include "http/http_conn.h"

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

#endif
