#ifndef CULTURE_RELIC_DETECT_DETECT_ROUTER_H
#define CULTURE_RELIC_DETECT_DETECT_ROUTER_H

#include "../dbutils.h"
#include "detect/detector.h"
#include "http/http_conn.h"
#include <jinja2cpp/template.h>


class detect_router : public router
{
    static detector _detector;
public:
    detect_router() : router("/detect",{POST})
    {

    }
    bool method_post(http_conn* con) override
    {
        auto ret = _detector.predict_bytes(con->req_body.files["file"].second);
        json _predict{
                {"url","static/tmp/img.jpg"},
                {"result",ret.first},
        };
        return con->send_str(_predict.dump(),"application/json");
    }
};
detector detect_router::_detector("../model/culture_relic");
ROUTER(detect_router)

#endif
