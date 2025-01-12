#include "main.h"

int main(){
    App& app = App::getInstance();
	app.setup(1024, 768, "lab5", true);

    while (!glfwWindowShouldClose(app.getWindow())){
        app.update();
        app.draw();
        glfwSwapBuffers(app.getWindow());
        glfwPollEvents();
    }

    cout << "main func()" << endl;
    app.exit();
    return 0;
}