#ifndef photodiode_H
#define photodiode_H

#include "component.h"

class photodiode : public Component
{
  public:
    photodiode();
    ~photodiode() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* photodiode_H */
