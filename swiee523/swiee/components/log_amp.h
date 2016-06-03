#ifndef log_amp_H
#define log_amp_H

#include "component.h"

class log_amp : public Component
{
  public:
    log_amp();
    ~log_amp() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* log_amp_H */
