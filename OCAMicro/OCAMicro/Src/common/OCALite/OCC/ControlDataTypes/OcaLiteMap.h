/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMap
 *
 */

#ifndef OCALITEMAP_H
#define OCALITEMAP_H

// ---- Include system wide include files ----
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
 * One element of an OcaLiteMap.
 * @tparam  KeyDataType     The data type of the key elements in the map.
 * @tparam  ValueDataType   The data type of the value elements in the map.
 */
template <class KeyDataType, class ValueDataType>
class OcaLiteMapItem
{
public:
    /**
     * Default constructor.
     */
    OcaLiteMapItem()
        : m_key(),
          m_value()
    {
    }

    /**
     * Constructor.
     *
     * @param[in]   key         The key of the item.
     * @param[in]   value       The value of the item.
     */
    OcaLiteMapItem(const KeyDataType& key, const ValueDataType& value)
        : m_key(key),
          m_value(value)
    {
    }

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteMapItem(const ::OcaLiteMapItem<KeyDataType, ValueDataType>& source)
        : m_key(source.m_key),
          m_value(source.m_value)
    {
    }

    /**
     * Destructor.
     */
    virtual ~OcaLiteMapItem()
    {
    }

    /**
     * Getter for Key.
     *
     * @return The key of the item.
     */
    const KeyDataType& GetKey() const
    {
        return m_key;
    }

    /**
     * Getter for Value.
     *
     * @return The value of the item.
     */
    const ValueDataType& GetValue() const
    {
        return m_value;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMapItem<KeyDataType, ValueDataType>& operator=(const ::OcaLiteMapItem<KeyDataType, ValueDataType>& source)
    {
        if (this != &source)
        {
            m_key = source.m_key;
            m_value = source.m_value;
        }

        return *this;
    }

private:
    /** The key of the item. */
    KeyDataType     m_key;

    /** The value of the item. */
    ValueDataType   m_value;
};

/**
 * Class representing a map of keys to values.
 * @tparam  KeyDataType     The data type of the key elements in the map.
 * @tparam  ValueDataType   The data type of the value elements in the map.
 */
template <class KeyDataType, class ValueDataType>
class OcaLiteMap : public ::IOcaLiteMarshal
{
public:
    /**
     * Iterator for the map.
     */
    class Iterator
    {
    public:
        /**
         * Constructor.
         *
         * @param[in]   iter    The iterator for the underlying std::map.
         */
        explicit Iterator(typename std::map<KeyDataType, ValueDataType>::const_iterator iter)
            : m_iter(iter)
        {
        }

        /**
         * Destructor
         */
        virtual ~Iterator()
        {
        }

        /**
         * Returns the key of the element pointed to.
         *
         * @return The key of the element pointed to.
         */
        const KeyDataType& GetKey() const
        {
            return m_iter->first;
        }

        /**
         * Returns the value of the element pointed to.
         *
         * @return The value of the element pointed to.
         */
        const ValueDataType& GetValue() const
        {
            return m_iter->second;
        }

        /**
         * The is-equal operator. The list is equal if all elements in
         * both lists are equal too.
         *
         * @param[in]   rhs         The object to compare with.
         * @return True if the classes are equal.
         */
        bool operator==(const Iterator& rhs) const
        {
            bool result(m_iter == rhs.m_iter);

            return result;
        }

        /**
         * The is-not-equal operator.
         *
         * @param[in]   rhs         The object to compare with.
         * @return True if the classes are not equal.
         */
        bool operator!=(const Iterator& rhs) const
        {
            return !(operator==(rhs));
        }

        /**
         * Increment operator.
         *
         * @return The reference to the iterator.
         */
        Iterator& operator++()
        {
            ++m_iter;
            return *this;
        }

        /**
         * Dereference operator.
         *
         * @return The map item the iterator points to.
         */
        ::OcaLiteMapItem<KeyDataType, ValueDataType> operator*() const
        {
            ::OcaLiteMapItem<KeyDataType, ValueDataType> mapItem(m_iter->first, m_iter->second);
            return mapItem;
        }

    private:
        /** The iterator for the underlying std::map. */
        typename std::map<KeyDataType, ValueDataType>::const_iterator   m_iter;
    };

    /** Default constructor. */
    OcaLiteMap()
        : ::IOcaLiteMarshal(),
          m_items()
    {
    }

    /**
     * Constructor.
     *
     * @param[in]   cnt         The number of entries (key value pairs) in the map.
     * @param[in]   items       The array of items of the map.
     */
    OcaLiteMap(::OcaUint16 cnt, const ::OcaLiteMapItem<KeyDataType, ValueDataType>* items)
        : ::IOcaLiteMarshal(),
          m_items()
    {
        assert(NULL != items);

        for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < cnt; i++)
        {
            static_cast<void>(m_items.insert(std::pair<KeyDataType, ValueDataType>(items[i].GetKey(), items[i].GetValue())));
        }
    }

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteMap(const ::OcaLiteMap<KeyDataType, ValueDataType>& source)
        : ::IOcaLiteMarshal(source),
          m_items(source.m_items)
    {
    }

    /**
     * Destructor.
     */
    virtual ~OcaLiteMap()
    {
    }

    /**
     * Getter for Count.
     *
     * @return The number of entries (key value pairs) in the map.
     */
    ::OcaUint16 GetCount() const
    {
        return static_cast< ::OcaUint16>(m_items.size());
    }

    /**
     * Indicates if the map contains an item with the given key.
     *
     * @param[in]   key         The key to find.
     * @return True if the map contains the key, false otherwise.
     */
    ::OcaBoolean ContainsKey(const KeyDataType& key) const
    {
        typename std::map<KeyDataType, ValueDataType>::const_iterator it(m_items.find(key));

        return static_cast< ::OcaBoolean>(it != m_items.end());
    }

    /**
     * Indicates if the map contains an item with the given value.
     *
     * @param[in]   value       The value to find.
     * @return True if the map contains the value, false otherwise.
     */
    ::OcaBoolean ContainsValue(const ValueDataType& value) const
    {
        typename std::map<KeyDataType, ValueDataType>::const_iterator it(m_items.begin());
        bool bFound(false);

        while (!bFound && (it != m_items.end()))
        {
            bFound = (it->second == value);
            ++it;
        }

        return static_cast< ::OcaBoolean>(bFound);
    }

    /**
     * Getter for the value with the given key.
     *
     * @param[in]   key         The key of the item to get.
     * @return The value with the given key.
     */
    const ValueDataType& GetValue(const KeyDataType& key) const
    {
        typename std::map<KeyDataType, ValueDataType>::const_iterator it(m_items.find(key));
        assert(it != m_items.end());

        return it->second;
    }

    /**
     * Adds an element to the end of the map.
     * @note It is not allowed to add an element when the maximum number of
     *       elements is already in the list. This is guarded by an
     *       assert.
     *
     * @param[in]   element     The element to add to the map.
     */
    void Add(const ::OcaLiteMapItem<KeyDataType, ValueDataType>& element)
    {
        assert(static_cast< ::OcaUint16>(m_items.size()) < (std::numeric_limits< ::OcaUint16>::max)());
        static_cast<void>(m_items.insert(std::pair<KeyDataType, ValueDataType>(element.GetKey(), element.GetValue())));
    }

    /**
     * Removes the element with the given key from the map.
     *
     * @param[in]   key         The key of the item to remove.
     */
    void Remove(const KeyDataType& key)
    {
        typename std::map<KeyDataType, ValueDataType>::iterator it(m_items.find(key));

        if (it != m_items.end())
        {
            static_cast<void>(m_items.erase(it));
        }
    }

    /**
    * Removes the element with the given key from the map.
    *
    * @param[in]   key         The key of the item to remove.
    */
    void Remove(const KeyDataType& key, const ValueDataType& value)
    {
        typename std::map<KeyDataType, ValueDataType>::iterator it(m_items.begin());

        while (it != m_items.end())
        {
            if ((it->first == key) && (it->second == value))
            {
                static_cast<void>(m_items.erase(it));
                break;
            }
            ++it;
        }
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * 
     * @return A reference to this object.
     */
    ::OcaLiteMap<KeyDataType, ValueDataType>& operator=(const ::OcaLiteMap<KeyDataType, ValueDataType>& source)
    {
        if (this != &source)
        {
            m_items = source.m_items;
        }

        return *this;
    }

    /**
     * Clears the map.
     */
    void Clear()
    {
        m_items.clear();
    }

    /**
     * Returns the iterator to the start of the map.
     *
     * @return The iterator to the start of the map.
     */
    Iterator Begin() const
    {
        return Iterator(m_items.begin());
    }

    /**
     * Returns the iterator past the last element of the map.
     *
     * @return The iterator past the last element of the map.
     */
    Iterator End() const
    {
        return Iterator(m_items.end());
    }

    /**
     * The is-equal operator. The list is equal if all elements in
     * both lists are equal too.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteMap<KeyDataType, ValueDataType>& rhs) const
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
    bool operator!=(const ::OcaLiteMap<KeyDataType, ValueDataType>& rhs) const
    {
        return !(operator==(rhs));
    }

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
    {
        writer.Write(static_cast< ::OcaUint16>(m_items.size()), destination);
        for (typename std::map<KeyDataType, ValueDataType>::const_iterator it(m_items.begin());
             it != m_items.end();
             ++it)
        {
            MarshalValue<KeyDataType>(it->first, destination, writer);
            MarshalValue<ValueDataType>(it->second, destination, writer);
        }
    }

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
    {
        m_items.clear();

        ::OcaUint16 cnt;
        bool result(reader.Read(bytesLeft, source, cnt));
        for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); (i < cnt) && result; i++)
        {
            KeyDataType key;
            result = result && UnmarshalValue<KeyDataType>(key, bytesLeft, source, reader);
            ValueDataType value;
            result = result && UnmarshalValue<ValueDataType>(value, bytesLeft, source, reader);
            static_cast<void>(m_items.insert(std::pair<KeyDataType, ValueDataType>(key, value)));
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
        for (typename std::map<KeyDataType, ValueDataType>::const_iterator it(m_items.begin());
             it != m_items.end();
             ++it)
        {
            length += GetSizeValue<KeyDataType>(it->first, writer);
            length += GetSizeValue<ValueDataType>(it->second, writer);
        }

        return length;
    }

private:
    /** The items of the map. */
    std::map<KeyDataType, ValueDataType>    m_items;
};

#endif // OCALITEMAP_H
