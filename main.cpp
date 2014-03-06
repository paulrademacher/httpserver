#include <stdio.h>
#include <string>
#include <stdlib.h>

#include "server.hpp"

void do_index(Request &request, Response &response) {
  response.write("<html>\n");
  response.write("<b>HI!</b>\n");
  response.write("<hr>POST:<form method=post><input type=submit></form>\n");
  response.write("<hr>GET:<form method=get><input type=submit></form>\n");
  response.write("</html>\n");
}

void do_index_post(Request &request, Response &response) {
  response.write("<html>\n");
  response.write("<b>DA POST</b>\n");
  response.write("<form method=post><input type=submit></form>\n");
  response.write("</html>\n");
}


void do_foo(Request &request, Response &response) {
  response.write("<html>\n");
  response.write("FOOOOOO!\n");
  response.write("</html>\n");
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

  Server server(hostname, port);

  server.add_route("/", do_index);
  server.add_route("/", do_index_post, METHOD_POST);
  server.add_route("/f.*", do_foo);

  server.run();

  return 0;
}
