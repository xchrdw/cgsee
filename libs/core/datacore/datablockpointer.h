#pragma once
#include "datablock.h"

#include <memory>
#include <type_traits>
#include <functional>

template <typename T, bool> struct TriggerOnDelete;

class DeleteListener: public QObject
{
    Q_OBJECT
public:
    DeleteListener(QObject *listenTo, const std::function<void()>& doIt):
        doMe(doIt)
    {
        connect(listenTo, SIGNAL(destroyed(QObject*)), SLOT(trigger()));
    }
protected slots:
    void trigger()
    {
        doMe();
    }
protected:
    std::function<void()> doMe;
};

template <typename T, bool shouldDelete = false>
class DataBlockPointer
{
    DataBlockPointer(T* obj);

    typedef T value_type;
    typedef DataBlockPointer<T, shouldDelete> t_myself;
    
    T & operator*() const;
    std::shared_ptr<T> operator->() const;

    t_myself presumeUnchanged() const;

protected:
    TriggerOnDelete<T, shouldDelete> m_trigger;
    // shared pointer with unusual "deleter" object
    // it doesn't destroy the data block, but instead will trigger an update 
    // message
    std::shared_ptr<T> m_pointee;
    std::shared_ptr<DeleteListener> m_deleteGuard;

    bool m_trackChanges;

    void markAsChanged() const;

    void initializeGuard(QObject* obj);
    void initializeGuard(void*);
};

#include "datablockpointer_impl.inl"