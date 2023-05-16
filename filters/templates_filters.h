#ifndef CULTURE_RELIC_DETECT_TEMPLATES_FILTERS_H
#define CULTURE_RELIC_DETECT_TEMPLATES_FILTERS_H

#include "http/http_conn.h"

class templates_filter : public filter{
public:
    templates_filter() : filter("/templates/*",{GET,POST})
    {
    }
};
FILTER(templates_filter)

#endif
