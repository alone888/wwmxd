#ifndef QUCS_MAIN_H
#define QUCS_MAIN_H

#include <QFont>
#include <QColor>
#include <Q3PtrList>
#include <QStringList>

#include "wire.h"
#include "node.h"
#include "diagrams/diagram.h"
#include <QSettings>

class SwieeApp;
class Component;

// constants may be missing on windows systems
#include <math.h>
#ifndef M_PI
#define M_PI     3.1415926535897932384626433832795029
#endif
//title {background: rgb(0, 255, 255);} 
#define BACKCOLOR_STYLE "background-color: rgb(255, 255, 255);"
#define BACKCOLOR 255,255,255
#define SCH_BACKCOLOR BACKCOLOR

struct tSwieeSettings {
  int x, y, dx, dy;    // position and size of main window
  QFont font;
  float largeFontSize;
  QColor BGColor;      // background color of view area
  QString Language;

  // syntax highlighting
  QColor Comment, String, Integer, Real, Character, Type,
    Attribute, Directive, Task;

  unsigned int maxUndo;    // size of undo stack
  QString Editor;
  QString BinDir;
  QString LangDir;
  QString LibDir;
  QString OctaveDir;  // m-files location
  QString ExamplesDir;
  QString DocDir;

  unsigned int NodeWiring;
  QDir SwieeWorkDir;
  QDir SwieeHomeDir;
  QDir AdmsXmlBinDir;  // dir of admsXml executable
  QDir AscoBinDir;     // dir of asco executable
  QDir OctaveBinDir;   // dir of octave executable

  // registered filename extensions with program to open the file
  QStringList FileTypes;

  unsigned int numRecentDocs;
  QStringList RecentDocs;

  bool IgnoreFutureVersion;

};

extern tSwieeSettings SwieeSettings;  // extern because nearly everywhere used
extern SwieeApp *SwieeMain;  // the Swiee application itself
extern QString lastDir;    // to remember last directory for several dialogs
extern QStringList swieePathList;

bool loadSettings();
bool saveApplSettings(SwieeApp*);
void swieeMessageOutput(QtMsgType type, const char *msg);

QString complexRect(double, double, int Precision=3);
QString complexDeg (double, double, int Precision=3);
QString complexRad (double, double, int Precision=3);
QString StringNum  (double, char form='g', int Precision=3);
void    str2num    (const QString&, double&, QString&, double&);
QString num2str    (double);
QString StringNiceNum(double);
void    convert2Unicode(QString&);
void    convert2ASCII(QString&);
QString properName(const QString&);
QString properAbsFileName(const QString&);
QString properFileName(const QString&);
bool    VHDL_Time(QString&, const QString&);
bool    VHDL_Delay(QString&, const QString&);
bool    Verilog_Time(QString&, const QString&);
bool    Verilog_Delay(QString&, const QString&);
QString Verilog_Param(const QString);
bool    checkVersion(QString&);

#endif // ifndef QUCS_MAIN_H
