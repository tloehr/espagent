#ifndef SCHEME_MACROS_H
#define SCHEME_MACROS_H

#include <Arduino.h>
#include <SimpleMap.h>

class SchemeMacros
{
private:
    SimpleMap<String, String> *macros = new SimpleMap<String, String>([](String &a, String &b) -> int
                                                                      {
                                                                          if (a == b)
                                                                              return 0; // a and b are equal
                                                                          else if (a > b)
                                                                              return 1; // a is bigger than b
                                                                          else
                                                                              return -1; // a is smaller than b
                                                                      });

public:
    SchemeMacros();
    String get_macro(String key);
};

#endif