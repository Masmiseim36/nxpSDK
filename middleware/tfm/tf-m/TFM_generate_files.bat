set TFM_TEST_PATH=../tf-m-tests/test

python tools/tfm_parse_manifest_list.py -m tools/tfm_manifest_list.yaml -f tools/tfm_generated_file_list.yaml -o output

pause