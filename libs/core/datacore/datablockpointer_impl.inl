
template <typename T, bool shouldDelete>
struct TriggerOnDelete
{
    std::shared_ptr<bool> m_changed;
    DataBlock* m_trigger;
    TriggerOnDelete(DataBlock* trigger):
        m_changed(new bool(false))
    ,   m_trigger(trigger)
    {
    }
    void operator()(typename std::enable_if< std::is_base_of<DataBlock, T>::value, T*>::type dataBlock)
    {
        if (*m_changed)
        {
            if (m_trigger)
                m_trigger->triggerUpdatedSignal();
            else 
                dataBlock->triggerUpdatedSignal();
        }
        if (shouldDelete)
            delete dataBlock;
    }
    void operator()(typename std::enable_if<! std::is_base_of<DataBlock, T>::value, T*>::type dataBlock)
    {
        if (*m_changed && m_trigger)
            m_trigger->triggerUpdatedSignal();
        if (shouldDelete)
            delete dataBlock;
    }
};

template <typename T, bool shouldDelete>
DataBlockPointer<T, shouldDelete>::DataBlockPointer(T* obj):
    m_trigger(TriggerOnDelete(obj))
,   m_pointee(shared_ptr<T>(obj, m_trigger))
,   m_trackChanges(true)
{
    initializeGuard(obj);
}

template <typename T, bool shouldDelete>
T& DataBlockPointer<T, shouldDelete>::operator*() const
{
    markAsChanged();
    return *m_pointee;
}

template <typename T, bool shouldDelete>
std::shared_ptr<T> DataBlockPointer<T, shouldDelete>::operator->()const
{
    markAsChanged();
    return m_pointee;
}

template <typename T, bool shouldDelete>
DataBlockPointer<T, shouldDelete> DataBlockPointer<T, shouldDelete>::presumeUnchanged() const
{
    DataBlockPointer<T, shouldDelete> newp(*this);
    newp.m_trackChanges = false;
    return newp;
}

template <typename T, bool shouldDelete>
void DataBlockPointer<T, shouldDelete>::markAsChanged() const
{
    if (!std::is_const<T>::value && m_trackChanges)
        *(m_trigger.m_changed) = true;
}

template <typename T, bool shouldDelete>
void DataBlockPointer<T, shouldDelete>::initializeGuard(QObject* obj)
{
    m_deleteGuard = make_shared<DeleteListener>(obj, [this](){m_trigger.reset()});
}

template <typename T, bool shouldDelete>
void DataBlockPointer<T, shouldDelete>::initializeGuard(void*)
{

}