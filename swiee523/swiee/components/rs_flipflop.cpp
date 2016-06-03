#include <QtGui>
#include "rs_flipflop.h"
#include "node.h"
#include "main.h"

RS_FlipFlop::RS_FlipFlop()
{
  Type = isDigitalComponent;
  Description = QObject::tr("RS flip flop");

  Props.append(new Property("t", "0", false, QObject::tr("delay time")));

  Lines.append(new Line(-20,-20, 20,-20,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-20, 20, 20, 20,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-20,-20,-20, 20,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 20,-20, 20, 20,QPen(Qt::darkBlue,2)));

  Lines.append(new Line(-30,-10,-20,-10,QPen(Qt::darkBlue,2)));
  Lines.append(new Line(-30, 10,-20, 10,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 30,-10, 20,-10,QPen(Qt::darkBlue,2)));
  Lines.append(new Line( 30, 10, 20, 10,QPen(Qt::darkBlue,2)));

  Texts.append(new Text(-18,-21, "R", Qt::darkBlue, 12.0));
  Texts.append(new Text(-18, -1, "S", Qt::darkBlue, 12.0));
  Texts.append(new Text(  6,-21, "Q", Qt::darkBlue, 12.0));
  Texts.append(new Text(  6, -1, "Q", Qt::darkBlue, 12.0));
  Texts.current()->over=true;

  Ports.append(new Port(-30,-10));  // R
  Ports.append(new Port(-30, 10));  // S
  Ports.append(new Port( 30,-10));  // Q
  Ports.append(new Port( 30, 10));  // nQ

  x1 = -30; y1 = -24;
  x2 =  30; y2 =  24;
  tx = x1+4;
  ty = y2+4;
  Model = "RSFF";
  Name  = "Y";
}

// -------------------------------------------------------
QString RS_FlipFlop::vhdlCode(int NumPorts)
{
  QString s = "";
  if(NumPorts <= 0) { // no truth table simulation ?
    QString td = Props.at(0)->Value;     // delay time
    if(!VHDL_Delay(td, Name)) return td; // time has not VHDL format
    s = td;
  }
  s += ";\n";

  s = "  " +
    Ports.at(2)->Connection->Name + " <= " +
    Ports.at(0)->Connection->Name + " nor " +
    Ports.at(3)->Connection->Name + s + "  " +
    Ports.at(3)->Connection->Name + " <= " +
    Ports.at(1)->Connection->Name + " nor " +
    Ports.at(2)->Connection->Name + s + '\n';
  return s;
}

// -------------------------------------------------------
QString RS_FlipFlop::verilogCode(int NumPorts)
{
  QString t = "";
  if(NumPorts <= 0) { // no truth table simulation ?
    QString td = Props.at(0)->Value;        // delay time
    if(!Verilog_Delay(td, Name)) return td; // time has not VHDL format
    t = td;
  }
  t += " ";
  
  QString l = "";

  QString s = Ports.at(1)->Connection->Name;
  QString r = Ports.at(0)->Connection->Name;
  QString q = Ports.at(2)->Connection->Name;
  QString b = Ports.at(3)->Connection->Name;
  
  l = "\n  // " + Name + " RS-flipflop\n" +
    "  assign" + t + q + " = ~(" + r + " | " + b + ");\n" +
    "  assign" + t + b + " = ~(" + s + " | " + q + ");\n\n";
  return l;
}

// -------------------------------------------------------
Component* RS_FlipFlop::newOne()
{
  return new RS_FlipFlop();
}

// -------------------------------------------------------
Element* RS_FlipFlop::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("RS-FlipFlop");
  BitmapFile = (char *) "rsflipflop";

  if(getNewOne)  return new RS_FlipFlop();
  return 0;
}