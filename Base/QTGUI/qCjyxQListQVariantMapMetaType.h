
#ifndef _qCjyxQListQVariantMapMetaType_h
#define _qCjyxQListQVariantMapMetaType_h

#include <QMetaType>
#include <QVariant>

template <typename T> class QList;
template <class Key, class T> class QMap;
typedef QMap<QString, QVariant> QVariantMap;

Q_DECLARE_METATYPE(QList<QVariantMap>);

#endif
