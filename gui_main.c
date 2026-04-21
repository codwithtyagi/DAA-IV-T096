#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils.h"
#include "../graph.h"
#include "../routing.h"
#include "../schedule.h"
#include "../optimize.h"
#include "../search_sort.h"
#include "../hash.h"
#include "../file_io.h"

/* ─── Forward declarations ─── */
static void build_ui(GtkApplication *app);
static void apply_css(void);

/* ─── Notebook pages ─── */
static void build_graph_page(GtkWidget *notebook);
static void build_orders_page(GtkWidget *notebook);
static void build_routing_page(GtkWidget *notebook);
static void build_schedule_page(GtkWidget *notebook);
static void build_optimize_page(GtkWidget *notebook);
static void build_fileio_page(GtkWidget *notebook);

/* ─── Callbacks ─── */
static void on_add_node(GtkWidget *btn, gpointer data);
static void on_add_edge(GtkWidget *btn, gpointer data);
static void on_refresh_graph(GtkWidget *btn, gpointer data);
static void on_add_order(GtkWidget *btn, gpointer data);
static void on_refresh_orders(GtkWidget *btn, gpointer data);
static void on_dijkstra(GtkWidget *btn, gpointer data);
static void on_bfs(GtkWidget *btn, gpointer data);
static void on_dfs(GtkWidget *btn, gpointer data);
static void on_schedule(GtkWidget *btn, gpointer data);
static void on_knapsack(GtkWidget *btn, gpointer data);
static void on_tsp(GtkWidget *btn, gpointer data);
static void on_sort_orders(GtkWidget *btn, gpointer data);
static void on_save_data(GtkWidget *btn, gpointer data);
static void on_load_data(GtkWidget *btn, gpointer data);
static void on_export_txt(GtkWidget *btn, gpointer data);
static void on_import_txt(GtkWidget *btn, gpointer data);

/* ─── Globals ─── */
static GtkWidget *window;
static GtkWidget *notebook;

/* Graph page widgets */
static GtkWidget *graph_node_entry;
static GtkWidget *graph_src_entry, *graph_dst_entry;
static GtkWidget *graph_dist_entry, *graph_cost_entry;
static GtkWidget *graph_view;
static GtkTextBuffer *graph_buffer;

/* Orders page widgets */
static GtkWidget *ord_id_entry, *ord_name_entry, *ord_pri_entry;
static GtkWidget *ord_wt_entry, *ord_val_entry;
static GtkWidget *ord_src_entry, *ord_dst_entry;
static GtkWidget *ord_dl_entry, *ord_status_entry;
static GtkWidget *orders_view;
static GtkTextBuffer *orders_buffer;

/* Routing page widgets */
static GtkWidget *route_src_entry, *route_dst_entry;
static GtkWidget *route_view;
static GtkTextBuffer *route_buffer;

/* Schedule page */
static GtkWidget *sched_view;
static GtkTextBuffer *sched_buffer;

/* Optimize page */
static GtkWidget *opt_cap_entry, *opt_tsp_entry;
static GtkWidget *opt_view;
static GtkTextBuffer *opt_buffer;
static GtkWidget *sort_combo;

/* File I/O page */
static GtkWidget *file_view;
static GtkTextBuffer *file_buffer;

/* ─── CSS ─── */
static const char *css_data =
    "window {"
    "  background-color: #1a1b2e;"
    "}"
    "headerbar {"
    "  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);"
    "  color: white;"
    "  border: none;"
    "}"
    "headerbar .title {"
    "  color: white;"
    "  font-weight: bold;"
    "  font-size: 16px;"
    "}"
    "notebook {"
    "  background-color: #1a1b2e;"
    "}"
    "notebook tab {"
    "  background-color: #252742;"
    "  color: #8b8fa3;"
    "  padding: 8px 16px;"
    "  border: none;"
    "  border-radius: 8px 8px 0 0;"
    "  margin: 2px 1px 0 1px;"
    "  font-weight: 600;"
    "}"
    "notebook tab:checked {"
    "  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);"
    "  color: white;"
    "}"
    "notebook > stack {"
    "  background-color: #1e2038;"
    "  border-radius: 0 0 12px 12px;"
    "}"
    ".card {"
    "  background-color: #252742;"
    "  border-radius: 12px;"
    "  padding: 16px;"
    "  margin: 8px;"
    "  border: 1px solid #353760;"
    "}"
    ".card-title {"
    "  color: #e0e0ff;"
    "  font-size: 14px;"
    "  font-weight: bold;"
    "  margin-bottom: 8px;"
    "}"
    "entry {"
    "  background-color: #1a1b2e;"
    "  color: #e0e0ff;"
    "  border: 1px solid #454780;"
    "  border-radius: 8px;"
    "  padding: 8px 12px;"
    "  min-height: 20px;"
    "  caret-color: #667eea;"
    "}"
    "entry:focus {"
    "  border-color: #667eea;"
    "  box-shadow: 0 0 0 2px rgba(102,126,234,0.3);"
    "}"
    ".action-btn {"
    "  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);"
    "  color: white;"
    "  border: none;"
    "  border-radius: 8px;"
    "  padding: 8px 20px;"
    "  font-weight: bold;"
    "  min-height: 20px;"
    "}"
    ".action-btn:hover {"
    "  background: linear-gradient(135deg, #7b93ff 0%, #8b5fbf 100%);"
    "}"
    ".success-btn {"
    "  background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%);"
    "  color: white;"
    "  border: none;"
    "  border-radius: 8px;"
    "  padding: 8px 20px;"
    "  font-weight: bold;"
    "  min-height: 20px;"
    "}"
    ".success-btn:hover {"
    "  background: linear-gradient(135deg, #15b3a6 0%, #4cff91 100%);"
    "}"
    ".warn-btn {"
    "  background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);"
    "  color: white;"
    "  border: none;"
    "  border-radius: 8px;"
    "  padding: 8px 20px;"
    "  font-weight: bold;"
    "  min-height: 20px;"
    "}"
    ".warn-btn:hover {"
    "  background: linear-gradient(135deg, #f5a9fc 0%, #f77083 100%);"
    "}"
    "textview {"
    "  font-family: 'SF Mono', 'Menlo', 'Monaco', monospace;"
    "  font-size: 12px;"
    "}"
    "textview text {"
    "  background-color: #0d0e1a;"
    "  color: #38ef7d;"
    "  padding: 12px;"
    "}"
    "scrolledwindow {"
    "  border-radius: 8px;"
    "  border: 1px solid #353760;"
    "}"
    "label {"
    "  color: #b0b3cc;"
    "  font-size: 13px;"
    "}"
    "comboboxtext button {"
    "  background-color: #1a1b2e;"
    "  color: #e0e0ff;"
    "  border: 1px solid #454780;"
    "  border-radius: 8px;"
    "  padding: 6px 12px;"
    "}"
    ".status-bar {"
    "  background-color: #151627;"
    "  color: #667eea;"
    "  padding: 4px 12px;"
    "  font-size: 11px;"
    "}";

/* ─── Helpers ─── */
static GtkWidget *make_label(const char *text)
{
    GtkWidget *lbl = gtk_label_new(text);
    gtk_widget_set_halign(lbl, GTK_ALIGN_START);
    return lbl;
}

static GtkWidget *make_entry(const char *placeholder)
{
    GtkWidget *e = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(e), placeholder);
    return e;
}

static GtkWidget *make_button(const char *label, const char *css_class)
{
    GtkWidget *btn = gtk_button_new_with_label(label);
    GtkStyleContext *ctx = gtk_widget_get_style_context(btn);
    gtk_style_context_add_class(ctx, css_class);
    return btn;
}

static GtkWidget *make_scrolled_text(GtkTextBuffer **buf)
{
    GtkWidget *sw = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(sw, TRUE);
    gtk_widget_set_hexpand(sw, TRUE);

    GtkWidget *tv = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(tv), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(tv), TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(tv), GTK_WRAP_WORD_CHAR);
    *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
    gtk_container_add(GTK_CONTAINER(sw), tv);
    return sw;
}

static void buf_append(GtkTextBuffer *buf, const char *text)
{
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buf, &end);
    gtk_text_buffer_insert(buf, &end, text, -1);
}

static void buf_clear(GtkTextBuffer *buf)
{
    gtk_text_buffer_set_text(buf, "", -1);
}

static const char *entry_text(GtkWidget *entry)
{
    return gtk_entry_get_text(GTK_ENTRY(entry));
}

static int entry_int(GtkWidget *entry)
{
    return atoi(gtk_entry_get_text(GTK_ENTRY(entry)));
}

/* ─── Application activate ─── */
static void on_activate(GtkApplication *app, gpointer user_data)
{
    (void)user_data;
    initGraph();
    hashClear();
    apply_css();
    build_ui(app);
}

static void apply_css(void)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css_data, -1, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

static void build_ui(GtkApplication *app)
{
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Smart Delivery & Routing System");
    gtk_window_set_default_size(GTK_WINDOW(window), 960, 700);

    GtkWidget *header = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
    gtk_header_bar_set_title(GTK_HEADER_BAR(header),
                             "🚚 Smart Delivery & Routing System v2.0");
    gtk_header_bar_set_subtitle(GTK_HEADER_BAR(header),
                                "DAA Project — Graph • Orders • Routing • Optimization");
    gtk_window_set_titlebar(GTK_WINDOW(window), header);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);
    gtk_widget_set_vexpand(notebook, TRUE);
    gtk_box_pack_start(GTK_BOX(main_box), notebook, TRUE, TRUE, 0);

    build_graph_page(notebook);
    build_orders_page(notebook);
    build_routing_page(notebook);
    build_schedule_page(notebook);
    build_optimize_page(notebook);
    build_fileio_page(notebook);

    /* Status bar */
    GtkWidget *status = gtk_label_new("  Ready  •  Nodes: 0  •  Orders: 0");
    gtk_widget_set_halign(status, GTK_ALIGN_START);
    GtkStyleContext *sctx = gtk_widget_get_style_context(status);
    gtk_style_context_add_class(sctx, "status-bar");
    gtk_box_pack_end(GTK_BOX(main_box), status, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}

/* ═══════════════════════════════════════════
 *  PAGE 1: GRAPH MANAGEMENT
 * ═══════════════════════════════════════════ */
static void build_graph_page(GtkWidget *nb)
{
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(page), 12);

    /* ── Add Node card ── */
    GtkWidget *node_card = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkStyleContext *nc = gtk_widget_get_style_context(node_card);
    gtk_style_context_add_class(nc, "card");

    GtkWidget *nl = make_label("Node Name:");
    graph_node_entry = make_entry("e.g. Delhi");
    GtkWidget *add_node_btn = make_button("➕ Add Node", "action-btn");
    g_signal_connect(add_node_btn, "clicked", G_CALLBACK(on_add_node), NULL);

    gtk_box_pack_start(GTK_BOX(node_card), nl, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(node_card), graph_node_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(node_card), add_node_btn, FALSE, FALSE, 0);

    /* ── Add Edge card ── */
    GtkWidget *edge_card = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkStyleContext *ec = gtk_widget_get_style_context(edge_card);
    gtk_style_context_add_class(ec, "card");

    graph_src_entry = make_entry("Source");
    graph_dst_entry = make_entry("Destination");
    graph_dist_entry = make_entry("Distance");
    graph_cost_entry = make_entry("Cost");
    GtkWidget *add_edge_btn = make_button("🔗 Add Edge", "action-btn");
    g_signal_connect(add_edge_btn, "clicked", G_CALLBACK(on_add_edge), NULL);

    gtk_box_pack_start(GTK_BOX(edge_card), graph_src_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(edge_card), graph_dst_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(edge_card), graph_dist_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(edge_card), graph_cost_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(edge_card), add_edge_btn, FALSE, FALSE, 0);

    /* ── Refresh button ── */
    GtkWidget *ref_btn = make_button("🔄 Refresh Graph View", "success-btn");
    g_signal_connect(ref_btn, "clicked", G_CALLBACK(on_refresh_graph), NULL);

    /* ── Text view ── */
    graph_view = make_scrolled_text(&graph_buffer);

    gtk_box_pack_start(GTK_BOX(page), node_card, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(page), edge_card, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(page), ref_btn, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(page), graph_view, TRUE, TRUE, 0);

    gtk_notebook_append_page(GTK_NOTEBOOK(nb), page, gtk_label_new("📊 Graph"));
}

/* ═══════════════════════════════════════════
 *  PAGE 2: ORDER MANAGEMENT
 * ═══════════════════════════════════════════ */
static void build_orders_page(GtkWidget *nb)
{
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(page), 12);

    /* ── Input grid card ── */
    GtkWidget *card = gtk_grid_new();
    GtkStyleContext *cc = gtk_widget_get_style_context(card);
    gtk_style_context_add_class(cc, "card");
    gtk_grid_set_row_spacing(GTK_GRID(card), 6);
    gtk_grid_set_column_spacing(GTK_GRID(card), 12);

    GtkWidget *title = make_label("Add Delivery Order");
    GtkStyleContext *tc = gtk_widget_get_style_context(title);
    gtk_style_context_add_class(tc, "card-title");
    gtk_grid_attach(GTK_GRID(card), title, 0, 0, 4, 1);

    gtk_grid_attach(GTK_GRID(card), make_label("ID:"), 0, 1, 1, 1);
    ord_id_entry = make_entry("101");
    gtk_grid_attach(GTK_GRID(card), ord_id_entry, 1, 1, 1, 1);
    gtk_widget_set_hexpand(ord_id_entry, TRUE);

    gtk_grid_attach(GTK_GRID(card), make_label("Name:"), 2, 1, 1, 1);
    ord_name_entry = make_entry("Electronics");
    gtk_grid_attach(GTK_GRID(card), ord_name_entry, 3, 1, 1, 1);
    gtk_widget_set_hexpand(ord_name_entry, TRUE);

    gtk_grid_attach(GTK_GRID(card), make_label("Priority:"), 0, 2, 1, 1);
    ord_pri_entry = make_entry("1");
    gtk_grid_attach(GTK_GRID(card), ord_pri_entry, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(card), make_label("Weight:"), 2, 2, 1, 1);
    ord_wt_entry = make_entry("5");
    gtk_grid_attach(GTK_GRID(card), ord_wt_entry, 3, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(card), make_label("Value:"), 0, 3, 1, 1);
    ord_val_entry = make_entry("500");
    gtk_grid_attach(GTK_GRID(card), ord_val_entry, 1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(card), make_label("Src Node:"), 2, 3, 1, 1);
    ord_src_entry = make_entry("Delhi");
    gtk_grid_attach(GTK_GRID(card), ord_src_entry, 3, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(card), make_label("Dst Node:"), 0, 4, 1, 1);
    ord_dst_entry = make_entry("Mumbai");
    gtk_grid_attach(GTK_GRID(card), ord_dst_entry, 1, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(card), make_label("Deadline:"), 2, 4, 1, 1);
    ord_dl_entry = make_entry("10");
    gtk_grid_attach(GTK_GRID(card), ord_dl_entry, 3, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(card), make_label("Status:"), 0, 5, 1, 1);
    ord_status_entry = make_entry("Pending");
    gtk_grid_attach(GTK_GRID(card), ord_status_entry, 1, 5, 1, 1);

    /* ── Buttons row ── */
    GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *add_btn = make_button("➕ Add Order", "action-btn");
    GtkWidget *ref_btn = make_button("🔄 Refresh", "success-btn");
    g_signal_connect(add_btn, "clicked", G_CALLBACK(on_add_order), NULL);
    g_signal_connect(ref_btn, "clicked", G_CALLBACK(on_refresh_orders), NULL);

    /* Sort combo */
    sort_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(sort_combo), "Value");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(sort_combo), "Weight");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(sort_combo), "Priority");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(sort_combo), "ID");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(sort_combo), "Name");
    gtk_combo_box_set_active(GTK_COMBO_BOX(sort_combo), 3);
    GtkWidget *sort_btn = make_button("⬆ Sort", "warn-btn");
    g_signal_connect(sort_btn, "clicked", G_CALLBACK(on_sort_orders), NULL);

    gtk_box_pack_start(GTK_BOX(btn_box), add_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btn_box), ref_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btn_box), gtk_separator_new(GTK_ORIENTATION_VERTICAL), FALSE, FALSE, 8);
    gtk_box_pack_start(GTK_BOX(btn_box), make_label("Sort by:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btn_box), sort_combo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btn_box), sort_btn, FALSE, FALSE, 0);

    /* ── Text view ── */
    orders_view = make_scrolled_text(&orders_buffer);

    gtk_box_pack_start(GTK_BOX(page), card, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(page), btn_box, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(page), orders_view, TRUE, TRUE, 0);

    gtk_notebook_append_page(GTK_NOTEBOOK(nb), page, gtk_label_new("📦 Orders"));
}

/* ═══════════════════════════════════════════
 *  PAGE 3: ROUTING (Dijkstra, BFS, DFS)
 * ═══════════════════════════════════════════ */
static void build_routing_page(GtkWidget *nb)
{
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(page), 12);

    GtkWidget *card = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkStyleContext *cc = gtk_widget_get_style_context(card);
    gtk_style_context_add_class(cc, "card");

    route_src_entry = make_entry("Source node");
    route_dst_entry = make_entry("Dest node (Dijkstra)");

    GtkWidget *dij_btn = make_button("🗺 Dijkstra", "action-btn");
    GtkWidget *bfs_btn = make_button("🔍 BFS", "success-btn");
    GtkWidget *dfs_btn = make_button("🔎 DFS", "warn-btn");

    g_signal_connect(dij_btn, "clicked", G_CALLBACK(on_dijkstra), NULL);
    g_signal_connect(bfs_btn, "clicked", G_CALLBACK(on_bfs), NULL);
    g_signal_connect(dfs_btn, "clicked", G_CALLBACK(on_dfs), NULL);

    gtk_box_pack_start(GTK_BOX(card), route_src_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(card), route_dst_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(card), dij_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(card), bfs_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(card), dfs_btn, FALSE, FALSE, 0);

    route_view = make_scrolled_text(&route_buffer);

    gtk_box_pack_start(GTK_BOX(page), card, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(page), route_view, TRUE, TRUE, 0);

    gtk_notebook_append_page(GTK_NOTEBOOK(nb), page, gtk_label_new("🗺 Routing"));
}

/* ═══════════════════════════════════════════
 *  PAGE 4: SCHEDULE (Priority Queue)
 * ═══════════════════════════════════════════ */
static void build_schedule_page(GtkWidget *nb)
{
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(page), 12);

    GtkWidget *btn = make_button("📅 Schedule Deliveries (Priority Queue)", "action-btn");
    g_signal_connect(btn, "clicked", G_CALLBACK(on_schedule), NULL);

    sched_view = make_scrolled_text(&sched_buffer);

    gtk_box_pack_start(GTK_BOX(page), btn, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(page), sched_view, TRUE, TRUE, 0);

    gtk_notebook_append_page(GTK_NOTEBOOK(nb), page, gtk_label_new("📅 Schedule"));
}

/* ═══════════════════════════════════════════
 *  PAGE 5: OPTIMIZATION (Knapsack + TSP)
 * ═══════════════════════════════════════════ */
static void build_optimize_page(GtkWidget *nb)
{
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(page), 12);

    GtkWidget *card = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkStyleContext *cc = gtk_widget_get_style_context(card);
    gtk_style_context_add_class(cc, "card");

    opt_cap_entry = make_entry("Vehicle capacity");
    GtkWidget *ks_btn = make_button("🎒 Knapsack", "action-btn");
    g_signal_connect(ks_btn, "clicked", G_CALLBACK(on_knapsack), NULL);

    gtk_box_pack_start(GTK_BOX(card), make_label("Capacity:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(card), opt_cap_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(card), ks_btn, FALSE, FALSE, 0);

    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(card), sep, FALSE, FALSE, 8);

    opt_tsp_entry = make_entry("Start node");
    GtkWidget *tsp_btn = make_button("🛣 TSP Route", "warn-btn");
    g_signal_connect(tsp_btn, "clicked", G_CALLBACK(on_tsp), NULL);

    gtk_box_pack_start(GTK_BOX(card), make_label("TSP Start:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(card), opt_tsp_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(card), tsp_btn, FALSE, FALSE, 0);

    opt_view = make_scrolled_text(&opt_buffer);

    gtk_box_pack_start(GTK_BOX(page), card, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(page), opt_view, TRUE, TRUE, 0);

    gtk_notebook_append_page(GTK_NOTEBOOK(nb), page, gtk_label_new("⚡ Optimize"));
}

/* ═══════════════════════════════════════════
 *  PAGE 6: FILE I/O
 * ═══════════════════════════════════════════ */
static void build_fileio_page(GtkWidget *nb)
{
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(page), 12);

    GtkWidget *card = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkStyleContext *cc = gtk_widget_get_style_context(card);
    gtk_style_context_add_class(cc, "card");

    GtkWidget *save_btn = make_button("💾 Save Graph", "action-btn");
    GtkWidget *load_btn = make_button("📂 Load Graph", "success-btn");
    GtkWidget *exp_btn = make_button("📤 Export TXT", "warn-btn");
    GtkWidget *imp_btn = make_button("📥 Import TXT", "action-btn");

    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_data), NULL);
    g_signal_connect(load_btn, "clicked", G_CALLBACK(on_load_data), NULL);
    g_signal_connect(exp_btn, "clicked", G_CALLBACK(on_export_txt), NULL);
    g_signal_connect(imp_btn, "clicked", G_CALLBACK(on_import_txt), NULL);

    gtk_box_pack_start(GTK_BOX(card), save_btn, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(card), load_btn, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(card), exp_btn, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(card), imp_btn, TRUE, TRUE, 0);

    file_view = make_scrolled_text(&file_buffer);

    gtk_box_pack_start(GTK_BOX(page), card, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(page), file_view, TRUE, TRUE, 0);

    gtk_notebook_append_page(GTK_NOTEBOOK(nb), page, gtk_label_new("💾 File I/O"));
}

/* ═══════════════════════════════════════════════════════════════
 *  CALLBACKS
 * ═══════════════════════════════════════════════════════════════ */

/* ── Graph callbacks ── */
static void on_add_node(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    const char *name = entry_text(graph_node_entry);
    if (strlen(name) == 0) return;

    char nm[MAX_NAME];
    strncpy(nm, name, MAX_NAME - 1);
    nm[MAX_NAME - 1] = '\0';

    int idx = addNode(nm);
    char msg[128];
    if (idx >= 0)
        snprintf(msg, sizeof(msg), "✅ Added node [%d]: %s\n", idx, nm);
    else
        snprintf(msg, sizeof(msg), "❌ Failed to add node (max reached).\n");
    buf_append(graph_buffer, msg);
    gtk_entry_set_text(GTK_ENTRY(graph_node_entry), "");
}

static void on_add_edge(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    const char *sn = entry_text(graph_src_entry);
    const char *dn = entry_text(graph_dst_entry);
    int dist = entry_int(graph_dist_entry);
    int cost = entry_int(graph_cost_entry);

    char sname[MAX_NAME], dname[MAX_NAME];
    strncpy(sname, sn, MAX_NAME - 1); sname[MAX_NAME - 1] = '\0';
    strncpy(dname, dn, MAX_NAME - 1); dname[MAX_NAME - 1] = '\0';

    int s = findNode(sname);
    int d = findNode(dname);
    char msg[256];

    if (s < 0 || d < 0)
    {
        snprintf(msg, sizeof(msg), "❌ Node not found: %s or %s\n", sname, dname);
    }
    else
    {
        addEdge(s, d, dist, cost);
        snprintf(msg, sizeof(msg), "✅ Edge: %s ↔ %s (dist=%d, cost=%d)\n",
                 sname, dname, dist, cost);
    }
    buf_append(graph_buffer, msg);
}

static void on_refresh_graph(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    buf_clear(graph_buffer);

    if (nNodes == 0)
    {
        buf_append(graph_buffer, "Graph is empty.\n");
        return;
    }

    char line[512];
    int i, j;

    snprintf(line, sizeof(line), "Nodes: %d\n\n", nNodes);
    buf_append(graph_buffer, line);

    buf_append(graph_buffer, "Adjacency Matrix (Distances):\n");
    snprintf(line, sizeof(line), "%12s", "");
    buf_append(graph_buffer, line);
    for (i = 0; i < nNodes; i++)
    {
        snprintf(line, sizeof(line), "%12s", names[i]);
        buf_append(graph_buffer, line);
    }
    buf_append(graph_buffer, "\n");

    for (i = 0; i < nNodes; i++)
    {
        snprintf(line, sizeof(line), "%12s", names[i]);
        buf_append(graph_buffer, line);
        for (j = 0; j < nNodes; j++)
        {
            if (adj[i][j] == INF)
                snprintf(line, sizeof(line), "%12s", "INF");
            else
                snprintf(line, sizeof(line), "%12d", adj[i][j]);
            buf_append(graph_buffer, line);
        }
        buf_append(graph_buffer, "\n");
    }
}

/* ── Order callbacks ── */
static void on_add_order(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    int id = entry_int(ord_id_entry);
    const char *name = entry_text(ord_name_entry);
    int pri = entry_int(ord_pri_entry);
    int wt = entry_int(ord_wt_entry);
    int val = entry_int(ord_val_entry);
    const char *sn = entry_text(ord_src_entry);
    const char *dn = entry_text(ord_dst_entry);
    int dl = entry_int(ord_dl_entry);
    const char *st = entry_text(ord_status_entry);

    char sname[MAX_NAME], dname[MAX_NAME], oname[MAX_NAME], ostatus[MAX_NAME];
    strncpy(sname, sn, MAX_NAME - 1); sname[MAX_NAME - 1] = '\0';
    strncpy(dname, dn, MAX_NAME - 1); dname[MAX_NAME - 1] = '\0';
    strncpy(oname, name, MAX_NAME - 1); oname[MAX_NAME - 1] = '\0';
    strncpy(ostatus, st, MAX_NAME - 1); ostatus[MAX_NAME - 1] = '\0';

    int src = findNode(sname);
    int dst = findNode(dname);
    char msg[256];

    if (src < 0 || dst < 0)
    {
        snprintf(msg, sizeof(msg), "❌ Source or dest node not found!\n");
    }
    else
    {
        insertOrder(id, oname, pri, wt, val, src, dst, dl, ostatus);
        hashInsert(id, ordCnt - 1);
        snprintf(msg, sizeof(msg), "✅ Order added: ID=%d, Name=%s\n", id, oname);
    }
    buf_append(orders_buffer, msg);
}

static void on_refresh_orders(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    buf_clear(orders_buffer);

    if (ordCnt == 0)
    {
        buf_append(orders_buffer, "No orders.\n");
        return;
    }

    char line[256];
    int i;
    snprintf(line, sizeof(line), "%-6s %-15s %-8s %-8s %-8s %-10s %-10s\n",
             "ID", "Name", "Pri", "Wt", "Val", "Deadline", "Status");
    buf_append(orders_buffer, line);
    buf_append(orders_buffer, "──────────────────────────────────────────────────────────────\n");

    for (i = 0; i < ordCnt; i++)
    {
        snprintf(line, sizeof(line), "%-6d %-15s %-8d %-8d %-8d %-10d %-10s\n",
                 orders[i].id, orders[i].name, orders[i].priority,
                 orders[i].weight, orders[i].value,
                 orders[i].deadline, orders[i].status);
        buf_append(orders_buffer, line);
    }
}

static void on_sort_orders(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    int key = gtk_combo_box_get_active(GTK_COMBO_BOX(sort_combo));
    if (key < 0) key = 0;

    const char *knames[] = {"Value", "Weight", "Priority", "ID", "Name"};
    mergeSort(orders, 0, ordCnt - 1, key);

    hashClear();
    int i;
    for (i = 0; i < ordCnt; i++)
        hashInsert(orders[i].id, i);

    char msg[128];
    snprintf(msg, sizeof(msg), "✅ Orders sorted by %s (Merge Sort)\n", knames[key % 5]);
    buf_append(orders_buffer, msg);
    on_refresh_orders(NULL, NULL);
}

/* ── Routing callbacks ── */

static void on_dijkstra(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    const char *sn = entry_text(route_src_entry);
    const char *dn = entry_text(route_dst_entry);

    char sname[MAX_NAME], dname[MAX_NAME];
    strncpy(sname, sn, MAX_NAME - 1); sname[MAX_NAME - 1] = '\0';
    strncpy(dname, dn, MAX_NAME - 1); dname[MAX_NAME - 1] = '\0';

    int s = findNode(sname);
    int d = findNode(dname);

    buf_clear(route_buffer);

    if (s < 0 || d < 0)
    {
        buf_append(route_buffer, "❌ Node not found!\n");
        return;
    }

    int dist[MAX_NODES], prev[MAX_NODES];
    dijkstra(s, dist, prev);

    char line[256];
    snprintf(line, sizeof(line), "🗺 Dijkstra: %s → %s\n", sname, dname);
    buf_append(route_buffer, line);

    if (dist[d] == INF)
    {
        buf_append(route_buffer, "No path exists.\n");
    }
    else
    {
        snprintf(line, sizeof(line), "Shortest distance: %d\n", dist[d]);
        buf_append(route_buffer, line);

        /* Reconstruct path */
        int path[MAX_NODES], plen = 0;
        int cur = d;
        while (cur != s && cur >= 0)
        {
            path[plen++] = cur;
            cur = prev[cur];
        }
        path[plen++] = s;

        buf_append(route_buffer, "Path: ");
        int k;
        for (k = plen - 1; k >= 0; k--)
        {
            buf_append(route_buffer, names[path[k]]);
            if (k > 0) buf_append(route_buffer, " → ");
        }
        buf_append(route_buffer, "\n");
    }

    /* Show all distances */
    buf_append(route_buffer, "\nAll distances from source:\n");
    int i;
    for (i = 0; i < nNodes; i++)
    {
        if (dist[i] == INF)
            snprintf(line, sizeof(line), "  %s: INF\n", names[i]);
        else
            snprintf(line, sizeof(line), "  %s: %d\n", names[i], dist[i]);
        buf_append(route_buffer, line);
    }
}

static void on_bfs(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    const char *sn = entry_text(route_src_entry);
    char sname[MAX_NAME];
    strncpy(sname, sn, MAX_NAME - 1); sname[MAX_NAME - 1] = '\0';

    int s = findNode(sname);
    buf_clear(route_buffer);

    if (s < 0)
    {
        buf_append(route_buffer, "❌ Node not found!\n");
        return;
    }

    buf_append(route_buffer, "🔍 BFS Traversal from ");
    buf_append(route_buffer, sname);
    buf_append(route_buffer, ":\n");

    /* BFS inline to capture output */
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES], front = 0, rear = 0;
    queue[rear++] = s;
    visited[s] = 1;
    int step = 1;
    char line[128];

    while (front < rear)
    {
        int cur = queue[front++];
        snprintf(line, sizeof(line), "  Step %d: %s\n", step++, names[cur]);
        buf_append(route_buffer, line);

        int i;
        for (i = 0; i < nNodes; i++)
        {
            if (adj[cur][i] != INF && adj[cur][i] != 0 && !visited[i])
            {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
}

static void on_dfs(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    const char *sn = entry_text(route_src_entry);
    char sname[MAX_NAME];
    strncpy(sname, sn, MAX_NAME - 1); sname[MAX_NAME - 1] = '\0';

    int s = findNode(sname);
    buf_clear(route_buffer);

    if (s < 0)
    {
        buf_append(route_buffer, "❌ Node not found!\n");
        return;
    }

    buf_append(route_buffer, "🔎 DFS Traversal from ");
    buf_append(route_buffer, sname);
    buf_append(route_buffer, ":\n");

    /* DFS iterative to capture output */
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES], top = -1;
    stack[++top] = s;
    int step = 1;
    char line[128];

    while (top >= 0)
    {
        int cur = stack[top--];
        if (visited[cur]) continue;
        visited[cur] = 1;

        snprintf(line, sizeof(line), "  Step %d: %s\n", step++, names[cur]);
        buf_append(route_buffer, line);

        int i;
        for (i = nNodes - 1; i >= 0; i--)
        {
            if (adj[cur][i] != INF && adj[cur][i] != 0 && !visited[i])
                stack[++top] = i;
        }
    }
}

/* ── Schedule callback ── */
static void on_schedule(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    buf_clear(sched_buffer);

    if (ordCnt == 0)
    {
        buf_append(sched_buffer, "No orders to schedule.\n");
        return;
    }

    pqClear();
    int i;
    for (i = 0; i < ordCnt; i++)
        pqPush(orders[i]);

    buf_append(sched_buffer, "📅 Scheduled Deliveries (by priority):\n\n");

    char line[256];
    snprintf(line, sizeof(line), "%-6s %-15s %-8s %-8s %-10s %-10s\n",
             "ID", "Name", "Pri", "Wt", "Deadline", "Status");
    buf_append(sched_buffer, line);
    buf_append(sched_buffer, "──────────────────────────────────────────────────────────\n");

    while (pqSize() > 0)
    {
        Order o = pqPop();
        snprintf(line, sizeof(line), "%-6d %-15s %-8d %-8d %-10d %-10s\n",
                 o.id, o.name, o.priority, o.weight, o.deadline, o.status);
        buf_append(sched_buffer, line);
    }
}

/* ── Optimize callbacks ── */
static void on_knapsack(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    buf_clear(opt_buffer);

    int cap = entry_int(opt_cap_entry);
    if (cap <= 0)
    {
        buf_append(opt_buffer, "❌ Enter a valid capacity.\n");
        return;
    }
    if (ordCnt == 0)
    {
        buf_append(opt_buffer, "No orders available.\n");
        return;
    }

    int w[MAX_ORDERS], v[MAX_ORDERS];
    int i;
    for (i = 0; i < ordCnt; i++)
    {
        w[i] = orders[i].weight;
        v[i] = orders[i].value;
    }

    /* Capture knapsack output via redirect */
    buf_append(opt_buffer, "🎒 Knapsack (Vehicle Loading)\n\n");

    /* Run DP */
    int dp[MAX_ORDERS + 1][cap + 1];
    int ii, ww;
    for (ii = 0; ii <= ordCnt; ii++)
        for (ww = 0; ww <= cap; ww++)
            dp[ii][ww] = 0;

    for (ii = 1; ii <= ordCnt; ii++)
    {
        for (ww = 0; ww <= cap; ww++)
        {
            dp[ii][ww] = dp[ii - 1][ww];
            if (w[ii - 1] <= ww && dp[ii - 1][ww - w[ii - 1]] + v[ii - 1] > dp[ii][ww])
                dp[ii][ww] = dp[ii - 1][ww - w[ii - 1]] + v[ii - 1];
        }
    }

    char line[256];
    snprintf(line, sizeof(line), "Vehicle Capacity: %d\n", cap);
    buf_append(opt_buffer, line);
    snprintf(line, sizeof(line), "Maximum Value: %d\n\n", dp[ordCnt][cap]);
    buf_append(opt_buffer, line);

    /* Traceback selected items */
    buf_append(opt_buffer, "Selected Orders:\n");
    snprintf(line, sizeof(line), "%-6s %-15s %-8s %-8s\n", "ID", "Name", "Weight", "Value");
    buf_append(opt_buffer, line);
    buf_append(opt_buffer, "─────────────────────────────────────────\n");

    ww = cap;
    for (ii = ordCnt; ii > 0; ii--)
    {
        if (dp[ii][ww] != dp[ii - 1][ww])
        {
            snprintf(line, sizeof(line), "%-6d %-15s %-8d %-8d\n",
                     orders[ii - 1].id, orders[ii - 1].name,
                     orders[ii - 1].weight, orders[ii - 1].value);
            buf_append(opt_buffer, line);
            ww -= w[ii - 1];
        }
    }
}

static void on_tsp(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    const char *sn = entry_text(opt_tsp_entry);
    char sname[MAX_NAME];
    strncpy(sname, sn, MAX_NAME - 1); sname[MAX_NAME - 1] = '\0';

    int s = findNode(sname);
    buf_clear(opt_buffer);

    if (s < 0)
    {
        buf_append(opt_buffer, "❌ Node not found!\n");
        return;
    }

    if (nNodes < 2)
    {
        buf_append(opt_buffer, "Need at least 2 nodes for TSP.\n");
        return;
    }

    buf_append(opt_buffer, "🛣 TSP (Travelling Salesman) from ");
    buf_append(opt_buffer, sname);
    buf_append(opt_buffer, "\n\n");

    /* Brute-force TSP */
    int perm[MAX_NODES], bestPerm[MAX_NODES];
    int visited[MAX_NODES] = {0};
    int n = nNodes;
    int i;
    int bestCost = INF;

    /* Generate permutations iteratively (small n) */
    /* For simplicity, use recursive approach stored in stack */
    int path[MAX_NODES];
    path[0] = s;
    visited[s] = 1;

    /* Simple recursive TSP with backtracking */
    /* Using iterative approach with stack frames */
    typedef struct { int depth; int node; int cost; int visited[MAX_NODES]; int path[MAX_NODES]; } Frame;
    Frame *stk = malloc(sizeof(Frame) * 10000);
    int top = 0;

    Frame f0;
    f0.depth = 1;
    f0.node = s;
    f0.cost = 0;
    memset(f0.visited, 0, sizeof(f0.visited));
    f0.visited[s] = 1;
    f0.path[0] = s;
    stk[top++] = f0;

    while (top > 0)
    {
        Frame cur = stk[--top];

        if (cur.depth == n)
        {
            int retCost = adj[cur.node][s];
            if (retCost != INF)
            {
                int total = cur.cost + retCost;
                if (total < bestCost)
                {
                    bestCost = total;
                    for (i = 0; i < n; i++) bestPerm[i] = cur.path[i];
                }
            }
            continue;
        }

        for (i = 0; i < n; i++)
        {
            if (!cur.visited[i] && adj[cur.node][i] != INF && top < 9999)
            {
                Frame nf;
                nf.depth = cur.depth + 1;
                nf.node = i;
                nf.cost = cur.cost + adj[cur.node][i];
                memcpy(nf.visited, cur.visited, sizeof(nf.visited));
                nf.visited[i] = 1;
                memcpy(nf.path, cur.path, sizeof(int) * cur.depth);
                nf.path[cur.depth] = i;
                stk[top++] = nf;
            }
        }
    }
    free(stk);

    char line[256];
    if (bestCost == INF)
    {
        buf_append(opt_buffer, "No complete tour found.\n");
    }
    else
    {
        snprintf(line, sizeof(line), "Best Tour Cost: %d\n\nRoute: ", bestCost);
        buf_append(opt_buffer, line);
        for (i = 0; i < n; i++)
        {
            buf_append(opt_buffer, names[bestPerm[i]]);
            buf_append(opt_buffer, " → ");
        }
        buf_append(opt_buffer, names[s]);
        buf_append(opt_buffer, "\n");
    }

    (void)perm;
}

/* ── File I/O callbacks ── */
static void on_save_data(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    saveGraphTXT(GRAPH_FILE);
    char msg[128];
    snprintf(msg, sizeof(msg), "💾 Graph saved to '%s' (%d nodes)\n",
             GRAPH_FILE, nNodes);
    buf_append(file_buffer, msg);
}

static void on_load_data(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    loadGraphTXT(GRAPH_FILE);
    char msg[128];
    snprintf(msg, sizeof(msg), "📂 Graph loaded from '%s' (%d nodes)\n",
             GRAPH_FILE, nNodes);
    buf_append(file_buffer, msg);
}

static void on_export_txt(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    exportOrdersTXT(ORDERS_FILE);
    char msg[128];
    snprintf(msg, sizeof(msg), "📤 Orders exported to '%s' (%d orders)\n",
             ORDERS_FILE, ordCnt);
    buf_append(file_buffer, msg);
}

static void on_import_txt(GtkWidget *btn, gpointer data)
{
    (void)btn; (void)data;
    importOrdersTXT(ORDERS_FILE);
    char msg[128];
    snprintf(msg, sizeof(msg), "📥 Orders imported from '%s' (total: %d)\n",
             ORDERS_FILE, ordCnt);
    buf_append(file_buffer, msg);
}

/* ─── Main ─── */
int main(int argc, char **argv)
{
    GtkApplication *app = gtk_application_new("com.daa.delivery",
                                              G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
