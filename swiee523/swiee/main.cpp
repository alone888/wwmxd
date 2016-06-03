#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <QtGui>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>

#include <QApplication>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTranslator>
#include <QFile>
#include <QMessageBox>
#include <QRegExp>

#include "swiee.h"
#include "main.h"
#include "node.h"

#include "schematic.h"
#include "module.h"


#ifdef _WIN32
#include <Windows.h>  //for OutputDebugString
#endif

tSwieeSettings SwieeSettings;

SwieeApp *SwieeMain = 0;  // the Swiee application itself
QString lastDir;    // to remember last directory for several dialogs
QStringList swieePathList;

// #########################################################################
// Loads the settings file and stores the settings.
bool loadSettings()
{
    QSettings settings("swiee","swiee");

    if(settings.contains("x"))SwieeSettings.x=settings.value("x").toInt();
    if(settings.contains("y"))SwieeSettings.y=settings.value("y").toInt();
    if(settings.contains("dx"))SwieeSettings.dx=settings.value("dx").toInt();
    if(settings.contains("dy"))SwieeSettings.dy=settings.value("dy").toInt();
    if(settings.contains("font"))SwieeSettings.font.fromString(settings.value("font").toString());
    if(settings.contains("largeFontSize"))SwieeSettings.largeFontSize=settings.value("largeFontSize").toDouble();
    if(settings.contains("maxUndo"))SwieeSettings.maxUndo=settings.value("maxUndo").toInt();
    if(settings.contains("NodeWiring"))SwieeSettings.NodeWiring=settings.value("NodeWiring").toInt();
    if(settings.contains("BGColor"))SwieeSettings.BGColor.setNamedColor(settings.value("BGColor").toString());
	//SwieeSettings.BGColor.setNamedColor(QString("#FFE6FF"));
	SwieeSettings.BGColor.setRgb(255, 230, 255);
    if(settings.contains("Editor"))SwieeSettings.Editor=settings.value("Editor").toString();
    if(settings.contains("FileTypes"))SwieeSettings.FileTypes=settings.value("FileTypes").toStringList();
    if(settings.contains("Language"))SwieeSettings.Language=settings.value("Language").toString();
    if(settings.contains("Comment"))SwieeSettings.Comment.setNamedColor(settings.value("Comment").toString());
    if(settings.contains("String"))SwieeSettings.String.setNamedColor(settings.value("String").toString());
    if(settings.contains("Integer"))SwieeSettings.Integer.setNamedColor(settings.value("Integer").toString());
    if(settings.contains("Real"))SwieeSettings.Real.setNamedColor(settings.value("Real").toString());
    if(settings.contains("Character"))SwieeSettings.Character.setNamedColor(settings.value("Character").toString());
    if(settings.contains("Type"))SwieeSettings.Type.setNamedColor(settings.value("Type").toString());
    if(settings.contains("Attribute"))SwieeSettings.Attribute.setNamedColor(settings.value("Attribute").toString());
    if(settings.contains("Directive"))SwieeSettings.Directive.setNamedColor(settings.value("Directive").toString());
    if(settings.contains("Task"))SwieeSettings.Comment.setNamedColor(settings.value("Task").toString());

    if(settings.contains("Editor"))SwieeSettings.Editor = settings.value("Editor").toString();
    //if(settings.contains("BinDir"))SwieeSettings.BinDir = settings.value("BinDir").toString();
    //if(settings.contains("LangDir"))SwieeSettings.LangDir = settings.value("LangDir").toString();
    //if(settings.contains("LibDir"))SwieeSettings.LibDir = settings.value("LibDir").toString();
    if(settings.contains("AdmsXmlBinDir"))SwieeSettings.AdmsXmlBinDir = settings.value("AdmsXmlBinDir").toString();
    if(settings.contains("AscoBinDir"))SwieeSettings.AscoBinDir = settings.value("AscoBinDir").toString();
    //if(settings.contains("OctaveDir"))SwieeSettings.OctaveDir = settings.value("OctaveDir").toString();
    //if(settings.contains("ExamplesDir"))SwieeSettings.ExamplesDir = settings.value("ExamplesDir").toString();
    //if(settings.contains("DocDir"))SwieeSettings.DocDir = settings.value("DocDir").toString();
    if(settings.contains("OctaveBinDir"))SwieeSettings.OctaveBinDir.setPath(settings.value("OctaveBinDir").toString());
    if(settings.contains("SwieeHomeDir"))
      if(settings.value("SwieeHomeDir").toString() != "")
         SwieeSettings.SwieeHomeDir.setPath(settings.value("SwieeHomeDir").toString());
    SwieeSettings.SwieeWorkDir = SwieeSettings.SwieeHomeDir;

    if (settings.contains("IngnoreVersion")) SwieeSettings.IgnoreFutureVersion = settings.value("IngnoreVersion").toBool();
    else SwieeSettings.IgnoreFutureVersion = false;


    SwieeSettings.RecentDocs = settings.value("RecentDocs").toString().split("*",QString::SkipEmptyParts);
    SwieeSettings.numRecentDocs = SwieeSettings.RecentDocs.count();


    // If present read in the list of directory paths in which Swiee should
    // search for subcircuit schematics
    int npaths = settings.beginReadArray("Paths");
    for (int i = 0; i < npaths; ++i)
    {
        settings.setArrayIndex(i);
        QString apath = settings.value("path").toString();
        swieePathList.append(apath);
    }
    settings.endArray();

    SwieeSettings.numRecentDocs = 0;

    return true;
}

// #########################################################################
// Saves the settings in the settings file.
bool saveApplSettings(SwieeApp *swiee)
{
    QSettings settings ("swiee","swiee");

    settings.setValue("x", SwieeSettings.x);
    settings.setValue("y", SwieeSettings.y);
    settings.setValue("dx", SwieeSettings.dx);
    settings.setValue("dy", SwieeSettings.dy);
    settings.setValue("font", SwieeSettings.font.toString());
    settings.setValue("largeFontSize", SwieeSettings.largeFontSize);
    settings.setValue("maxUndo", SwieeSettings.maxUndo);
    settings.setValue("NodeWiring", SwieeSettings.NodeWiring);
    settings.setValue("BGColor", SwieeSettings.BGColor.name());
    settings.setValue("Editor", SwieeSettings.Editor);
    settings.setValue("FileTypes", SwieeSettings.FileTypes);
    settings.setValue("Language", SwieeSettings.Language);
    settings.setValue("Comment", SwieeSettings.Comment.name());
    settings.setValue("String", SwieeSettings.String.name());
    settings.setValue("Integer", SwieeSettings.Integer.name());
    settings.setValue("Real", SwieeSettings.Real.name());
    settings.setValue("Character", SwieeSettings.Character.name());
    settings.setValue("Type", SwieeSettings.Type.name());
    settings.setValue("Attribute", SwieeSettings.Attribute.name());
    settings.setValue("Directive", SwieeSettings.Directive.name());
    settings.setValue("Task", SwieeSettings.Comment.name());
    settings.setValue("Editor", SwieeSettings.Editor);
    //settings.setValue("BinDir", SwieeSettings.BinDir);
    //settings.setValue("LangDir", SwieeSettings.LangDir);
    //settings.setValue("LibDir", SwieeSettings.LibDir);
    settings.setValue("AdmsXmlBinDir", SwieeSettings.AdmsXmlBinDir.canonicalPath());
    settings.setValue("AscoBinDir", SwieeSettings.AscoBinDir.canonicalPath());
    //settings.setValue("OctaveDir", SwieeSettings.OctaveDir);
    //settings.setValue("ExamplesDir", SwieeSettings.ExamplesDir);
    //settings.setValue("DocDir", SwieeSettings.DocDir);
    settings.setValue("OctaveBinDir", SwieeSettings.OctaveBinDir.canonicalPath());
    settings.setValue("SwieeHomeDir", SwieeSettings.SwieeHomeDir.canonicalPath());
    settings.setValue("IngnoreVersion",SwieeSettings.IgnoreFutureVersion);

    // Copy the list of directory paths in which Swiee should
    // search for subcircuit schematics from swieePathList
    settings.remove("Paths");
    settings.beginWriteArray("Paths");
    int i = 0;
    foreach(QString path, swieePathList) {
         settings.setArrayIndex(i);
         settings.setValue("path", path);
         i++;
     }
     settings.endArray();

  return true;

}

/*!
 * \brief swieeMessageOutput handles qDebug, qWarning, qCritical, qFatal.
 * \param type Message type (Qt enum)
 * \param msg Message
 *
 * The message handler is used to get control of the messages.
 * Particulary on Windows, as the messages are sent to the debugger and do not
 * show on the terminal. The handler could aslo be extended to create a log
 * mechanism.
 * <http://qt-project.org/doc/qt-4.8/debug.html#warning-and-debugging-messages>
 * <http://qt-project.org/doc/qt-4.8/qtglobal.html#qInstallMsgHandler>
 */
void swieeMessageOutput(QtMsgType type, const char *msg)
{
  switch (type) {
  case QtDebugMsg:
    fprintf(stderr, "Debug: %s\n", msg);
    break;
  case QtWarningMsg:
    fprintf(stderr, "Warning: %s\n", msg);
    break;
  case QtCriticalMsg:
    fprintf(stderr, "Critical: %s\n", msg);
    break;
  case QtFatalMsg:
    fprintf(stderr, "Fatal: %s\n", msg);
    abort();
  }

#ifdef _WIN32
  OutputDebugStringA(msg);
#endif
}

// #########################################################################
// #########################################################################
// ##########                                                     ##########
// ##########            MOVE SOMEWHERE ELSE START                ##########
// ##########                                                     ##########
// #########################################################################



QString complexRect(double real, double imag, int Precision)
{
  QString Text;
  if(fabs(imag) < 1e-250) Text = QString::number(real,'g',Precision);
  else {
    Text = QString::number(imag,'g',Precision);
    if(Text.at(0) == '-') {
      Text.replace(0,1,'j');
      Text = '-'+Text;
    }
    else  Text = "+j"+Text;
    Text = QString::number(real,'g',Precision) + Text;
  }
  return Text;
}

QString complexDeg(double real, double imag, int Precision)
{
  QString Text;
  if(fabs(imag) < 1e-250) Text = QString::number(real,'g',Precision);
  else {
    Text  = QString::number(sqrt(real*real+imag*imag),'g',Precision) + " / ";
    Text += QString::number(180.0/M_PI*atan2(imag,real),'g',Precision) + QString::fromUtf8("°");
  }
  return Text;
}

QString complexRad (double real, double imag, int Precision)
{
  QString Text;
  if(fabs(imag) < 1e-250) Text = QString::number(real,'g',Precision);
  else {
    Text  = QString::number(sqrt(real*real+imag*imag),'g',Precision);
    Text += " / " + QString::number(atan2(imag,real),'g',Precision) + "rad";
  }
  return Text;
}

// #########################################################################
QString StringNum(double num, char form, int Precision)
{
  int a = 0;
  char *p, Buffer[512], Format[6] = "%.00g";

  if(Precision < 0) {
    Format[1]  = form;
    Format[2]  = 0;
  }
  else {
    Format[4]  = form;
    Format[2] += Precision / 10;
    Format[3] += Precision % 10;
  }
  sprintf(Buffer, Format, num);
  p = strchr(Buffer, 'e');
  if(p) {
    p++;
    if(*(p++) == '+') { a = 1; }   // remove '+' of exponent
    if(*p == '0') { a++; p++; }    // remove leading zeros of exponent
    if(a > 0)
      do {
        *(p-a) = *p;
      } while(*(p++) != 0);    // override characters not needed
  }

  return QString(Buffer);
}

// #########################################################################
QString StringNiceNum(double num)
{
  char Format[6] = "%.8e";
  if(fabs(num) < 1e-250)  return QString("0");  // avoid many problems
  if(fabs(log10(fabs(num))) < 3.0)  Format[3] = 'g';

  int a = 0;
  char *p, *pe, Buffer[512];

  sprintf(Buffer, Format, num);
  p = pe = strchr(Buffer, 'e');
  if(p) {
    if(*(++p) == '+') { a = 1; }    // remove '+' of exponent
    if(*(++p) == '0') { a++; p++; } // remove leading zeros of exponent
    if(a > 0)
      do {
        *(p-a) = *p;
      } while(*(p++) != 0);  // override characters not needed

    // In 'g' format, trailing zeros are already cut off !!!
    p = strchr(Buffer, '.');
    if(p) {
      if(!pe)  pe = Buffer + strlen(Buffer);
      p = pe-1;
      while(*p == '0')   // looking for unneccessary zero characters
        if((--p) <= Buffer)  break;
      if(*p != '.')  p++;  // no digit after decimal point ?
      while( (*(p++) = *(pe++)) != 0 ) ;  // overwrite zero characters
    }
  }

  return QString(Buffer);
}

// #########################################################################
void str2num(const QString& s_, double& Number, QString& Unit, double& Factor)
{
  QString str = s_.stripWhiteSpace();

/*  int i=0;
  bool neg = false;
  if(str[0] == '-') {      // check sign
    neg = true;
    i++;
  }
  else if(str[0] == '+')  i++;

  double num = 0.0;
  for(;;) {
    if(str[i] >= '0')  if(str[i] <= '9') {
      num = 10.0*num + double(str[i]-'0');
    }
  }*/

  QRegExp Expr( QRegExp("[^0-9\\x2E\\x2D\\x2B]") );
  int i = str.find( Expr );
  if(i >= 0)
    if((str.at(i).latin1() | 0x20) == 'e') {
      int j = str.find( Expr , ++i);
      if(j == i)  j--;
      i = j;
    }

  Number = str.left(i).toDouble();
  Unit   = str.mid(i).stripWhiteSpace();
  if(Unit.length()>0)
  {
    switch(Unit.at(0).latin1()) {
      case 'T': Factor = 1e12;  break;
      case 'G': Factor = 1e9;   break;
      case 'M': Factor = 1e6;   break;
      case 'k': Factor = 1e3;   break;
      case 'c': Factor = 1e-2;  break;
      case 'm': Factor = 1e-3;  break;
      case 'u': Factor = 1e-6;  break;
      case 'n': Factor = 1e-9;  break;
      case 'p': Factor = 1e-12; break;
      case 'f': Factor = 1e-15; break;
  //    case 'd':
      default:  Factor = 1.0;
    }
  }
  else
  {
    Factor = 1.0;
  }

  return;
}

// #########################################################################
QString num2str(double Num)
{
  char c = 0;
  double cal = fabs(Num);
  if(cal > 1e-20) {
    cal = log10(cal) / 3.0;
    if(cal < -0.2)  cal -= 0.98;
    int Expo = int(cal);

    if(Expo >= -5) if(Expo <= 4)
      switch(Expo) {
        case -5: c = 'f'; break;
        case -4: c = 'p'; break;
        case -3: c = 'n'; break;
        case -2: c = 'u'; break;
        case -1: c = 'm'; break;
        case  1: c = 'k'; break;
        case  2: c = 'M'; break;
        case  3: c = 'G'; break;
        case  4: c = 'T'; break;
      }

    if(c)  Num /= pow(10.0, double(3*Expo));
  }

  QString Str = QString::number(Num);
  if(c)  Str += c;

  return Str;
}

// #########################################################################
void convert2Unicode(QString& Text)
{
  bool ok;
  int i = 0;
  QString n;
  unsigned short ch;
  while((i=Text.find("\\x", i)) >= 0) {
    n = Text.mid(i, 6);
    ch = n.mid(2).toUShort(&ok, 16);
    if(ok)  Text.replace(n, QChar(ch));
    i++;
  }
  Text.replace("\\n", "\n");
  Text.replace("\\\\", "\\");
}

// #########################################################################
void convert2ASCII(QString& Text)
{
  Text.replace('\\', "\\\\");
  Text.replace('\n', "\\n");

  int i = 0;
  QChar ch;
  char Str[8];
  while(Text.size()<i) {  // convert special characters
    if(ch > QChar(0x7F)) {
      sprintf(Str, "\\x%04X", ch.unicode());
      Text.replace(ch, Str);
    }
  }
}

// #########################################################################
// Converts a path to an absolute path and resolves paths relative to the
// Swiee home directory
QString properAbsFileName(const QString& Name)
{
  QString s = Name;
  QFileInfo Info(s);

  if(Info.isRelative())
  {
      // if it's a relative file, look for it relative to the
      // working directory (the swiee home directory)
      s = SwieeSettings.SwieeWorkDir.filePath(s);
  }
  // return the clean path
  return QDir::cleanPath(s);
}

// #########################################################################
QString properFileName(const QString& Name)
{
  QFileInfo Info(Name);
  return Info.fileName();
}

// #########################################################################
// Takes a file name (with path) and replaces all special characters.
QString properName(const QString& Name)
{
  QString s = Name;
  QFileInfo Info(s);
  if(Info.extension() == "sch")
    s = s.left(s.length()-4);
  if(s.at(0) <= '9') if(s.at(0) >= '0')
    s = 'n' + s;
  s.replace(QRegExp("\\W"), "_"); // none [a-zA-Z0-9] into "_"
  s.replace("__", "_");  // '__' not allowed in VHDL
  if(s.at(0) == '_')
    s = 'n' + s;
  return s;
}

// #########################################################################
// Creates and returns delay time for VHDL entities.
bool VHDL_Delay(QString& td, const QString& Name)
{
  if(strtod(td.latin1(), 0) != 0.0) {  // delay time property
    if(!VHDL_Time(td, Name))
      return false;    // time has not VHDL format
    td = " after " + td;
    return true;
  }
  else if(isalpha(td.latin1()[0])) {
    td = " after " + td;
    return true;
  }
  else {
    td = "";
    return true;
  }
}

// #########################################################################
// Checks and corrects a time (number & unit) according VHDL standard.
bool VHDL_Time(QString& t, const QString& Name)
{
  char *p;
  double Time = strtod(t.latin1(), &p);
  while(*p == ' ') p++;
  for(;;) {
    if(Time >= 0.0) {
      if(strcmp(p, "fs") == 0)  break;
      if(strcmp(p, "ps") == 0)  break;
      if(strcmp(p, "ns") == 0)  break;
      if(strcmp(p, "us") == 0)  break;
      if(strcmp(p, "ms") == 0)  break;
      if(strcmp(p, "sec") == 0) break;
      if(strcmp(p, "min") == 0) break;
      if(strcmp(p, "hr") == 0)  break;
    }
    t = QString::fromUtf8("§")  + QObject::tr("Error: Wrong time format in \"%1\". Use positive number with units").arg(Name)
            + " fs, ps, ns, us, ms, sec, min, hr.\n";
    return false;
  }

  t = QString::number(Time) + " " + QString(p);  // the space is mandatory !
  return true;
}

// #########################################################################
// Returns parameters for Verilog modules.
QString Verilog_Param(const QString Value)
{
  if(strtod(Value.latin1(), 0) != 0.0) {
    QString td = Value;
    if(!Verilog_Time(td, "parameter"))
      return Value;
    else
      return td;
  }
  else
    return Value;
}

// #########################################################################
// Creates and returns delay time for Verilog modules.
bool Verilog_Delay(QString& td, const QString& Name)
{
  if(strtod(td.latin1(), 0) != 0.0) {  // delay time property
    if(!Verilog_Time(td, Name))
      return false;    // time has not Verilog format
    td = " #" + td;
    return true;
  }
  else if(isalpha(td.latin1()[0])) {
    td = " #" + td;
    return true;
  }
  else {
    td = "";
    return true;
  }
}

// #########################################################################
// Checks and corrects a time (number & unit) according Verilog standard.
bool Verilog_Time(QString& t, const QString& Name)
{
  char *p;
  double Time = strtod(t.latin1(), &p);
  double factor = 1.0;
  while(*p == ' ') p++;
  for(;;) {
    if(Time >= 0.0) {
      if(strcmp(p, "fs") == 0) { factor = 1e-3; break; }
      if(strcmp(p, "ps") == 0) { factor = 1;  break; }
      if(strcmp(p, "ns") == 0) { factor = 1e3;  break; }
      if(strcmp(p, "us") == 0) { factor = 1e6;  break; }
      if(strcmp(p, "ms") == 0) { factor = 1e9;  break; }
      if(strcmp(p, "sec") == 0) { factor = 1e12; break; }
      if(strcmp(p, "min") == 0) { factor = 1e12*60; break; }
      if(strcmp(p, "hr") == 0)  { factor = 1e12*60*60; break; }
    }
    t = QString::fromUtf8("§")  + QObject::tr("Error: Wrong time format in \"%1\". Use positive number with units").arg(Name)
            + " fs, ps, ns, us, ms, sec, min, hr.\n";
    return false;
  }

  t = QString::number(Time*factor);
  return true;
}

// #########################################################################
bool checkVersion(QString& Line)
{
  QStringList sl = QStringList::split('.',PACKAGE_VERSION);
  QStringList ll = QStringList::split('.',Line);
  if (ll.count() != 3 || sl.count() != 3)
    return false;
  int sv = (int)sl.at(1).toLongLong()*10000+sl.at(2).toLongLong()*100;
  int lv = (int)ll.at(1).toLongLong()*10000+ll.at(2).toLongLong()*100;
  if(lv > sv) // wrong version number ? (only backward compatible)
    return false;
  return true;
}
// #########################################################################
// ##########                                                     ##########
// ##########            MOVE SOMEWHERE ELSE END                  ##########
// ##########                                                     ##########
// #########################################################################

int doNetlist(QString schematic, QString netlist)
{
  qDebug() << "*** try to load schematic :" << schematic;

  QFile file(schematic);  // save simulator messages
  if(file.open(QIODevice::ReadOnly)) {
    file.close();
  }
  else {
    fprintf(stderr, "Error: Could not load schematic %s\n", schematic.ascii());
    return 1;
  }

  // populate Modules list
  Module::registerModules ();

  // new schematic from file
  Schematic *sch = new Schematic(0, schematic);

  // load schematic file if possible
  if(!sch->loadDocument()) {
    fprintf(stderr, "Error: Could not load schematic %s\n", schematic.ascii());
    delete sch;
    return 1;
  }

  qDebug() << "*** try to write netlist  :" << netlist;

  QStringList Collect;

  QTextEdit *ErrText = new QTextEdit();  //dummy
  QFile NetlistFile;
  QTextStream   Stream;

  Collect.clear();  // clear list for NodeSets, SPICE components etc.

  NetlistFile.setName(netlist);
  if(!NetlistFile.open(QIODevice::WriteOnly)) {
    fprintf(stderr, "Error: Could not load netlist %s\n", netlist.ascii());
    return -1;
  }

  Stream.setDevice(&NetlistFile);
  int SimPorts = sch->prepareNetlist(Stream, Collect, ErrText);

  if(SimPorts < -5) {
    NetlistFile.close();
    fprintf(stderr, "Error: Could not prepare the netlist...\n");
    return 1;
  }

  // output NodeSets, SPICE simulations etc.
  for(QStringList::Iterator it = Collect.begin();
  it != Collect.end(); ++it) {
    // don't put library includes into netlist...
    if ((*it).right(4) != ".lst" &&
    (*it).right(5) != ".vhdl" &&
    (*it).right(4) != ".vhd" &&
    (*it).right(2) != ".v") {
      Stream << *it << '\n';
    }
  }

  Stream << '\n';

  QString SimTime = sch->createNetlist(Stream, SimPorts);

  NetlistFile.close();

  return 0;
}


// #########################################################################
// ##########                                                     ##########
// ##########                  Program Start                      ##########
// ##########                                                     ##########
// #########################################################################
int main(int argc, char *argv[])
{
  // apply default settings
  //SwieeSettings.font = QFont("Helvetica", 12); // original one
  SwieeSettings.font = QFont("SimSun", 9); // xuliang added for Chinese
  //SwieeSettings.largeFontSize = 16.0;
  SwieeSettings.largeFontSize = 12.0;
  SwieeSettings.maxUndo = 20;
  SwieeSettings.NodeWiring = 0;

  // initially center the application
  QApplication a(argc, argv);
  //a.setStyle(new Qtitan::RibbonStyle());
  QDesktopWidget *d = a.desktop();
  int w = d->width();
  int h = d->height();
  SwieeSettings.x = w/8;
  SwieeSettings.y = h/8;
  SwieeSettings.dx = w*3/4;
  SwieeSettings.dy = h*3/4;

  // check for relocation env variable
  char* var = getenv("SWIEEDIR");
  QDir SwieeDir;
  if (var!= NULL)
  {
      SwieeDir = QDir(QString(var));
      qDebug() << "SWIEEDIR set: " << SwieeDir.absolutePath();
  }
  else
  {
     QString SwieeApplicationPath = QCoreApplication::applicationDirPath();
     #ifdef __APPLE__
     SwieeDir = QDir(SwieeApplicationPath.section("/bin",0,0));
     #else
     SwieeDir = QDir(SwieeApplicationPath.replace("/bin",""));
     #endif

  }

  SwieeSettings.BinDir =      SwieeDir.absolutePath() + "/bin/";
  SwieeSettings.LangDir =     SwieeDir.canonicalPath() + "/share/swiee/lang/";
  SwieeSettings.LibDir =      SwieeDir.canonicalPath() + "/share/swiee/library/";
  SwieeSettings.OctaveDir =   SwieeDir.canonicalPath() + "/share/swiee/octave/";
  SwieeSettings.ExamplesDir = SwieeDir.canonicalPath() + "/share/swiee/docs/examples/";
  SwieeSettings.DocDir =      SwieeDir.canonicalPath() + "/share/swiee/docs/";

  SwieeSettings.Editor = "swiee";
  SwieeSettings.SwieeHomeDir.setPath(QDir::homeDirPath()+QDir::convertSeparators ("/.swiee"));
  SwieeSettings.SwieeWorkDir.setPath(SwieeSettings.SwieeHomeDir.canonicalPath());


  var = getenv("ADMSXMLBINDIR");
  if(var != NULL) {
      SwieeSettings.AdmsXmlBinDir.setPath(QString(var));
  }
  else {
      // default admsXml bindir same as Swiee
      QString admsExec;
#ifdef __MINGW32__
      admsExec = QDir::toNativeSeparators(SwieeSettings.BinDir+"/"+"admsXml.exe");
#else
      admsExec = QDir::toNativeSeparators(SwieeSettings.BinDir+"/"+"admsXml");
#endif
      QFile adms(admsExec);
      if(adms.exists())
        SwieeSettings.AdmsXmlBinDir.setPath(SwieeSettings.BinDir);
  }

  var = getenv("ASCOBINDIR");
  if(var != NULL)  {
      SwieeSettings.AscoBinDir.setPath(QString(var));
  }
  else  {
      // default ASCO bindir same as Swiee
      QString ascoExec;
#ifdef __MINGW32__
      ascoExec = QDir::toNativeSeparators(SwieeSettings.BinDir+"/"+"asco.exe");
#else
      ascoExec = QDir::toNativeSeparators(SwieeSettings.BinDir+"/"+"asco");
#endif
      QFile asco(ascoExec);
      if(asco.exists())
        SwieeSettings.AscoBinDir.setPath(SwieeSettings.BinDir);
  }

  var = getenv("OCTAVEBINDIR");
  if(var != NULL)  {
      SwieeSettings.OctaveBinDir.setPath(QString(var));
  }
  else  {
#ifdef __MINGW32__
      SwieeSettings.OctaveBinDir.setPath(QString("C:/Software/Octave-3.6.4/bin/"));
#else
      QFile octaveExec("/usr/bin/octave");
      if(octaveExec.exists())SwieeSettings.OctaveBinDir.setPath(QString("/usr/bin/"));
      QFile octaveExec1("/usr/local/bin/octave");
      if(octaveExec1.exists()) SwieeSettings.OctaveBinDir.setPath(QString("/usr/local/bin/"));
#endif
  }
  loadSettings();

  if(!SwieeSettings.BGColor.isValid())
    SwieeSettings.BGColor.setRgb(255, 250, 225);

//  SwieeSettings.BGColor.setRgb(220, 220, 220);

  // syntax highlighting
  if(!SwieeSettings.Comment.isValid())
    SwieeSettings.Comment = Qt::gray;
  if(!SwieeSettings.String.isValid())
    SwieeSettings.String = Qt::red;
  if(!SwieeSettings.Integer.isValid())
    SwieeSettings.Integer = Qt::blue;
  if(!SwieeSettings.Real.isValid())
    SwieeSettings.Real = Qt::darkMagenta;
  if(!SwieeSettings.Character.isValid())
    SwieeSettings.Character = Qt::magenta;
  if(!SwieeSettings.Type.isValid())
    SwieeSettings.Type = Qt::darkRed;
  if(!SwieeSettings.Attribute.isValid())
    SwieeSettings.Attribute = Qt::darkCyan;
  if(!SwieeSettings.Directive.isValid())
    SwieeSettings.Directive = Qt::darkCyan;
  if(!SwieeSettings.Task.isValid())
    SwieeSettings.Task = Qt::darkRed;


  a.setFont(SwieeSettings.font);

  // set codecs
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

  QTranslator tor( 0 );
  QString lang = SwieeSettings.Language;
  if(lang.isEmpty())
    lang = QTextCodec::locale();
  //tor.load( QString("swiee_") + lang, SwieeSettings.LangDir);
  tor.load( QString("swiee_") + lang, QString("./"));
  a.installTranslator( &tor );

  // This seems to be neccessary on a few system to make strtod()
  // work properly !???!
  setlocale (LC_NUMERIC, "C");

  QString schematic;
  QString netlist;

  QString operation;

  // simple command line parser
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      fprintf(stdout,
  "Usage: %s [OPTION]...\n\n"
  "  -h, --help     display this help and exit\n"
  "  -v, --version  display version information and exit\n"
  "  -n, --netlist  convert Swiee schematic into netlist\n"
  "  -i FILENAME    use file as input schematic\n"
  "  -o FILENAME    use file as output netlist\n"
  , argv[0]);
      return 0;
    }
    else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
      fprintf(stdout, "Swiee " PACKAGE_VERSION "\n");
      return 0;
    }
    else if (!strcmp(argv[i], "-n") || !strcmp(argv[i], "--netlist")) {
      operation = "netlist";
    }
    else if (!strcmp(argv[i], "-i")) {
      schematic = argv[++i];
    }
    else if (!strcmp(argv[i], "-o")) {
      netlist = argv[++i];
    }
    else {
      fprintf(stderr, "Error: Unknown option: %s\n", argv[i]);
      return -1;
    }
  }

  // check operation and its required arguments
  if (operation == "netlist") {
    if (schematic.isEmpty()) {
      fprintf(stderr, "Error: Expected input schematic file.\n");
      return -1;
    }
    if (netlist.isEmpty()) {
      fprintf(stderr, "Error: Expected output netlist file.\n");
      return -1;
    }
    // create netlist from schematic
    return doNetlist(schematic, netlist);
  }




  SwieeMain = new SwieeApp();

	//  SwieeMain->setFont(SwieeSettings.font);
	//  /*设置背景图片*/
	//QMainWindow{
	//border-image:url(":/images/background_mainwnd.png");
	//}

	//
  
  
  a.setStyleSheet ("QMainWindow {background-color:rgb(43, 60, 89)}");
	//a.setStyleSheet ("QDialog {background-color:rgb(250, 250, 0)}");
	//a.setPalette(QPalette(QColor("#F0F0F0")));

	//加载CSS文件  
	//QString strPath = QCoreApplication::applicationDirPath();  
	//qDebug()<<strPath;  
	//QString strCssFile = strPath + "/default.css";  
	//QFile fCss(strCssFile);  
	//if( !fCss.open(QFile::ReadOnly))  
	//{  
	//	qDebug("css File %s load false",strCssFile);  
	////        return;  
	//}  
	//QString strCssContent(fCss.readAll());  
	//a.setStyleSheet(strCssContent);  
	//fCss.close(); 



  a.setMainWidget(SwieeMain);
  qInstallMsgHandler(swieeMessageOutput);
  SwieeMain->show();
  int result = a.exec();
  //saveApplSettings(SwieeMain);
  return result;
}
