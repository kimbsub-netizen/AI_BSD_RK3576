#include <chrono>
#include <cstdio>
#include <cstdlib>
#include "yolo11.h"
#include "image_utils.h"

// Link with the Model Zoo YOLO11 implementation. Image decoding and model
// initialization are outside the timer; preprocessing and postprocessing are inside.
int main(int argc, char **argv) {
    if (argc != 3) return 2;
    rknn_app_context_t ctx = {};
    image_buffer_t image = {};
    object_detect_result_list results = {};
    init_post_process();
    if (init_yolo11_model(argv[1], &ctx) != 0) return 3;
    if (read_image(argv[2], &image) != 0) return 4;
    rknn_sdk_version version = {};
    if (rknn_query(ctx.rknn_ctx, RKNN_QUERY_SDK_VERSION, &version, sizeof(version)) == 0)
        fprintf(stderr, "runtime=%s driver=%s\n", version.api_version, version.drv_version);
    // Suppress per-frame demo logging so terminal I/O does not dominate timing.
    if (!freopen("/dev/null", "w", stdout)) return 5;
    for (int i = 0; i < 10; ++i)
        if (inference_yolo11_model(&ctx, &image, &results) != 0) return 6;
    for (int trial = 0; trial < 3; ++trial) {
        const auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 100; ++i) {
            if (inference_yolo11_model(&ctx, &image, &results) != 0 || results.count == 0) {
                fprintf(stderr, "inference failed or empty detection\n");
                return 7;
            }
        }
        const double ms = std::chrono::duration<double, std::milli>(
            std::chrono::steady_clock::now() - start).count();
        fprintf(stderr, "trial=%d frames=100 total_ms=%.3f mean_ms=%.3f fps=%.3f detections=%d\n",
                trial + 1, ms, ms / 100, 100000 / ms, results.count);
    }
    free(image.virt_addr);
    release_yolo11_model(&ctx);
    deinit_post_process();
    return 0;
}
