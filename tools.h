#ifndef UTILITY_H
#define UTILITY_H
#include <array>
#include <QChar>
QChar int2localeChar (int x)
{
    static std::array<wchar_t, 11> map{{L'〇',L'一', L'二', L'三', L'四', L'五', L'六', L'七', L'八', L'九', L'十'}};
    return map[x];
}

#endif // UTILITY_H
