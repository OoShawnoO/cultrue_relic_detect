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

#endif
