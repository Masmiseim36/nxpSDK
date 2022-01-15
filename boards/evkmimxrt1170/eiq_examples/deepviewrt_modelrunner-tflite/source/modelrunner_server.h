#include <inttypes.h>
#include <stdlib.h>

/**
 * DeepView ModelRunner Server
 */
typedef int(inference_func)(uint8_t *constantWeight, uint8_t *mutableWeight, uint8_t *activations);
struct nn_server {
    const char* model_name;
    size_t      model_size;
    char*       json_buffer;
    size_t      json_size;
    int*   input_dims_data;
    struct {
        int64_t run;
        int64_t decode;
        int64_t input;
        int64_t output;
    } timing;
    struct {
        int32_t num_outputs;
        char* name[1024];
        char* type[1024];
        int64_t timing[1024];
        int32_t index[16];
        size_t bytes[16];
        char* data[16];
        char data_type[16][20];
        int32_t outputs_size;
        int* shape_data[16];
        int32_t shape_size[16];
    } output;
    uint8_t* image_upload_data;
    char* model_upload;
    int inference_count;
    int64_t run_ns;
};

typedef struct nn_server NNServer;

int64_t os_clock_now();
