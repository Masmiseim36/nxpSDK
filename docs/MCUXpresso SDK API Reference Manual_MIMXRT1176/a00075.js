var a00075 =
[
    [ "rdc_hardware_config_t", "a00075.html#a00628", [
      [ "domainNumber", "a00075.html#a739ea4a24186ac54d3d97ec1bcda5631", null ],
      [ "masterNumber", "a00075.html#a81842ff8d313ca38916e3226efdd3bde", null ],
      [ "periphNumber", "a00075.html#ac02b7c759b76aac047c8dfd86fd93a59", null ],
      [ "memNumber", "a00075.html#afb605761a847c152ab23a5790b44dee9", null ]
    ] ],
    [ "rdc_domain_assignment_t", "a00075.html#a00627", [
      [ "domainId", "a00075.html#a6e918db4f50ef773621e9f90a8dd0aa1", null ],
      [ "__pad0__", "a00075.html#a8d76c2ce9f3ab1bd748c21c409e14b08", null ],
      [ "lock", "a00075.html#add7373049bb837ae009d9d6f54d6c042", null ]
    ] ],
    [ "rdc_periph_access_config_t", "a00075.html#a00631", [
      [ "periph", "a00075.html#a7472e8b72f7646864b1244f491559d48", null ],
      [ "lock", "a00075.html#a9a39e94946c18bba0593a7d063e39ead", null ],
      [ "enableSema", "a00075.html#ae3359bd3ece8b0075310586208b0f18e", null ],
      [ "policy", "a00075.html#a8e401a7aabd8cd79807858b4eb89cd70", null ]
    ] ],
    [ "rdc_mem_access_config_t", "a00075.html#a00629", [
      [ "mem", "a00075.html#ad4e1e98d9c5b9bf9fbbadf2f90c46dfc", null ],
      [ "lock", "a00075.html#af7b74c739bdc6cf8d13c3a9679ea934b", null ],
      [ "baseAddress", "a00075.html#a9407a23678502889ab8eccd55cba8377", null ],
      [ "endAddress", "a00075.html#a2d4f63927e154de9e68610e77d1a5978", null ],
      [ "policy", "a00075.html#aa8fa0fa776b881c0b884461f4f96f827", null ]
    ] ],
    [ "rdc_mem_status_t", "a00075.html#a00630", [
      [ "hasViolation", "a00075.html#a64e77210e7d78eab8f492aec96ab1812", null ],
      [ "domainID", "a00075.html#a9cdb8148ebf80d343b95716e03964127", null ],
      [ "address", "a00075.html#ac26082e85ff1d7e0874fc5a2467a904c", null ]
    ] ],
    [ "_rdc_interrupts", "a00075.html#ga43f5d8d1ed667a8c73ab204a74925892", [
      [ "kRDC_RestoreCompleteInterrupt", "a00075.html#gga43f5d8d1ed667a8c73ab204a74925892a5088cfe20c6ef9e259657e6f6ecc7387", null ]
    ] ],
    [ "_rdc_flags", "a00075.html#ga30afa024532b0207bdeeee3344af9ebd", [
      [ "kRDC_PowerDownDomainOn", "a00075.html#gga30afa024532b0207bdeeee3344af9ebda5e58c4dabb46b1f4a47b7ae8cd0f61e3", null ]
    ] ],
    [ "_rdc_access_policy", "a00075.html#gad1f726221167c740e1500317ea80beb2", [
      [ "kRDC_NoAccess", "a00075.html#ggad1f726221167c740e1500317ea80beb2a3abbfd7f6818ca32df8d30300aba7f1a", null ],
      [ "kRDC_WriteOnly", "a00075.html#ggad1f726221167c740e1500317ea80beb2af704f2f758ba0d96ddea53e686325efe", null ],
      [ "kRDC_ReadOnly", "a00075.html#ggad1f726221167c740e1500317ea80beb2acad6ad0fa9f2d31d48ec9e7b2898cdb6", null ],
      [ "kRDC_ReadWrite", "a00075.html#ggad1f726221167c740e1500317ea80beb2a45a95e9ad49ab28807dc57a67e770ebd", null ]
    ] ],
    [ "RDC_Init", "a00075.html#ga5e55854cf5534e9ea2844c6447630d29", null ],
    [ "RDC_Deinit", "a00075.html#gad67bd7441a6770382a42871a73cab1e9", null ],
    [ "RDC_GetHardwareConfig", "a00075.html#ga70d043c746306a1518a9d76c708a45a7", null ],
    [ "RDC_EnableInterrupts", "a00075.html#gac4fd27fb3b91add96b402a316f5257d1", null ],
    [ "RDC_DisableInterrupts", "a00075.html#ga8431d5742c72d100e9d5ea8acb828164", null ],
    [ "RDC_GetInterruptStatus", "a00075.html#ga2daf610d7330112bdefdaf737d88057d", null ],
    [ "RDC_ClearInterruptStatus", "a00075.html#ga46324815d4934688c553fa427068aebf", null ],
    [ "RDC_GetStatus", "a00075.html#ga607d3cb8467e3dad81c01b99b262b025", null ],
    [ "RDC_ClearStatus", "a00075.html#gaa78b6ce74c4abe2fb4f8696dba4bab58", null ],
    [ "RDC_SetMasterDomainAssignment", "a00075.html#gabe2b9840b82e64d3cc421df537450dca", null ],
    [ "RDC_GetDefaultMasterDomainAssignment", "a00075.html#ga5079802654902bb5b1bb618958b136e1", null ],
    [ "RDC_LockMasterDomainAssignment", "a00075.html#ga7bac728610bbf0d1fc8ef54684453a67", null ],
    [ "RDC_SetPeriphAccessConfig", "a00075.html#gad7761fcab34b52ebf80024199d0390bc", null ],
    [ "RDC_GetDefaultPeriphAccessConfig", "a00075.html#ga3960b46c770474ae90a3efe6bbf0848f", null ],
    [ "RDC_LockPeriphAccessConfig", "a00075.html#gad726f804c6f29e6f08014749883816f8", null ],
    [ "RDC_GetPeriphAccessPolicy", "a00075.html#ga5629cde26ba781eeb933acef2a6f95e5", null ],
    [ "RDC_SetMemAccessConfig", "a00075.html#ga8a2282cbee8cbb2eeb38c98325351d64", null ],
    [ "RDC_GetDefaultMemAccessConfig", "a00075.html#gad38513d641ad47ccbb003c47bb3c9727", null ],
    [ "RDC_LockMemAccessConfig", "a00075.html#ga8e9f9b9bb90724ed625ee5b5cd2a8d41", null ],
    [ "RDC_SetMemAccessValid", "a00075.html#ga50e5d41f56901ea8924e6bb1f6e92fbd", null ],
    [ "RDC_GetMemViolationStatus", "a00075.html#ga76711caa424d57a32847831f60cac1aa", null ],
    [ "RDC_ClearMemViolationFlag", "a00075.html#gaf594d7e71d47df3bdc89a62e45b944a1", null ],
    [ "RDC_GetMemAccessPolicy", "a00075.html#gab4b8ce2df7a0ccd22d90d897a80616fb", null ],
    [ "RDC_GetCurrentMasterDomainId", "a00075.html#gacf0861a99d3a45d022fef5dd35f158c5", null ]
];