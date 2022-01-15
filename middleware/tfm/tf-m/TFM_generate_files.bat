set TFM_TEST_PATH=../tf-m-tests/test

python tools/tfm_parse_manifest_list.py -m tools/tfm_manifest_list.yaml ../tf-m-tests/test/test_services/tfm_test_manifest_list.yaml -f tools/tfm_generated_file_list.yaml platform/ext/target/nxp/common/generated_file_list.yaml -t %TFM_TEST_PATH% -o .

pause