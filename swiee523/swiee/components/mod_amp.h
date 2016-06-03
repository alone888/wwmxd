#ifndef mod_amp_H
#define mod_amp_H

#include "component.h"

class mod_amp : public Component
{
  public:
    mod_amp();
    ~mod_amp() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* mod_amp_H */
