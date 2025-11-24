#include <gtk/gtk.h>
#include "factor.h"
#include "prime.h"

static void on_entry_insert_text(GtkEditable *editable,
                                 const gchar *text,
                                 gint length,
                                 gint *position,
                                 gpointer user_data);

static void on_activate(GtkApplication *app, gpointer user_data)
{
    GtkBuilder *builder = gtk_builder_new_from_file("ui/interface.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "entry_window"));

    GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_display"));
    g_signal_connect(entry, "insert-text", G_CALLBACK(on_entry_insert_text), NULL);


    if (!window) {
        g_printerr("Could not find 'entry_window' in interface.glade\n");
        g_object_unref(builder);
        return;
    }

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
    // Only allow digits
    for (int i = 0; i < length; i++) {
        if (!g_ascii_isdigit(text[i])) {
            g_signal_stop_emission_by_name(editable, "insert-text");
            return;
        }
    }
}

int main(int argc, char **argv)
{
    GtkApplication *app = gtk_application_new("com.henry.RSAlite", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
