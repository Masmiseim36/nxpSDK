set TFM_TEST_PATH=../tf-m-tests/test/secure_fw
set TFM_OUTPUT_PATH=./output

rem python tools/tfm_parse_manifest_list.py -l 2 -b IPC -m tools/tfm_manifest_list.yaml ./ ../tf-m-tests/test/secure_fw/tfm_test_manifest_list.yaml ./ -f tools/tfm_generated_file_list.yaml platform/ext/target/nxp/common/generated_file_list.yaml -t %TFM_TEST_PATH% -o %TFM_OUTPUT_PATH%

python tools/tfm_parse_manifest_list.py -m tools/tfm_manifest_list.yaml ../tf-m-tests/test/secure_fw/tfm_test_manifest_list.yaml -f tools/tfm_generated_file_list.yaml platform/ext/target/nxp/common/generated_file_list.yaml -c ./manifest_config.h -t %TFM_TEST_PATH% -o %TFM_OUTPUT_PATH%


pause

rem More info: https://tf-m-user-guide.trustedfirmware.org/integration_guide/services/tfm_manifest_tool_user_guide.html
