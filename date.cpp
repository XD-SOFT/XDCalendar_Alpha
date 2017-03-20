#include "date.h"

#include <QDebug>

#define qsTr QObject::tr

#define T_D_YAER   1924 //  天干地支开始计算年月

//每年春节对应的公历日期
static const int springFestival[] = {
    130,217,206,                                // 1968,1969,1970
    127,215,203,123,211,131,218,207,128,216,    // 1971--1980
    205,125,213,202,220,209,219,217,206,127,    // 1981---1990
    215,204,123,210,131,219,207,128,216,205,    // 1991--2000
    124,212,201,122,209,129,218,207,126,214,    // 2001--2010
    203,123,210,131,219,208,128,216,205,125,    // 2011--2020                                       //2011--2020
    212,201,122,210,129,217,206,126,213,203,    // 2021--2030
    123,211,131,219,208,128,215,204,124,212     // 2031--2040
};

//16--18位表示闰几月，0--12位表示农历每月的数据，高位表示1月，低位表示12月（农历闰月就会多一个月）
static const int  nLunarData[] = {
    461653,1386,2413,                                            // 1968,1969,1970
    330077,1197,2637,268877,3365,531109,2900,2922,398042,2395,   // 1971--1980
    1179,267415,2635,661067,1701,1748,398772,2742,2391,330031,   // 1981---1990
    1175,1611,200010,3749,527717,1452,2742,332397,2350,3222,     // 1991--2000
    268949,3402,3493,133973,1386,464219,605,2349,334123,2709,    // 2001--2010
    2890,267946,2773,592565,1210,2651,395863,1323,2707,265877,   // 2011--2020
    1706,2773,133557,1206,398510,2638,3366,335142,3411,1450,     // 2021 -- 2030
    200042,2413,723293,1197,2637,399947,3365,3410,334676,2906    // 2031 -- 2040
};

static const int chineseTwentyFourData[] = {
    0x95,0xB4,0x96,0xA5,0x96,0x97,0x88,0x78,0x78,0x69,0x78,0x87,     // 1970
    0x96,0xB4,0x96,0xA6,0x97,0x97,0x78,0x79,0x79,0x69,0x78,0x77,     // 1971
    0x96,0xA4,0xA5,0xA5,0xA6,0xA6,0x88,0x88,0x88,0x78,0x87,0x87,     // 1972
    0xA5,0xB5,0x96,0xA5,0xA6,0x96,0x88,0x78,0x78,0x78,0x87,0x87,     // 1973
    0x95,0xB4,0x96,0xA5,0x96,0x97,0x88,0x78,0x78,0x69,0x78,0x87,     // 1974
    0x96,0xB4,0x96,0xA6,0x97,0x97,0x78,0x79,0x78,0x69,0x78,0x77,     // 1975
    0x96,0xA4,0xA5,0xB5,0xA6,0xA6,0x88,0x89,0x88,0x78,0x87,0x87,     // 1976
    0xA5,0xB4,0x96,0xA5,0x96,0x96,0x88,0x88,0x78,0x78,0x87,0x87,     // 1977
    0x95,0xB4,0x96,0xA5,0x96,0x97,0x88,0x78,0x78,0x79,0x78,0x87,     // 1978
    0x96,0xB4,0x96,0xA6,0x96,0x97,0x78,0x79,0x78,0x69,0x78,0x77,     // 1979
    0x96,0xA4,0xA5,0xB5,0xA6,0xA6,0x88,0x88,0x88,0x78,0x87,0x87,     // 1980
    0xA5,0xB4,0x96,0xA5,0xA6,0x96,0x88,0x88,0x78,0x78,0x77,0x87,     // 1981
    0x95,0xB4,0x96,0xA5,0x96,0x97,0x88,0x78,0x78,0x79,0x77,0x87,     // 1982
    0x95,0xB4,0x96,0xA5,0x96,0x97,0x78,0x79,0x78,0x69,0x78,0x77,     // 1983
    0x96,0xB4,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x88,0x78,0x87,0x87,     // 1984
    0xA5,0xB4,0xA6,0xA5,0xA6,0x96,0x88,0x88,0x78,0x78,0x87,0x87,     // 1985
    0xA5,0xB4,0x96,0xA5,0x96,0x97,0x88,0x78,0x78,0x79,0x77,0x87,     // 1986
    0x95,0xB4,0x96,0xA5,0x96,0x97,0x88,0x79,0x78,0x69,0x78,0x87,     // 1987
    0x96,0xB4,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x88,0x78,0x87,0x86,     // 1988
    0xA5,0xB4,0xA5,0xA5,0xA6,0x96,0x88,0x88,0x88,0x78,0x87,0x87,     // 1989
    0xA5,0xB4,0x96,0xA5,0x96,0x96,0x88,0x78,0x78,0x79,0x77,0x87,     // 1990
    0x95,0xB4,0x96,0xA5,0x86,0x97,0x88,0x78,0x78,0x69,0x78,0x87,     // 1991
    0x96,0xB4,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x88,0x78,0x87,0x86,     // 1992
    0xA5,0xB3,0xA5,0xA5,0xA6,0x96,0x88,0x88,0x88,0x78,0x87,0x87,     // 1993
    0xA5,0xB4,0x96,0xA5,0x96,0x96,0x88,0x78,0x78,0x78,0x87,0x87,     // 1994
    0x95,0xB4,0x96,0xA5,0x96,0x97,0x88,0x76,0x78,0x69,0x78,0x87,     // 1995
    0x96,0xB4,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x88,0x78,0x87,0x86,     // 1996
    0xA5,0xB3,0xA5,0xA5,0xA6,0xA6,0x88,0x88,0x88,0x78,0x87,0x87,     // 1997
    0xA5,0xB4,0x96,0xA5,0x96,0x96,0x88,0x78,0x78,0x78,0x87,0x87,     // 1998
    0x95,0xB4,0x96,0xA5,0x96,0x97,0x88,0x78,0x78,0x69,0x78,0x87,     // 1999
    0x96,0xB4,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x88,0x78,0x87,0x86,     // 2000
    0xA5,0xB3,0xA5,0xA5,0xA6,0xA6,0x88,0x88,0x88,0x78,0x87,0x87,     // 2001
    0xA5,0xB4,0x96,0xA5,0x96,0x96,0x88,0x78,0x78,0x78,0x87,0x87,     // 2002
    0x95,0xB4,0x96,0xA5,0x96,0x97,0x88,0x78,0x78,0x69,0x78,0x87,     // 2003
    0x96,0xB4,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x88,0x78,0x87,0x86,     // 2004
    0xA5,0xB3,0xA5,0xA5,0xA6,0xA6,0x88,0x88,0x88,0x78,0x87,0x87,     // 2005
    0xA5,0xB4,0x96,0xA5,0xA6,0x96,0x88,0x88,0x78,0x78,0x87,0x87,     // 2006
    0x95,0xB4,0x96,0xA5,0x96,0x97,0x88,0x78,0x78,0x69,0x78,0x87,     // 2007
    0x96,0xB4,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x87,0x78,0x87,0x86,     // 2008
    0xA5,0xB3,0xA5,0xB5,0xA6,0xA6,0x88,0x88,0x88,0x78,0x87,0x87,     // 2009
    0xA5,0xB4,0x96,0xA5,0xA6,0x96,0x88,0x88,0x78,0x78,0x87,0x87,     // 2010
    0x95,0xB4,0x96,0xA5,0x96,0x97,0x88,0x78,0x78,0x79,0x78,0x87,     // 2011
    0x96,0xB4,0xA5,0xB5,0xA5,0xA6,0x87,0x88,0x87,0x78,0x87,0x86,     // 2012
    0xA5,0xB3,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x88,0x78,0x87,0x87,     // 2013
    0xA5,0xB4,0x96,0xA5,0xA6,0x96,0x88,0x88,0x78,0x78,0x87,0x87,     // 2014
    0x95,0xB4,0x96,0xA5,0x96,0x97,0x88,0x78,0x78,0x79,0x77,0x87,     // 2015
    0x95,0xB4,0xA5,0xB4,0xA5,0xA6,0x87,0x88,0x87,0x78,0x87,0x86,     // 2016
    0xA5,0xC3,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x88,0x78,0x87,0x87,     // 2017
    0xA5,0xB4,0xA6,0xA5,0xA6,0x96,0x88,0x88,0x78,0x78,0x87,0x87,     // 2018
    0xA5,0xB4,0x96,0xA5,0x96,0x96,0x88,0x78,0x78,0x79,0x77,0x87,     // 2019
    0x95,0xB4,0xA5,0xB4,0xA5,0xA6,0x97,0x87,0x87,0x78,0x87,0x86,     // 2020
    0xA5,0xC3,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x88,0x78,0x87,0x86,     // 2021
    0xA5,0xB4,0xA5,0xA5,0xA6,0x96,0x88,0x88,0x88,0x78,0x87,0x87,     // 2022
    0xA5,0xB4,0x96,0xA5,0x96,0x96,0x88,0x78,0x78,0x79,0x77,0x87,     // 2023
    0x95,0xB4,0xA5,0xB4,0xA5,0xA6,0x97,0x87,0x87,0x78,0x87,0x96,     // 2024
    0xA5,0xC3,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x88,0x78,0x87,0x86,     // 2025
    0xA5,0xB3,0xA5,0xA5,0xA6,0xA6,0x88,0x88,0x88,0x78,0x87,0x87,     // 2026
    0xA5,0xB4,0x96,0xA5,0x96,0x96,0x88,0x78,0x78,0x78,0x87,0x87,     // 2027
    0x95,0xB4,0xA5,0xB4,0xA5,0xA6,0x97,0x87,0x87,0x78,0x87,0x96,     // 2028
    0xA5,0xC3,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x88,0x78,0x87,0x86,     // 2029
    0xA5,0xB3,0xA5,0xA5,0xA6,0xA6,0x88,0x88,0x88,0x78,0x87,0x87,     // 2030
    0xA5,0xB4,0x96,0xA5,0x96,0x96,0x88,0x78,0x78,0x78,0x87,0x87,     // 2031
    0x95,0xB4,0xA5,0xB4,0xA5,0xA6,0x97,0x87,0x87,0x78,0x87,0x96,     // 2032
    0xA5,0xC3,0xA5,0xB5,0xA6,0xA6,0x88,0x88,0x88,0x78,0x87,0x86,     // 2033
    0xA5,0xB3,0xA5,0xA5,0xA6,0xA6,0x88,0x78,0x88,0x78,0x87,0x87,     // 2034
    0xA5,0xB4,0x96,0xA5,0xA6,0x96,0x88,0x88,0x78,0x78,0x87,0x87,     // 2035
    0x95,0xB4,0xA5,0xB4,0xA5,0xA6,0x97,0x87,0x87,0x78,0x87,0x96,     // 2036
    0xA5,0xC3,0xA5,0xB5,0xA6,0xA6,0x87,0x88,0x88,0x78,0x87,0x86,     // 2037
    0xA5,0xB3,0xA5,0xA5,0xA6,0xA6,0x88,0x88,0x88,0x78,0x87,0x87      // 2038
};

static const QString dayName[] =  {
    qsTr("*"),
    qsTr("初一"), qsTr("初二"), qsTr("初三"), qsTr("初四"), qsTr("初五"),
    qsTr("初六"), qsTr("初七"), qsTr("初八"), qsTr("初九"), qsTr("初十"),
    qsTr("十一"), qsTr("十二"), qsTr("十三"), qsTr("十四"), qsTr("十五"),
    qsTr("十六"), qsTr("十七"), qsTr("十八"), qsTr("十九"), qsTr("二十"),
    qsTr("廿一"), qsTr("廿二"), qsTr("廿三"), qsTr("廿四"), qsTr("廿五"),
    qsTr("廿六"), qsTr("廿七"), qsTr("廿八"), qsTr("廿九"), qsTr("三十")
};

/*农历月份名*/
static const QString monName[] = {
    qsTr("*"),
    qsTr("正月"), qsTr("二月"), qsTr("三月"), qsTr("四月"),
    qsTr("五月"), qsTr("六月"), qsTr("七月"), qsTr("八月"),
    qsTr("九月"), qsTr("十月"), qsTr("冬月"), qsTr("腊月")
};

// 24节气
static const QString solarTerm[] = {
    qsTr("小寒"), qsTr("大寒"), qsTr("立春"), qsTr("雨水"), qsTr("惊蛰"), qsTr("春分"), qsTr("清明"), qsTr("谷雨"),
    qsTr("立夏"), qsTr("小满"), qsTr("芒种"), qsTr("夏至"), qsTr("小暑"), qsTr("大暑"), qsTr("立秋"), qsTr("处暑"),
    qsTr("白露"), qsTr("秋分"), qsTr("寒露"), qsTr("霜降"), qsTr("立冬"), qsTr("小雪"), qsTr("大雪"), qsTr("冬至")
};


// 天干算法
static const QString strTiangan[] = {
    qsTr("甲"), qsTr("乙"), qsTr("丙"), qsTr("丁"), qsTr("戊"),
    qsTr("己"), qsTr("庚"), qsTr("辛"), qsTr("壬"), qsTr("癸"),
};

// 动物属性
static const QString strAnimal[] = {
    qsTr("鼠"), qsTr("牛"), qsTr("虎"), qsTr("兔"), qsTr("龙"), qsTr("蛇"),
    qsTr("马"), qsTr("羊"), qsTr("猴"), qsTr("鸡"), qsTr("狗"), qsTr("猪"),
};

// 地址算法
static const QString strDizhi[] = {
    qsTr("子"), qsTr("丑"), qsTr("寅"), qsTr("卯"), qsTr("辰"), qsTr("巳"),
    qsTr("午"), qsTr("未"), qsTr("申"), qsTr("酉"), qsTr("戌"), qsTr("亥"),
};

/*公历每月前面的天数*/
int monthAdd[] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};


Date::Date(QObject *parent) : QObject(parent)
{

}

/**
 * @brief Date::getSpringFestivalMonth 获取当年春节是几月几日
 * @param year
 * @return
 */
quint16 Date::getSpringFestivalDate(int year)
{
    int nTemp = year - 1968;
    int springFestivalMonth = springFestival[nTemp] / 100;
    int springFestivalDay = springFestival[nTemp] % 100;
    qDebug() << springFestivalMonth << springFestivalDay;

    return (springFestivalMonth << 8) | springFestivalDay;
}

int Date::getFirstDayOfWeek(int year, int month)
{
    int week = 0;
    // 获取当年1月1日是星期几
    week = (year + (year- 1) / 4 -(year - 1) / 100 + (year - 1) / 400) % 7;
    week += getTotalMonthDays(year, month);
    return week % 7;
}

int Date::getTotalMonthDays(int year, int month)
{
    int nDays = 0;
    int nLoopDay = isLoopYaer(year) ? 1 : 0;
    switch (month) {
    case  1: nDays =   0;            break;
    case  2: nDays =  31;            break;
    case  3: nDays =  59 + nLoopDay; break;
    case  4: nDays =  90 + nLoopDay; break;
    case  5: nDays = 120 + nLoopDay; break;
    case  6: nDays = 151 + nLoopDay; break;
    case  7: nDays = 181 + nLoopDay; break;
    case  8: nDays = 212 + nLoopDay; break;
    case  9: nDays = 243 + nLoopDay; break;
    case 10: nDays = 273 + nLoopDay; break;
    case 11: nDays = 304 + nLoopDay; break;
    case 12: nDays = 334 + nLoopDay; break;
    default: nDays = 0; break;
    }
    return nDays;
}

int Date::getMonthDays(int year, int month)
{
    int nDays = 0;
    int nLoopDay = isLoopYaer(year) ? 1 : 0;
    switch (month) {
    case  1: nDays = 31;            break;
    case  2: nDays = 28 + nLoopDay; break;
    case  3: nDays = 31;            break;
    case  4: nDays = 30;            break;
    case  5: nDays = 31;            break;
    case  6: nDays = 30;            break;
    case  7: nDays = 31;            break;
    case  8: nDays = 31;            break;
    case  9: nDays = 30;            break;
    case 10: nDays = 31;            break;
    case 11: nDays = 30;            break;
    case 12: nDays = 31;            break;
    default: nDays = 30;            break;
    }

    return nDays;
}

bool Date::isLoopYaer(int year)
{
    return (((0 == (year % 4)) && (0 != (year % 100))) || (0 == (year % 400)));
}

/**
 * @brief Date::getLunarDate 计算农历
 * @param year  年
 * @param month 月
 * @param day 日
 * @return
 */
QString Date::getLunarDate (int year, int month, int day){
    int nTheDate,nIsEnd,nMonTemp,k,n,nBit;
    // 先获取公历节日
    QString strDate = holiday(month, day);
    // 计算24节气
    QString strSolarTerms = solarTerms(year, month, day);

    /*现在计算农历：获得当年春节的公历日期（比如：2015年春节日期为（2月19日）），
            以此为分界点，2.19前面的农历是2014年农历（用2014年农历数据来计算），
            2.19以及之后的日期，农历为2015年农历（用2015年农历数据来计算）。*/
    nMonTemp = year - 1968;
    int springFestivalMonth = springFestival[nMonTemp] / 100;
    int springFestivalDaty = springFestival[nMonTemp] % 100;

    if(month < springFestivalMonth )
    {
        nMonTemp--;
        nTheDate = 365 * 1 + day + monthAdd[month - 1] - 31 * ((springFestival[nMonTemp] / 100) - 1) - springFestival[nMonTemp] % 100 + 1;

        if((!(year % 4)) && (month > 2))
            nTheDate = nTheDate + 1;

        if((!((year - 1) % 4)))
            nTheDate = nTheDate + 1;
    }
    else if (month == springFestivalMonth)
    {
        if (day < springFestivalDaty) {
            nMonTemp--;
            nTheDate = 365 * 1 + day + monthAdd[month - 1] - 31 * ((springFestival[nMonTemp] / 100) - 1) - springFestival[nMonTemp] % 100 + 1;

            if((!(year % 4)) && (month > 2))
                nTheDate = nTheDate + 1;

            if((!((year-1) % 4)))
                nTheDate = nTheDate + 1;
        }
        else {
            nTheDate = day + monthAdd[month - 1] - 31 * ((springFestival[nMonTemp] / 100) - 1) - springFestival[nMonTemp] % 100 + 1;

            if((!(year % 4)) && (month > 2))
                nTheDate = nTheDate + 1;
        }
    }else{
        nTheDate = day + monthAdd[month - 1] - 31 * ((springFestival[nMonTemp] / 100) - 1) - springFestival[nMonTemp] % 100 + 1;
        if((!(year % 4)) && (month > 2))
            nTheDate = nTheDate + 1;
    }
    /*--计算农历天干、地支、月、日---*/
    nIsEnd = 0;

    while(nIsEnd != 1)  {
        if(nLunarData[nMonTemp] < 4095)
            k = 11;
        else
            k = 12;
        n = k;
        while(n >= 0)   {
            // 获取wNongliData(m)的第n个二进制位的值
            nBit = nLunarData[nMonTemp];

            nBit = nBit >> n;
            nBit = nBit % 2;
            if (nTheDate <= (29 + nBit))    {
                nIsEnd = 1;
                break;
            }

            nTheDate = nTheDate - 29 - nBit;
            n = n - 1;
        }

        if(nIsEnd)
            break;

        nMonTemp = nMonTemp + 1;
    }

    // 农历的年月日
    year = 1969 + nMonTemp -1;
    month = k - n + 1;
    day = nTheDate;

    if (k == 12)  {
        if (month == (nLunarData[nMonTemp] / 65536) + 1)
            month = 1 - month;
        else if (month > (nLunarData[nMonTemp] / 65536) + 1)
            month = month - 1;
    }

    // 显示装换的农历
    // only day == 1 ,return month name
    if (1 == day) {
        if(month < 1){
            strDate = "闰" + monName[month * -1];
            return strDate;
        }

        // 公历节日
        if ("" != strDate) return strDate;

        // 计算农历节日
        strDate = lunarFestival(month, day);

        // 如果有节日，直接显示
        if ("" == strDate) {
            // 如果当天不是24节气，显示农历日子
            strDate = strSolarTerms;
            if ("" == strDate) {
                strDate = monName[month];
            }
        }

    } else {
        // 公历节日
        if ("" != strDate) return strDate;

        // 计算农历节日
        strDate = lunarFestival(month, day);
        // 如果有节日，直接显示
        if ("" == strDate) {
            // 如果当天不是24节气，显示农历日子
            strDate = strSolarTerms;
            if ("" == strDate) {
                strDate = dayName[day];
            }
        }
    }

    return strDate;
}

/**
 * @brief Date::getLunarMonAndDay 响应点击时的农历显示
 * @param year
 * @param month
 * @param day
 * @return
 */
QString Date::getLunarMonAndDay(int year, int month, int day)
{
    int nTheDate, nIsEnd, nMonTemp, k, n, nBit;
    QString strDate = "";
    /*现在计算农历：获得当年春节的公历日期（比如：2015年春节日期为（2月19日）），
            以此为分界点，2.19前面的农历是2014年农历（用2014年农历数据来计算），
            2.19以及之后的日期，农历为2015年农历（用2015年农历数据来计算）。*/
    nMonTemp = year - 1968;
    int springFestivalMonth = springFestival[nMonTemp] / 100;
    int springFestivalDaty = springFestival[nMonTemp] % 100;

    if(month < springFestivalMonth )
    {
        nMonTemp--;
        nTheDate = 365 * 1 + day + monthAdd[month - 1] - 31 * ((springFestival[nMonTemp] / 100) - 1) - springFestival[nMonTemp] % 100 + 1;

        if((!(year % 4)) && (month > 2))
            nTheDate = nTheDate + 1;

        if((!((year - 1) % 4)))
            nTheDate = nTheDate + 1;
    }
    else if (month == springFestivalMonth)
    {
        if (day < springFestivalDaty) {
            nMonTemp--;
            nTheDate = 365 * 1 + day + monthAdd[month - 1] - 31 * ((springFestival[nMonTemp] / 100) - 1) - springFestival[nMonTemp] % 100 + 1;

            if((!(year % 4)) && (month > 2))
                nTheDate = nTheDate + 1;

            if((!((year-1) % 4)))
                nTheDate = nTheDate + 1;
        }
        else {
            nTheDate = day + monthAdd[month - 1] - 31 * ((springFestival[nMonTemp] / 100) - 1) - springFestival[nMonTemp] % 100 + 1;

            if((!(year % 4)) && (month > 2))
                nTheDate = nTheDate + 1;
        }
    }else{
        nTheDate = day + monthAdd[month - 1] - 31 * ((springFestival[nMonTemp] / 100) - 1) - springFestival[nMonTemp] % 100 + 1;
        if((!(year % 4)) && (month > 2))
            nTheDate = nTheDate + 1;
    }
    /*--计算农历天干、地支、月、日---*/
    nIsEnd = 0;

    while(nIsEnd != 1)  {
        if(nLunarData[nMonTemp] < 4095)
            k = 11;
        else
            k = 12;
        n = k;
        while(n >= 0)   {
            // 获取wNongliData(m)的第n个二进制位的值
            nBit = nLunarData[nMonTemp];

            nBit = nBit >> n;
            nBit = nBit % 2;
            if (nTheDate <= (29 + nBit))    {
                nIsEnd = 1;
                break;
            }

            nTheDate = nTheDate - 29 - nBit;
            n = n - 1;
        }

        if(nIsEnd)
            break;

        nMonTemp = nMonTemp + 1;
    }

    // 农历的年月日
    year = 1969 + nMonTemp -1;
    month = k - n + 1;
    day = nTheDate;

    if (k == 12)  {
        if (month == (nLunarData[nMonTemp] / 65536) + 1)
            month = 1 - month;
        else if (month > (nLunarData[nMonTemp] / 65536) + 1)
            month = month - 1;
    }

    // 显示装换的农历
    // only day == 1 ,return month name
    if (1 == day) {
        if (month < 1){
            strDate = tr("闰") + monName[month * -1];
            return strDate;
        }

        strDate = monName[month];

    } else {
        strDate = monName[month]+ dayName[day];
    }

    return strDate;
}

/**
 * @brief Date::getLunarTime  计算今年是什么年如 ：甲子年
 * @param year
 * @return
 */
QString Date::getLunarTime(int year)
{
    int ntemp = 0;
    // 农历时辰
    QString strTime = "";
    if (year > T_D_YAER) {
        ntemp = year - T_D_YAER;
        strTime.append(strTiangan[ntemp % 10]);
        strTime.append(strDizhi[ntemp % 12]);
        strTime.append(strAnimal[ntemp % 12]);
        strTime.append(tr("年"));
    }

    return strTime;
}


/**
 * @brief Date::holiday 公历假日
 * @param month
 * @param day
 * @return
 */
QString Date::holiday(int month, int day)
{
    int temp = (month << 8) | day;
    QString strHoliday = "";
    switch (temp) {
    case 0x0101: strHoliday = tr("元旦");  break;
    case 0x020E: strHoliday = tr("情人节"); break;
    case 0x0308: strHoliday = tr("妇女节"); break;
    case 0x0401: strHoliday = tr("愚人节"); break;
    case 0x0501: strHoliday = tr("劳动节"); break;
    case 0x0504: strHoliday = tr("青年节"); break;
    case 0x0601: strHoliday = tr("儿童节"); break;
    case 0x0701: strHoliday = tr("建党节"); break;
    case 0x0801: strHoliday = tr("建军节"); break;
    case 0x090A: strHoliday = tr("教师节"); break;
    case 0x0A01: strHoliday = tr("国庆节"); break;
    case 0x0C18: strHoliday = tr("圣诞节"); break;
    default: break;
    }

    return strHoliday;
}

/**
 * @brief Date::chineseTwentyFourDay 计算24节气
 * @param year
 * @param month
 * @param day
 * @return
 */
QString Date::solarTerms(int year, int month, int day){
    int dayTemp = 0;
    int index = (year - 1970) * 12 + month - 1;

    if (day < 15) {
        dayTemp = 15 - day;
        if((chineseTwentyFourData[index] >> 4) == dayTemp)
            return solarTerm[2 * (month - 1)];
        else
            return "";

    } else if (day > 15) {
        dayTemp = day - 15;
        if((chineseTwentyFourData[index] & 0x0f) == dayTemp)
            return solarTerm[2 * (month - 1) + 1];
    }

    return "";
}

/**
 * @brief Date::lunarFestival 农历春节节日
 * @param month
 * @param day
 * @return 节日
 */
QString Date::lunarFestival(int month, int day) {
    int temp = (month << 8) | day;
    QString strFestival = "";
    switch (temp) {
    case 0x0101: strFestival = tr("春节");  break;
    case 0x010F: strFestival = tr("元宵节"); break;
    case 0x0202: strFestival = tr("龙抬头"); break;
    case 0x0505: strFestival = tr("端午节"); break;
    case 0x0707: strFestival = tr("七夕节"); break;
    case 0x080F: strFestival = tr("中秋节"); break;
    case 0x0909: strFestival = tr("重阳节"); break;
    case 0x0C08: strFestival = tr("腊八节"); break;
    default: break;
    }

    return strFestival;
}
