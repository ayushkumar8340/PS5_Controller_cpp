#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <vector>
#include <string.h>

#define DEVICE "/dev/input/js0" // Adjust if necessary

int main() {
    int js_fd = open(DEVICE, O_RDONLY | O_NONBLOCK);
    if (js_fd < 0) {
        std::cerr << "Failed to open " << DEVICE << std::endl;
        return 1;
    }

    int num_axes = 0, num_buttons = 0;
    char name[128];

    ioctl(js_fd, JSIOCGAXES, &num_axes);
    ioctl(js_fd, JSIOCGBUTTONS, &num_buttons);
    ioctl(js_fd, JSIOCGNAME(sizeof(name)), name);

    std::cout << "Controller Name: " << name << std::endl;
    std::cout << "Axes: " << num_axes << ", Buttons: " << num_buttons << std::endl;

    std::vector<float> axes(num_axes, 0.0f);
    std::vector<int> buttons(num_buttons, 0);

    struct js_event js;

    while (true) {
        ssize_t bytes = read(js_fd, &js, sizeof(js_event));
        if (bytes == sizeof(js_event)) {
            js.type &= ~JS_EVENT_INIT; // Filter init events

            if (js.type == JS_EVENT_AXIS && js.number < axes.size()) {
                axes[js.number] = js.value / 32767.0f;
            } else if (js.type == JS_EVENT_BUTTON && js.number < buttons.size()) {
                buttons[js.number] = js.value;
            }

            // Print current state
            std::cout << "\rAxes: ";
            // for (float a : axes) std::cout << a << " ";
            
            // std::cout<<axes[7]<<std::endl;

            std::cout << "| Buttons: ";
            for (int b : buttons) std::cout << b << " ";
            std::cout << std::flush;
        }

        usleep(1000); // Small delay to avoid 100% CPU usage
    }

    close(js_fd);
    return 0;
}
