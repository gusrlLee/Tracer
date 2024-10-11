#include "mt_pch.h"
#include "mt_application.h"

int main()
{
    auto app = make<MtApplication>();
    app->prepare();
    app->test();
    app->run();
    app->release();
    return 0;
}