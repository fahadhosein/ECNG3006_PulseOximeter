idf_component_register(SRCS "report.c"
                        INCLUDE_DIRS "include" "."
                        REQUIRES "spiffs")
link_directories(COMPONENTS)