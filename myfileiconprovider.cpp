#include "myfileiconprovider.h"
#include <QFileInfo>
MyFileIconProvider::MyFileIconProvider()
{

}

QIcon MyFileIconProvider::icon (const QFileInfo& file) const
{
    auto suffix = file.suffix ();
    if (suffix == QStringLiteral ("doc") || suffix == QStringLiteral ("docx"))
    {
        return QIcon(QPixmap::fromImage (QImage(":/Icon/doc.png")));
    }
    else if (suffix == QStringLiteral ("jpg"))
    {
        return QIcon(QPixmap::fromImage (QImage(":/Icon/jpg.png")));
    }
    else if (suffix == QStringLiteral ("mp3"))
    {
        return QIcon(QPixmap::fromImage (QImage(":/Icon/mp3.png")));
    }
    else if (suffix == QStringLiteral ("rar"))
    {
        return QIcon(QPixmap::fromImage (QImage(":/Icon/rar.png")));
    }
    else if (suffix == QStringLiteral ("ppt") || suffix == QStringLiteral ("pptx"))
    {
        return QIcon(QPixmap::fromImage (QImage(":/Icon/ppt.png")));
    }
    else if (suffix == QStringLiteral ("pdf"))
    {
        return QIcon(QPixmap::fromImage (QImage(":/Icon/pdf.png")));
    }
    else if (suffix == QStringLiteral ("txt"))
    {
        return QIcon(QPixmap::fromImage (QImage(":/Icon/txt.png")));
    }
    else
        return QIcon(QPixmap::fromImage (QImage(":/Icon/file.png")));
}


