#include <gtest/gtest.h>
#include <uvw.hpp>


TEST(Async, Send) {
    auto loop = uvw::Loop::getDefault();
    auto handle = loop->resource<uvw::AsyncHandle>();

    bool checkAsyncEvent = false;

    handle->on<uvw::ErrorEvent>([](const uvw::ErrorEvent &, uvw::AsyncHandle &) { FAIL(); });

    handle->on<uvw::AsyncEvent>([&checkAsyncEvent](const uvw::AsyncEvent &, uvw::AsyncHandle &hndl) {
        ASSERT_FALSE(checkAsyncEvent);
        checkAsyncEvent = true;
        hndl.close();
        ASSERT_TRUE(hndl.closing());
    });

    handle->send();

    ASSERT_TRUE(handle->active());
    ASSERT_FALSE(handle->closing());

    loop->run();

    ASSERT_TRUE(checkAsyncEvent);
}


TEST(Async, Fake) {
    auto loop = uvw::Loop::getDefault();
    auto handle = loop->resource<uvw::AsyncHandle>();

    handle->on<uvw::ErrorEvent>([](const uvw::ErrorEvent &, uvw::AsyncHandle &) { FAIL(); });
    handle->on<uvw::AsyncEvent>([](const uvw::AsyncEvent &, uvw::AsyncHandle &) { FAIL(); });

    handle->send();
    handle->close();

    ASSERT_FALSE(handle->active());
    ASSERT_TRUE(handle->closing());

    loop->run();
}
