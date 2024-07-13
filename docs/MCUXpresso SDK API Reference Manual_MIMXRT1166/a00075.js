var a00075 =
[
    [ "_rdc_hardware_config", "a00075.html#a00625", [
      [ "domainNumber", "a00075.html#a0a1c71ea8794188dc975c8308c737dbb", null ],
      [ "masterNumber", "a00075.html#a5173b5ae360696c710d9c28696342e7e", null ],
      [ "periphNumber", "a00075.html#a7f5b8abeaf670f544b25bd22e51f584d", null ],
      [ "memNumber", "a00075.html#a8ed986154479237da36ccd3f6241ffee", null ]
    ] ],
    [ "_rdc_domain_assignment", "a00075.html#a00624", [
      [ "domainId", "a00075.html#a5bbfcb6e3db56bcca10e6c5f7ee9de4d", null ],
      [ "__pad0__", "a00075.html#a48fe208537eb4029cbe45bbb77fcae66", null ],
      [ "lock", "a00075.html#a41881095499e658420cfc5efda6c5caa", null ]
    ] ],
    [ "_rdc_periph_access_config", "a00075.html#a00628", [
      [ "periph", "a00075.html#a242a1e54f86e21e833da3140902eaec1", null ],
      [ "lock", "a00075.html#ab8b7b20b2b41579fea486131de105468", null ],
      [ "enableSema", "a00075.html#a8d0c915fee91b8ec6fea7c2391c17e8f", null ],
      [ "policy", "a00075.html#a7b0f6bba021eb4c36e304ff2ac9a6b66", null ]
    ] ],
    [ "_rdc_mem_access_config", "a00075.html#a00626", [
      [ "mem", "a00075.html#aab16c859d011f27f18e8ef76f81439f4", null ],
      [ "lock", "a00075.html#ab2dac0a75be9929c8e12a92b37f1b8e4", null ],
      [ "baseAddress", "a00075.html#af42d89ec16e7b15c75c8a63b667d5ded", null ],
      [ "endAddress", "a00075.html#a57f9536d7f3b6f2f8b4bd38104e280eb", null ],
      [ "policy", "a00075.html#aac58d77bc6cc194f6902dffe4e80e0d6", null ]
    ] ],
    [ "_rdc_mem_status", "a00075.html#a00627", [
      [ "hasViolation", "a00075.html#ad4dad1cd9f4e66b664b525ecd4b1be37", null ],
      [ "domainID", "a00075.html#a28ba9cfc8af0f862635ded4738718c78", null ],
      [ "address", "a00075.html#a8613c0efb2da82d09f7f67a5a3efee4f", null ]
    ] ],
    [ "rdc_hardware_config_t", "a00075.html#ga5809f23a3763c0c2d114c8b991b19224", null ],
    [ "rdc_domain_assignment_t", "a00075.html#ga2fa0f432125eabe3c7444a58ac406b10", null ],
    [ "rdc_periph_access_config_t", "a00075.html#gab34ae3929d80075974affc8e2e4a1225", null ],
    [ "rdc_mem_access_config_t", "a00075.html#ga7fa4bee53cb416b150b75ad4a5bfca61", null ],
    [ "rdc_mem_status_t", "a00075.html#gaccf5405a9b3fc24040bd8f066f8b8a83", null ],
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