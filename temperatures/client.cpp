

#include "client.h"


void Client::run() {
  sock_.connect(host_, port_);
  sock_.write("Foo bar baz\n");
}
