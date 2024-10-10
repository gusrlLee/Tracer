#import "mt_window_connector.h"

namespace MTL {
NS::Window* ConnectWindow(GLFWwindow* glfwWindow, CA::MetalLayer* layer) {
    CALayer* objLayer = (__bridge CALayer*) layer;
    NSWindow* objWindow = glfwGetCocoaWindow(glfwWindow);
    objWindow.contentView.layer = objLayer;
    objWindow.contentView.wantsLayer = YES;

    return (__bridge NS::Window*)objWindow;
}
}