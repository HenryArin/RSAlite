#include <gtk/gtk.h>
#include <stdint.h>
#include <limits.h>
#include "factor.h"
#include "prime.h"
#include <errno.h>
#include <time.h>

typedef struct
{
    GtkWidget *window;
    GtkWidget *entry;
    GtkWidget *result_view;

    FactorMethod method;

    GtkWidget *trial_button;
    GtkWidget *sqrt_button;
    GtkWidget *wheel_button;
    GtkWidget *sieve_button;
    GtkWidget *fermat_button;
    GtkWidget *pollard_button;
    GtkWidget *benchmark_button;

    struct OptimizationContext opt;
} AppWidgets;

static void on_entry_insert_text(GtkEditable *, const gchar *, gint, gint *, gpointer);

static double now_seconds(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

static void on_factor_clicked(GtkButton *, gpointer);
static void on_clear_clicked(GtkButton *, gpointer);
static void on_quit_clicked(GtkButton *, gpointer);

static void on_trial_division_clicked(GtkButton *, gpointer);
static void on_square_root_clicked(GtkButton *, gpointer);
static void on_wheel_clicked(GtkButton *, gpointer);
static void on_fermat_clicked(GtkButton *, gpointer);
static void on_pollard_clicked(GtkButton *, gpointer);

static void on_sieve_toggled(GtkToggleButton *, gpointer);

static void on_benchmark_toggled(GtkToggleButton *, gpointer);

static void on_activate(GtkApplication *app, gpointer user_data)
{
    GtkBuilder *builder = gtk_builder_new_from_file("ui/interface.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "entry_window"));

    if (!window)
    {
        g_printerr("Could not find entry_window\n");
        g_object_unref(builder);
        return;
    }

    AppWidgets *w = g_new0(AppWidgets, 1);

    w->window = window;
    w->entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_display"));
    w->result_view = GTK_WIDGET(gtk_builder_get_object(builder, "result_output"));

    GtkWidget *factor_button = GTK_WIDGET(gtk_builder_get_object(builder, "factor_button"));
    GtkWidget *clear_button = GTK_WIDGET(gtk_builder_get_object(builder, "clear_button"));
    GtkWidget *quit_button = GTK_WIDGET(gtk_builder_get_object(builder, "quit_button"));

    w->trial_button = GTK_WIDGET(gtk_builder_get_object(builder, "trial_division_button"));
    w->sqrt_button = GTK_WIDGET(gtk_builder_get_object(builder, "square_root_button"));
    w->wheel_button = GTK_WIDGET(gtk_builder_get_object(builder, "wheel_factorization_button"));
    w->fermat_button = GTK_WIDGET(gtk_builder_get_object(builder, "fermats_primality_test_button"));
    w->pollard_button = GTK_WIDGET(gtk_builder_get_object(builder, "pollards_rho_button"));

    w->sieve_button = GTK_WIDGET(gtk_builder_get_object(builder, "sieve_button"));

    w->benchmark_button = GTK_WIDGET(gtk_builder_get_object(builder, "benchmark_button"));

    w->method = FACTOR_METHOD_TRIAL;

    w->opt.USE_SIEVE = false;
    w->opt.USE_SIMD = false;
    w->opt.USE_MULTITHREADING = false;
    w->opt.USE_GPU = false;
    w->opt.USE_BENCHMARKING = false;

    gtk_button_set_label(GTK_BUTTON(w->sieve_button), "Sieve OFF");

    gtk_button_set_label(GTK_BUTTON(w->benchmark_button), "Benchmarking OFF");

    /* ---- Signals ---- */
    g_signal_connect(w->entry, "insert-text",
                     G_CALLBACK(on_entry_insert_text), NULL);

    g_signal_connect(factor_button, "clicked",
                     G_CALLBACK(on_factor_clicked), w);
    g_signal_connect(clear_button, "clicked",
                     G_CALLBACK(on_clear_clicked), w);
    g_signal_connect(quit_button, "clicked",
                     G_CALLBACK(on_quit_clicked), w);

    g_signal_connect(w->trial_button, "clicked",
                     G_CALLBACK(on_trial_division_clicked), w);
    g_signal_connect(w->sqrt_button, "clicked",
                     G_CALLBACK(on_square_root_clicked), w);
    g_signal_connect(w->wheel_button, "clicked",
                     G_CALLBACK(on_wheel_clicked), w);
    g_signal_connect(w->fermat_button, "clicked",
                     G_CALLBACK(on_fermat_clicked), w);
    g_signal_connect(w->pollard_button, "clicked",
                     G_CALLBACK(on_pollard_clicked), w);

    g_signal_connect(w->sieve_button, "toggled",
                     G_CALLBACK(on_sieve_toggled), w);

    g_signal_connect(w->benchmark_button, "toggled",
                     G_CALLBACK(on_benchmark_toggled), w);

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
    for (int i = 0; i < length; i++)
    {
        if (!g_ascii_isdigit(text[i]))
        {
            g_signal_stop_emission_by_name(editable, "insert-text");
            return;
        }
    }
}

static void on_factor_clicked(GtkButton *button, gpointer user_data)
{
    AppWidgets *w = user_data;
    GtkTextBuffer *buffer =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->result_view));

    const gchar *text = gtk_entry_get_text(GTK_ENTRY(w->entry));
    if (text[0] == '\0')
    {
        gtk_text_buffer_set_text(buffer, "Please enter a number.\n", -1);
        return;
    }

    gchar *endptr = NULL;
    errno = 0;
    unsigned long long n64 = g_ascii_strtoull(text, &endptr, 10);

    if (endptr == text || *endptr != '\0' || errno == ERANGE || n64 <= 1ULL)
    {
        gtk_text_buffer_set_text(buffer, "Invalid or unsupported number.\n", -1);
        return;
    }

    uint64_t n = (uint64_t)n64;
    uint64_t factors[64];

    double elapsed = 0.0;

    if (w->opt.USE_BENCHMARKING)
    {
        double start = now_seconds();
        int count = factor_number(n, w->method, factors, 64, &w->opt);
        double end = now_seconds();
        elapsed = end - start;

        if (count <= 0)
        {
            gtk_text_buffer_set_text(buffer, "Prime or unfactorable.\n", -1);
            return;
        }

        GString *out = g_string_new(NULL);

        const char *method_name =
            (w->method == FACTOR_METHOD_TRIAL) ? "Trial Division" : (w->method == FACTOR_METHOD_SQRT)  ? "Square Root"
                                                                : (w->method == FACTOR_METHOD_WHEEL)   ? "Wheel Factorization"
                                                                : (w->method == FACTOR_METHOD_FERMAT)  ? "Fermat"
                                                                : (w->method == FACTOR_METHOD_POLLARD) ? "Pollard"
                                                                                                       : "Other";

        g_string_append_printf(out,
                               "Benchmark Mode: ON\nMethod: %s\nTime: %.6f seconds\n\n%llu = ",
                               method_name,
                               elapsed,
                               (unsigned long long)n);

        for (int i = 0; i < count; i++)
        {
            g_string_append_printf(out, "%llu", (unsigned long long)factors[i]);
            if (i < count - 1)
                g_string_append(out, " × ");
        }

        g_string_append(out, "\n");
        gtk_text_buffer_set_text(buffer, out->str, -1);
        g_string_free(out, TRUE);
    }
    else
    {
        int count = factor_number(n, w->method, factors, 64, &w->opt);
        if (count <= 0)
        {
            gtk_text_buffer_set_text(buffer, "Prime or unfactorable.\n", -1);
            return;
        }

        const char *method_name =
            (w->method == FACTOR_METHOD_TRIAL) ? "Trial Division" : (w->method == FACTOR_METHOD_SQRT)  ? "Square Root"
                                                                : (w->method == FACTOR_METHOD_WHEEL)   ? "Wheel Factorization"
                                                                : (w->method == FACTOR_METHOD_FERMAT)  ? "Fermat"
                                                                : (w->method == FACTOR_METHOD_POLLARD) ? "Pollard"
                                                                                                       : "Other";

        GString *out = g_string_new(NULL);
        g_string_append_printf(out,
                               "Method: %s\n%llu = ",
                               method_name,
                               (unsigned long long)n);

        for (int i = 0; i < count; i++)
        {
            g_string_append_printf(out, "%llu", (unsigned long long)factors[i]);
            if (i < count - 1)
                g_string_append(out, " × ");
        }

        g_string_append(out, "\n");
        gtk_text_buffer_set_text(buffer, out->str, -1);
        g_string_free(out, TRUE);
    }
}

static void on_clear_clicked(GtkButton *button, gpointer user_data)
{
    AppWidgets *w = user_data;
    gtk_entry_set_text(GTK_ENTRY(w->entry), "");
    gtk_text_buffer_set_text(
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->result_view)),
        "", -1);
}

static void on_quit_clicked(GtkButton *button, gpointer user_data)
{
    gtk_window_close(GTK_WINDOW(((AppWidgets *)user_data)->window));
}

static void reset_method_labels(AppWidgets *w)
{
    gtk_button_set_label(GTK_BUTTON(w->trial_button), "Trial Division");
    gtk_button_set_label(GTK_BUTTON(w->sqrt_button), "Square Root");
    gtk_button_set_label(GTK_BUTTON(w->wheel_button), "Wheel Factorization");
    gtk_button_set_label(GTK_BUTTON(w->fermat_button), "Fermat's Primality Test");
    gtk_button_set_label(GTK_BUTTON(w->pollard_button), "Pollard's Rho");
}

static void on_trial_division_clicked(GtkButton *b, gpointer u)
{
    AppWidgets *w = u;
    w->method = FACTOR_METHOD_TRIAL;
    reset_method_labels(w);
    gtk_button_set_label(b, "Trial Division (Selected)");
}

static void on_square_root_clicked(GtkButton *b, gpointer u)
{
    AppWidgets *w = u;
    w->method = FACTOR_METHOD_SQRT;
    reset_method_labels(w);
    gtk_button_set_label(b, "Square Root (Selected)");
}

static void on_wheel_clicked(GtkButton *b, gpointer u)
{
    AppWidgets *w = u;
    w->method = FACTOR_METHOD_WHEEL;
    reset_method_labels(w);
    gtk_button_set_label(b, "Wheel Factorization (Selected)");
}

static void on_fermat_clicked(GtkButton *b, gpointer u)
{
    AppWidgets *w = u;
    w->method = FACTOR_METHOD_FERMAT;
    reset_method_labels(w);
    gtk_button_set_label(b, "Fermat's Primality Test (Selected)");
}

static void on_pollard_clicked(GtkButton *b, gpointer u)
{
    AppWidgets *w = u;
    w->method = FACTOR_METHOD_POLLARD;
    reset_method_labels(w);
    gtk_button_set_label(b, "Pollard's Rho (Selected)");
}

static void on_sieve_toggled(GtkToggleButton *button, gpointer user_data)
{
    AppWidgets *w = user_data;
    w->opt.USE_SIEVE = gtk_toggle_button_get_active(button);

    gtk_button_set_label(GTK_BUTTON(button),
                         w->opt.USE_SIEVE ? "Sieve ON" : "Sieve OFF");
}

static void on_benchmark_toggled(GtkToggleButton *button, gpointer user_data)
{
    AppWidgets *w = user_data;
    w->opt.USE_BENCHMARKING = gtk_toggle_button_get_active(button);

    gtk_button_set_label(GTK_BUTTON(button),
                         w->opt.USE_BENCHMARKING ? "Benchmarking ON" : "Benchmarking OFF");
}

int main(int argc, char **argv)
{
    GtkApplication *app =
        gtk_application_new("com.henry.RSAlite", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
