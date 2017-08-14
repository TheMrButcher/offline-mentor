#ifndef SMALLBIMAP_H
#define SMALLBIMAP_H

#include <QList>

template <typename T1, typename T2>
class SmallBimap {
public:
    SmallBimap() {}

    SmallBimap(const QList<T1>& values1, const QList<T2>& values2)
        : values1(values1)
        , values2(values2)
    {
        if (values1.size() != values2.size()) {
            int size = std::min(values1.size(), values2.size());
            this->values1.erase(this->values1.begin() + size, this->values1.end());
            this->values2.erase(this->values2.begin() + size, this->values2.end());
        }
    }

    void insert(const T1& t1, const T2& t2)
    {
        values1.append(t1);
        values2.append(t2);
    }

    T2 valueByFirst(const T1& t) const
    {
        return valueByFirstOr(t, T2());
    }

    T2 valueByFirstOr(const T1& t, const T2& defaultValue) const
    {
        for (int i = 0; i < values1.size(); ++i)
            if (values1[i] == t)
                return values2[i];
        return defaultValue;
    }

    T1 valueBySecond(const T2& t) const
    {
        return valueBySecondOr(t, T1());
    }

    T1 valueBySecondOr(const T2& t, const T1& defaultValue) const
    {
        for (int i = 0; i < values2.size(); ++i)
            if (values2[i] == t)
                return values1[i];
        return defaultValue;
    }

private:
    QList<T1> values1;
    QList<T2> values2;
};

#endif // SMALLBIMAP_H
