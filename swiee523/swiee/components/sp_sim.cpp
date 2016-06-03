#include <QtGui>
#include "sp_sim.h"
#include "main.h"


SP_Sim::SP_Sim()
{
  Description = QObject::tr("S parameter simulation");

  QString s = Description;
  int a = s.find(" ");
  int b = s.findRev(" ");
  if (a != -1 && b != -1) {
    if (a > (int) s.length() - b)  b = a;
  }
  if (b != -1) s[b] = '\n';

  Texts.append(new Text(0, 0, s.left(b), Qt::darkBlue, SwieeSettings.largeFontSize));
  if (b != -1)
    Texts.append(new Text(0, 0, s.mid(b+1), Qt::darkBlue, SwieeSettings.largeFontSize));

  x1 = -10; y1 = -9;
  x2 = x1+121; y2 = y1+59;

  tx = 0;
  ty = y2+1;
  Model = ".SP";
  Name  = "SP";

  // The index of the first 4 properties must not changed. Used in recreate().
  Props.append(new Property("Type", "lin", true,
	QObject::tr("sweep type")+" [lin, log, list, const]"));
  Props.append(new Property("Start", "1 GHz", true,
	QObject::tr("start frequency in Hertz")));
  Props.append(new Property("Stop", "10 GHz", true,
	QObject::tr("stop frequency in Hertz")));
  Props.append(new Property("Points", "19", true,
	QObject::tr("number of simulation steps")));
  Props.append(new Property("Noise", "no", false,
	QObject::tr("calculate noise parameters")+
	" [yes, no]"));
  Props.append(new Property("NoiseIP", "1", false,
	QObject::tr("input port for noise figure")));
  Props.append(new Property("NoiseOP", "2", false,
	QObject::tr("output port for noise figure")));
  Props.append(new Property("saveCVs", "no", false,
	QObject::tr("put characteristic values into dataset")+
	" [yes, no]"));
  Props.append(new Property("saveAll", "no", false,
	QObject::tr("save subcircuit characteristic values into dataset")+
	" [yes, no]"));
}

SP_Sim::~SP_Sim()
{
}

Component* SP_Sim::newOne()
{
  return new SP_Sim();
}

Element* SP_Sim::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("S-parameter simulation");
  BitmapFile = (char *) "sparameter";

  if(getNewOne)  return new SP_Sim();
  return 0;
}

void SP_Sim::recreate(Schematic*)
{
  Property *pp = Props.first();
  if((pp->Value == "list") || (pp->Value == "const")) {
    // Call them "Symbol" to omit them in the netlist.
    pp = Props.next();
    pp->Name = "Symbol";
    pp->display = false;
    pp = Props.next();
    pp->Name = "Symbol";
    pp->display = false;
    Props.next()->Name = "Values";
  }
  else {
    Props.next()->Name = "Start";
    Props.next()->Name = "Stop";
    Props.next()->Name = "Points";
  }
}
