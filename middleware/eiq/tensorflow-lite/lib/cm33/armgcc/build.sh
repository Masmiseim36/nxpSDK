git clone https://github.com/tensorflow/tflite-micro.git tflite-micro-2023-09-18-127f88f
cd tflite-micro-2023-09-18-127f88f
git checkout 127f88f
make -f tensorflow/lite/micro/tools/make/Makefile BUILD_TYPE=release_with_logs TARGET=cortex_m_generic TARGET_ARCH=cortex-m33 OPTIMIZED_KERNEL_DIR=cmsis_nn microlite
cp gen/cortex_m_generic_cortex-m33_release_with_logs/lib/libtensorflow-microlite.a ../libtflm.a
cd ..
