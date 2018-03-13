#include <gtk/gtk.h>

gboolean
on_button1_button_release_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void find_cell(GtkToggleButton *togglebutton, guint8 *i, guint8 *j); // finds wich button in the table was pressed
void set_others_sensitive(gboolean const state);
void set_empty_cells_sensitive(gboolean const state);
gboolean check_validity(guint8 const *row, guint8 const *col); // checks whether the move was legal
guint8 count_cells_remained(); // counts filled cells in the table

void
cell_changed                           (GtkToggleButton *togglebutton,
                                        GtkWindow       *dialogparent);


void on_btnReset_release_event(GtkButton *button, gpointer *user_data);

void DNDBeginCB(GtkToggleButton *togglebutton, GdkDragContext *dc, GdkPixbuf *data);

void DNDEndCB(GtkToggleButton *togglebutton, GdkDragContext *dc, GtkWindow *dialogparent);

gboolean DNDDragMotionCB(GtkToggleButton *togglebutton, GdkDragContext *dc, gint x, gint y, guint t, gpointer data);

void DNDDataRequestCB(GtkToggleButton *togglebutton, GdkDragContext *dc, 
                             GtkSelectionData *selection_data, guint info, guint t, gpointer data);

void DNDDataRecievedCB(GtkToggleButton *togglebutton, GdkDragContext *dc, gint x, gint y, 
                              GtkSelectionData *selection_data, guint info, guint t, gpointer data);
                              
void DNDDataDeleteCB(GtkToggleButton *togglebutton, GdkDragContext *dc, gpointer data);


void destroy(GtkWidget *window,gpointer data);

gboolean delete_event(GtkWidget *window, GdkEvent *event, gpointer data);

