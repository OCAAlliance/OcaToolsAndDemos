/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteList
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteList.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

// ---- Specialized Template Class Implementation ----

#ifndef DOXYGEN

OcaLiteList< ::OcaBoolean>::OcaLiteList()
    : ::IOcaLiteMarshal(),
      m_items()
{
}

OcaLiteList< ::OcaBoolean>::OcaLiteList(::OcaUint16 cnt, const ::OcaBoolean* items)
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

template <class FromDataType>
OcaLiteList< ::OcaBoolean>::OcaLiteList(const ::OcaLiteList<FromDataType>& source)
    : ::IOcaLiteMarshal(),
      m_items()
{
    m_items.reserve(static_cast<size_t>(source.GetCount()));
    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < source.GetCount(); i++)
    {
        m_items.push_back(source.GetItem(i));
    }
}

OcaLiteList< ::OcaBoolean>::OcaLiteList(const ::OcaLiteList< ::OcaBoolean>& source)
    : ::IOcaLiteMarshal(source),
      m_items(source.m_items)
{
}

OcaLiteList< ::OcaBoolean>::~OcaLiteList()
{
}

::OcaUint16 OcaLiteList< ::OcaBoolean>::GetCount() const
{
    return static_cast< ::OcaUint16>(m_items.size());
}

::OcaBoolean OcaLiteList< ::OcaBoolean>::GetItem(::OcaUint16 position) const
{
    assert(position < static_cast< ::OcaUint16>(m_items.size()));

    return m_items[static_cast<size_t>(position)];
}

::OcaBoolean OcaLiteList< ::OcaBoolean>::GetItem(::OcaUint16 position)
{
    assert(position < static_cast< ::OcaUint16>(m_items.size()));

    return m_items[static_cast<size_t>(position)];
}

void OcaLiteList< ::OcaBoolean>::Add(::OcaBoolean element)
{
    Insert(GetCount(), element);
}

void OcaLiteList< ::OcaBoolean>::Insert(::OcaUint16 position, ::OcaBoolean element)
{
    assert(static_cast< ::OcaUint16>(m_items.size()) < (std::numeric_limits< ::OcaUint16>::max)());
    assert(position <= static_cast< ::OcaUint16>(m_items.size()));

    std::vector< ::OcaBoolean>::iterator it(m_items.begin());
    std::advance(it, position);
    static_cast<void>(m_items.insert(it, element));
}

void OcaLiteList< ::OcaBoolean>::RemovePosition(::OcaUint16 position)
{
    assert(position < static_cast< ::OcaUint16>(m_items.size()));

    std::vector< ::OcaBoolean>::iterator it(m_items.begin());
    std::advance(it, position);
    static_cast<void>(m_items.erase(it));
}

void OcaLiteList< ::OcaBoolean>::RemoveElement(::OcaBoolean element)
{
    std::vector< ::OcaBoolean>::iterator it(std::find(m_items.begin(), m_items.end(), element));
    if (m_items.end() != it)
    {
        static_cast<void>(m_items.erase(it));
    }
}

::OcaBoolean OcaLiteList< ::OcaBoolean>::Contains(::OcaBoolean element)
{
    std::vector< ::OcaBoolean>::iterator it(std::find(m_items.begin(), m_items.end(), element));
    return static_cast< ::OcaBoolean>(m_items.end() != it);
}

void OcaLiteList< ::OcaBoolean>::Clear()
{
    m_items.clear();
}

void OcaLiteList< ::OcaBoolean>::Reserve(::OcaUint16 reserveCount)
{
    ::OcaUint16 size(static_cast< ::OcaUint16>(m_items.size()));
    if (reserveCount > size)
    {
        m_items.reserve(static_cast<size_t>(reserveCount));
    }
}

::OcaLiteList< ::OcaBoolean>& OcaLiteList< ::OcaBoolean>::operator=(const ::OcaLiteList< ::OcaBoolean>& source)
{
    if (this != &source)
    {
        m_items = source.m_items;
    }

    return *this;
}

bool OcaLiteList< ::OcaBoolean>::operator==(const ::OcaLiteList< ::OcaBoolean>& rhs) const
{
    bool result(m_items == rhs.m_items);

    return result;
}

bool OcaLiteList< ::OcaBoolean>::operator!=(const ::OcaLiteList< ::OcaBoolean>& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteList< ::OcaBoolean>::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(static_cast< ::OcaUint16>(m_items.size()), destination);
    for (std::vector< ::OcaBoolean>::const_iterator it(m_items.begin());
         it != m_items.end();
         ++it)
    {
        writer.Write(*it, destination);
    }
}

bool OcaLiteList< ::OcaBoolean>::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    m_items.clear();

    ::OcaUint16 cnt;
    bool result(reader.Read(bytesLeft, source, cnt));
    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); (i < cnt) && result; i++)
    {
        ::OcaBoolean element;
        result = result && reader.Read(bytesLeft, source, element);
        m_items.push_back(element);
    }
        
    if (!result)
    {
        m_items.clear();
    }

    return result;
}

::OcaUint32 OcaLiteList< ::OcaBoolean>::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(static_cast< ::OcaUint16>(m_items.size())));
    for (std::vector< ::OcaBoolean>::const_iterator it(m_items.begin());
         it != m_items.end();
         ++it)
    {
        length += writer.GetSize(*it);
    }

    return length;
}

#endif
