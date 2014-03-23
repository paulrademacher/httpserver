#include <string>

#include <boost/thread/thread.hpp>

#include "server.hpp"

Server *server;

void do_index(RequestSharedPtr request, ResponseSharedPtr response) {
  std::string p2 = (*request)["p2"];

  if (true)
  server->async_wait([=]() mutable {
      printf("INSIDE HANDLER\n");
      *response << "<html>\n";
      *response << "<b>HI!</b>\n";
      *response << "<hr>POST:<form method=post><input type=text name=p1 value=1 /><input type=text name=p1 value=1 /><input type=text name=p2 value=2 /><input type=submit></form>\n";
      *response << "P2= " << p2 << "<br>";
      *response << "<hr>GET:<form method=get><input type=text name=p1 value=1 /><input type=text name=p1 value=1 /><input type=text name=p2 value=2 /><input type=submit></form>\n";
      *response << "</html>\n";
    }, 5000);
}

void do_index_post(RequestSharedPtr request, ResponseSharedPtr response) {
  *response << "<html>\n";
  *response << "<b>DA POST</b>\n";
  *response << "<form method=post><input type=submit></form>\n";
  *response << "</html>\n";
}


void do_foo(RequestSharedPtr request, ResponseSharedPtr response) {
  *response << "<html>\n";
  *response << "FOOOOOO!\n";
  *response << "</html>\n";
}

int main(int argc, char *argv[]) {
  std::string hostname = "localhost";
  int port = 8080;

  if (argc != 1) {
    if (argc != 2) {
      fprintf(stderr, "USAGE: %s <port_number>\n", argv[0]);
     return 1;
    }
    port = atoi(argv[1]);
  }

  server = new Server(hostname, port);

  server->route("/", METHOD_GET) >> do_index;
  server->route("/", METHOD_POST) >> do_index_post;
  server->route("/f.*") >> do_foo;

  server->run();

  return 0;
}
