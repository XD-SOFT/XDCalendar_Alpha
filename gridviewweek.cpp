#include "gridviewweek.h"
#include "coursecellwidget.h"
#include "tools.h"
#include "config.h"
#include "teacher.h"
#include "term.h"
#include "lesson.h"
#include "mybutton.h"
#include "daywidget.h"
#include "ccu.h"
#include "dataclassinstancemanage.h"
#include "termdb.h"
//#include "loginstatuswidget.h"
#include "resfilesdb.h"
#include "messagedisplaywidget.h"
#include "lessonremarkdb.h"
#include <stdexcept>
#include <QPainter>
#include <QCalendarWidget>
#include "calendarwidget.h"
#include <QLabel>
#include <QEventLoop>
#include <QDebug>
#include "ccu.h"
#include "MainScreen.h"

const int DEFAULT_GRID_ROW = 8;

GridViewWeek::GridViewWeek(QWidget *parent) : QWidget(parent)
{
    dayNum = 0;
    currentDaySchem = false;
    mainLayout = new QVBoxLayout(this);
    weekLayout = new QHBoxLayout;
    for(int i=0;i<7;i++)
    {
        TransparentButton *button = new TransparentButton("",this);
        button->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Maximum);
        button->setObjectName ("WeekdayButton");
        button->setProperty ("selected", false);
        weekDayBtns[i]=button;
        weekDayBtns[i]->setOpacityEnabled(true);
        weekLayout->addWidget(button);
    }
//    connect (Arg::getInstance (), static_cast<void(Arg::*)(int)>(&Arg::currentWeekdayChanged), [&, this] (int weeknumber)
//    {
//        for (int i = 0; i < 7; ++i)
//        {
//            if (i == weeknumber)
//                weekDayBtns[i]->setProperty ("selected", true);
//            else
//                weekDayBtns[i]->setProperty ("selected", false);
//            weekDayBtns[i]->style ()->polish (weekDayBtns[i]);
//            weekDayBtns[i]->update ();
//        }
//    });
    weekDayBtns[static_cast<int>(QDate::currentDate ().dayOfWeek ()) - 1]->setProperty ("selected", true);
//    weekDayBtns[static_cast<int>(QDate::currentDate ().dayOfWeek ()) - 1]->setSelected(true);

    mainLayout->addLayout(weekLayout);
    grid = new QGridLayout;
    mainLayout->addLayout(grid);
    rearrange ();
}

GridViewWeek::GridViewWeek(QWidget *parent, int day, bool today) :
    QWidget(parent),
    dayNum(day),
    currentDaySchem(today)
{
    mainLayout = new QVBoxLayout(this);
    weekLayout = new QHBoxLayout;
    weekLayout->setSpacing(1);
    weekLayout->setContentsMargins(10,10,8,0);

    if(!currentDaySchem)
    {
        for(int i=0;i<7;i++)
        {
            TransparentButton *button = new TransparentButton("",this);
            weekDayBtns[i]=button;
            weekDayBtns[i]->installEventFilter(this);
            button->setObjectName("DayGridWeekButton"+QString::number(i));
            button->setMinimumHeight(45);
//            QString str;
//            str+="TransparentButton#DayGridWeekButton"+QString::number(i)+" {background-color:rgba(255,255,255,0.3)}";
//            str+="TransparentButton#DayGridWeekButton"+QString::number(i)+":hover {background-color:rgba(255,255,255,0.4)}";
//            button->setStyleSheet(str);
            weekLayout->addWidget(button);

            ///默认启用透明度,使用主窗口的前景透明度.
            weekDayBtns[i]->setOpacityEnabled(true);
        }
    }
    else
    {
        TransparentButton *button = new TransparentButton("",this);
        currentDay=button;
        button->setObjectName("DayGridWeekButton"+QString::number(0));
        button->setMinimumHeight(45);
//        QString str;
//        str+="TransparentButton#DayGridWeekButton"+QString::number(0)+" {background-color:rgba(255,255,255,0.3)}";
//        str+="TransparentButton#DayGridWeekButton"+QString::number(0)+":hover {background-color:rgba(255,255,255,0.4)}";
//        button->setStyleSheet(str);
        weekLayout->addWidget(button);

        ///默认启用透明度,使用主窗口的前景透明度.
        button->setOpacityEnabled(true);
        button->setSelected(true);
    }

    grid = new QGridLayout;
    mainLayout->addLayout(weekLayout);
    mainLayout->addLayout(grid);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(0);
    if(!currentDaySchem) rearrange();
    else rearrange (dayNum);
}

void GridViewWeek::rearrange(const QDate &curDate)
{
//    //重置文本显示属性.
//    for(int index = 0; index < 7; ++index) {
//        weekDayBtns[index]->setTextColorChangedEnabled(false);
//    }

    int nYear = curDate.year();
    int month = curDate.month();

    QDate todayDate = QDate::currentDate();
    int nTodayYear = todayDate.year();
    int nTodayMonth = todayDate.month();
    int nTodayDay = todayDate.day();

    qDebug() << "come in rearrange: " << currentDaySchem << endl;
    ///Mark2017.02.04,这里计算不对，出现负数代表是上一个月日期出现，程序没做处理,重新调整这里计算.
//    QDate curDate = QDate::currentDate();
    int day = curDate.day() - curDate.dayOfWeek() + Arg::weekBeginDay;
    int nDaysOfLastMonth = 0;
    //当月天数.
    int nMonthDaysNum = curDate.daysInMonth();

    // 处理上一个月现象.
    if(day <= 0) {
        QDate lastMonthDate = curDate.addMonths(-1);
        nDaysOfLastMonth = lastMonthDate.daysInMonth();
        qDebug() << "the last month day  number is:" << nDaysOfLastMonth << day;
    }

    for(int i=0;i<7;i++)
    {
        bool bExistNextMonth = false;

        int nMonthDay = ++day;
        // 处理上一个月需要显示的day.
        if(day <= 0) {
            nMonthDay = nDaysOfLastMonth + day;
            qDebug() << "the last month day is:" << nMonthDay;
        }
        else if(nMonthDay > nMonthDaysNum) { //处理下一个月现象.
            nMonthDay = nMonthDay % nMonthDaysNum;
            bExistNextMonth = true;
        }

        QString sDate = Arg::label_text[(Arg::weekBeginDay+i)%7]  + "\n" + QString::number(nMonthDay);
        //            qDebug()<<"i: "<<i<<endl;

        TransparentButton *button = weekDayBtns[i];

        if((day <= 0) || (bExistNextMonth)) {
            //上一个月和下一个月的日期文本变色处理.
            button->setTextColorChangedEnabled(true);
        }
        else {
            button->setTextColorChangedEnabled(false);
        }

        qDebug()<< "text:" << sDate;
        button->setText(sDate);
        button->setVisible(Arg::showWeekLabel);

        bool bSelected = false;

        //看是否是今天.变色处理.
        if((nTodayYear == nYear) && (nTodayMonth == month)) {
            if(nTodayDay == nMonthDay) {
                bSelected = true;
            }
        }

        button->setProperty("selected", bSelected);
    }
    //        qDebug() << "finish" <<endl;
}

void GridViewWeek::rearrange(int i)
{
    //        qDebug()<<"rearrange "<<i<<endl;
    TransparentButton *button = currentDay;
    button->setText(Arg::label_text[i] + "\n" + QDate::currentDate().toString("d"));
    button->setVisible(Arg::showWeekLabel);
}

void GridViewWeek::setBackground (const QImage& image)
{
    mBackgroundImage = image;
    update ();
}

void GridViewWeek::setBackground (double opacity)
{
    mOpacity = opacity;

    if(!currentDaySchem)
    {
        for(int i=0;i<7;i++)
        {
            weekDayBtns[i]->setOpacity(opacity);
        }
    }
//    else
//    {
//        TransparentButton *button = new TransparentButton("",this);
//        currentDay=button;
//        button->setObjectName("DayGridWeekButton"+QString::number(0));
//        button->setMinimumHeight(45);
//        QString str;
//        str+="TransparentButton#DayGridWeekButton"+QString::number(0)+" {background-color:rgba(255,255,255,0.3)}";
//        str+="TransparentButton#DayGridWeekButton"+QString::number(0)+":hover {background-color:rgba(255,255,255,0.4)}";
//        button->setStyleSheet(str);
//        weekLayout->addWidget(button);

//        ///默认启用透明度,使用主窗口的前景透明度.
//        button->setOpacityEnabled(true);
//    }

    update();
}

void GridViewWeek::paintEvent (QPaintEvent* ev)
{
    QPainter p (this);
    //paint background
    p.save ();
    //paint opatity
    p.fillRect (0, 0, this->width (), this->height (), QBrush (QColor (255, 255, 255, static_cast<int>(mOpacity * 255))));
    p.setCompositionMode (QPainter::CompositionMode_Multiply);
    if (!mBackgroundImage.isNull ())
    {
        p.drawImage (QRect(0, 0, this->width (), this->height ()), mBackgroundImage);
    }
    else
    {
    }
    p.restore ();
    QWidget::paintEvent (ev);
}

DayGrid::DayGrid(QWidget *parent):
    GridViewWeek(parent)
{
    x_maxlen=6;
    y_maxlen=7;
    x_len=y_len=0;
    for(int i=0;i<x_maxlen;i++)
        for(int j=0;j<y_maxlen;j++)
        {
            Days[i][j] = new DayWidget(this);
            grid->addWidget(Days[i][j],i,j);
            setMinimumHeight(Days[i][j]->height()*x_maxlen+20);
            setMinimumWidth(Days[i][j]->width()*y_maxlen+20);
            Days[i][j]->setVisible(false);
        }
    setMonth(QDateTime::currentDateTime().date());
}

void DayGrid::setMonth(const QDate &date_data)
{
    rearrange ();
    int y=date_data.year();
    int m=date_data.month();
    date = date_data;
    QDate date_(y,m,1);
    int q=(date_.dayOfWeek()-Arg::weekBeginDay);
    if(q<0)q+=7;
    QDate startDate = date_.addDays(1-q);
    x_len=(date_.daysInMonth()+q+5)/7;
    y_len=y_maxlen;
    for(int i=0;i<y_maxlen*x_len;i++)
    {
        DayWidget **now = &Days[i/y_maxlen][i%y_maxlen];
        (*now)->setDate(startDate);
        (*now)->setVisible(true);
        startDate=startDate.addDays(1);
    }
    for(int i=y_maxlen*x_len;i<y_maxlen*x_maxlen;i++)
    {
        DayWidget **now = &Days[i/y_maxlen][i%y_maxlen];
        (*now)->setVisible(false);
    }
}

void DayGrid::reset(){setMonth(date);}

CourseGrid::CourseGrid (QWidget* parent):
    GridViewWeek (parent)
{
//    headerLayout = new QVBoxLayout ();
//    headerLayout->setSpacing (0);
//    headerLayout->setMargin (0);
    ///Mark,这里为了不做大的改动，暂时这么处理，重构不再分成GridViewWeek与CourseGrid.
    QLayout* parentLayout = GridViewWeek::layout ();
    weekHeaderLayout = static_cast<QLayout*>(parentLayout->takeAt (0));
    grid = static_cast<QGridLayout*>(parentLayout->takeAt (0));
    delete weekHeaderLayout;
    weekHeaderLayout = Q_NULLPTR;
    delete parentLayout;
    parentLayout = Q_NULLPTR;

    //月份显示.
    /*TransparentButton **/placeHolderButton = new TransparentButton (this);
    placeHolderButton->installEventFilter(this);
    connect(placeHolderButton, &TransparentButton::clicked, this, &CourseGrid::showDateWidget);
    placeHolderButton->setText (QDate::currentDate ().toString ("MMMM"));
//    placeHolderButton->setOpacity (0.2);
    placeHolderButton->setObjectName ("PlaceHolderButton");
    placeHolderButton->setFixedWidth(60);
    grid->addWidget(placeHolderButton, 0, 0);

    ///Test 2017.02.18.
//    QGridLayout* mainLayout = new QGridLayout (this);
//    mainLayout->addWidget(placeHolderButton, 0, 0);
//    mainLayout->setSpacing (0);
//    mainLayout->setMargin (0);
//    mainLayout->addLayout(weekHeaderLayout, 0, 1);
    ///Test 2017.02.18.
//    mainLayout->addLayout(headerLayout, 1, 0);
//    mainLayout->addLayout(courseGridLayout, 1, 1);
//    mainLayout->addLayout(courseGridLayout, 1, 0, 1, 2);
//    grid->setContentsMargins(0, 0, 0, 0);
//    grid->setMargin(0);
////    grid->setColumnMinimumWidth(0, placeHolderButton->width());
//    mainLayout->setColumnMinimumWidth (0, 46);
//    mainLayout->setRowMinimumHeight (0, 50);
    ///Mark，这里暂时这么处理，以后这些ui部件归到一个类上.
    for(int nBtn = 0; nBtn < 7; ++nBtn) {
        grid->addWidget(weekDayBtns[nBtn], 0, nBtn + 1);
        weekDayBtns[nBtn]->installEventFilter(this);
    }

    grid->setSpacing(0);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setColumnMinimumWidth (0, 46);
    grid->setRowMinimumHeight (0, 50);
    setLayout(grid);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    //    mainLayout->setColumnStretch (0, 1);
//    mainLayout->setColumnStretch (1, 1000);


    m_nBreakNum = 0;

//    initializeDefaultClassGrid();

    ///暂时测试写这里，数据与ui整合还需要仔细调整.
    Arg *pArgInstance = Arg::getInstance();
    connect(pArgInstance, &Arg::termLoadFinished, this, &CourseGrid::termLoadFinished);
    connect(CCU::ccu, SIGNAL(changeEdit()), this, SLOT(changeToEdit()));
    connect(CCU::ccu, SIGNAL(changeToStandardMode()), this, SLOT(changeToStandardMode()));

    ///Mark 这块处理暂时放着个类中，重构时候再设计.
    DataClassInstanceManage *pDataInstanceMgr = DataClassInstanceManage::getInstance();
    TermDB *pTermDB = pDataInstanceMgr->getTemDBPtr();
    connect(pTermDB, &TermDB::queryTermByDateFinished, this, &CourseGrid::queryTermByDateFinishied);

    ResFilesDB *pResFileDB = pDataInstanceMgr->getResFilesDBPtr();
    connect(pResFileDB, SIGNAL(findFinish(const QJsonObject&)), this, SLOT(getFilesState(const QJsonObject&)));

    //连接备注数据库.
    LessonRemarkDB *pLesRemarkDB = pDataInstanceMgr->getLessonRemarkDBPtr();
    connect(pLesRemarkDB, &LessonRemarkDB::findFinish, this, &CourseGrid::setLessonRemark);


    mLinkedTerm = Arg::currentTerm();

    setup();

    mapWeekLessons(QDate::currentDate());

    resize(sizeHint());
}

CourseGrid::~CourseGrid()
{
    if(m_pCalendarWgt != Q_NULLPTR) {
        delete m_pCalendarWgt;
        m_pCalendarWgt = Q_NULLPTR;
    }
}

void CourseGrid::setup()
{
    ///Mark,这个函数以后还需要分函数，现在先实现功能.
#if 0
    auto breaks = mLinkedTerm->getBreaks ();
    int ptr = 0;
    int cnt = 0;
#endif
    if (mLinkedTerm == nullptr)
        return;
//    while (headerLayout->count ())
//        headerLayout->takeAt (0);

    QMap<int, QMap<int, QPair<int, int> > > typeSectionsMap;
    mLinkedTerm->getSections(typeSectionsMap);
    QList<int> keyList = typeSectionsMap.keys();
    int nSecTypeNum = keyList.size();

    ///Mark2017.02.21，如果作息时间表没有，则使用默认的.
    if(nSecTypeNum <= 0) {
        return;
    }

//    Q_ASSERT_X(nSecTypeNum > 0, "CourseGrid::setup", "The Section type equals to zero");

    m_nBreakNum = nSecTypeNum - 1;
    int nSectionNum = 0;

    for(int keyPos = 0; keyPos < nSecTypeNum; ++keyPos) {
        int nKey = keyList.at(keyPos);
        QMap<int, QPair<int, int> > sectionMap = typeSectionsMap.value(nKey);
        QList<QPair<int, int> > sectionList = sectionMap.values();
        nSectionNum += sectionList.size();
    }

    cellWidgets.resize(nSectionNum);
    int weekDayNum = static_cast<int>(daysPerWeek);

    for(int row = 0; row < nSectionNum; ++row) {
        cellWidgets[row].resize(weekDayNum);
    }

    m_breakWidgetList.clear();

    ///Mark 2017.02.17,暂时文本程序里写死处理，以后可以让用户设置每个分割的文本，比如早饭，课间.
    qDebug() << "the linked term section type number is:" << nSecTypeNum;
    struct HeaderLb{
        bool bBreak; //false, 课程，true,break.
        QString sText;
    };

    QList<HeaderLb> headerList;
    HeaderLb header;
//    QStringList textList;
    int nRow = 1; //grid布局里总的行数.
    int nCourseRow = 0;
//    QList<QWidget*> courseWgtList; //暂存加入的CourseCellWidget对象，一行一行加入.

    //有早读,暂时按照现行数据库表设计做，不考虑用户自己设计.
    if(typeSectionsMap.contains(0)) {
        header.bBreak = false;
        header.sText.append(tr("早读"));
        headerList.append(header);

        header.bBreak = true;
        header.sText = tr(" ");
        headerList.append(header);

        QMap<int, QPair<int, int> > section = typeSectionsMap.value(0);
        //早读不能大于1节,所以直接取第0个.
        int nSectionID = section.keys().at(0);
        QPair<int, int> timePair = section.value(nSectionID);

        for (int col = 1; col <= weekDayNum; ++col) {
            CourseCellWidget *pCell = addCourseCell(nSectionID, nRow, col, timePair.first, timePair.second);
            cellWidgets[nCourseRow][col - 1] = pCell;
        }

        ++nRow;
        ++nCourseRow;

        //早餐空白.
        BreakWidget *pWgt = new BreakWidget(tr("早餐"));
        pWgt->setFixedHeight(20);
        grid->addWidget(pWgt, nRow, 1, 1, weekDayNum);
        m_breakWidgetList.append(pWgt);

        ++nRow;
    }

    int secNum = 1;
    //上午.
    if(typeSectionsMap.contains(1)) {
        QMap<int, QPair<int, int> > sectionMap = typeSectionsMap.value(1);
        int nCourseNum = sectionMap.size();
        QList<int> secIDList = sectionMap.keys();

        for(int nNum = 0; nNum < nCourseNum; ++nNum) {
            header.bBreak = false;
            header.sText = tr("第%1节").arg(secNum);
            headerList.append(header);

            ++secNum;
            int nSectionID = secIDList.at(nNum);
            QPair<int, int> secTimePair = sectionMap.value(nSectionID);

            for (int col = 1; col <= weekDayNum; ++col) {
                CourseCellWidget *pCell = addCourseCell(nSectionID, nRow, col, secTimePair.first, secTimePair.second);
                cellWidgets[nCourseRow][col - 1] = pCell;
            }

            ++nRow;
            ++nCourseRow;
        }

        BreakWidget *pWgt = new BreakWidget(tr("午休"));
        pWgt->setFixedHeight(20);
        grid->addWidget(pWgt, nRow, 1, 1, weekDayNum);

        m_breakWidgetList.append(pWgt);

        header.bBreak = true;
        header.sText = tr(" ");
        headerList.append(header);

        ++nRow;
    }

    //下午.
    if(typeSectionsMap.contains(2)) {
        QMap<int, QPair<int, int> > sectionMap = typeSectionsMap.value(2);
        int nCourseNum = sectionMap.size();
        QList<int> secIDList = sectionMap.keys();

        for(int nNum = 0; nNum < nCourseNum; ++nNum) {
            header.bBreak = false;
            header.sText = tr("第%1节").arg(secNum);
            headerList.append(header);

            ++secNum;
            int nSectionID = secIDList.at(nNum);
            QPair<int, int> secTimePair = sectionMap.value(nSectionID);

            for (int col = 1; col <= weekDayNum; ++col) {
                CourseCellWidget *pCell = addCourseCell(nSectionID, nRow, col, secTimePair.first, secTimePair.second);
                cellWidgets[nCourseRow][col - 1] = pCell;
            }

            ++nRow;
            ++nCourseRow;
        }
    }

    //含有晚自习处理.
    if(typeSectionsMap.contains(3)) {
        header.bBreak = true;
        header.sText = tr(" ");
        headerList.append(header);

        BreakWidget *pWgt = new BreakWidget(tr("晚餐"));
        pWgt->setFixedHeight(20);
        grid->addWidget(pWgt, nRow, 1, 1, weekDayNum);
        m_breakWidgetList.append(pWgt);

        ++nRow;

        QMap<int, QPair<int, int> > sectionMap = typeSectionsMap.value(3);
        int nCourseNum = sectionMap.size();
        QList<int> secIDList = sectionMap.keys();

        for(int nNum = 0; nNum < nCourseNum; ++nNum) {
            header.bBreak = false;
            header.sText = tr("第%1节").arg(secNum);
            headerList.append(header);

            ++secNum;

            int nSectionID = secIDList.at(nNum);
            QPair<int, int> secTimePair = sectionMap.value(nSectionID);

            for (int col = 1; col <= weekDayNum; ++col) {
                CourseCellWidget *pCell = addCourseCell(nSectionID, nRow, col, secTimePair.first, secTimePair.second);
                cellWidgets[nCourseRow][col - 1] = pCell;
            }

            ++nRow;
            ++nCourseRow;
        }
    }

  ///左侧课程文本显示.Replace by new logic.
    for(int nRow = 0; nRow < headerList.size(); ++nRow) {
        HeaderLb header = headerList.at(nRow);
        TransparentButton *btn = new TransparentButton(header.sText, this);
        btn->setOpacity(m_fOpacity);
        btn->installEventFilter(this);
        btn->setFixedWidth(placeHolderButton->width());

        if(!header.bBreak) {
//            btn->setFixedHeight(cellWidgets.first().first()->height());
            btn->setObjectName ("SectionNumberButton");
        }
        else {
            btn->setFixedHeight(m_breakWidgetList.at(0)->height());
            btn->setObjectName("BreakSectionButton");
        }

        grid->addWidget(btn, nRow + 1, 0);
        m_textDisplayWgtList.append(btn);
    }

    emit requestResizeMainWindow();

    ///Mark TODO 2017.02.18,课程还没有对应上，考虑好一个接口.
    //link lesson.
//    QList<Lesson*> &lessonList =  mLinkedTerm->lessonWeek(0);
//    for(int nWeekDay = 0; nWeekDay < daysPerWeek; ++nWeekDay) {

//    }

#if 0
    for (int i = 1; i <= mLinkedTerm->getSectionNumber (); ++i)
    {
        auto btn = new TransparentButton(QString("第%1节").arg(int2localeChar(i - cnt)), this);
        if (ptr < breaks.size () && i == breaks[ptr].first)
        {
            ++cnt;
            btn->setObjectName ("BreakSectionButton");
//            btn->setObjectName ("SectionNumberButton");

            btn->setFixedHeight (Const::MainScreen::breakHeight);
            btn->setText (breaks[ptr].second);
            ++ptr;
        }
        else
        {
            btn->setObjectName ("SectionNumberButton");
        }
        btn->setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Expanding);
        headerLayout->addWidget (btn, 180);
    }

    for (const auto& list: cellWidgets)
    {
        for (const auto& w: list)
            delete w;
    }
    cellWidgets.clear ();
    while (grid->count ())
        grid->takeAt (0);

    cellWidgets.resize (mLinkedTerm->getSectionNumber ());
    for (auto& line: cellWidgets)
    {
        line.resize (daysPerWeek);
    }
    //resize grid.

    for (int i = 0; i < mLinkedTerm->getSectionNumber (); ++i)
    {
        for (int j = 0; j < static_cast<int>(daysPerWeek); ++j)
        {
            CourseCellWidget* cell = new CourseCellWidget (this);
            connect(cell, &CourseCellWidget::requestShowLessons, this, &CourseGrid::requestShowLessons);

            cell->setObjectName ("class");
            cell->setStyleSheet("border-right: 0px solid black; border-bottom: 0px solid black; margin: 0;");
            cell->style ()->polish (cell);
            cellWidgets[i][j] = cell;
            cell->setSecIndex(i);
            cell->setDayIndex(j);
            cell->reset ();
            grid->addWidget (cell, i, j);
        }
//        grid->setRowStretch (i, 180);
    }
    for (auto b: breaks)
    {
        for (int j = 0; j < static_cast<int>(daysPerWeek); ++j)
        {
            cellWidgets[b.first - 1][j]->setObjectName ("Break");
            cellWidgets[b.first - 1][j]->setFixedHeight (Const::MainScreen::breakHeight);
            cellWidgets[b.first - 1][j]->style ()->polish (cellWidgets[b.first - 1][j]);
        }
//        grid->setRowStretch (b.first - 1, 60);
    }

    update ();
#endif

}

void CourseGrid::initializeDefaultClassGrid()
{
//    int weekDayNum = static_cast<int>(daysPerWeek);
//    cellWidgets.resize(DEFAULT_GRID_ROW  - 1); //一个默认午休.

//    for(int row = 0; row < DEFAULT_GRID_ROW; ++row) {
//        QString sText;
//        if(row != 4) {
//            sText.append(tr("第 %1 节").arg(row + 1));
//        }
//        else {
//            sText.append(tr("午 休"));
//        }

//        if(row < 4) {
//            cellWidgets[row].resize(weekDayNum);
//        }
//        else if(row > 4){
//            cellWidgets[row - 1].resize(weekDayNum);
//        }

//        for(int col = 0; col <= weekDayNum; ++col) {
//            if((col != 0) && (row != 4)) {
//                int sectionID = row + 1;

//                //暂时这么处理.
//                int startTime = 0;
//                int endTime = 0;
//                CourseCellWidget *cell = addCourseCell(sectionID, row, col, startTime, endTime);

//                int cellWidgetIndex = row;

//                if(row > 4) {
//                    cellWidgetIndex -= 1;
//                }

//                cellWidgets[cellWidgetIndex][col - 1] = cell;
//            }
//            else {
//                TransparentButton *btn = new TransparentButton(sText, this);
//                btn->setObjectName("BreakSectionButton");
//                btn->setFixedWidth(20);
//                grid->addWidget(btn, row, 0);
//                m_breakWidgetList.append(btn);
//            }
//        }
    //    }
}

void CourseGrid::setCurrentTerm(Term *pTerm)
{
    if(pTerm != Q_NULLPTR) {
        //先判断是否需要重置ui.
        Term *pLastTerm = mLinkedTerm;
        mLinkedTerm = pTerm;

        if(pLastTerm != Q_NULLPTR) {
            //section 类型个数相同，则继续判断, 不同则重置.
            if(mLinkedTerm->getSectionTypeNumber() == pLastTerm->getSectionTypeNumber()) {
                QMap<int, QMap<int, QPair<int, int> > > lastTermSecMap;
                QMap<int, QMap<int, QPair<int, int> > > curTermSecMap;
                pLastTerm->getSections(lastTermSecMap);
                mLinkedTerm->getSections(curTermSecMap);
                int nTypeNum = mLinkedTerm->getSectionTypeNumber();
                bool bSectionIsSame = true;

                //section type 目前从0开始，0，1，2...所以直接这样表示.
                for(int type = 0; type < nTypeNum; ++type) {
                    int nLastSecNum = lastTermSecMap.value(type).size();
                    int nCurSecNum = curTermSecMap.value(type).size();

                    bSectionIsSame = (nLastSecNum == nCurSecNum) ? true : false;

                    if(!bSectionIsSame) {
                        break;
                    }
                }

                if(bSectionIsSame) {
                     return;
                }
            }
        }

        reset();

        mapWeekLessons(m_curDisplayDate);
    }
    else { //如果学期为空，要做清除处理.
        mLinkedTerm = Q_NULLPTR;
    }
}

void CourseGrid::showDateWidget()
{
    if(Q_NULLPTR == m_pCalendarWgt) {
        m_pCalendarWgt = new CalenderSelectWidget;
        connect(m_pCalendarWgt, &CalenderSelectWidget::dateSelected, this, &CourseGrid::mapWeekLessons);
        //m_pCalendarWgt->resize(330, 230);
        m_pCalendarWgt->resize(330 +20 , 230 + 60);
    }

    if(m_curDisplayDate.isNull()) {
        m_curDisplayDate = QDate::currentDate();
    }

    QPoint movePt = mapToGlobal(placeHolderButton->pos()) - QPoint(30, 30);
    m_pCalendarWgt->move(movePt);
    m_pCalendarWgt->setCurrentDate(m_curDisplayDate.year(), m_curDisplayDate.month());
    m_pCalendarWgt->setWindowOpacity(0.9);
    m_pCalendarWgt->show();
}

void CourseGrid::changeToEdit()
{
    //如果当前日期不在学期内，则不能显示课程编辑.
    Term *pCurTerm = Arg::currentTerm();
    int nTermWeekNum = pCurTerm->getDateWeekNumber(m_curDisplayDate);

    if(0 == nTermWeekNum) {
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("日期不处于学期中，请先选择处于学期中的日期再进行编辑。"));
#else
        MessageDisplayWidget::showMessage(tr("教师客户端"), tr("日期不处于学期中，请先选择处于学期中的日期再进行编辑。"));
#endif
        CCU::ccu->changeViewMode(ViewMode::Standard);

        return;
    }

    for(int row = 0; row < cellWidgets.size(); ++row) { //section
        for(int col = 0; col < cellWidgets[row].size(); ++col ) { //weekday
            if(!cellWidgets[row][col]->isEditStatus()) {
                cellWidgets[row][col]->setEdit(true);
            }
        }
    }
}

void CourseGrid::changeToStandardMode()
{
    for(int row = 0; row < cellWidgets.size(); ++row) { //section
        for(int col = 0; col < cellWidgets[row].size(); ++col ) { //weekday
            if(cellWidgets[row][col]->isEditStatus()) {
                cellWidgets[row][col]->setEdit(false);
            }
        }
    }
}

void CourseGrid::queryTermByDateFinishied(const QJsonObject &jsonObj)
{
    qDebug() << "the query Term by date json date:" << jsonObj;

    if(jsonObj["status"] == "false") {
        m_pEvLoop->exit(-1);

        return;
    }

    //先切换回正常模式.
    Arg::viewMode = ViewMode::Standard;

    ///Mark Todo.
    Arg::nw->changeTerm(jsonObj);
}

void CourseGrid::link(Term* term, int weekNumber)
{
    if (term != nullptr && (weekNumber < 0 || weekNumber >= term->getTermWeeksNumber()))
        return;

//    disconnect (term, SIGNAL(changed()), this, SLOT(reset()));
    mLinkedTerm = term;
    mWeekNumber = weekNumber;

    if (mLinkedTerm != nullptr)
    {
//        connect (term, SIGNAL(changed()), this, SLOT(reset()));
        reset ();
    }
}

void CourseGrid::unlink()
{
    link (nullptr, -1);
}

void CourseGrid::reset ()
{
    if (mLinkedTerm == nullptr) {
        return;
    }

    //记得清空grid布局.
    for(int nPos = 0; nPos < m_breakWidgetList.size(); ++nPos) {
        QWidget *pWgt = m_breakWidgetList.at(nPos);
        grid->removeWidget(pWgt);
        delete pWgt;
        pWgt = Q_NULLPTR;
    }

    m_breakWidgetList.clear();

    for(int nRow =0; nRow < cellWidgets.size(); ++nRow) {
        for(int nCol = 0; nCol < cellWidgets[nRow].size(); ++nCol) {
            QWidget *pWgt = cellWidgets[nRow][nCol];
            grid->removeWidget(pWgt);
            delete pWgt;
            pWgt = Q_NULLPTR;
        }
    }

    cellWidgets.clear();

    for(int index = 0; index < m_textDisplayWgtList.size(); ++index) {
        QWidget *pWgt = m_textDisplayWgtList.at(index);
        grid->removeWidget(pWgt);

        delete pWgt;
        pWgt = Q_NULLPTR;
    }

    m_textDisplayWgtList.clear();

    m_nBreakNum = 0;  

    setup ();

#if 0
    qDebug()<<"coursegrid reset"<<endl;
    for (const auto& list: cellWidgets)
        for (const auto& w: list)
            static_cast<CourseCellWidget*> (w)->unlink ();

    qDebug()<<"cellWidgets size: "<<cellWidgets.size()<<" sectionNum: "<<mLinkedTerm->getSectionNumber()<<endl;
    if (cellWidgets.size () != mLinkedTerm->getSectionNumber ())
    {
        qDebug()<<"setup"<<endl;
        setup ();
    }

    qDebug()<<"mWeekNumber: "<<mWeekNumber<<endl;

    QList<Lesson*> &lessonList =  mLinkedTerm->lessonWeek(mWeekNumber);

    if(!lessonList.isEmpty()) {
        for (const auto& lesson : lessonList)
        {
            //int i = lesson->section () - 1, j = static_cast<int>(lesson->day ()) - 1;
            //int i = lesson->section (), j = static_cast<int>(lesson->day ());

            qDebug()<<"lesson section: "<<lesson->section()<<endl;
            int i = lesson->section () - 1;
            int j = 0;
            if(lesson->getWeekId() == -1) //从本地加载
            {
                j = static_cast<int>(lesson->day ()) - 1;
            }
            else //从网络加载
            {
                j = lesson->getWeekId() - 1;
            }

//            qDebug()<<"cellWidgets size: "<<cellWidgets.size()<<endl;
//            qDebug()<<"i: "<<i<<" j: "<<j<<endl;
//    //        qDebug()<<"cellWidget  j size: "<<cellWidgets[i].size()<<endl;

            if(i >= 0 && i < mLinkedTerm->getSectionNumber () && j >= 0 && j < daysPerWeek)
            {
                static_cast<CourseCellWidget*>(cellWidgets[i][j])->link (lesson);
            }
            else
            {
                qDebug()<<"数据库作息详细表和课程详细表数据冲突"<<endl;
                continue;
            }

        }
    }
#endif
}

bool CourseGrid::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress) {
        if((m_pCalendarWgt != Q_NULLPTR) && (m_pCalendarWgt->isVisible())) {
            m_pCalendarWgt->hide();
        }
    }

    return QWidget::eventFilter(watched, event);
}

CourseCellWidget *CourseGrid::addCourseCell(int nSectionID, int nRow, int nWeek, int nStartTime,
                                            int nEndTime)
{
    CourseCellWidget* cell = new CourseCellWidget(this);
    connect(cell, &CourseCellWidget::requestShowLessons, this, &CourseGrid::requestShowLessons);
    cell->setOpacity(m_fOpacity);
    cell->setObjectName ("class");
    cell->setStyleSheet("border-right: 0px solid black; border-bottom: 0px solid black; margin: 0;");
//    cell->style ()->polish (cell);
//    cellWidgets[i][j] = cell;
    ///Mark 2017.02.21,这里重构的时候考虑是否直接把从数据库得到的数据以字符串传过来，现在暂时不动已经完成的.
    int nHours = nStartTime / 3600;
    int nRemainTime = nStartTime % 3600;
    int nMinutes = nRemainTime / 60;
    QString sStartTime = QString("%1:%2").arg(nHours).arg(nMinutes);
    cell->setStartTime(sStartTime);
    cell->setStartTime(nStartTime);

    nHours = nEndTime / 3600;
    nRemainTime = nEndTime % 3600;
    nMinutes = nRemainTime / 60;
    QString sEndTime = QString("%1:%2").arg(nHours).arg(nMinutes);
    cell->setEndTime(sEndTime);
    cell->setEndTime(nEndTime);

    cell->setSecIndex(nSectionID);
//    qDebug() << "the cell section id is:" << nSectionID;

    cell->setDayIndex(nWeek);
    cell->reset ();
    grid->addWidget(cell, nRow, nWeek);

    return cell;
}

void CourseGrid::showOneDay (int weekday)
{
    qDebug() << "one day the current weekday is" << weekday;

    showingDay = weekday;
    if (weekday < 0 || weekday >= 7)
    {

        auto showAll = [&] (auto& container)
        {
            for (int i = 0; i < 7; ++i)
            {
                container[i]->show ();
            }
        };
        showAll(weekDayBtns);
        for (auto& list: cellWidgets)
        {
            showAll (list);
        }
    }
    else
    {
        auto hideByWeekday = [&] (auto& container)
        {
            for (int i = 0; i < 7; ++i)
            {
                if (i == weekday)
                    continue;
                container[i]->hide ();
            }
        };
        hideByWeekday(weekDayBtns);
        for (auto& list: cellWidgets)
        {
            hideByWeekday (list);
        }
    }
}

void CourseGrid::showAllDay()
{
    showOneDay(-1);
}

void CourseGrid::setBackground (double opacity)
{
    m_fOpacity = opacity;

    GridViewWeek::setBackground (opacity);

    for (auto& list: cellWidgets)
    {
        for (auto& c: list)
        {
            dynamic_cast<CourseCellWidget*>(c)->setOpacity (opacity);
        }
    }

    for(auto &widget : m_breakWidgetList) {
        dynamic_cast<BreakWidget*>(widget)->setBackgroundAlpha(opacity);
    }
}

QSize CourseGrid::sizeHint () const
{
#ifdef USE_LIZEYAN_SIZE
    bool oneday = showingDay >= 0 && showingDay < 7;
    QSize ret = {cellWidgets.front ().front ()->sizeHint ().width () * (oneday?1:7) + 40,
                 cellWidgets.front ().front ()->sizeHint ().height () * (mLinkedTerm?mLinkedTerm->getSectionNumber (): 6) + (Const::MainScreen::breakHeight - 65) * (mLinkedTerm?mLinkedTerm->getBreaks ().size (): 2) + 60};
    return ret;
#else
    QSize size(800, 600);

    if(cellWidgets.size() > 0) {
        bool oneday = showingDay >= 0 && showingDay < 7;
        CourseCellWidget *pWgt = cellWidgets.front().front();
        QSize cellSize = (pWgt != Q_NULLPTR) ? pWgt->sizeHint() : QSize(90, 60);
        size.setWidth(cellSize.width() * (oneday ? 1 : 7) + placeHolderButton->width()/* + 46*/);
        size.setHeight(cellSize.height() * (mLinkedTerm ? cellWidgets.size() : 6)
                       + 20 * (mLinkedTerm ? m_nBreakNum : 1) + weekDayBtns[0]->height()/*+ weekHeaderLayout->sizeHint().height()*/);
    }
    //    qDebug() << "the mLinedTerm is:" << mLinkedTerm->getSectionNumber () << mLinkedTerm->getBreaks ().size () << size << cellSize.width();

    return size;
#endif
}

void CourseGrid::getFilesState(const QJsonObject & json)
{
    if(json["status"].toString() == "false"){
        qDebug()<<"api status is false"<<endl;
        return;
    }

    Lesson* les = Arg::nw->getFilesState(json, m_weekLessonVec);
    //les是Arg::nw->getFilesState里面返回的m_weekLessonVec里面对应的Lesson指针,不过目前没啥用
    if(les != nullptr){
        qDebug()<<"callback pointer lesson: "<<les->getLessonDetailId()<<endl;
    }

//
//    if(m_resDetailIDCourseCellWgtHash.contains(detailID)) {
//        CourseCellWidget *pCellWgt = m_resDetailIDCourseCellWgtHash.value(detailID);
//        pCellWgt->reset();

//        m_resDetailIDCourseCellWgtHash.remove(detailID);
//    }

    ///Add by BiXiaoxia,添加包含这节课程的widget刷新.
    for(int row = 0; row < cellWidgets.size(); ++row) {
        for(int col = 0; col < cellWidgets[row].size(); ++col) {
            CourseCellWidget *pCellWgt = cellWidgets[row][col];

            if(pCellWgt->getLesson() == les) {
                pCellWgt->reset();
            }
        }
    }
/*
    //    getNWDate = new GetDataNW();
    if(m_resDetailIDDateHash.contains(nLessonDetailID)) {
        QDate date = m_resDetailIDDateHash.value(nLessonDetailID);

        //存在请求资源的请求.
        if(date == lessonDate) {
            ///Mark,其实目前版本暂时可以不需要对比date，因为一周中不会有相同的detaileID.
            m_resDetailIDDateHash.remove(nLessonDetailID);
            //查找这节课.

            Lesson *pLesson = Q_NULLPTR;

            for(int weekDay = 0; weekDay < m_weekLessonVec.size();  ++weekDay) {
                QMap<int, Lesson*> dayLessonMap = m_weekLessonVec.value(weekDay);
                QList<int> dayKeysList = dayLessonMap.keys();

                for(int pos = 0; pos < dayKeysList.size(); ++pos) {
                    int nSection = dayKeysList.at(pos);
                    Lesson *pLesson = dayLessonMap.value(nSection);

                    if((nLessonDetailID == pLesson->getLessonDetailId())
                            && (lessonDate == pLesson->getDate())) {
                        qDebug()<<"come in zzzzz"<<endl;
                        break;
                    }
                }
            }

            if(pLesson != Q_NULLPTR) {
                qDebug()<<"invoke Arg::nw getFilesState"<<endl;
                Arg::nw->getFilesState(json, pLesson);
            }
        }
    }
*/


}

void CourseGrid::setLessonRemark(int nDetailID, const QString &sRemark)
{
    for(int weekDayPos = 0; weekDayPos < m_weekLessonVec.size(); ++weekDayPos) {
        QMap<int, Lesson*> secLessonMap = m_weekLessonVec.at(weekDayPos);

        for(auto cItor = secLessonMap.constBegin(); cItor != secLessonMap.constEnd(); ++cItor) {
            Lesson *pLesson = cItor.value();
            int nCurLessDetailID = pLesson->getLessonDetailId();

            if(nDetailID == nCurLessDetailID) {
                pLesson->setNote(sRemark);

                break;
            }
        }
    }
}

void CourseGrid::mapWeekLessons(const QDate &date)
{
    //重置相关日期显示.
    m_curDisplayDate = date;
    placeHolderButton->setText(m_curDisplayDate.toString ("MMMM"));
    rearrange(m_curDisplayDate);
    bool bNotInTerm = false;
    int nTermWeek = -1;

    //先看是否处于同一周，同一周不再做处理.
    if(mLinkedTerm != Q_NULLPTR) {
        int nLastDispWeekNum = m_nCurDisplayCourseDateWeekNum;
        m_nCurDisplayCourseDateWeekNum = mLinkedTerm->getDateWeekNumber(date);

        //日期不在学期中处理.
        if(m_nCurDisplayCourseDateWeekNum == 0) {
            getTermByDate(date);

            if(m_pEvLoop == Q_NULLPTR) {
                m_pEvLoop = new QEventLoop();
            }

            int nResult = m_pEvLoop->exec();

            if(nResult == -1) {
//                QMessageBox::information(this, tr("不存在学期"), tr("当前日期不存在学期."));
                bNotInTerm = true;
//                return;
            }
        }
        else if(m_nCurDisplayCourseDateWeekNum == nLastDispWeekNum) {
            if(!m_bMustReload) {
                return;
            }

            m_bMustReload = false;
        }

        Arg::setCurrentWeekIndex(m_nCurDisplayCourseDateWeekNum);
    }

    for(int row = 0; row < cellWidgets.size(); ++row) {
        for(int col = 0; col < cellWidgets[row].size(); ++col) {
            cellWidgets[row][col]->unlink();
        }
    }

    //销毁前面获取的周课程.
    for(int index = 0; index < m_weekLessonVec.size(); ++index) {
        QMap<int, Lesson*> secLessonMap = m_weekLessonVec.at(index);
        QList<int> sectionList = secLessonMap.keys();

        for(int index = 0; index < sectionList.size(); ++index) {
            int nSection = sectionList.at(index);
            Lesson *pLesson = secLessonMap.value(nSection);
            delete pLesson;
            pLesson = Q_NULLPTR;

            secLessonMap.remove(nSection);
        }
    }

    m_weekLessonVec.clear();

    if(bNotInTerm) {
        emit termWeekChanged(nTermWeek);
        return;
    }

    nTermWeek = mLinkedTerm->getDateWeekNumber(date);
    emit termWeekChanged(nTermWeek);

    if(mLinkedTerm != Q_NULLPTR) {
        if(m_weekLessonVec.size() == 0) {
            m_weekLessonVec.resize(7);
        }

        bool bGetStatus = mLinkedTerm->getWeekLessonsByDate(m_weekLessonVec, date);

        if(!bGetStatus) {
            return;
        }
        //下载一周课程所需资源文件和备注.
        else{
            ///Modified by BiXiaoxia 2017.02.24.
            DataClassInstanceManage *pDataInstanceMgr = DataClassInstanceManage::getInstance();
            ResFilesDB *pResFileDB = pDataInstanceMgr->getResFilesDBPtr();
            LessonRemarkDB *pLesRemarkDB = pDataInstanceMgr->getLessonRemarkDBPtr();
//            m_resDetailIDCourseCellWgtHash.clear();

            for (int i = 0; i < m_weekLessonVec.size(); i++)
            {
                QMap<int, Lesson*>::const_iterator lesson_mapIt;
                for( lesson_mapIt=m_weekLessonVec[i].constBegin(); lesson_mapIt!=m_weekLessonVec[i].constEnd(); ++lesson_mapIt)
                {
                    Lesson* les = lesson_mapIt.value();
                    qDebug()<<"loop in lesson map: "<<les->getDate().toString("yyyy-M-d")
                           <<" secNum: "<<les->section()<<endl;

//                    res_file = new ResFilesDB();

                    int nDetailID = les->getLessonDetailId();
                    QDate lessonDate = les->getDate();

                    //请求资源.
                    pResFileDB->setLessonDetailId(nDetailID);
                    pResFileDB->setUseDataTime(lessonDate);
                    pResFileDB->find();

                    //请求note.
                    pLesRemarkDB->setLessonDetailId(nDetailID);
                    pLesRemarkDB->setTeachingDate(lessonDate);
                    pLesRemarkDB->find();
                }

            }

        }

        //对应到coursegrid里面.
        for(int nWeekDay = 0; nWeekDay < m_weekLessonVec.size(); ++nWeekDay) {
            Q_ASSERT_X((nWeekDay >= 0) && (nWeekDay < 7), "CourseGrid::mapWeekLessons", "weekDay overflow");

            QMap<int, Lesson*> secLessons = m_weekLessonVec.at(nWeekDay);
            QMap<int, Lesson*>::const_iterator cItor = secLessons.constBegin();
            for(; cItor != secLessons.constEnd(); ++cItor) {
                int nSection = cItor.key();
                Lesson *pLesson = cItor.value();
                //课表里的section从1开始.cellWidgets的第一维为section.
                cellWidgets[nSection - 1][nWeekDay]->link(pLesson);
                qDebug() << "lesson weekday is:" << pLesson->getWeekId()
                         << "lesson section is:" << pLesson->section()
                         << "less name is:" << pLesson->subject();
            }
        }
    }
}

void CourseGrid::getTermByDate(const QDate &date)
{
    TermDB *pTermDB = DataClassInstanceManage::getInstance()->getTemDBPtr();
    pTermDB->queryTermByDate(date);
}

void CourseGrid::termLoadFinished()
{
    if((m_pEvLoop != Q_NULLPTR) && (m_pEvLoop->isRunning())) {
        m_pEvLoop->exit(0);
    }

    Term *pTerm = Arg::currentTerm();
    setCurrentTerm(pTerm);
}

void CourseGrid::hideCalendarSelect()
{
    if(m_pCalendarWgt != Q_NULLPTR) {
        m_pCalendarWgt->hide();
    }
}

void CourseGrid::reloadLessons()
{
    m_bMustReload = true;

    mapWeekLessons(m_curDisplayDate);
}

void CourseGrid::getCurrentTimeLesson(Lesson *&pLesson, int &nLeftTime)
{
    int disDayWeekNum = m_curDisplayDate.dayOfWeek();
    int nAdjustNum = disDayWeekNum - 1;
    //请求课程的周的第一天日期.
    QDate weekStartDate = m_curDisplayDate.addDays(-nAdjustNum);

    QDate date = QDate::currentDate();
    int curDayofWeek = date.dayOfWeek();
    nAdjustNum = curDayofWeek - 1;
    QDate curWeekStartDate = date.addDays(-nAdjustNum);

    if(weekStartDate != curWeekStartDate) {
        //如果显示的周数不是当前周数，要先切换当前周，否则无意义.
        mapWeekLessons(QDate::currentDate());
    }

    Q_ASSERT_X(((m_weekLessonVec.size() > 0) && (m_weekLessonVec.size() <= 7)), "", "");

    pLesson = Q_NULLPTR;
    Lesson *pCurTimeLesson = Q_NULLPTR;


    int weekDay = date.dayOfWeek();
    int weekDayIndex = weekDay - 1;
    QMap<int, Lesson*> dayLessons = m_weekLessonVec.at(weekDayIndex);

//    QString sTime;
    QTime time = QTime::currentTime();
    int hour = time.hour();
    int minite = time.minute();
    int allSecond = hour * 3600 + minite * 60 + time.second();

    for(auto constItor = dayLessons.constBegin(); constItor != dayLessons.constEnd(); ++constItor) {
        pCurTimeLesson = constItor.value();
//        pCurTimeLesson->getStartTime(sTime);
        int sectionIndex = constItor.key() - 1;
        CourseCellWidget *pCellWgt = cellWidgets[sectionIndex][weekDayIndex];
        int nStartTime = pCellWgt->getStartTime();
        int nEndTime = pCellWgt->getEndTime();

        if((allSecond >= nStartTime) && (allSecond < nEndTime)) {
            pLesson = pCurTimeLesson;
            nLeftTime = nEndTime - allSecond;
            break;
        }
    }
}

//void CourseGridLayout::setCellFixedSize(int width, int height)
//{
//    m_nCellWidth = width;
//    m_nCellHeight = height;
//}

//QSize CourseGridLayout::sizeHint() const
//{
//    QSize size(rowCount() * m_nCellWidth, columnCount() * m_nCellHeight);

//    return size;

//}

BreakWidget::BreakWidget(const QString &sText, QWidget *parent):
    QWidget(parent),
    m_fAlapha(0.0f)
{
    m_pTextLb = new QLabel;
    m_pTextLb->setAlignment(Qt::AlignCenter);
    m_pTextLb->setText(sText);
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->addWidget(m_pTextLb);

    setLayout(pLayout);
}

void BreakWidget::setBackgroundAlpha(qreal alpha)
{
//    m_fAlapha = alpha;

//    //不能全透明.
//    if((m_fAlapha > 0.0f) && (m_fAlapha < 0.0f)) {
//        m_fAlapha = 0.2f;
//    }

//    update();
}

void BreakWidget::paintEvent(QPaintEvent *pEv)
{
    Q_UNUSED(pEv)

    QPainter painter(this);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QPalette pal = opt.palette;
    QBrush brush = pal.base();
    QColor color = brush.color();
    color.setAlpha(m_fAlapha);
    brush.setColor(color);
    painter.fillRect(rect(), brush);
}

CalenderSelectWidget::CalenderSelectWidget(QWidget *parent):
    BaseShadowWidget(parent)
{
    m_pCalendarWgt = new CalendarWidget;
    connect(m_pCalendarWgt, &CalendarWidget::clicked, this, &CalenderSelectWidget::dateSelected);

    int nShadowWidth = getShadowMargin();

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(nShadowWidth, nShadowWidth, nShadowWidth, nShadowWidth);
    pLayout->addWidget(m_pCalendarWgt);
    setLayout(pLayout);
}

CalenderSelectWidget::~CalenderSelectWidget()
{

}

void CalenderSelectWidget::setCurrentDate(int year, int month)
{
    m_pCalendarWgt->setCurrentPage(year, month);
}

void CalenderSelectWidget::leaveEvent(QEvent *pEv)
{
    hide();
}
