#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "string.h"

gboolean
on_button1_button_release_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  gtk_main_quit();
  return FALSE;
}

void find_cell(GtkToggleButton *togglebutton, guint8 *i, guint8 *j) // finds wich button in the table was pressed
 {
  for(*i=0; *i<7; ++*i)
   {
    for(*j=0; *j<7; ++*j)
     {
      if(togglebutton==GTK_TOGGLE_BUTTON(board[*i][*j].btn))
       {
        return;
       }
     }
   }
 }
 
void set_others_sensitive(gboolean const state)
 {
  guint8 i,j;
  for(i=0; i<7; ++i)
   {
    for(j=0; j<7; ++j)
     {
      // if cell is not empty, and inside the boundary of the shape
      if(arr[i][j] && shape[i][j])
       {
        gtk_widget_set_sensitive(board[i][j].btn, state);
       }
     }
   }
 }

void set_empty_cells_sensitive(gboolean const state)
 {
  guint8 i,j;
  for(i=0; i<7; ++i)
   {
    for(j=0; j<7; ++j)
     {
      if(!arr[i][j] && shape[i][j]) // if cell is empty, AND inside the boundary of the shape
       {
        // printf("ActiveCell.row: %d, .col: %d\n",activeCell.row,activeCell.col);           // DEBUG ONLY
        // printf("Validity of (i:%d,j:%d): %d, TRUE: %d\n",i,j,check_validity(&i,&j),TRUE); // DEBUG ONLY
        if(check_validity(&i,&j))
         gtk_widget_set_sensitive(board[i][j].btn, state);
        //gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(board[i][j].btn), FALSE);
        // gtk_button_set_label(GTK_BUTTON(board[i][j].btn),""); // TEXT VERSION, FILLED CELL REPRESENTED BY "O"
        // g_object_set(G_OBJECT(board[i][j].img), "visible", FALSE, NULL); // IMAGE VERSION
        /*if(state && check_validity(&i,&j)) g_object_set(G_OBJECT(board[i][j].img), "visible", TRUE, NULL);
        else g_object_set(G_OBJECT(board[i][j].img), "visible", FALSE, NULL); //*/
       }
     }
   }
 }

// checks whether the move was legal
gboolean check_validity(guint8 const *row, guint8 const *col)
 {
  gboolean b=FALSE;
  guint8 ar,ac;
  ar=activeCell.row;
  ac=activeCell.col;
  if((ar==*row && ((ac==(*col-2)) || (ac==(*col+2)))) ||
     (((ar==(*row-2)) || (ar==(*row+2))) && ac==*col))
   { 
    if(((ar==*row) && arr[*row][(ac+*col)/2]) ||
       ((ac==*col) && arr[(ar+*row)/2][*col] ))
     {
      b=TRUE;
     }
   }
  return b;
 }

void board_show()
 {
  guint8 i,j;
  for(i=0; i<7; ++i)
   {
    for(j=0; j<7; ++j)
     {
      if(shape[i][j])
       {
        if(arr[i][j])
         {
          // gtk_button_set_label(GTK_BUTTON(board[i][j].btn),"O"); // TEXT VERSION, FILLED CELL REPRESENTED BY "O"
          g_object_set(G_OBJECT(board[i][j].img), "visible", TRUE, NULL); // IMAGE VERSION
          gtk_widget_set_sensitive(board[i][j].btn, TRUE);
          //g_object_set(G_OBJECT(board[i][j].btn), "visible", TRUE, NULL);
         }
        else
         {
          gtk_widget_set_sensitive(board[i][j].btn, FALSE);
          g_object_set(G_OBJECT(board[i][j].img), "visible", FALSE, NULL);
         } // */
        // TODO: stop signal emmission if neccessary 
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(board[i][j].btn), FALSE);
       }
     }
   }
  activeCell.row=7;
  activeCell.col=7;
  alreadyActive=FALSE;
 }

gboolean check_continuable()
 {
  gboolean b=FALSE;
  guint8 i,j;
  for(i=0; i<7; ++i)
   {
    for(j=0; j<7; ++j)
     {
      if(arr[i][j] && shape[i][j]) // only check filled cells
       {
        // Second cycle
        guint8 y,x;
        guint8 pr,pc; // to store active cell's value
        pr=activeCell.row;
        pc=activeCell.col;
        activeCell.row=i;
        activeCell.col=j;
        for(y=0; y<7; ++y)
         {
          for(x=0; x<7; ++x)
           {
            if(!arr[y][x] && shape[y][x]) // only check empty cells
             {
              if(check_validity(&y,&x))
               {
                // printf("(%d,%d)->(%d,%d)\n",i,j,y,x); // HINT, DEBUG
                return TRUE; // stop continuing the loop to find other possible moves
               }
             }
           }
         }
        // restore activeCell value
        activeCell.row=pr;
        activeCell.col=pc;
       }
     }
   }
  return b;
 }

guint8 count_cells_remained()
 {
  guint8 i,j;
  guint8 c=0;
  for(i=0; i<7; ++i)
   {
    for(j=0; j<7; ++j)
     {
      if(arr[i][j]) // only check filled cells
       {
        c++;
       }
     }
   }
  return c;
 }

void dialog_show(GtkWindow *parent)
 {
  GtkWidget *dialog;
  guint8 c;
  c=count_cells_remained();
  dialog = gtk_message_dialog_new (parent, GTK_DIALOG_MODAL,
                                   GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                   "Congratulations!");
  gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%d cells remained filled.\n", c);
  gtk_window_set_title(GTK_WINDOW(dialog), "Game Over");
  
  gtk_dialog_run(GTK_DIALOG(dialog));
  
  gtk_widget_destroy(dialog);
 }

void
cell_changed                           (GtkToggleButton *togglebutton,
                                        GtkWindow       *dialogparent)
{
 guint8 col,row; // position of the pressed button in the table
 find_cell(togglebutton, &row, &col); // find which button in the table was pressed
 // printf("Atadott sor: %d, oszlop: %d\n",row+1,col+1);
 
 if(!arr[row][col]) // if an empty button was pressed
  {
   // is it a legal move? 
   if(check_validity(&row,&col))
    {
     // do jump & rearrange table
     // printf("Legal move.\n"); // DEBUG FEATURE
     arr[row][col]=TRUE;
     arr[activeCell.row][activeCell.col]=FALSE;
     arr[(row+activeCell.row)/2][(col+activeCell.col)/2]=FALSE;
     // gtk_button_set_label(GTK_BUTTON(board[row][col].btn),"O"); // TEXT VERSION
     g_object_set(G_OBJECT(board[row][col].img), "visible", TRUE, NULL); // IMAGE VERSION
     board_show();
     return;
    }
   else
    {
     // printf("Illegal move.\n"); // DEBUG FEATURE
    }
  }
 
 if(alreadyActive) // if any of the buttons has already been activated
  {
   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(board[activeCell.row][activeCell.col].btn),FALSE); // disactivate that
  }
  
 if(gtk_toggle_button_get_active(togglebutton) && !alreadyActive)
  {
   activeCell.row=row;
   activeCell.col=col;
   // set insensible (empty) buttons sensible
   set_empty_cells_sensitive(TRUE);
   // set all other buttons insensible
   // set_others_sensitive(FALSE);
  }
 else
  {
   set_empty_cells_sensitive(FALSE);
   // set_others_sensitive(TRUE);
  }
 
 // check the possibility of further moves
 if(check_continuable())
  {
   // printf("The game is still continuable.\n");
  }
 else
  {
   // inform the user about
   // printf("The game is over.\n"); // DUMMY NOTIFICATION
   dialog_show((gpointer) dialogparent);
   // reset game environment
   reset_environment();
   board_show();
   return;
  }
 
 alreadyActive=gtk_toggle_button_get_active(togglebutton);
 activeCell.row=row; // here we store the position of the last pressed (activated) button
 activeCell.col=col; 
 
 gtk_widget_set_sensitive(board[activeCell.row][activeCell.col].btn, arr[activeCell.row][activeCell.col]);
 
 // printf("Aktiv sor: %d, oszlop: %d, allapot: %d\n",activeCell.row+1, activeCell.col+1, alreadyActive); // DEBUG FEATURE
}

void on_btnReset_release_event(GtkButton *button, gpointer *user_data)
 {
  // TODO: May ask
  reset_environment();
  board_show();
 }

/**********************************************************************************************************************\
 * DRAG AND DROP HANDLERS                                                                                             *
\**********************************************************************************************************************/
 
/*
 * DND "drag_begin" handler, this is called whenever a drag starts.
 */
void DNDBeginCB(GtkToggleButton *togglebutton, GdkDragContext *dc, GdkPixbuf *data)
 {
  //GdkPixbuf *pxbuf;
  //pxbuf = gdk_pixbuf_copy (pixbuf);
  // my_prog_window_struct *win = (my_prog_window_struct *)data; // CAST USER DATA HERE
  if((togglebutton == NULL) || (data == NULL) || (dc == NULL))
   return;
  /* Put any needed drag begin setup code here. */
  guint8 row,col;
  find_cell(togglebutton, &row, &col); // find which button in the table was pressed
  
  g_object_set(G_OBJECT(board[row][col].img), "visible", FALSE, NULL); // IMAGE VERSION
  
  gtk_drag_set_icon_pixbuf(dc, data, 16, 16);
   
  // g_print("DNDBeginCB, row: %d, col: %d\n",row,col); // DEBUG
  
  activeCell.row=row;
  activeCell.col=col;
  // set insensible (empty) buttons sensible
  set_empty_cells_sensitive(TRUE);
  // set all other buttons insensible
  // set_others_sensitive(FALSE);
 }

/*
 *      DND "drag_end" handler, this is called when a drag and drop has
 * completed. So this function is the last one to be called in
 * any given DND operation.
 */
void DNDEndCB(GtkToggleButton *togglebutton, GdkDragContext *dc, GtkWindow *dialogparent)
 {
  // my_prog_window_struct *win = (my_prog_window_struct *)data;
  if((togglebutton == NULL) || (dialogparent == NULL) || (dc == NULL))
   return;
  /* Put any needed drag end cleanup code here. */
  guint8 row,col;
  find_cell(togglebutton, &row, &col); // find which button in the table was pressed
  // g_print("DNDEndCB, row: %d, col: %d\n",row,col); // DEBUG
  
  g_object_set(G_OBJECT(board[row][col].img), "visible", arr[row][col], NULL); // IMAGE VERSION
 
  if(!check_continuable())
   {
    // inform the user about
    // printf("The game is over.\n"); // DUMMY NOTIFICATION
    dialog_show((gpointer) dialogparent);
    // reset game environment
    reset_environment();
    board_show();
    return;
   }
 }

/*
 * DND "drag_motion" handler, this is called whenever the 
 * pointer is dragging over the target widget.
 */
gboolean DNDDragMotionCB(GtkToggleButton *togglebutton, GdkDragContext *dc, gint x, gint y, guint t, gpointer data)
 {
  // g_print("DNDDragMotionCB\n");
  gboolean same_widget;
  GdkDragAction suggested_action;
  GtkWidget *src_widget, *tar_widget;
  // my_prog_window_struct *win = (my_prog_window_struct *)data;
  if((togglebutton == NULL) || (data == NULL) || (dc == NULL))
   return FALSE;
 
  /* Get source widget and target widget. */
  src_widget = gtk_drag_get_source_widget(dc);
  tar_widget = GTK_WIDGET(togglebutton);
 
  /* Note if source widget is the same as the target. */
  same_widget = (src_widget == tar_widget) ? TRUE : FALSE;
 
 
  /* If this is the same widget, our suggested action should be
   * move.  For all other case we assume copy.
   */
  if(same_widget)
   suggested_action = GDK_ACTION_MOVE;
  else
   suggested_action = GDK_ACTION_COPY;
 
  /* Respond with default drag action (status). First we check
   * the dc's list of actions. If the list only contains
   * move, copy, or link then we select just that, otherwise we
   * return with our default suggested action.
   * If no valid actions are listed then we respond with 0.
   */
 
   /* Only move? */
   //if(dc->actions == GDK_ACTION_MOVE)
   if(gdk_drag_context_get_actions(dc) == GDK_ACTION_MOVE)
       gdk_drag_status(dc, GDK_ACTION_MOVE, t);
   /* Only copy? */
   //else if(dc->actions == GDK_ACTION_COPY)
   else if(gdk_drag_context_get_actions(dc) == GDK_ACTION_COPY)
       gdk_drag_status(dc, GDK_ACTION_COPY, t);
   /* Only link? */
   //else if(dc->actions == GDK_ACTION_LINK)
   else if(gdk_drag_context_get_actions(dc) == GDK_ACTION_LINK)
       gdk_drag_status(dc, GDK_ACTION_LINK, t);
   /* Other action, check if listed in our actions list? */
   //else if(dc->actions & suggested_action)
   else if(gdk_drag_context_get_actions(dc) & suggested_action)
       gdk_drag_status(dc, suggested_action, t);
   /* All else respond with 0. */
   else
       gdk_drag_status(dc, 0, t);
 
  return(FALSE);
 }

/*
 * DND "drag_data_get" handler, for handling requests for DND
 * data on the specified widget. This function is called when
 * there is need for DND data on the source, so this function is
 * responsable for setting up the dynamic data exchange buffer
 * (DDE as sometimes it is called) and sending it out.
 */
void DNDDataRequestCB(GtkToggleButton *togglebutton, GdkDragContext *dc,
                             GtkSelectionData *selection_data, guint info, guint t, gpointer data)
 {
  gboolean data_sent = FALSE;
  gchar text[4];
  // my_prog_window_struct *win = (my_prog_window_struct *)data;
  if((togglebutton == NULL) || (data == NULL) || (dc == NULL))
   return;
 
  guint8 row,col;
  find_cell(togglebutton, &row, &col); // find which button in the table was pressed
  // g_print("DNDRequestCB, row: %d, col: %d\n",row,col); // DEBUG
  col=row*10+col;
  sprintf(text,"%d",col);

  gtk_selection_data_set(selection_data, GDK_SELECTION_TYPE_STRING, 8, /* 8 bits per character. */ text, strlen(text));
  data_sent = TRUE;
 
  /* If we did not send out any data (for whatever reason),
   * then send out an error response since this function
   * needs to gaurantee a response when reasonably possible.
   */
  if(!data_sent)
  {
   const gchar *cstrptr = "Error";
   // g_print("DNDDataRequestCB data sent error.\n"); // DEBUG
   gtk_selection_data_set(selection_data, GDK_SELECTION_TYPE_STRING, 8, /* 8 bits per character. */ cstrptr, strlen(cstrptr));
   data_sent = TRUE;
  }
 }

/*
 *      DND "drag_data_received" handler. When DNDDataRequestCB()
 * calls gtk_selection_data_set() to send out the data, this function
 * recieves it and is responsible for handling it.
 *
 * This is also the only DND callback function where the given
 * inputs may reflect those of the drop target so we need to check
 * if this is the same structure or not.
 */
void DNDDataRecievedCB(GtkToggleButton *togglebutton, GdkDragContext *dc, gint x, gint y, 
                              GtkSelectionData *selection_data, guint info, guint t, gpointer data)
 {
  gboolean same;
  guint8 row, col;
  GtkWidget *source_widget;
  if((togglebutton == NULL) || (data == NULL) || (dc == NULL))
   return;
 
  find_cell(togglebutton, &row, &col); // find which button in the table was pressed
  /* Important, check if we actually got data.  Sometimes errors
   * occure and selection_data will be NULL.
   */
  if(selection_data == NULL)
   return;
  //if(selection_data->length < 0)
  if(gtk_selection_data_get_length(selection_data) < 0)
   return;
 
  // g_print("DNDReceivedCB, row: %d, col: %d\nSelection data: %s\n",row,col,selection_data->data); // DEBUG
  //sscanf(selection_data->data, "%d", (int *)&activeCell.col);
  sscanf(gtk_selection_data_get_data(selection_data), "%d", (int *)&activeCell.col);
  activeCell.row=activeCell.col/10; activeCell.col=activeCell.col%10;
  // g_print("Feldolgozva: row=%d, col=%d\n",activeCell.row,activeCell.col); // DEBUG
 
  /* Source and target widgets are the same? */
  source_widget = gtk_drag_get_source_widget(dc);
  same = (source_widget == GTK_WIDGET(togglebutton)) ? TRUE : FALSE;
 
  if(!same)
   {
    if(!arr[row][col] && check_validity(&row,&col))
     {
      // do jump & rearrange table
      // printf("Legal move.\n"); // DEBUG FEATURE
      arr[row][col]=TRUE;
      arr[activeCell.row][activeCell.col]=FALSE;
      arr[(row+activeCell.row)/2][(col+activeCell.col)/2]=FALSE;
      // gtk_button_set_label(GTK_BUTTON(board[row][col].btn),"O"); // TEXT VERSION
      g_object_set(G_OBJECT(board[row][col].img), "visible", TRUE, NULL); // IMAGE VERSION
      board_show();
      return;
     }
   }
  /* Now check if the data format type is one that we support
   * (remember, data format type, not data type).
   *
   * We check this by testing if info matches one of the info
   * values that we have defined.
   *
   * Note that we can also iterate through the atoms in:
   * GList *glist = dc->targets;
   *
   * while(glist != NULL)
   * {
   *     gchar *name = gdk_atom_name((GdkAtom)glist->data);
   *      * strcmp the name to see if it matches
   *      * one that we support
   *      *
   *     glist = glist->next;
   * }
   */
  /*if((info == DRAG_TAR_INFO_0) ||
     (info == DRAG_TAR_INFO_1) ||
     (info == DRAG_TAR_INFO_2))
  {
   /* Update the new row's cell with the pixmap (if
    * available) and the received data as the text.
    *
  }*/
 }

/*
 * DND "drag_data_delete" handler, this function is called when
 * the data on the source `should' be deleted (ie if the DND was
 * a move).
 */
void DNDDataDeleteCB(GtkToggleButton *togglebutton, GdkDragContext *dc, gpointer data)
 {
  // g_print("DNDDataDeleteCB\n"); // DEBUG
  if((togglebutton == NULL) || (data == NULL) || (dc == NULL))
   return;
 }  
 
/**********************************************************************************************************************/

void destroy(GtkWidget *window,gpointer data)
 {
  gtk_main_quit();
 }

gboolean delete_event(GtkWidget *window, GdkEvent *event, gpointer data)
 {
  return FALSE;
 }
