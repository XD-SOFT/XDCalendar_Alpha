#ifndef FILENUMWIDGET_H
#define FILENUMWIDGET_H

#include <QWidget>

class FileNumWidget:public QWidget
{
public:
   FileNumWidget(QWidget *parent = 0, int num = 0);
   int getFileNum(void){ return fileNum; }
   void setFileNum(int fileNum){ this->fileNum = fileNum;}
   void addFile(void) { fileNum = fileNum + 1; }
   void delFile(void) { fileNum = fileNum - 1; }

   void setDisplayFileNumEnabled(bool bEnabled);
   void paintEvent(QPaintEvent*);

private:
 int fileNum;

 bool m_bDisplayFileNumEnabled;
};



#endif // FILENUMWIDGET_H
