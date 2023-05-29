#ifndef CULTURE_RELIC_DETECT_TEMPLATES_FILTER_H
#define CULTURE_RELIC_DETECT_TEMPLATES_FILTER_H

#include "http/http_conn.h"

class templates_filter : public filter{
public:
    templates_filter() : filter("/templates/*",{GET,POST})
    {
    }
};

#endif
