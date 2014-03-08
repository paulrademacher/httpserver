env = Environment(
    CXX="c++",
    CCFLAGS="-std=c++11 -stdlib=libc++", ## -Weverything",
    LIBS=["boost_system", "boost_regex-mt"],
    LINKFLAGS="-stdlib=libc++")

env.Program(
    target="main",
    source=["main.cpp",
            "encoding.cpp",
            "request.cpp",
            "response.cpp",
            "response_code.cpp",
            "route.cpp",
            "server.cpp",
            "socket.cpp",
            "transaction.cpp",
            "utils.cpp"])
