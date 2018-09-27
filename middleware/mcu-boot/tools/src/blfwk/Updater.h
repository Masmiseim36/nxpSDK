/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_Updater_h_)
#define _Updater_h_

#include "Bootloader.h"
#include "SourceFile.h"

//! @addtogroup host_updater
//! @{

using namespace blfwk;

namespace blfwk
{
//! @name Updater states with descriptions.
//@{
struct updater_enum_t
{
    uint32_t value;
    std::string description;

    updater_enum_t()
        : value(0)
        , description("")
    {
    }

    updater_enum_t(uint32_t value, const char *description)
        : value(value)
        , description(description)
    {
    }

    updater_enum_t(const updater_enum_t &old_enum)
    {
        value = old_enum.value;
        description = old_enum.description.c_str();
    }
};
const updater_enum_t kUpdaterState_NotReady(0, "Not ready");
const updater_enum_t kUpdaterState_Ready(1, "Ready");
const updater_enum_t kUpdaterState_Working(2, "Working");
const updater_enum_t kUpdaterState_Idle(3, "Idle");
const updater_enum_t kUpdaterState_Complete(4, "Complete");
//@}

//! @name Updater tasks with descriptions.
//@{
const updater_enum_t kUpdaterTask_Erasing(0, "Erasing");
const updater_enum_t kUpdaterTask_Flashing(1, "Writing");
const updater_enum_t kUpdaterTask_Reseting(2, "Reseting");
const updater_enum_t kUpdaterTask_Executing(3, "Jumping");

struct updater_task_t
{
    updater_enum_t task_desc;
    uint32_t current;
    uint32_t total;

    updater_task_t(const updater_enum_t &task_desc, uint32_t total)
        : task_desc(task_desc)
        , current(0)
        , total(total)
    {
    }

    updater_task_t()
        : task_desc()
        , current(0)
        , total(0)
    {
    }
};

typedef std::vector<updater_task_t> updater_task_vector_t;

//! @name Updater operations with descriptions.
//@{
const updater_enum_t kUpdaterOperation_Update(0, "Update");

//! @name Updater operation.
//@{
struct updater_operation_t
{
    updater_enum_t operation_desc;
    updater_task_vector_t tasks;
    uint32_t current_task;
    bool user_stopped;

    uint32_t current()
    {
        uint32_t current = 0;

        for (uint32_t i = 0; i < current_task; ++i)
        {
            current += tasks[i].total;
        }

        current += tasks[current_task].current;

        return current;
    }

    uint32_t total()
    {
        uint32_t total = 0;
        for (uint32_t i = 0; i < tasks.size(); ++i)
        {
            total += tasks[i].total;
        }
        return total;
    }

    updater_operation_t(updater_enum_t operation_desc)
        : operation_desc(operation_desc)
        , tasks()
        , current_task(0)
        , user_stopped(false)
    {
    }

    updater_operation_t(uint32_t value, const char *description)
        : operation_desc(value, description)
        , tasks()
        , current_task(0)
        , user_stopped(false)
    {
    }
};
//@}

/*!
 * \brief Update class contains the functionality necessary to update the
 * firmware on a device running Bootloader.
 *
 * The purpose of this class is to provide a common interface for
 * updating any device running the Bootloader from several different file formats.
 */
class Updater : public Bootloader
{
public:
    //! \brief Default constructor.
    Updater(const Peripheral::PeripheralConfigData &config);

    //! \brief Destructor.
    virtual ~Updater();

    //! \name Update API.
    //@{

    //! @brief Type for the progress callback routine.
    typedef void (*progress_callback_t)(updater_operation_t *op);

    //! @brief Struct used to monitor the Operation progress.
    updater_operation_t m_operation;

    //! \brief  Set the user-defined function to call on progress events.
    //!
    //! \param  callback    The function to callback with the progress data.
    void setCallback(progress_callback_t callback) { m_progressCallback = callback; }
    //! \brief  Set callback for progress and abort control.
    //!
    //! \param  callback    The function to callback with the progress data.
    //! \param  abort       The variable used for abort phase control.
    void registerCallback(void (*callback)(int, int, int), bool *abort)
    {
        m_progress.registerCallback(callback, abort);
    }

    //! \brief  Program flash on the device.
    //!
    //! \exception  std::runtime_error  Raised if the file could not be opened successfully.
    //!
    //! \param  filename       The file to program into the device.
    //! \param  base_address   The address on the device where the file wiill be written.
    status_t flashFirmware(const char *filename, uint32_t base_address, uint32_t memoryId);

    //! \brief Erase all flash blocks and release MCU security
    //!
    //! \exception  std::runtime_error   Raised if the FlashEraseAllUnsecure command does not
    //!                                  return success.
    //!                                  Raised if the FlashEraseAllUnsecure command is not
    //!                                  supported.
    void eraseAllUnsecure();

    //! \brief Erase the specified FLASH region.
    //!
    //! \exception  std::runtime_error  Thrown if an error occurred.
    //!
    //! \param  start   The beginning address of the memory region to be erased.
    //!
    //! \param  length  The length in bytes of the memory region to be erased.
    //!
    //! \param  memoryId  The ID of the memory to erase.
    void eraseRegion(uint32_t start, uint32_t length, uint32_t memoryId);

    //! \brief Execute the FlashEraseAll bootloader command.
    //!
    //! \exception  std::runtime_error  Thrown if an error occurred
    //!
    //! \param  memoryId  The ID of the memory to erase.
    void eraseAll(uint32_t memoryId);

    //! \brief Release security using BackdoorKey
    //!
    //! \exception  std::runtime_error   Raised if the FlashSecurityDisable command does not
    //!                                  return success.
    //!                                  Raised if the FlashSecurityDisable command is not
    //!                                  supported.
    //!                                  Raised if the parameter is illegal
    //! \param backdoor_key     The 16 hex digitals used to release the security
    void unlock(string backdoor_key);

    //! \brief get total internal flash size of current device.
    //!
    //! \exception  std::runtime_error  Raised if the operation is failed.
    //!
    //! \return the size in bytes. Return 0 means no internal Flash available.
    uint32_t getInternalFlashSize(void);

    //! \brief get total internal RAM size of current device.
    //!
    //! \exception  std::runtime_error  Raised if the operation is failed.
    //!
    //! \return the size in bytes. Return 0 means no internal RAM available.
    uint32_t getInternalRAMSize(void);

    //! \brief Execute the FlashProgramOnce bootloader command.
    //!
    //! \exception  std::runtime_error  Thrown if an error occurred while sending the
    //!                                 FlashEraseAll bootloader command.
    //!
    //! \param  index   The index of a specific program once field.
    //!
    //! \param  byteCount  The length in bytes of a specific program once field.
    //!
    //! \param  data    The 8/16 hex digitals to write.
    void programOnce(uint32_t index, uint32_t byteCount, string data, bool isLsb);
    //@}
    //

protected:
    //! \name Bootloader commands
    //@{

    //! \brief Execute the FlashEraseRegion bootloader command.
    //!
    //! \exception  std::runtime_error  Thrown if an error occurred while sending the
    //!                                 FlashEraseRegion(start, length) bootloader command.
    //!
    //! \param  start   The beginning address of the memory region to be erased.
    //!
    //! \param  length  The length in bytes of the memory region to be erased.
    void eraseFlashRegion(uint32_t start, uint32_t length);

    //! \brief Execute the FlashEraseAll bootloader command.
    //!
    //! \exception  std::runtime_error  Thrown if an error occurred while sending the
    //!                                 FlashEraseAll bootloader command.
    void eraseFlashAll();

    //! \brief Execute the write-memory bootloader command.
    //!
    //! \exception  std::runtime_error  Thrown if an error occurred while sending the
    //!                                 WriteMemory(segment) bootloader command.
    //!
    //! \param [in,out] segment The DatSource::Segment that represents the data to be written to the device.
    void writeMemory(DataSource::Segment *segment);

    //! \brief Execute the write-memory bootloader command.
    //!
    //! \exception  std::runtime_error  Thrown if an error occurred while sending the
    //!                                 WriteMemory(vector<uint8_t) bootloader command.
    //!
    //! \param [in] data A vector<uchar> refernce that contains the data to be written to the device.
    //! \param [in] address The address on the device where the data will be written.
    void writeMemory(uint32_t address, const uchar_vector_t &data);

    //! \brief Program flash procedure for SourceFile types.
    status_t flashFromSourceFile();

    //! \brief Program flash procedure for SB files.
    status_t flashFromSBFile(const char *filename);

    //! \brief Check if the memory is supported by current device.
    bool isMemorySupported(uint32_t memoryId);

protected:
    uint32_t m_base_address;                //!< Base address of the image.
    SourceFile *m_sourceFile;               //!< SourceFile object.
    uint32_t m_memoryId;                    //!< ID of the memory to flush the image to.
    progress_callback_t m_progressCallback; //!< Callback used to report update progress.
    Progress m_progress;                    //!< Progress control.
    standard_version_t m_version;           //!< Version of the bootloader running on current device.
};

}; // namespace blfwk

//! @}

#endif // _Updater_h_
