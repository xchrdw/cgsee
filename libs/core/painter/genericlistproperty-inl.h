
template <class ValueClass>
GenericListProperty<ValueClass>::GenericListProperty()
:   AbstractListProperty()
,   m_map(new QMap<QString, ValueClass *>())
,   m_selectedKey(nullptr)
{
}

template <class ValueClass>
GenericListProperty<ValueClass>::GenericListProperty(QString name)
:   AbstractListProperty(name)
,   m_map(new QMap<QString, ValueClass *>())
,   m_selectedKey(nullptr)
{
}

template <class ValueClass>
GenericListProperty<ValueClass>::GenericListProperty(QString name, QString description)
:   AbstractListProperty(name, description)
,   m_map(new QMap<QString, ValueClass *>())
,   m_selectedKey(nullptr)
{
}

template <class ValueClass>
GenericListProperty<ValueClass>::~GenericListProperty()
{
    qDeleteAll(m_map->begin(), m_map->end());
    delete m_map;
    delete m_selectedKey;
}

template <class ValueClass>
QStringList GenericListProperty<ValueClass>::descriptionList() const
{
    for (auto key : m_map->keys())
        qDebug("%s", qPrintable(key));
    return m_map->keys();
}

template <class ValueClass>
QString GenericListProperty<ValueClass>::selectedDescription() const
{
    return m_selectedKey ? *m_selectedKey : "";
}

template <class ValueClass>
bool GenericListProperty<ValueClass>::select(QString description)
{
    if (m_map->contains(description)) {
        m_selectedKey = new QString(description);
        return true;
    } else
        return false;
}

template <class ValueClass>
QList<ValueClass> GenericListProperty<ValueClass>::valueList()
{
    return m_map->values();
}

template <class ValueClass>
ValueClass * GenericListProperty<ValueClass>::selectedValue()
{
    return this->value(*m_selectedKey);
}

template <class ValueClass>
ValueClass * GenericListProperty<ValueClass>::value(QString key)
{
    return this->value(key, nullptr);
}

template <class ValueClass>
bool GenericListProperty<ValueClass>::select(ValueClass * value)
{
    QList<QString> keys = m_map->keys(value);
    if (keys.empty())
        return false;
    else {
        m_selectedKey = keys.first();
        return true;
    }
}

template <class ValueClass>
bool GenericListProperty<ValueClass>::insert(QString key, ValueClass * value)
{
    if (m_map->find(key) != m_map->end())
        return false;
    else {
        m_map->insert(key, value);
        return true;
    }
}

template <class ValueClass>
bool GenericListProperty<ValueClass>::remove(QString key)
{
    return m_map->remove(key);
}

template <class ValueClass>
bool GenericListProperty<ValueClass>::remove(ValueClass * value)
{
    QList<QString> keys = m_map->keys(value);
    if (keys.empty())
        return false;
    else {
        for (QString key : keys)
            m_map->remove(key);
        return true;
    }
}
