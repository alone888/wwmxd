#ifndef hicumL2V2p31n_H
#define hicumL2V2p31n_H

#include "component.h"

class hicumL2V2p31n : public Component 
{
  public:
    hicumL2V2p31n();
    ~hicumL2V2p31n() { };
    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne=false);
  protected:
    void createSymbol();
};

#endif /* hicumL2V2p31n_H */
