#ifndef MOONCALENDARSTYLE_H
#define MOONCALENDARSTYLE_H
QString getStyle() {
return QString {"* { \
    font-family: \"Hiragino Sans GB\", \"Microsoft YaHei\", \"WenQuanYi Micro Hei\", sans-serif; \
    font-size: 12px; \
  } \
TransparentButton { \
    background-color: transparent; \
    border: none; \
    color: white; \
    font-size: 14px; \
} \
NoteWidget { \
    background: transparent; \
    width: 275px; \
    height: 100px; \
    font-size: 13px; \
    padding: 0px; \
    margin: 0px; \
} \
NoteWidget > QPushButton#Cancel{ \
    background: #3a991b;\
    font-size: 14px; \
    color: white; \
    font-weight: bold; \
    padding-top: 2px; \
    padding-bottom: 2px; \
    margin: 5px; \
    border: none; \
} \
NoteWidget > QPushButton#Tool { \
    background-color: transparent; \
    border: none; \
} \
NoteWidget QTextEdit { \
    font-size: 13px; \
    border: none; \
    background-color: white \
} \
UserInfoView { \
    min-width: 180px; \
    background: #0A3D8E; \
    height: 50px; \
    /*padding: 0 50px;*/ \
    margin: 0;  \
} \
UserInfoView QLabel#Name { \
    /* font-family: \"Monaco\"; */ \
    font-size: 17px; \
    /*padding-left: 10px;*/ \
    color: white; \
    margin-top: 15px; \
} \
UserInfoView QLabel#Icon { \
    background-color: red; \
    height: 50px; \
    width: 50px; \
    margin: 0; \
    padding: 0; \
}\
UserInfoView QProgressBar {\
    margin-right: 50px; \
    font-size: 10px; \
    text-align: right; \
    color: orange; \
    height: 10px; \
    margin-top: 3px; \
    margin-bottom: 4px; \
} \
UserInfoView QProgressBar::chunk \
{ \
    background-color: #ECAE33; \
} \
MainScreen QWidget#TopLine \
{ \
    background-color: #1151B5; \
    max-height: 50px; \
    min-height: 50px; \
} \
MainScreen QWidget#TopLine QLabel \
{\
    color: white;\
    font: 14px; \
    margin: 0 1em; \
} \
MainScreen Sidebar { \
    width: 240px; \
} \
MainScreen Sidebar QTabBar { \
    alignment: top; \
} \
MainScreen Sidebar QTabBar:tab:first { \
    background-image: url(\":/Icon/Slide/Resources_default.png\"); \
} \
MainScreen Sidebar QTabBar:tab:first:selected { \
    background-image: url(\":/Icon/Slide/Resources_highlight.png\"); \
} \
MainScreen Sidebar QTabBar:tab:last { \
    background-image: url(\":/Icon/Slide/circle_default.png\"); \
} \
MainScreen Sidebar QTabBar:tab:last:selected { \
    background-image: url(\":/Icon/Slide/circle_highlight.png\"); \
} \
MainScreen Sidebar QTabBar:tab:middle { \
    background-image: url(\":/Icon/Slide/Friends_default.png\"); \
} \
MainScreen Sidebar QTabBar:tab:middle:selected { \
    background-image: url(\":/Icon/Slide/Friends_highlight.png\"); \
} \
MainScreen Sidebar QTabBar:tab{ \
    background-color: grey; \
    /*大小在代码中设置*/ \
    background-repeat: no-repeat; \
    background-position: center; \
} \
MainScreen Sidebar QTabBar:tab:selected { \
    background-color: white; \
} \
MainScreen Sidebar::pane { \
    background-color: transparent; \
} \
MainScreen FileListWidget { \
    background-color: transparent; \
} \
MainScreen FileListWidget QTreeWidget { \
    background-color: transparent; \
} \
AnimationStackedWidget { \
    background-color: transparent; \
} \
FriendListWidget, LiveListWidget { \
    background-color: white; \
} \
FriendListWidget QScrollArea, FriendListWidget QScrollArea .QWidget, \
LiveListWidget QScrollArea, LiveListWidget QScrollArea .QWidget { \
    background-color: transparent; \
} \
FriendCardWidget:hover, LiveCardWidget:hover { \
    background-color: #B7D1FA; \
    border-radius: 8px; \
} \
FriendCardWidget:pressed, LiveCardWidget:pressed { \
    background-color: #5178BB; \
    border-radius: 8px; \
} \
LiveListWidget .QFrame { \
    color: lightgray; \
} \
LiveListWidget #NickNameLabel{ \
    color: DodgerBlue; \
} \
 \
FriendCardWidget QLabel#instituteLabel { \
    color: DarkGray; \
} \
  \
FriendDetailDialog{ \
    background-color: white; \
    min-width: 300px; \
} \
 \
GrayTextLabel{ \
    color: DarkGray; \
} \
 \
CourseGrid TransparentButton#SectionNumberButton \
{ \
    background-color: rgba(18, 80, 181, 178); \
    border: 0; \
    color: white; \
    font-size: 14px; \
    width: 56px; \
    height: 100%; \
} \

CourseGrid TransparentButton#BreakSectionButton
{
    background-color: rgba(44, 86, 156, 178);
    border: 0;
    color: white;
    font-size: 14px;
    width: 56px;
    height: 30%;
}

CourseGrid TransparentButton#PlaceHolderButton
{
    color: black;
    font-weight: bold;
}
CourseCellWidget#Break
{
    height: 30%;
    border: 0;
    background-color: #5178BB;
}
CourseCellWidget QLabel
{
    padding: 0;
    margin: 0;
    font-size: 13px;
}

GridViewWeek TransparentButton#WeekdayButton
{
    height: 53px;
    border: 0;
    color: white;
    font-weight: bold;
}
GridViewWeek TransparentButton#WeekdayButton[selected='false']
{
    background-color: rgba(85, 85, 85, 128);
}
GridViewWeek TransparentButton#WeekdayButton[selected='true']
{
    background-color: rgba(58, 153, 27, 255);
}

GridViewWeek TransparentButton#WeekdayButton:hover:!pressed
{
    background-color: #2260c3;
}

CourseGrid TransparentButton#PlaceHolderButton
{
    height: 53px;
    background-color: rgba(85, 85, 85, 128);
    border: 0;
}

CourseCellWidget FileNumWidget
{
    float: right;
}

MiniMode--InfoView {
    background-color: #1151B5;
    color: white;
}
MiniMode--InfoView *{
    color: white;
}

MiniMode--InfoView QLabel#Subject {
    font-size: 16px;
    margin: 0;
    padding: 0;
    height: 16px;
}
MiniMode--InfoView QLabel#Location {
    font-size: 12px;
    margin: 0;
    padding: 0;
    height: 10px;
}
MiniMode--InfoView QLabel#Class {
    font-size: 10px;
    margin: 0;
    padding: 0;
    height: 8px;
}
MiniMode--MiniDateWidget QLabel#Time {
    font-size: 16px;
    font-weight: bolder;
}
MiniMode--MiniDateWidget QLabel#Date {
    font-size: 12px;
    color: #888888;
}
MiniMode--MainScreen MiniMode--MiniDateWidget {
    background-color: white;
}
MiniMode--MainScreen QWidget#Operation
{
    background-color: white;
    margin: 0;
    padding: 0;
}
MiniMode--MainScreen TransparentButton#TopLine {
    background-color: transparent;
    border: none;
    width: 10px;
    height: 10px;
}
MiniMode--MainScreen {
    background-color: white;
}



MiniMode--MainScreen {
    background-color: transparent;
}
TitleBar {
    background-color: #1151B5;
}
TitleBar QLabel#title {
    font-weight: bold;
    font-size: 2em;
    color: white;
}

SetupDialog TitleBar {
    background-color: #1151B5;
}

SetupDialog QListView {
    show-decoration-selected: 1; /* make the selection span the entire width of the view */
    padding: 0;
    background-color: transparent;
}

SetupDialog QListView::item:selected{
    background: #134EB2;
}

FeedBackDialog QPushButton#submit {
    background-color: #1151B5;
    color: white;
    border: none;
    float: right;
}

ModifyPasswordDialog QPushButton#submit
{
    background-color: #1151B5;
    color: white;
    font-weight: bold;
}

MainScreen QWidget#TopLine QPushButton#FixButton,
MainScreen QWidget#TopLine QPushButton#ConfigLookAndFeel,
MainScreen QWidget#TopLine QPushButton#TopMore,
MainScreen QWidget#TopLine QPushButton#BlockButton,
MainScreen QWidget#TopLine QPushButton#MinButton,
MainScreen QWidget#TopLine QPushButton#CloseButton
{
    min-width: 28px;
    min-height: 28px;
    margin-left: 3px;
    margin-right: 3px;
}

TitleBar TransparentButton#CloseButton:hover {
 background-color: #d74631
}

MainScreen QWidget#TopLine QPushButton#FixButton:hover,
MainScreen QWidget#TopLine QPushButton#ConfigLookAndFeel:hover,
MainScreen QWidget#TopLine QPushButton#TopMore:hover,
MainScreen QWidget#TopLine QPushButton#BlockButton:hover,
MainScreen QWidget#TopLine QPushButton#MinButton:hover
{
    background-color: #2260c5;
}

QPushButton#CloseButton:hover
{
    background-color: #d74631;
}

/*
QComboBox {
　border: 1px solid lightGray;
　border-top-left-radius: 3px;
　border-top-right-radius: 3px;
}

QComboBox::drop-down {
 width:30px;
 background:transparent;
 border-width: 0px
}

QComboBox::down-arrow {
　image: url(:/image/downarrow-normal.png);
}

QComboBox::down-arrow:pressed {
　image :url(:image/downarrow-pressed.png);
}*/

QScrollBar:vertical {
　　margin:　0px 0px 0px 0px;
　　border: 1px solid #dadbde;
  　border-radius: 5px;
   　background-color:#C6E2FF;
　　width:10px;
}

QScrollBar::handle:vertical {
　border: 1px solid gray;
　background-color:gray;
　margin:0px 0px 0px 0px;
　border-radius: 3px;
　min-height: 60px;
}

QScrollBar::add-line:vertical {
border: 0px solid grey;
height: 0px;
subcontrol-position: bottom;
subcontrol-origin: margin;
}

QScrollBar::sub-line:vertical {
　border: 0px solid grey;
　height: 0px;
　subcontrol-position: top;
　subcontrol-origin: margin;
}

.QPushButton{
　background:#1250b5;
　border:1px solid #1250b5;
　color:white;
}

/*
QLineEdit{
　border: 1px solid lightGray;
　border-bottom-left-radius: 3px;
　border-bottom-right-radius: 3px;
}*/

}

#endif // MOONCALENDARSTYLE_H
