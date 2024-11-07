#include "HelloVK.h"
#include "Tools.h"

#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <cstdlib>

struct VulkanEngine {
    struct android_app *app;
    HelloVK *app_backend;
    bool canRender = false;
};

/**
 * Called by the Android runtime whenever events happen so the
 * app can react to it.
 */
static void HandleCmd(struct android_app *app, int32_t cmd) {
    auto *engine = (VulkanEngine *)app->userData;
    switch (cmd) {
        case APP_CMD_START:
            if (engine->app->window != nullptr) {
                engine->app_backend->reset(app->window, app->activity->assetManager);
                engine->app_backend->initVulkan();
                engine->canRender = true;
            }
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            LOGI("Called - APP_CMD_INIT_WINDOW");
            if (engine->app->window != nullptr) {
                LOGI("Setting a new surface");
                engine->app_backend->reset(app->window, app->activity->assetManager);
                if (!engine->app_backend->initialized) {
                    LOGI("Starting application");
                    engine->app_backend->initVulkan();
                }
                engine->canRender = true;
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            engine->canRender = false;
            break;
        case APP_CMD_DESTROY:
            // The window is being hidden or closed, clean it up.
            LOGI("Destroying");
            engine->app_backend->cleanup();
        default:
            break;
    }
}

/*
 * Key events filter to GameActivity's android_native_app_glue. This sample does
 * not use/process any input events, return false for all input events so system
 * can still process them.
 */
extern "C" bool VulkanKeyEventFilter(const GameActivityKeyEvent *event) {
    return false;
}
extern "C" bool VulkanMotionEventFilter(const GameActivityMotionEvent *event) {
    return false;
}

/*
 * Process user touch and key events. GameActivity double buffers those events,
 * applications can process at any time. All of the buffered events have been
 * reported "handled" to OS. For details, refer to:
 * d.android.com/games/agdk/game-activity/get-started#handle-events
 */
static void HandleInputEvents(struct android_app *app) {
    auto inputBuf = android_app_swap_input_buffers(app);
    if (inputBuf == nullptr) {
        return;
    }

    // For the minimum, apps need to process the exit event (for example,
    // listening to AKEYCODE_BACK). This sample has done that in the Kotlin side
    // and not processing other input events, we just reset the event counter
    // inside the android_input_buffer to keep app glue code in a working state.
    android_app_clear_motion_events(inputBuf);
    android_app_clear_motion_events(inputBuf);
}

/*
 * Entry point required by the Android Glue library.
 * This can also be achieved more verbosely by manually declaring JNI functions
 * and calling them from the Android application layer.
 */
void android_main(struct android_app *state) {
    VulkanEngine engine{};
    HelloVK vulkanBackend{};

    engine.app = state;
    engine.app_backend = &vulkanBackend;
    state->userData = &engine;
    state->onAppCmd = HandleCmd;

    android_app_set_key_event_filter(state, VulkanKeyEventFilter);
    android_app_set_motion_event_filter(state, VulkanMotionEventFilter);

    android_poll_source *source;
    int result;

    while (!state->destroyRequested) {
        result = ALooper_pollOnce(engine.canRender ? 0 : -1, nullptr, nullptr,
                                       (void **)&source);
        if (result == ALOOPER_POLL_ERROR) {
            LOGE("ALooper_pollOnce returned an error");
            std::abort();
        }

        if (source != nullptr) {
            source->process(state, source);
        }

        HandleInputEvents(state);

        if (engine.canRender) {
            engine.app_backend->render();
        }
    }
}