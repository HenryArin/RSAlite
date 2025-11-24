#include <gtk/gtk.h>
#include <stdint.h>
#include "factor.h"
#include "prime.h"
#include <limits.h>

typedef struct {
    GtkWidget *window;
    GtkWidget *entry;
    GtkWidget *result_view;

    FactorMethod method;
    GtkWidget *trial_button;
    GtkWidget *sqrt_button;
} AppWidgets;


static void on_entry_insert_text(GtkEditable *editable,
                                 const gchar *text,
                                 gint length,
                                 gint *position,
                                 gpointer user_data);

static void on_factor_clicked(GtkButton *button, gpointer user_data);
static void on_clear_clicked(GtkButton *button, gpointer user_data);
static void on_quit_clicked(GtkButton *button, gpointer user_data);
static void on_trial_division_clicked(GtkButton *button, gpointer user_data);
static void on_square_root_clicked(GtkButton *button, gpointer user_data);

static void on_activate(GtkApplication *app, gpointer user_data)
{
    GtkBuilder *builder = gtk_builder_new_from_file("ui/interface.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "entry_window"));

    if (!window) {
        g_printerr("Could not find 'entry_window' in interface.glade\n");
        g_object_unref(builder);
        return;
    }

    AppWidgets *w = g_new0(AppWidgets, 1);
    w->window = window;
    w->entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_display"));
    w->result_view = GTK_WIDGET(gtk_builder_get_object(builder, "result_output"));

    GtkWidget *factor_button  = GTK_WIDGET(gtk_builder_get_object(builder, "factor_button"));
    GtkWidget *clear_button   = GTK_WIDGET(gtk_builder_get_object(builder, "clear_button"));
    GtkWidget *quit_button    = GTK_WIDGET(gtk_builder_get_object(builder, "quit_button"));

    w->trial_button = GTK_WIDGET(gtk_builder_get_object(builder, "trial_division_button"));
    w->sqrt_button  = GTK_WIDGET(gtk_builder_get_object(builder, "square_root_button"));

    w->method = FACTOR_METHOD_TRAIL;

    g_signal_connect(w->entry,
                     "insert-text",
                     G_CALLBACK(on_entry_insert_text),
                     NULL);

    g_signal_connect(factor_button,
                     "clicked",
                     G_CALLBACK(on_factor_clicked),
                     w);

    g_signal_connect(clear_button,
                     "clicked",
                     G_CALLBACK(on_clear_clicked),
                     w);

    g_signal_connect(quit_button,
                     "clicked",
                     G_CALLBACK(on_quit_clicked),
                     w);

    g_signal_connect(w->trial_button,
                     "clicked",
                     G_CALLBACK(on_trial_division_clicked),
                     w);

    g_signal_connect(w->sqrt_button,
                     "clicked",
                     G_CALLBACK(on_square_root_clicked),
                     w);

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_widget_show_all(window);

    g_object_unref(builder);
}


static void on_entry_insert_text(GtkEditable *editable,
                                 const gchar *text,
                                 gint length,
                                 gint *position,
                                 gpointer user_data)
{
    for (int i = 0; i < length; i++) {
        if (!g_ascii_isdigit(text[i])) {
            g_signal_stop_emission_by_name(editable, "insert-text");
            return;
        }
    }
}

static void on_factor_clicked(GtkButton *button, gpointer user_data)
{
    AppWidgets *w = (AppWidgets *)user_data;
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(w->entry));
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->result_view));

    if (text[0] == '\0') {
        gtk_text_buffer_set_text(buffer, "Please enter a number.\n", -1);
        return;
    }

    gchar *endptr = NULL;
    unsigned long long n64 = g_ascii_strtoull(text, &endptr, 10);
    if (endptr == text || *endptr != '\0') {
        gtk_text_buffer_set_text(buffer, "Invalid number.\n", -1);
        return;
    }

    if (n64 <= 1 || n64 > INT32_MAX) {
        gtk_text_buffer_set_text(buffer, "Number out of supported range.\n", -1);
        return;
    }

    int n = (int)n64;

    int factors[64];
    int count = factor_number(n, w->method, factors, 64);
    if (count <= 0) {
        gtk_text_buffer_set_text(buffer, "Number is prime or cannot be factored.\n", -1);
        return;
    }

    GString *out = g_string_new(NULL);
    g_string_append_printf(out, "%d = ", n);

    for (int i = 0; i < count; i++) {
        g_string_append_printf(out, "%d", factors[i]);
        if (i < count - 1) {
            g_string_append(out, " × ");
        }
    }
    g_string_append(out, "\n");

    gtk_text_buffer_set_text(buffer, out->str, -1);
    g_string_free(out, TRUE);
}

static void on_clear_clicked(GtkButton *button, gpointer user_data)
{
    AppWidgets *w = user_data;
    gtk_entry_set_text(GTK_ENTRY(w->entry), "");

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->result_view));
    gtk_text_buffer_set_text(buffer, "", -1);
}

static void on_quit_clicked(GtkButton *button, gpointer user_data)
{
    AppWidgets *w = user_data;
    gtk_window_close(GTK_WINDOW(w->window));
}

static void on_trial_division_clicked(GtkButton *button, gpointer user_data)
{
    AppWidgets *w = (AppWidgets *)user_data;
    w->method = FACTOR_METHOD_TRAIL;

    gtk_button_set_label(GTK_BUTTON(w->trial_button), "Trial Division (Selected)");
    gtk_button_set_label(GTK_BUTTON(w->sqrt_button), "Square Root");
}

static void on_square_root_clicked(GtkButton *button, gpointer user_data)
{
    AppWidgets *w = (AppWidgets *)user_data;
    w->method = FACTOR_METHOD_SQRT;

    gtk_button_set_label(GTK_BUTTON(w->sqrt_button), "Square Root (Selected)");
    gtk_button_set_label(GTK_BUTTON(w->trial_button), "Trial Division");
}

int main(int argc, char **argv)
{
    GtkApplication *app = gtk_application_new("com.henry.RSAlite", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
