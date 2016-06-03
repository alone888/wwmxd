#ifndef potentiometer_H
#define potentiometer_H

#include "component.h"

class potentiometer : public Component
{
  public:
    potentiometer();
    ~potentiometer() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* potentiometer_H */
