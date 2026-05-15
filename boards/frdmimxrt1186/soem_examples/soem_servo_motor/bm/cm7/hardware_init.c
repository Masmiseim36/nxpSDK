/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${macro:start}*/
/*!< PHY reset pins. */
#define EXAMPLE_SWT_PORT0_PHY_RESET_PIN RGPIO1, 15
#define EXAMPLE_SWT_PORT2_PHY_RESET_PIN RGPIO1, 20

#define PHY_EXT_ADDR_REG             0x1EU
#define PHY_EXT_DATA_REG             0x1FU
#define PHY_RGMII_CONFIG1_REG        0xA003U
#define PHY_RGMII_CONFIG1_TXDLY_MASK 0xFU
/*${macro:end}*/

/*${variable:start}*/
/* Rx buffer memeory type. */
typedef uint8_t rx_buffer_t[EP_RXBUFF_SIZE_ALIGN];

#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
extern struct netc_ep_if_port if_port;
#endif

#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
extern struct netc_swt_if_port if_port;
#endif

volatile uint64_t system_time_ns = 0;

#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
/* SWT resource. */
static swt_config_t g_swt_config;
static swt_transfer_config_t swtTxRxConfig;
#endif

/* Buffer descriptor resource. */
AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t g_rxBuffDescrip[EP_RING_NUM][EP_RXBD_NUM],
                              EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static rx_buffer_t g_rxDataBuff[EP_RING_NUM][EP_RXBD_NUM],
                              EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_txFrame[EP_TXBUFF_SIZE], EP_BUFF_SIZE_ALIGN);
#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_mgmtTxBuffDescrip[EP_TXBD_NUM], EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_cmd_bd_t g_cmdBuffDescrip[EP_TXBD_NUM], EP_BD_ALIGN);
#endif
static uint64_t rxBuffAddrArray[EP_RING_NUM][EP_RXBD_NUM];
#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
static netc_tx_frame_info_t g_mgmtTxDirty[EP_TXBD_NUM];
#endif
#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_txBuffDescrip[EP_RING_NUM][EP_TXBD_NUM],
                              EP_BD_ALIGN);
static netc_tx_frame_info_t g_txDirty[EP_RING_NUM][EP_TXBD_NUM];
#endif

/* PHY operation. */
struct netc_swt_if_port if_port;
static phy_yt8521_resource_t s_phy_resource[5];
/*${variable:end}*/

status_t NETC_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = NETC_FREQ,
    };

    mdioConfig.mdio.type = kNETC_EMdio;
    result               = NETC_MDIOInit(&if_port.s_emdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

static status_t NETC_EP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&if_port.s_emdio_handle, phyAddr, regAddr, data);
}

static status_t NETC_EP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&if_port.s_emdio_handle, phyAddr, regAddr, pData);
}

uint64_t gettime()
{
	uint64_t nsec_base;
	uint32_t cur_nsec;

	nsec_base  = system_time_ns;
	cur_nsec   = GPT_GetCurrentTimerCount(OSAL_TIMER);

	if (nsec_base != system_time_ns)
	{
		nsec_base  = system_time_ns;
		cur_nsec   = GPT_GetCurrentTimerCount(OSAL_TIMER);
	}
	return nsec_base + cur_nsec * CLOCK_GRANULARITY_NS;
}

void nsleep_to (uint64_t nsec_target)
{
	while (nsec_target > gettime());
}

void osal_gettime(struct timeval *current_time)
{
	uint64_t nsec = gettime();
	
	current_time->tv_sec  = nsec / CLOCK_INCREASE_PER_SEC;
	current_time->tv_usec = (nsec % CLOCK_INCREASE_PER_SEC) / 1000;

	return;
}

void irq_wake_task(void)
{
    return;
}

void OSAL_TIMER_IRQHandler(void)
{
	/* Clear interrupt flag. */
	GPT_ClearStatusFlags(OSAL_TIMER, kGPT_OutputCompare1Flag);

	system_time_ns += CLOCK_INCREASE_PER_SEC;

	irq_wake_task();
	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
 	 exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
	__DSB();
#endif
}

void msgintrCallback(MSGINTR_Type *base, uint8_t channel, uint32_t pendingIntr)
{
    /* Transmit interrupt */
    if ((pendingIntr & (1U << TX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanTxIntrFlags(&if_port.g_ep_handle, 1, 0);
        if_port.txOver = true;
    }
    /* Receive interrupt */
    if ((pendingIntr & (1U << RX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanRxIntrFlags(&if_port.g_ep_handle, 1);
    }
}

void osal_timer_init(uint32_t priority)
{
	uint32_t gptFreq;
	gpt_config_t gptConfig;

	assert(priority < (1UL << __NVIC_PRIO_BITS));

	GPT_GetDefaultConfig(&gptConfig);
	GPT_Init(OSAL_TIMER, &gptConfig);
	gptFreq = OSAL_TIMER_CLK_FREQ;
	/* Divide GPT clock source frequency to 1MHz */
	GPT_SetClockDivider(OSAL_TIMER, gptFreq / CLOCK_GRANULARITY_FRE);
	/* Set both GPT modules to 1 second duration */
	GPT_SetOutputCompareValue(OSAL_TIMER, kGPT_OutputCompare_Channel1, CLOCK_GRANULARITY_FRE);
	/* Enable GPT Output Compare1 interrupt */
	GPT_EnableInterrupts(OSAL_TIMER, kGPT_OutputCompare1InterruptEnable);
	/* Enable at the Interrupt */
	NVIC_SetPriority(OSAL_TIMER_IRQ_ID, priority);
	EnableIRQ(OSAL_TIMER_IRQ_ID);
	GPT_StartTimer(OSAL_TIMER);
}

static netc_rx_bdr_config_t rxBdrConfig = {0};
static netc_tx_bdr_config_t txBdrConfig = {0};
static netc_bdr_config_t bdrConfig;
static netc_msix_entry_t msixEntry[2];
static ep_config_t ep_config;

static status_t APP_SwtReclaimCallback(swt_handle_t *handle, netc_tx_frame_info_t *frameInfo, void *userData)
{
    return kStatus_Success;
}

status_t APP_SWT_AddTableEntry()
{
    status_t result = kStatus_Success;
    uint32_t entryID;

#if defined(EXAMPLE_SWT_USE_IPF) && EXAMPLE_SWT_USE_IPF
    netc_tb_ipf_config_t ipfEntryCfg;

    memset(&ipfEntryCfg, 0U, sizeof(netc_tb_ipf_config_t));
    ipfEntryCfg.keye.srcPortMask = EXAMPLE_SWT_USED_PORT_BITMAP;
    ipfEntryCfg.cfge.hr          = kNETC_SoftwareDefHR0;
    ipfEntryCfg.cfge.fltfa       = kNETC_IPFRedirectToMgmtPort;

    /* Frame from used port redirect to management port. */
    for (uint32_t i = 0; i < EXAMPLE_SWT_MAX_PORT_NUM; i++)
    {
        /* Only check the enabled port. */
        if (((1U << i) & EXAMPLE_SWT_USED_PORT_BITMAP) == 0U)
        {
            continue;
        }

        ipfEntryCfg.keye.srcPort = i;
        result                   = SWT_RxIPFAddTableEntry(&if_port.g_swt_handle, &ipfEntryCfg, &entryID);
        if ((kStatus_Success != result) && (entryID != 0xFFFFFFFF))
        {
            PRINTF("\r\n%s: %d, Failed to add IPF table!, result = %u\r\n, entryID = %u", __func__, __LINE__, result,
                   entryID);
            return kStatus_Fail;
        }
    }
#else
    /* Set FDB table, input frame only forwards to pseudo MAC port. */
    netc_tb_fdb_config_t fdbEntryCfg = {
        .keye.fid = FRAME_FID, .cfge.portBitmap = (1U << EXAMPLE_SWT_PSEUDO_PORT), .cfge.dynamic = 1};
    memset(&fdbEntryCfg.keye.macAddr[0], 0xFF, 6U);
    result = SWT_BridgeAddFDBTableEntry(&if_port.g_swt_handle, &fdbEntryCfg, &entryID);
    if ((kStatus_Success != result) || (0xFFFFFFFFU == entryID))
    {
        PRINTF("\r\n%s: %d, Failed to add FDB table!, result = %d, entryID = %d\r\n", __func__, __LINE__, result,
               entryID);
        return kStatus_Fail;
    }
#endif

    return result;
}

int if_port_swt_init(void)
{
	struct soem_if_port soem_port;
    bdrConfig.rxBdrConfig = &rxBdrConfig;
	bdrConfig.txBdrConfig = &txBdrConfig;
    uint32_t msgAddr;

    PRINTF("\r\nNETC swtich frame loopback example start.\r\n");

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(MSGINTR, &msgintrCallback);
    msgAddr              = MSGINTR_GetIntrSelectAddr(MSGINTR, 0);
    msixEntry[0].control = kNETC_MsixIntrMaskBit;
    msixEntry[0].msgAddr = msgAddr;
    msixEntry[0].msgData = TX_INTR_MSG_DATA;
    msixEntry[1].control = kNETC_MsixIntrMaskBit;
    msixEntry[1].msgAddr = msgAddr;
    msixEntry[1].msgData = RX_INTR_MSG_DATA;

    /* BD ring configuration. */
    bdrConfig.rxBdrConfig[0].bdArray       = &g_rxBuffDescrip[0][0];
    bdrConfig.rxBdrConfig[0].len           = EP_RXBD_NUM;
    bdrConfig.rxBdrConfig[0].buffAddrArray = &rxBuffAddrArray[0][0];
    bdrConfig.rxBdrConfig[0].buffSize      = EP_RXBUFF_SIZE_ALIGN;
    bdrConfig.rxBdrConfig[0].msixEntryIdx  = RX_MSIX_ENTRY_IDX;
    bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    bdrConfig.rxBdrConfig[0].enThresIntr   = true;
    bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
    bdrConfig.rxBdrConfig[0].intrThreshold = 1;

    /* Endpoint configuration. */
    (void)EP_GetDefaultConfig(&ep_config);
    ep_config.si                    = EXAMPLE_SWT_SI;
    ep_config.siConfig.txRingUse    = 1;
    ep_config.siConfig.rxRingUse    = 1;
    //ep_config.reclaimCallback       = ReclaimCallback;
    ep_config.msixEntry             = &msixEntry[0];
    ep_config.entryNum              = 2;
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    ep_config.rxCacheMaintain = true;
    ep_config.txCacheMaintain = true;
#endif

    SWT_GetDefaultConfig(&g_swt_config);

    for (int i = 0; i < EXAMPLE_SWT_MAX_PORT_NUM; i++)
    {
        g_swt_config.ports[i].ethMac.miiMode   = KNETC_HW_MII_MODE;
        g_swt_config.ports[i].ethMac.miiSpeed  = kNETC_MiiSpeed100M;
        g_swt_config.ports[i].ethMac.miiDuplex = kNETC_MiiFullDuplex;
    }

    /* Wait a moment for PHY status to be stable. */
    SDK_DelayAtLeastUs(PHY_STABILITY_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    g_swt_config.bridgeCfg.dVFCfg.portMembership = (1U << EXAMPLE_SWT_PSEUDO_PORT) | EXAMPLE_SWT_USED_PORT_BITMAP;
#if defined(EXAMPLE_SWT_USE_IPF) && EXAMPLE_SWT_USE_IPF
    g_swt_config.ports[0].commonCfg.ipfCfg.enIPFTable = true;
    g_swt_config.ports[2].commonCfg.ipfCfg.enIPFTable = true;
#else
    g_swt_config.bridgeCfg.dVFCfg.enUseFilterID = true;
    g_swt_config.bridgeCfg.dVFCfg.filterID      = FRAME_FID;
    g_swt_config.bridgeCfg.dVFCfg.mfo           = kNETC_FDBLookUpWithDiscard;
    g_swt_config.bridgeCfg.dVFCfg.mlo           = kNETC_DisableMACLearn;
#endif

    g_swt_config.cmdRingUse            = 1U;
    g_swt_config.cmdBdrCfg[0].bdBase   = &g_cmdBuffDescrip[0];
    g_swt_config.cmdBdrCfg[0].bdLength = 8U;

    /* Configure switch transfer resource. */
    swtTxRxConfig.enUseMgmtRxBdRing            = false;
    swtTxRxConfig.enUseMgmtTxBdRing            = true;
    swtTxRxConfig.mgmtTxBdrConfig.bdArray      = &g_mgmtTxBuffDescrip[0];
    swtTxRxConfig.mgmtTxBdrConfig.len          = EP_TXBD_NUM;
    swtTxRxConfig.mgmtTxBdrConfig.dirtyArray   = &g_mgmtTxDirty[0];
    swtTxRxConfig.mgmtTxBdrConfig.msixEntryIdx = TX_MSIX_ENTRY_IDX;
    swtTxRxConfig.mgmtTxBdrConfig.enIntr       = true;
    swtTxRxConfig.reclaimCallback              = APP_SwtReclaimCallback;
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    swtTxRxConfig.rxCacheMaintain = true;
    swtTxRxConfig.txCacheMaintain = true;
#endif

	for (uint8_t ring = 0U; ring < EP_RING_NUM; ring++)
    {
        for (uint8_t index = 0U; index < EP_RXBD_NUM; index++)
        {
            rxBuffAddrArray[ring][index] = (uint64_t)(uintptr_t)&g_rxDataBuff[ring][index];
        }
    }
	if_port.ep_config = &ep_config;
    if_port.swt_config = &g_swt_config;
	if_port.swtTxRxConfig = &swtTxRxConfig;
    if_port.bdrConfig = &bdrConfig;
	if_port.g_txFrame = g_txFrame;
	strncpy(soem_port.ifname, SOEM_PORT_NAME, SOEM_IF_NAME_MAXLEN);
	strncpy(soem_port.dev_name, "netc_swt", SOEM_DEV_NAME_MAXLEN);
	soem_port.port_init = netc_swt_init;
	soem_port.port_send = netc_swt_send;
	soem_port.port_recv = netc_swt_recv;
	soem_port.port_close = netc_swt_close;
	soem_port.port_link_status= netc_swt_link_status;
	soem_port.port_pri = &if_port;
    return register_soem_port(&soem_port);
}

static status_t APP_PHY_SetPort(uint32_t port, phy_config_t *phyConfig)
{
    status_t result = kStatus_Success;
    uint16_t regValue;

    s_phy_resource[port].write = NETC_EP_EMDIOWrite;
    s_phy_resource[port].read  = NETC_EP_EMDIORead;

    result = PHY_Init(&if_port.phy_handle, phyConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    PHY_Write(&if_port.phy_handle, PHY_EXT_ADDR_REG, PHY_RGMII_CONFIG1_REG);
    PHY_Read(&if_port.phy_handle, PHY_EXT_DATA_REG, &regValue);
    regValue &= ~PHY_RGMII_CONFIG1_TXDLY_MASK;
    /* 150ps per step. */
    regValue |= 0x2;
    PHY_Write(&if_port.phy_handle, PHY_EXT_DATA_REG, regValue);

    return result;
}

status_t NETC_PHY_Init(void)
{
    status_t result            = kStatus_Success;
    phy_config_t phyyt8521Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed100M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyyt8521_ops,
    };

    /* Reset all PHYs even some are not used in case unstable status has effect on other PHYs. */
    /* Reset PHY8201 for ETH4(EP), ETH0(Switch port0). Power on 150ms, reset 10ms, wait 150ms. */
    /* Reset PHY8211 for ETH1(Switch port1), ETH2(Switch port2), ETH3(Switch port3). Reset 10ms, wait 30ms. */
    RGPIO_PinWrite(EXAMPLE_SWT_PORT0_PHY_RESET_PIN, 0);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT2_PHY_RESET_PIN, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    RGPIO_PinWrite(EXAMPLE_SWT_PORT0_PHY_RESET_PIN, 1);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT2_PHY_RESET_PIN, 1);
    SDK_DelayAtLeastUs(150000, CLOCK_GetFreq(kCLOCK_CpuClk));

    /* Initialize PHY for switch port0. */
    phyyt8521Config.resource = &s_phy_resource[EXAMPLE_SWT_PORT0];
    phyyt8521Config.phyAddr  = BOARD_SWT_PORT0_PHY_ADDR;
    result                 = APP_PHY_SetPort(EXAMPLE_SWT_PORT0, &phyyt8521Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

status_t BOARD_InitHardware(void)
{
    bool link;
    status_t result = kStatus_Success;
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitNETPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_NETC_Init();

    PRINTF("Init BOARD_InitHardware success...\r\n");

    result = NETC_MDIO_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nMDIO Init failed!\r\n");
        return result;
    }

    result = NETC_PHY_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nPHY Init failed!\r\n");
        return result;
    }

    do
    {
		PHY_GetLinkStatus(&if_port.phy_handle, &link);
    } while (!link);

    return result;
}