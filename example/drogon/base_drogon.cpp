#include <drogon/HttpController.h>
#include <drogon/HttpSimpleController.h>
#include <drogon/drogon.h>

#include<memory>
#include<string>

using namespace drogon;

#define DROGON_NO_NAMESPACE_AND_CLASSNAME_IN_PATH

// HttpControllers are automatically added to Drogon upon Drogon initializing.
class SayHello : public HttpController<SayHello, false>
{
    public:
        SayHello() : HttpController() {};
        METHOD_LIST_BEGIN
        ADD_METHOD_TO(SayHello::genericHello, "/", Get, Post);
        ADD_METHOD_TO(SayHello::personalizedHello, "/hello?user={user-name}", Get, Post);
        METHOD_ADD(SayHello::genericHello, "/hello", Get, Post);
        METHOD_ADD(SayHello::personalizedHello, "/hello?user={user-name}", Get, Post);
        METHOD_LIST_END

    protected:
        void genericHello(const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& callback)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody(
                "Hello, this is a generic hello message from the SayHello "
                "controller");
            callback(resp);
        }

        void personalizedHello(const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& callback, const std::string& name)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("Hello, " + name + ", this is a personalized hello from the SayHello Controller");
            callback(resp);
        }
};

int main()
{
    LOG_INFO << "Server running on 127.0.0.1:8848";
    app().registerController(std::make_shared<SayHello>());
    app().addListener("127.0.0.1", 8848).run();
    return 0;
}