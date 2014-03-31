env = Environment(
    CXX="c++",
    CCFLAGS="-g -std=c++11 -stdlib=libc++", ## -Weverything",

    # /usr/local/lib
    LIBS=["boost_system", "boost_regex-mt", "boost_thread-mt", "boost_coroutine-mt"],

    LINKFLAGS="-stdlib=libc++")

env.Program(
    target="main",
    source=["main.cpp",
            "async_http_client.cpp",
            "async_methods.cpp",
            "encoding.cpp",
            "request.cpp",
            "response.cpp",
            "response_code.cpp",
            "route.cpp",
            "server.cpp",
            "socket.cpp",
            "transaction.cpp",
            "utils.cpp"])
