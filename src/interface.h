
typedef struct
{
  GtkWidget *btn;
  GtkWidget *img;
  guint8 row;
  guint8 col;
} TCell;

extern GdkPixbuf *pixbuf; // Ball image

extern gboolean const shape[7][7]; // Board shape
extern gboolean arr[7][7]; // Array of cells, TRUE denotes filled (active) cells

extern TCell board[7][7];

// struct to store the position of the last pressed (activated) button in the table. 7 means none (table index 0-6)
typedef struct {
 guint8 row;
 guint8 col;
} TActiveCell;

extern TActiveCell activeCell;

extern gboolean alreadyActive; // variable to make sure that only one button can be active at once

void reset_environment();

GtkWidget* create_window1 (void);
