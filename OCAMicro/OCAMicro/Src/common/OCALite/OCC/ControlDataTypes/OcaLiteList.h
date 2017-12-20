/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteList
 *
 */

#ifndef OCALITELIST_H
#define OCALITELIST_H

// ---- Include system wide include files ----
#include <algorithm>
#include <limits>
#include <assert.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteString.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Class representing a list of items.
 * @tparam  DataType        The data type of the elements in the list.
 */
template <class DataType> class OcaLiteList : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteList()
        : ::IOcaLiteMarshal(),
          m_items()
    {
    }

    /**
     * Constructor.
     *
     * @param[in]   cnt         The number of entries in the list.
     * @param[in]   items       The array of items of the list.
     */
    OcaLiteList(::OcaUint16 cnt, const DataType* items)
        : ::IOcaLiteMarshal(),
          m_items()
    {
        assert(NULL != items);

        m_items.reserve(static_cast<size_t>(cnt));
        for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < cnt; i++)
        {
            m_items.push_back(items[i]);
        }
    }

    /**
     * Constructor to convert a list of type FromDataType to a list of DataType
     *
     * @param[in]   source      The list to copy from.
     */
    template <class FromDataType> explicit
    OcaLiteList(const ::OcaLiteList<FromDataType>& source)
        : ::IOcaLiteMarshal(),
          m_items()
    {
        m_items.reserve(static_cast<size_t>(source.GetCount()));
        for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < source.GetCount(); i++)
        {
            m_items.push_back(source.GetItem(i));
        }
    }

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteList(const ::OcaLiteList<DataType>& source)
        : ::IOcaLiteMarshal(source),
          m_items(source.m_items)
    {
    }

    /**
     * Destructor.
     */
    virtual ~OcaLiteList()
    {
    }

    /**
     * Getter for Count.
     *
     * @return The number of entries in the list.
     */
    ::OcaUint16 GetCount() const
    {
        return static_cast< ::OcaUint16>(m_items.size());
    }

    /**
     * Getter for the item on the given position.
     * @note The returned reference is only valid until an Add, Insert
     *       or Remove operation is performed on the list.
     * @note For a list of OcaBoolean, the returned reference is also only
     *       valid until another item is retrieved from the list.
     *
     * @param[in]   position    The position to get the item from
     * @return The item on the given position.
     */
    const DataType& GetItem(::OcaUint16 position) const
    {
        assert(position < static_cast< ::OcaUint16>(m_items.size()));

        const DataType& element(m_items[static_cast<size_t>(position)]);
        return element;
    }

    /**
     * Getter for the item on the given position.
     * @note The returned reference is only valid until an Add, Insert
     *       or Remove operation is performed on the list.
     * @note For a list of OcaBoolean, the returned reference is also only
     *       valid until another item is retrieved from the list.
     *
     * @param[in]   position    The position to get the item from
     * @return The item on the given position.
     */
    DataType& GetItem(::OcaUint16 position)
    {
        assert(position < static_cast< ::OcaUint16>(m_items.size()));

        DataType& element(m_items[static_cast<size_t>(position)]);
        return element;
    }

    /**
     * Adds an element to the end of the list.
     * @note It is not allowed to add an element when the maximum number of
     *       elements is already in the list. This is guarded by an
     *       assert.
     *
     * @param[in]   element     The element to add to the list.
     */
    void Add(const DataType& element)
    {
        Insert(GetCount(), element);
    }

    /**
     * Inserts an element on the given position in the list.
     * If the position is past the end of the list, the element is added
     * at the end of the list.
     * @note It is not allowed to insert an element when the maximum number of
     *       elements is already in the list. This is guarded by an
     *       assert.
     *
     * @param[in]   position    The position to add the element.
     * @param[in]   element     The element to add to the list.
     */
    void Insert(::OcaUint16 position, const DataType& element)
    {
        assert(static_cast< ::OcaUint16>(m_items.size()) < (std::numeric_limits< ::OcaUint16>::max)());
        assert(position <= static_cast< ::OcaUint16>(m_items.size()));

        typename std::vector<DataType>::iterator it(m_items.begin());
        std::advance(it, position);
        static_cast<void>(m_items.insert(it, element));
    }

    /**
     * Removes the element at the given position in the list.
     *
     * @param[in]   position    The position to remove the element from.
     */
    void RemovePosition(::OcaUint16 position)
    {
        assert(position < static_cast< ::OcaUint16>(m_items.size()));

        typename std::vector<DataType>::iterator it(m_items.begin());
        std::advance(it, position);
        static_cast<void>(m_items.erase(it));
    }

    /**
     * Removes the first occurrence of the given element from the list.
     *
     * @param[in]   element     The element to remove from the list.
     */
    void RemoveElement(const DataType& element)
    {
        typename std::vector<DataType>::iterator it(std::find(m_items.begin(), m_items.end(), element));
        if (m_items.end() != it)
        {
            static_cast<void>(m_items.erase(it));
        }
    }

    /**
     * Indicates if the list contains the given element.
     *
     * @param[in]   element     The element to find in the list.
     * @return True if the element is already in the list, false otherwise.
     */
    ::OcaBoolean Contains(const DataType& element)
    {
        typename std::vector<DataType>::iterator it(std::find(m_items.begin(), m_items.end(), element));
        return static_cast< ::OcaBoolean>(m_items.end() != it);
    }

    /**
     * Clears the list.
     */
    void Clear()
    {
        m_items.clear();
    }

    /**
     * Reserves spaces in the list for the given number of items and allocates enough memory to hold
     * this information. This does not in a change in the actual number of items in the list.
     * When a value is given that is less than the current number of items in the list, memory is
     * reserved for the current number of items in the list.
     *
     * @param[in]   reserveCount    The number of items to reserve memory for.
     */
    void Reserve(::OcaUint16 reserveCount)
    {
        ::OcaUint16 size(static_cast< ::OcaUint16>(m_items.size()));
        if (reserveCount > size)
        {
            m_items.reserve(static_cast<size_t>(reserveCount));
        }
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteList<DataType>& operator=(const ::OcaLiteList<DataType>& source)
    {
        if (this != &source)
        {
            m_items = source.m_items;
        }

        return *this;
    }

    /**
     * The is-equal operator. The list is equal if all elements in
     * both lists are equal too.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteList<DataType>& rhs) const
    {
        bool result(m_items == rhs.m_items);

        return result;
    }

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteList<DataType>& rhs) const
    {
        return !(operator==(rhs));
    }

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
    {
        writer.Write(static_cast< ::OcaUint16>(m_items.size()), destination);
        for (typename std::vector<DataType>::const_iterator it(m_items.begin());
             it != m_items.end();
             ++it)
        {
            MarshalValue(*it, destination, writer);
        }
    }

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
    {
        m_items.clear();

        ::OcaUint16 cnt;
        bool result(reader.Read(bytesLeft, source, cnt));
        for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); (i < cnt) && result; i++)
        {
            DataType element;
            result = result && UnmarshalValue(element, bytesLeft, source, reader);
            m_items.push_back(element);
        }
        
        if (!result)
        {
            m_items.clear();
        }

        return result;
    }

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const
    {
        ::OcaUint32 length(writer.GetSize(static_cast< ::OcaUint16>(m_items.size())));
        for (typename std::vector<DataType>::const_iterator it(m_items.begin());
             it != m_items.end();
             ++it)
        {
            length += GetSizeValue(*it, writer);
        }

        return length;
    }

private:
    /** The items of the list. */
    std::vector<DataType>   m_items;
};

// ---- Specialized Template Class Definition ----

// Hide the specialized template class for the documentation.
#ifndef DOXYGEN

template<> class OcaLiteList< ::OcaBoolean> : public ::IOcaLiteMarshal
{
public:
    OcaLiteList();

    OcaLiteList(::OcaUint16 cnt, const ::OcaBoolean* items);

    template <class FromDataType> explicit
    OcaLiteList(const ::OcaLiteList<FromDataType>& source);

    OcaLiteList(const ::OcaLiteList< ::OcaBoolean>& source);

    virtual ~OcaLiteList();

    ::OcaUint16 GetCount() const;

    ::OcaBoolean GetItem(::OcaUint16 position) const;

    ::OcaBoolean GetItem(::OcaUint16 position);

    void Add(::OcaBoolean element);

    void Insert(::OcaUint16 position, ::OcaBoolean element);

    void RemovePosition(::OcaUint16 position);

    void RemoveElement(::OcaBoolean element);

    ::OcaBoolean Contains(::OcaBoolean element);

    void Clear();

    void Reserve(::OcaUint16 reserveCount);

    ::OcaLiteList< ::OcaBoolean>& operator=(const ::OcaLiteList< ::OcaBoolean>& source);

    bool operator==(const ::OcaLiteList< ::OcaBoolean>& rhs) const;

    bool operator!=(const ::OcaLiteList< ::OcaBoolean>& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    std::vector< ::OcaBoolean>  m_items;
};

#endif

#endif // OCALITELIST_H
