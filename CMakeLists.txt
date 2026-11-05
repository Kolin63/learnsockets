cmake_minimum_required(VERSION 3.28.3)

project(server)
add_executable(server "server.c")
set_target_properties(server PROPERTIES
  C_STANDARD 23
  C_STANDARD_REQUIRED ON
)

project(client)
add_executable(client "client.c")
set_target_properties(client PROPERTIES
  C_STANDARD 23
  C_STANDARD_REQUIRED ON
)
