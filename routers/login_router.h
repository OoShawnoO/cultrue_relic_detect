#ifndef CULTURE_RELIC_DETECT_LOGIN_ROUTER_H
#define CULTURE_RELIC_DETECT_LOGIN_ROUTER_H

#include "../dbutils.h"
#include "http/http_conn.h"
#include <jinja2cpp/template.h>

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
        std::string username = con->req_body.form["username"];
        std::string password = con->req_body.form["password"];
        std::cout << username << std::endl;
        std::cout << password << std::endl;
        return con->redirect("/");
    }
};
ROUTER(login_router);

#endif
