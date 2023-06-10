#include <gtk/gtk.h>
#include <GL/glew.h>

/* GLOBAL DATA (for now...) */

float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
};

const char *vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}";

const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    vec2 uv = gl_FragCoord.xy / vec2(600.0f, 600.0f);\n"
//        "    FragColor = vec4(uv.xy, 0.0f, 1.0f);\n"
        "    FragColor = vec4(uv.x, 0.0f, uv.y, 1.0f);\n"
        "} ";


static gboolean render(GtkGLArea *area, GdkGLContext *context) {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    return TRUE;
}

static void init_objects() {

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);


}

static void init_shaders() {

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glUseProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Aqui é inicializado o OpenGL
static void on_realize(GtkGLArea *area) {
    gtk_gl_area_make_current(area); // Precisamos tornar o context atual se quisermos chamar a API do OpenGL

    if (gtk_gl_area_get_error(area) != NULL) return;

    glewInit(); // Inicializa os ponteiros das funções do OpenGL

    GdkGLContext *context = gtk_gl_area_get_context(GTK_GL_AREA (area));

    init_objects();
    init_shaders();

}

void setup_glarea(GtkWidget *window) {
    GtkWidget *gl_area = gtk_gl_area_new();
    g_signal_connect(gl_area, "realize", G_CALLBACK(on_realize), NULL);
    g_signal_connect(gl_area, "render", G_CALLBACK(render), NULL);
    gtk_window_set_child(GTK_WINDOW(window), gl_area);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "OpenGL shenanigans");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
    gtk_widget_show(window);

    setup_glarea(window);

}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.willianschuck.learning_c", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
