var group__mpu =
[
    [ "mpu_hardware_info_t", "group__mpu.html#structmpu__hardware__info__t", [
      [ "hardwareRevisionLevel", "group__mpu.html#a9151d94a0f729f0a81141799f835a0b7", null ],
      [ "slavePortsNumbers", "group__mpu.html#a32c5de85469bce1845c9d3f5ae099f61", null ],
      [ "regionsNumbers", "group__mpu.html#aada15bb7a3c8da54850ce36b38205193", null ]
    ] ],
    [ "mpu_access_err_info_t", "group__mpu.html#structmpu__access__err__info__t", [
      [ "master", "group__mpu.html#a0908225f6e8e83d72202f302d9a6256e", null ],
      [ "attributes", "group__mpu.html#ab3ed10ced06d1599aa69d1a36961888a", null ],
      [ "accessType", "group__mpu.html#a3924a882672ce6f2395d56e71511b809", null ],
      [ "accessControl", "group__mpu.html#a43eb5bf276b4403136983951b0d970e6", null ],
      [ "address", "group__mpu.html#a7dcb9e77f50805cbe5de0aa1d92370cc", null ],
      [ "processorIdentification", "group__mpu.html#a8b4017758e22781531fbfe233e1acb7d", null ]
    ] ],
    [ "mpu_rwxrights_master_access_control_t", "group__mpu.html#structmpu__rwxrights__master__access__control__t", [
      [ "superAccessRights", "group__mpu.html#a66ca797172b664b3aee3fc9a7b72e601", null ],
      [ "userAccessRights", "group__mpu.html#a543446fd81432567d537da95d5dfc0fa", null ],
      [ "processIdentifierEnable", "group__mpu.html#a8874eba4efe22b98a569ff76554f8fe7", null ]
    ] ],
    [ "mpu_rwrights_master_access_control_t", "group__mpu.html#structmpu__rwrights__master__access__control__t", [
      [ "writeEnable", "group__mpu.html#ad33f073e12c847021eff2f94599a0582", null ],
      [ "readEnable", "group__mpu.html#a26e3197aa3988f9ed4caa51607978d7b", null ]
    ] ],
    [ "mpu_region_config_t", "group__mpu.html#structmpu__region__config__t", [
      [ "regionNum", "group__mpu.html#a1de06a7112ca3fcf8d317091a7d8ca9d", null ],
      [ "startAddress", "group__mpu.html#a164cc4080c21cd692476167478306e75", null ],
      [ "endAddress", "group__mpu.html#a6c5a67a1d96b35c14bc62104a4e33307", null ],
      [ "accessRights1", "group__mpu.html#a1d59068d1cd8710f1095673c8f7e0002", null ],
      [ "accessRights2", "group__mpu.html#a57ad694a629c3ba5123e956246caeaf0", null ],
      [ "processIdentifier", "group__mpu.html#aa2e1423b049a12fa9447cf261fcbd06d", null ],
      [ "processIdMask", "group__mpu.html#a24d425f1e16768b1efd8efbffd8fe492", null ]
    ] ],
    [ "mpu_config_t", "group__mpu.html#structmpu__config__t", [
      [ "regionConfig", "group__mpu.html#a4ab6118addd1c60c568c37216841e9de", null ],
      [ "next", "group__mpu.html#a00d6c4fbccadbe15301fce8fd1b1749c", null ]
    ] ],
    [ "FSL_MPU_DRIVER_VERSION", "group__mpu.html#ga4c460ce03eda34f01b0f9f0ac5565a96", null ],
    [ "MPU_REGION_RWXRIGHTS_MASTER_SHIFT", "group__mpu.html#gaa9bfcc16e335a6926acce19bebe45158", null ],
    [ "MPU_REGION_RWXRIGHTS_MASTER_MASK", "group__mpu.html#ga83936968fa9d031c6d466b21c2b460c4", null ],
    [ "MPU_REGION_RWXRIGHTS_MASTER_WIDTH", "group__mpu.html#gaf58231719dc265b21a8478e544bdb371", null ],
    [ "MPU_REGION_RWXRIGHTS_MASTER", "group__mpu.html#ga013f9e6e3b148b93fa34c20eae50be6e", null ],
    [ "MPU_REGION_RWXRIGHTS_MASTER_PE_SHIFT", "group__mpu.html#ga67a94038067c90f0436311d6b45fbd32", null ],
    [ "MPU_REGION_RWXRIGHTS_MASTER_PE_MASK", "group__mpu.html#ga53808fa3feb98f7b8c387177471aa008", null ],
    [ "MPU_REGION_RWXRIGHTS_MASTER_PE", "group__mpu.html#ga15aee92c186c08bb5df9fa163ece6969", null ],
    [ "MPU_REGION_RWRIGHTS_MASTER_SHIFT", "group__mpu.html#ga6d92d09430dbcfad0ae779f64c815712", null ],
    [ "MPU_REGION_RWRIGHTS_MASTER_MASK", "group__mpu.html#gabe26c88762cb71ffa79da663ad7b2616", null ],
    [ "MPU_REGION_RWRIGHTS_MASTER", "group__mpu.html#ga80ae79132df08f45deaf340d2162ec98", null ],
    [ "MPU_SLAVE_PORT_NUM", "group__mpu.html#ga475cf5caf2d379b6d71ee55c81710c7e", null ],
    [ "MPU_PRIVILEGED_RIGHTS_MASTER_MAX_INDEX", "group__mpu.html#ga0b360e26aadd9430b4556f7d4ca8bc54", null ],
    [ "mpu_region_total_num_t", "group__mpu.html#ga22906b087e45d639905ecb796af01bb7", [
      [ "kMPU_8Regions", "group__mpu.html#gga22906b087e45d639905ecb796af01bb7ae6854272f7f31b1ad5b8d3300f14a24a", null ],
      [ "kMPU_12Regions", "group__mpu.html#gga22906b087e45d639905ecb796af01bb7ae37ef4d6add90ca862be928b5f033ad4", null ],
      [ "kMPU_16Regions", "group__mpu.html#gga22906b087e45d639905ecb796af01bb7a4145bdad95839fc4506fee285ef8ad81", null ]
    ] ],
    [ "mpu_slave_t", "group__mpu.html#ga1fe07463657cc0882a88ab9c02a989aa", [
      [ "kMPU_Slave0", "group__mpu.html#gga1fe07463657cc0882a88ab9c02a989aaaf496eb7d8701cc2e9c5099d3c0a33085", null ],
      [ "kMPU_Slave1", "group__mpu.html#gga1fe07463657cc0882a88ab9c02a989aaa242909166988d8133b610fc9dc673a08", null ],
      [ "kMPU_Slave2", "group__mpu.html#gga1fe07463657cc0882a88ab9c02a989aaa8500d6b41cada6f00bf46f19210f062a", null ],
      [ "kMPU_Slave3", "group__mpu.html#gga1fe07463657cc0882a88ab9c02a989aaabd052a781b0d639f29a1e6bc9a1af359", null ],
      [ "kMPU_Slave4", "group__mpu.html#gga1fe07463657cc0882a88ab9c02a989aaaeacc91dc2d68cc87fb08858f706e7c6c", null ]
    ] ],
    [ "mpu_err_access_control_t", "group__mpu.html#ga68a5f43a3a05a8a80f61db308e2d7a37", [
      [ "kMPU_NoRegionHit", "group__mpu.html#gga68a5f43a3a05a8a80f61db308e2d7a37a74e15a7d0dbe525d0a3321409b748e76", null ],
      [ "kMPU_NoneOverlappRegion", "group__mpu.html#gga68a5f43a3a05a8a80f61db308e2d7a37acbf14cd65cfedf4d9b3f4cc23db2d2ee", null ],
      [ "kMPU_OverlappRegion", "group__mpu.html#gga68a5f43a3a05a8a80f61db308e2d7a37a478d3db549b6e14ddcffb725faab1649", null ]
    ] ],
    [ "mpu_err_access_type_t", "group__mpu.html#ga688f512b6643c1861646c5e3fb944fb5", [
      [ "kMPU_ErrTypeRead", "group__mpu.html#gga688f512b6643c1861646c5e3fb944fb5ad1137740dfd53f93a311799e0cc30567", null ],
      [ "kMPU_ErrTypeWrite", "group__mpu.html#gga688f512b6643c1861646c5e3fb944fb5a806728b032863e650715be3db2940c8b", null ]
    ] ],
    [ "mpu_err_attributes_t", "group__mpu.html#ga8643ef655a2714210f68b1b7b183fdbb", [
      [ "kMPU_InstructionAccessInUserMode", "group__mpu.html#gga8643ef655a2714210f68b1b7b183fdbbad55b006b1f0a45e37e08c75c66f113b5", null ],
      [ "kMPU_DataAccessInUserMode", "group__mpu.html#gga8643ef655a2714210f68b1b7b183fdbbaf0dd3c8e53bf446c0812a7faf46ebafb", null ],
      [ "kMPU_InstructionAccessInSupervisorMode", "group__mpu.html#gga8643ef655a2714210f68b1b7b183fdbba468fc5454ccd964bc57f5b54231b5b7a", null ],
      [ "kMPU_DataAccessInSupervisorMode", "group__mpu.html#gga8643ef655a2714210f68b1b7b183fdbba4ca85c46fb13dc782e44499d7487c2e8", null ]
    ] ],
    [ "mpu_supervisor_access_rights_t", "group__mpu.html#ga55d1dc350059dad9011157b7ca807751", [
      [ "kMPU_SupervisorReadWriteExecute", "group__mpu.html#gga55d1dc350059dad9011157b7ca807751a53875d9221ed801dc21fb1d82a214a19", null ],
      [ "kMPU_SupervisorReadExecute", "group__mpu.html#gga55d1dc350059dad9011157b7ca807751aa1560f5c356ff583ebd4f474a3c6aa45", null ],
      [ "kMPU_SupervisorReadWrite", "group__mpu.html#gga55d1dc350059dad9011157b7ca807751a056152cddabc1d0adc58e7c90cf928bf", null ],
      [ "kMPU_SupervisorEqualToUsermode", "group__mpu.html#gga55d1dc350059dad9011157b7ca807751ab2d6f97e6384959cddc969ed0caddf6e", null ]
    ] ],
    [ "mpu_user_access_rights_t", "group__mpu.html#gaa0dbe6632c1bf7e38ac7b77337192161", [
      [ "kMPU_UserNoAccessRights", "group__mpu.html#ggaa0dbe6632c1bf7e38ac7b77337192161afa3ebc7d527613a7fd56abee1cfcc995", null ],
      [ "kMPU_UserExecute", "group__mpu.html#ggaa0dbe6632c1bf7e38ac7b77337192161af3918eb4b24270b99b93d322b3cbdcbd", null ],
      [ "kMPU_UserWrite", "group__mpu.html#ggaa0dbe6632c1bf7e38ac7b77337192161a0ab4da6522e492c023f13288832fa1d0", null ],
      [ "kMPU_UserWriteExecute", "group__mpu.html#ggaa0dbe6632c1bf7e38ac7b77337192161af1be142c0d368184a4e14d58f78e0823", null ],
      [ "kMPU_UserRead", "group__mpu.html#ggaa0dbe6632c1bf7e38ac7b77337192161ac3bf01311a494883741f20a7601357b3", null ],
      [ "kMPU_UserReadExecute", "group__mpu.html#ggaa0dbe6632c1bf7e38ac7b77337192161ad886815a8418d632a11580241cec12ab", null ],
      [ "kMPU_UserReadWrite", "group__mpu.html#ggaa0dbe6632c1bf7e38ac7b77337192161af73525f8a94509fc8e4307d536c1f72d", null ],
      [ "kMPU_UserReadWriteExecute", "group__mpu.html#ggaa0dbe6632c1bf7e38ac7b77337192161a9d640b8cf940124f0dee9ad511c09ef2", null ]
    ] ],
    [ "MPU_Init", "group__mpu.html#ga65263dc5500d98c4a505c9e828cc8abc", null ],
    [ "MPU_Deinit", "group__mpu.html#gac592c73f8bbc7daf72c2ebaf7d3424cf", null ],
    [ "MPU_Enable", "group__mpu.html#ga986f9d3d06f4ed56212aaf3525c699a5", null ],
    [ "MPU_RegionEnable", "group__mpu.html#ga093f27a588f101052779d50606d4c2ee", null ],
    [ "MPU_GetHardwareInfo", "group__mpu.html#ga769e4d49b69bfadc6c575c30b49ca164", null ],
    [ "MPU_SetRegionConfig", "group__mpu.html#ga97a9ec7eee3fc0f8ed19214fe437676b", null ],
    [ "MPU_SetRegionAddr", "group__mpu.html#ga5b072302d50c4ecc54906f643ce86f91", null ],
    [ "MPU_SetRegionRwxMasterAccessRights", "group__mpu.html#ga2ff28bf83fcf210b50dc86f75e95205d", null ],
    [ "MPU_SetRegionRwMasterAccessRights", "group__mpu.html#ga316ea2119d62d7f350e0f5b9eb987713", null ],
    [ "MPU_GetSlavePortErrorStatus", "group__mpu.html#gad36649693bde12744ac6aeb317a57e88", null ],
    [ "MPU_GetDetailErrorAccessInfo", "group__mpu.html#gaca61af7eb9e6f2358049826c2a9058a0", null ]
];