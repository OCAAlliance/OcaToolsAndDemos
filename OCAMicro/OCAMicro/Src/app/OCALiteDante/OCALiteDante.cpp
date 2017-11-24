/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

// OCALite.cpp : Defines the entry point for the console application.
//


#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCF/Timer/IOcfLiteTimer.h>
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteSubscriptionManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteFirmwareManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteMediaClockManager.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCP.1/Ocp1LiteNetwork.h>
#include <OCP.1/Ocp1LiteNetworkNodeID.h>
#include <OCP.1/Ocp1LiteNetworkSystemInterfaceID.h>
#include <StandardLib/StandardLib.h>

#include "OcaLiteStreamNetworkDante.h"
#include "DanteLiteHostInterface.h"

#ifdef OCA_RUN
extern void Ocp1LiteServiceRun();
#else
extern int Ocp1LiteServiceGetSocket();
extern void Ocp1LiteServiceRunWithFdSet(fd_set* readSet);
#endif // OCA_RUN

void InitializePool();
void ShutdownPool();

extern "C" {
	// reference to our Dante API metering callback
	void conmon_cb_metering_message(conmon_client_t * client, 
								conmon_channel_type_t type, 
								conmon_channel_direction_t channel_direction, 
								const conmon_message_head_t * head, 
								const conmon_message_body_t * body);
    
	// user-defined application call-back for ConMon message monitoring
    // Application should override / replace as required.  See Audinate type
    // conmon_client_handle_monitoring_message_fn for the function definition
    void *GetDeviceStatusMessageCB(void)
    {
        return NULL;
    }

    void *GetDeviceMeteringMessageCB(void)
    {
#ifdef DANTE_CM_METERING_SUBSCRIPTIONS
        return (void *)&conmon_cb_metering_message;
#else
        return NULL;
#endif // DANTE_CM_METERING_SUBSCRIPTIONS
    }
}

extern OcaLiteStatus OCALiteOCCFactoryCreate();
extern OcaLiteStatus OCALiteOCCFactoryDestroy();
extern void RunEventPropertyChanges(void);

int main(int argc, const char* argv[])
{
    std::string manufacturer, name, version;
    char *devNameOCA;
    manufacturer.assign("OCAAlliance");
    name.assign("OCAMicroTestDevice");
    version.assign("1.0.0");
    // set device information
    OcfLiteConfigureSetModelGUID(0x123456, 0x01);
    OcfLiteConfigureSetModelDescription(manufacturer, name, version);
    // Initialize the host interfaces
    bool bSuccess = ::OcfLiteHostInterfaceInitialize();
    bSuccess = bSuccess && ::Ocp1LiteHostInterfaceInitialize();
	// Are we connecting to a local or remote Dante device?
	if(argc > 1)
	{
		printf("Device Name passed in command line\n");
		devNameOCA = (char *)argv[1];
		bSuccess = bSuccess && ::DanteLiteHostInterfaceInitialize(devNameOCA);
	}
	else
	{
		bSuccess = bSuccess && ::DanteLiteHostInterfaceInitialize(NULL);
		devNameOCA = (char *)DanteLiteHostInterfaceGetDeviceName();
	}
    printf("Trying to connect to <%s>\n", devNameOCA);
    bSuccess = bSuccess && ::DanteLiteHostInterfaceInitializeConMon(devNameOCA);

    static_cast<void>(::OcaLiteBlock::GetRootBlock());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteNetworkManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteSubscriptionManager::GetInstance().SetNrEvents(1/*OCA_NR_EVENTS*/));
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteSubscriptionManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteDeviceManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteFirmwareManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteMediaClockManager::GetInstance().Initialize());

    if (bSuccess)
    {
        Ocp1LiteNetworkSystemInterfaceID interfaceId = ::Ocp1LiteNetworkSystemInterfaceID(static_cast< ::OcaUint32>(0));
        std::vector<std::string> txtRecords;
        txtRecords.push_back(name.c_str());
        name += "@";
        ::OcaLiteString nodeId = ::OcaLiteString(name.c_str() + OcfLiteConfigureGetDeviceName());
        ::Ocp1LiteNetwork* ocp1Network = new ::Ocp1LiteNetwork(static_cast< ::OcaONo>(0x1000), static_cast< ::OcaBoolean>(true), 
                                                               ::OcaLiteString("Ocp1LiteNetwork"), ::Ocp1LiteNetworkNodeID(nodeId),
                                                               interfaceId, txtRecords, ::OcaLiteString("local"), OcaInt16(65000U));
        if (ocp1Network->Initialize())
        {
            OCALiteOCCFactoryCreate();
            if (::OcaLiteBlock::GetRootBlock().AddObject(*ocp1Network))
            {
                OcaLiteStatus rc(ocp1Network->Startup());
                if (OCASTATUS_OK == rc)
                {
                    bSuccess = bSuccess && ::OcaLiteCommandHandler::GetInstance().Initialize();
                    ::OcaLiteDeviceManager::GetInstance().SetErrorAndOperationalState(static_cast< ::OcaBoolean>(!bSuccess), ::OcaLiteDeviceManager::OCA_OPSTATE_OPERATIONAL);
                    ::OcaLiteDeviceManager::GetInstance().SetEnabled(static_cast< ::OcaBoolean>(bSuccess));

					// From now onwards use the memory pool
					InitializePool();

					INT32 highestFd;
					OcfLiteSelectableSet readSet;
					OcfLiteSelectableSet writeSet;
					OcfLiteSelectableSet exceptSet;
					bool pendingMessageAvailable(false);
					while (bSuccess)
                    {
#ifdef OCA_RUN
                        ::OcaLiteCommandHandler::GetInstance().RunWithTimeout(100);
                        Ocp1LiteServiceRun();
#else
						FD_ZERO(&readSet);
						FD_ZERO(&writeSet);
						FD_ZERO(&exceptSet);

						pendingMessageAvailable = ::OcaLiteCommandHandler::GetInstance().AddSelectables(highestFd, readSet, writeSet, exceptSet);

						int serviceSocket = Ocp1LiteServiceGetSocket();
						if (-1 != serviceSocket)
						{
							FD_SET(serviceSocket, &readSet);
							if (serviceSocket > highestFd)
							{
								highestFd = serviceSocket;
							}
						}
						int danteSocket = DanteLiteHostInterfaceGetSocket();
						if (-1 != danteSocket)
						{
							FD_SET(danteSocket, &readSet);
							if (danteSocket > highestFd)
							{
								highestFd = danteSocket;
							}
						}
						timeval timeout = { 0, pendingMessageAvailable ? 0 : 10000};
						select(highestFd + 1, &readSet, &writeSet, &exceptSet, &timeout);

						// We always need to Run OCA because of keep alive handling.
						::OcaLiteCommandHandler::GetInstance().RunWithSelectSet(readSet, writeSet, exceptSet);

						if (FD_ISSET(serviceSocket, &readSet))
						{
							// Service only needs to run when something usefull to do.
							Ocp1LiteServiceRunWithFdSet(&readSet);
						}

						// Always run dante for metering.
						DanteLiteHostInterfaceRunWithFdSet(&readSet);

#ifdef DANTE_CM_METERING_SUBSCRIPTIONS
						// Run for round robin processing of property changed events on metering data
						RunEventPropertyChanges();
#endif // DANTE_CM_METERING_SUBSCRIPTIONS
#endif // OCA_RUN
                    }
					ShutdownPool();
                }
            }
            OCALiteOCCFactoryDestroy();
        }
    }
    DanteLiteHostInterfaceStop();
    return 0;
}

#ifndef _WIN32
// ---- Memory manager implementation ---- 

/* The size of a memory block header */
#define MEMORY_BLOCK_HEADER_SIZE sizeof(struct MemoryBlock)
/* The size in bytes of a small block */
#define SMALL_BLOCK_SIZE 64
/* The size in bytes of a middle block */
#define MIDDLE_BLOCK_SIZE 512
/* The size in bytes of a large block */
#define LARGE_BLOCK_SIZE 3096
/* The size in bytes of a big block */
#define BIG_BLOCK_SIZE 4096

/** Definition of a memory block */
typedef struct MemoryBlock
{
	/* Is the block in use? */
	char inInuse;
	/* The index in the array for a reverse lookup */
	char index;
	/* The actual size */
	short size;
	/* The original size */
	size_t origSize;
	/* Pointer to the memory */
	unsigned int memory;
} MemoryBlock;

/* The maximum number of blocks */
#define MAX_NR_SMALL_BLOCKS 100
#define MAX_NR_MIDDLE_BLOCKS 30
#define MAX_NR_LARGE_BLOCKS 48
#define MAX_NR_BIG_BLOCKS 16 

/* The small blocks */
static MemoryBlock* gs_smallBlocks[MAX_NR_SMALL_BLOCKS];
/* The middle sized blocks */
static MemoryBlock* gs_middleBlocks[MAX_NR_MIDDLE_BLOCKS];
/* Large blocks */
static MemoryBlock* gs_largeBlocks[MAX_NR_LARGE_BLOCKS];
/* Big blocks */
static MemoryBlock* gs_bigBlocks[MAX_NR_BIG_BLOCKS];

#ifdef MEMORY_MANAGER_PROFILE
static int gs_highestSmallBlock = 0;
static int gs_highestMiddleBlock = 0;
static int gs_highestLargeBlock = 0;
static int gs_highestBigBlock = 0;
#endif

int gs_initialized = 0;

void ShutdownPool()
{
	int i = 0;
	for (i = 0; i < MAX_NR_SMALL_BLOCKS; i++)
	{
#ifdef MEMORY_MANAGER_PROFILE
		if (gs_smallBlocks[i]->inInuse)
		{
			printf("Small block %d still in use. Deleting anyhow\r\n", i);
		}
#endif
		free(gs_smallBlocks[i]);
		gs_smallBlocks[i] = NULL;
	}

	for (i = 0; i < MAX_NR_MIDDLE_BLOCKS; i++)
	{
#ifdef MEMORY_MANAGER_PROFILE
		if (gs_middleBlocks[i]->inInuse)
		{
			printf("Middle block %d still in use. Deleting anyhow\r\n", i);
		}
#endif
		free(gs_middleBlocks[i]);
		gs_middleBlocks[i] = NULL;
	}

	for (i = 0; i < MAX_NR_LARGE_BLOCKS; i++)
	{
#ifdef MEMORY_MANAGER_PROFILE
		if (gs_largeBlocks[i]->inInuse)
		{
			printf("Large block %d still in use. Deleting anyhow\r\n", i);
		}
#endif
		free(gs_largeBlocks[i]);
		gs_largeBlocks[i] = NULL;
	}

	for (i = 0; i < MAX_NR_BIG_BLOCKS; i++)
	{
#ifdef MEMORY_MANAGER_PROFILE
		if (gs_bigBlocks[i]->inInuse)
		{
			printf("Big block %d still in use. Deleting anyhow\r\n", i);
		}
#endif
		free(gs_bigBlocks[i]);
		gs_bigBlocks[i] = NULL;
	}
}

void InitializePool()
{
	int i;

	printf("Memory manager allocating %d Bytes (%d KB) in %d blocks for client\r\n", 
							((MAX_NR_SMALL_BLOCKS * SMALL_BLOCK_SIZE) + (MAX_NR_MIDDLE_BLOCKS * MIDDLE_BLOCK_SIZE) + (MAX_NR_LARGE_BLOCKS * LARGE_BLOCK_SIZE) + (MAX_NR_BIG_BLOCKS * BIG_BLOCK_SIZE)), 
							(((MAX_NR_SMALL_BLOCKS * SMALL_BLOCK_SIZE) + (MAX_NR_MIDDLE_BLOCKS * MIDDLE_BLOCK_SIZE) + (MAX_NR_LARGE_BLOCKS * LARGE_BLOCK_SIZE) + (MAX_NR_BIG_BLOCKS * BIG_BLOCK_SIZE)) / 1024),
							(MAX_NR_SMALL_BLOCKS + MAX_NR_MIDDLE_BLOCKS + MAX_NR_LARGE_BLOCKS + MAX_NR_BIG_BLOCKS));

	// Allocate the memory
	for (i = 0; i < MAX_NR_SMALL_BLOCKS; i++)
	{
		gs_smallBlocks[i] = (MemoryBlock*)malloc(SMALL_BLOCK_SIZE + MEMORY_BLOCK_HEADER_SIZE);
		MemoryBlock* mbPtr = gs_smallBlocks[i];
		mbPtr->index = i + 1;
		mbPtr->inInuse = 0;
		mbPtr->size = SMALL_BLOCK_SIZE;
		mbPtr->origSize = SMALL_BLOCK_SIZE;
		mbPtr->memory = ((unsigned int)gs_smallBlocks[i]) + (MEMORY_BLOCK_HEADER_SIZE);
	}

	for (i = 0; i < MAX_NR_MIDDLE_BLOCKS; i++)
	{
		gs_middleBlocks[i] = (MemoryBlock*)malloc(MIDDLE_BLOCK_SIZE + MEMORY_BLOCK_HEADER_SIZE);
		MemoryBlock* mbPtr = gs_middleBlocks[i];
		mbPtr->index = i + 1;
		mbPtr->inInuse = 0;
		mbPtr->size = MIDDLE_BLOCK_SIZE;
		mbPtr->origSize = MIDDLE_BLOCK_SIZE;
		mbPtr->memory = ((unsigned int)gs_middleBlocks[i]) + (MEMORY_BLOCK_HEADER_SIZE);
	}

	for (i = 0; i < MAX_NR_LARGE_BLOCKS; i++)
	{
		gs_largeBlocks[i] = (MemoryBlock*)malloc(LARGE_BLOCK_SIZE + MEMORY_BLOCK_HEADER_SIZE);
		MemoryBlock* mbPtr = gs_largeBlocks[i];
		mbPtr->index = i + 1;
		mbPtr->inInuse = 0;
		mbPtr->size = LARGE_BLOCK_SIZE;
		mbPtr->origSize = LARGE_BLOCK_SIZE;
		mbPtr->memory = ((unsigned int)gs_largeBlocks[i]) + (MEMORY_BLOCK_HEADER_SIZE);
	}

	for (i = 0; i < MAX_NR_BIG_BLOCKS; i++)
	{
		gs_bigBlocks[i] = (MemoryBlock*)malloc(BIG_BLOCK_SIZE + MEMORY_BLOCK_HEADER_SIZE);
		MemoryBlock* mbPtr = gs_bigBlocks[i];
		mbPtr->index = i + 1;
		mbPtr->inInuse = 0;
		mbPtr->size = BIG_BLOCK_SIZE;
		mbPtr->origSize = BIG_BLOCK_SIZE;
		mbPtr->memory = ((unsigned int)gs_bigBlocks[i]) + (MEMORY_BLOCK_HEADER_SIZE);
	}

	gs_initialized = 1;
}

void* GetFromPool(size_t size)
{
	void* ptr = NULL;

	if (gs_initialized)
	{
		int i;
		if (SMALL_BLOCK_SIZE >= size)
		{
			int nrBlocks = MAX_NR_SMALL_BLOCKS;
			for (i = 0; i < nrBlocks; i++)
			{
				if (0 == gs_smallBlocks[i]->inInuse)
				{
					gs_smallBlocks[i]->inInuse = 1;
					gs_smallBlocks[i]->size = size;
					ptr = (void*)gs_smallBlocks[i]->memory;
					break;
				}
			}

#ifdef MEMORY_MANAGER_PROFILE
			if (!ptr)
			{
				printf("Not enough small sized blocks, trying bigger block (%d,%d)\r\n", i, nrBlocks);
			}

			if (i > gs_highestSmallBlock)
			{
				gs_highestSmallBlock = i;
			}
#endif
		}
		
		if (!ptr && (MIDDLE_BLOCK_SIZE >= size))
		{
			int nrBlocks = MAX_NR_MIDDLE_BLOCKS;
			for (i = 0; i < nrBlocks; i++)
			{
				if (0 == gs_middleBlocks[i]->inInuse)
				{
					gs_middleBlocks[i]->inInuse = 1;
					gs_middleBlocks[i]->size = size;
					ptr = (void*)gs_middleBlocks[i]->memory;
					break;
				}
			}

#ifdef MEMORY_MANAGER_PROFILE
			if (!ptr)
			{
				printf("Not enough middle sized blocks, trying bigger block (%d,%d)\r\n", i, nrBlocks);
			}

			if (i > gs_highestMiddleBlock)
			{
				gs_highestMiddleBlock = i;
			}
#endif
		}
		
		if (!ptr && (LARGE_BLOCK_SIZE >= size))
		{
			int nrBlocks = MAX_NR_LARGE_BLOCKS;
			for (i = 0; i < nrBlocks; i++)
			{
				if (0 == gs_largeBlocks[i]->inInuse)
				{
					gs_largeBlocks[i]->inInuse = 1;
					gs_largeBlocks[i]->size = size;
					ptr = (void*)gs_largeBlocks[i]->memory;
					break;
				}
			}

#ifdef MEMORY_MANAGER_PROFILE
			if (!ptr)
			{
				printf("Not enough large sized blocks, trying bigger block (%d,%d)\r\n", i, nrBlocks);
			}

			if (i > gs_highestLargeBlock)
			{
				gs_highestLargeBlock = i;
			}
#endif
		}

		if (!ptr && (BIG_BLOCK_SIZE >= size))
		{
			int nrBlocks = MAX_NR_BIG_BLOCKS;
			for (i = 0; i < nrBlocks; i++)
			{
				if (0 == gs_bigBlocks[i]->inInuse)
				{
					gs_bigBlocks[i]->inInuse = 1;
					gs_bigBlocks[i]->size = size;
					ptr = (void*)gs_bigBlocks[i]->memory;
					break;
				}
			}

#ifdef MEMORY_MANAGER_PROFILE
			if (!ptr)
			{
				printf("Not enough big sized blocks, using dynamic memory (%d,%d)\r\n", i, nrBlocks);
			}

			if (i > gs_highestBigBlock)
			{
				gs_highestBigBlock = i;
			}
#endif
		}

#ifdef MEMORY_MANAGER_PROFILE
		if (BIG_BLOCK_SIZE < size)
		{
			printf("Using dynamic memory allocation for size %zu\r\n", size);
		}
		else
		{
			printf("Assigned pool block (%d-%d,%d-%d,%d-%d,%d-%d). Size=%ld\r\n",
				gs_highestSmallBlock, MAX_NR_SMALL_BLOCKS,
				gs_highestMiddleBlock, MAX_NR_MIDDLE_BLOCKS,
				gs_highestLargeBlock, MAX_NR_LARGE_BLOCKS,
				gs_highestBigBlock, MAX_NR_BIG_BLOCKS,
				size);
		}
#endif
	}

	if (!ptr)
	{
		void* result = malloc(size + MEMORY_BLOCK_HEADER_SIZE);
		
		MemoryBlock* mbPtr = (MemoryBlock*)result;
		mbPtr->index = 0;
		mbPtr->inInuse = 1;
		mbPtr->size = size;
		mbPtr->origSize = size;
		mbPtr->memory = ((unsigned int)result) + (MEMORY_BLOCK_HEADER_SIZE);
		ptr = (void*)mbPtr->memory;
	}

	return ptr;
}

void ReturnToPool(void* ptr)
{
	if (ptr)
	{
		MemoryBlock* mbPtr = (MemoryBlock*)(((unsigned int)ptr) - (MEMORY_BLOCK_HEADER_SIZE));
		if ((1 == mbPtr->inInuse) && ((void*)mbPtr->memory == ptr))
		{
			if (!mbPtr->index)
			{
				/* Dynamic allocated item */
				free(mbPtr);
			}
			else
			{
				mbPtr->inInuse = 0;
			}
		}
		else
		{
			/* This should not occur */
			printf("Free: Strange memory pointer 0x%x and %p?!\r\n", mbPtr->memory, ptr);
			free(ptr);
		}
	}
}

void* operator new (size_t size)
{
	return GetFromPool(size);
}

void operator delete(void* p)
{
	ReturnToPool(p);
}
#else
void InitializePool()
{
}
void ShutdownPool()
{
}
#endif
