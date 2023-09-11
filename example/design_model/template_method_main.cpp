#include "template_method/application.h"

void library_code(Library* obj) {
  // ...
  obj->run();
  // ...
}

int main() {
    Library* base = new Application;
    library_code(base);
    /*
    Library* base = new Application_xxx;
    library_code(base);
    */
    return 0;
}