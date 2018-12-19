/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_Operation_h_)
#define _Operation_h_

#include "stdafx.h"
#include <vector>
#include "DataSource.h"
#include "DataTarget.h"
#include "smart_ptr.h"
#include "Keyblob.h"

namespace elftosb
{
/*!
 * \brief Abstract base class for all boot operations.
 */
class Operation
{
public:
    Operation() {}
    virtual ~Operation() {}
};

/*!
 * \brief Load data into memory operation.
 */
class LoadOperation : public Operation
{
public:
    LoadOperation()
        : Operation()
        , m_source()
        , m_target()
        , m_isDCDLoad(false)
    {
    }

    void setSource(DataSource *source);
    inline DataSource *getSource() { return m_source; }
    void setTarget(DataTarget *target);
    inline DataTarget *getTarget() { return m_target; }
    inline void setDCDLoad(bool isDCD) { m_isDCDLoad = isDCD; }
    inline bool isDCDLoad() const { return m_isDCDLoad; }
protected:
    smart_ptr<DataSource> m_source;
    smart_ptr<DataTarget> m_target;
    bool m_isDCDLoad;
};

/*!
 * \brief Operation to execute code at a certain address.
 */
class ExecuteOperation : public Operation
{
public:
    enum execute_type_t
    {
        kJump,
        kCall
    };

public:
    ExecuteOperation()
        : Operation()
        , m_target()
        , m_argument(0)
        , m_type(kCall)
        , m_isHAB(false)
        , m_stackPointer(0)
        , m_isStackPointerSet(false)
    {
    }

    inline void setTarget(DataTarget *target) { m_target = target; }
    inline DataTarget *getTarget() { return m_target; }
    inline void setArgument(uint32_t arg) { m_argument = arg; }
    inline uint32_t getArgument() { return m_argument; }
    inline void setExecuteType(execute_type_t type) { m_type = type; }
    inline execute_type_t getExecuteType() { return m_type; }
    inline void setIsHAB(bool isHAB) { m_isHAB = isHAB; }
    inline bool isHAB() const { return m_isHAB; }
    inline void setStackPointer(uint32_t sp) { m_stackPointer = sp; }
    inline uint32_t getStackPointer() { return m_stackPointer; }
    inline void setIsStackPointerSet(bool isSet) { m_isStackPointerSet = isSet; }
    inline bool isStackPonterSet() const { return m_isStackPointerSet; }
protected:
    smart_ptr<DataTarget> m_target;
    uint32_t m_argument;
    uint32_t m_stackPointer;
    execute_type_t m_type;
    bool m_isHAB;
    bool m_isStackPointerSet;
};

/*!
 * \brief Authenticate with HAB and execute the entry point.
 */
class HABExecuteOperation : public ExecuteOperation
{
public:
    HABExecuteOperation()
        : ExecuteOperation()
    {
    }
};

/*!
 * \brief Operation to switch boot modes.
 */
class BootModeOperation : public Operation
{
public:
    BootModeOperation()
        : Operation()
        , m_bootMode(0)
    {
    }

    inline void setBootMode(uint32_t mode) { m_bootMode = mode; }
    inline uint32_t getBootMode() const { return m_bootMode; }
protected:
    uint32_t m_bootMode; //!< The new boot mode value.
};

/*!
* \brief Operation to reset the device.
*/
class ResetOperation : public Operation
{
public:
    ResetOperation()
        : Operation()
    {
    }
};

/*!
 * \brief Operation to erase flash memory.
 */
class FlashEraseOperation : public Operation
{
public:
    FlashEraseOperation()
        : Operation()
        , m_startAddress(0)
        , m_byteCount(0)
    {
    }

    void setRange(uint32_t start, uint32_t count)
    {
        m_startAddress = start;
        m_byteCount = count;
    }
    void getRange(uint32_t *start, uint32_t *count) const;

protected:
    uint32_t m_startAddress;
    uint32_t m_byteCount;
};

/*!
* \brief Operation to erase external flash memory.
*/
class FlashEraseExternalOperation : public FlashEraseOperation
{
public:
    FlashEraseExternalOperation()
        : FlashEraseOperation()
        , m_memoryId(0)
    {
    }

    void setMemoryId(uint32_t memoryId) { m_memoryId = memoryId; }
    uint32_t getMemoryId() const { return m_memoryId; }
protected:
    uint32_t m_memoryId;
};

/*!
 * \brief Operation to erase the entire flash memory array.
 */
class FlashEraseAllOperation : public FlashEraseOperation
{
public:
    FlashEraseAllOperation()
        : FlashEraseOperation()
    {
    }
};

/*!
* \brief Operation to erase the entire external flash memory array.
*/
class FlashEraseAllExternalOperation : public FlashEraseOperation
{
public:
    FlashEraseAllExternalOperation()
        : FlashEraseOperation()
        , m_memoryId(0)
    {
    }

    void setMemoryId(uint32_t memoryId) { m_memoryId = memoryId; }
    uint32_t getMemoryId() const { return m_memoryId; }
protected:
    uint32_t m_memoryId;
};

/*!
* \brief Operation to erase the entire flash memory array, unsecure version.
*/
class FlashEraseAllUnsecureOperation : public FlashEraseOperation
{
public:
    FlashEraseAllUnsecureOperation()
        : FlashEraseOperation()
    {
    }
};

/*!
* \brief Operation to enable external memory access.
*/
class MemEnableOperation : public Operation
{
public:
    MemEnableOperation()
        : Operation()
        , m_startAddress(0)
        , m_byteCount(0)
        , m_memoryId(0)
    {
    }

    void setMemoryId(uint32_t memoryId) { m_memoryId = memoryId; }
    uint32_t getMemoryId() const { return m_memoryId; }
    void setRange(uint32_t start, uint32_t count)
    {
        m_startAddress = start;
        m_byteCount = count;
    }
    void getRange(uint32_t *start, uint32_t *count) const;

protected:
    uint32_t m_startAddress;
    uint32_t m_byteCount;
    uint32_t m_memoryId;
};

/*!
* \brief Operation to program persistent bits.
*/
class LoadExternalOperation : public LoadOperation
{
public:
    //! \brief Default constructor.
    LoadExternalOperation()
        : LoadOperation()
        , m_memoryId(0)
    {
    }

    //! \brief Constructor. Takes a memory space argument.
    LoadExternalOperation(uint32_t memoryId)
        : LoadOperation()
        , m_memoryId(memoryId)
    {
    }

    void setMemoryId(uint32_t memoryId) { m_memoryId = memoryId; }
    uint32_t getMemoryId() const { return m_memoryId; }
protected:
    uint32_t m_memoryId;
};

/*!
* \brief Operation to program persistent bits.
*/
class ProgramOperation : public LoadOperation
{
public:
    //! \brief Default constructor.
    ProgramOperation()
        : LoadOperation()
        , m_memoryId(0)
    {
    }

    //! \brief Constructor. Takes a memory space argument.
    ProgramOperation(uint32_t memoryId)
        : LoadOperation()
        , m_memoryId(memoryId)
    {
    }

    void setMemoryId(uint32_t memoryId) { m_memoryId = memoryId; }
    uint32_t getMemoryId() const { return m_memoryId; }
protected:
    uint32_t m_memoryId;
};

/*!
* \brief Operation to wrap and program keys.
*/
class KeywrapOperation : public LoadOperation
{
public:
    //! \brief Default constructor.
    KeywrapOperation()
        : LoadOperation()
        , m_keyblob()
    {
    }

    //! \brief Constructor. Takes a keyblob argument.
    KeywrapOperation(Keyblob *keyblob)
        : LoadOperation()
        , m_keyblob(keyblob)
    {
    }

    void setKeyblob(Keyblob *keyblob) { m_keyblob = keyblob; }
    Keyblob *getKeyblob() { return m_keyblob; }
protected:
    smart_ptr<Keyblob> m_keyblob; //!< Active keyblob during this load.
};

/*!
* \brief Operation to encrypt data for OTFAD.
*/
class EncryptOperation : public LoadOperation
{
public:
    //! \brief Default constructor.
    EncryptOperation()
        : LoadOperation()
        , m_keyblob()
    {
    }

    //! \brief Constructor. Takes a keyblob argument.
    EncryptOperation(Keyblob *keyblob)
        : LoadOperation()
        , m_keyblob(keyblob)
    {
    }

    void setKeyblob(Keyblob *keyblob) { m_keyblob = keyblob; }
    Keyblob *getKeyblob() { return m_keyblob; }
protected:
    smart_ptr<Keyblob> m_keyblob; //!< Active keyblob during this load.
};

/*!
 * \brief Ordered sequence of operations.
 *
 * The operation objects owned by the sequence are \e not deleted when the
 * sequence is destroyed. The owner of the sequence must manually delete
 * the operation objects.
 */
class OperationSequence
{
public:
    typedef std::vector<Operation *> operation_list_t;         //!< Type for a list of operation objects.
    typedef operation_list_t::iterator iterator_t;             //!< Iterator over operations.
    typedef operation_list_t::const_iterator const_iterator_t; //!< Const iterator over operations.

public:
    //! \brief Default constructor.
    OperationSequence() {}
    //! \brief Constructor. Makes a one-element sequence from \a soleElement.
    OperationSequence(Operation *soleElement) { m_operations.push_back(soleElement); }
    //! \brief Destructor.
    virtual ~OperationSequence();

    //! \name Iterators
    //@{
    inline iterator_t begin() { return m_operations.begin(); }
    inline const_iterator_t begin() const { return m_operations.begin(); }
    inline iterator_t end() { return m_operations.end(); }
    inline const_iterator_t end() const { return m_operations.end(); }
    //@}

    inline Operation *operator[](unsigned index) const { return m_operations[index]; }
    //! \name Status
    //@{
    //! \brief Returns the number of operations in the sequence.
    inline unsigned getCount() const { return m_operations.size(); }
    //@}

    //! \name Operations
    //@{
    //! \brief Append one operation object to the sequence.
    inline void append(Operation *op) { m_operations.push_back(op); }
    //! \brief Append the contents of \a other onto this sequence.
    void append(const OperationSequence *other);

    //! \brief Appends \a other onto this sequence.
    OperationSequence &operator+=(const OperationSequence *other)
    {
        append(other);
        return *this;
    }
    //@}

protected:
    operation_list_t m_operations; //!< The list of operations.
};

}; // namespace elftosb

#endif // _Operation_h_
