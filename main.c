/*
 * Shop Stock Management System  (English / Luganda)
 * Course: ICT 1101B/D Principles of Programming
 * Language: C (compile with gcc, NOT g++)
 *
 * Features: add, view, search, update, remove (mark inactive),
 *           sell, report, file saving/loading, 2 languages.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCTS 200
#define NAME_LEN     40
#define LOW_STOCK    5
#define DATA_FILE    "stock.dat"

/* ---------- Record structure ---------- */
typedef struct {
    int    id;
    char   name[NAME_LEN];
    int    qty;
    double price;
    int    active;      /* 1 = active, 0 = removed (status change) */
} Product;

/* ---------- Message IDs (index into language table) ---------- */
enum {
    M_TITLE, M_MENU1, M_MENU2, M_MENU3, M_MENU4, M_MENU5, M_MENU6, M_MENU7, M_MENU0,
    M_CHOICE, M_BADCHOICE, M_NAME, M_QTY, M_PRICE, M_ID, M_STATUS,
    M_ACTIVE, M_INACTIVE, M_BADNUM, M_BADNAME, M_FULL, M_NOFILE, M_LOADED,
    M_ADDED, M_UPDATED, M_REMOVED, M_SOLD, M_NOSTOCK, M_NOTFOUND, M_NONE,
    M_SEARCHFOR, M_SELLQTY, M_REPORT, M_TOTALITEMS, M_TOTALVALUE, M_LOWSTOCK,
    M_SAVEERR, M_BYE, M_CURRENT, M_NEWQTY, M_NEWPRICE, M_NEWNAME,
    M_COUNT
};

/* ---------- Language table: [0]=English, [1]=Luganda ---------- */
static const char *TXT[2][M_COUNT] = {
  { /* ENGLISH */
    [M_TITLE]="===== SHOP STOCK SYSTEM =====",
    [M_MENU1]="1. Add product",
    [M_MENU2]="2. View all products",
    [M_MENU3]="3. Search product",
    [M_MENU4]="4. Update product",
    [M_MENU5]="5. Remove product",
    [M_MENU6]="6. Sell product",
    [M_MENU7]="7. Stock report",
    [M_MENU0]="0. Save and exit",
    [M_CHOICE]="Enter your choice: ",
    [M_BADCHOICE]="Error: invalid choice. Try again.",
    [M_NAME]="Product name", [M_QTY]="Quantity", [M_PRICE]="Price (UGX)",
    [M_ID]="Product ID", [M_STATUS]="Status",
    [M_ACTIVE]="Active", [M_INACTIVE]="Removed",
    [M_BADNUM]="Error: enter a valid number in the allowed range.",
    [M_BADNAME]="Error: name cannot be empty or contain '|'.",
    [M_FULL]="Error: stock is full.",
    [M_NOFILE]="No saved data found. Starting with an empty stock.",
    [M_LOADED]="Records loaded: ",
    [M_ADDED]="Success: product added.",
    [M_UPDATED]="Success: product updated.",
    [M_REMOVED]="Success: product removed (marked inactive).",
    [M_SOLD]="Success: sale recorded. Amount (UGX): ",
    [M_NOSTOCK]="Error: not enough stock.",
    [M_NOTFOUND]="Error: product not found.",
    [M_NONE]="No products to show.",
    [M_SEARCHFOR]="Search name: ",
    [M_SELLQTY]="Quantity to sell",
    [M_REPORT]="----- STOCK REPORT -----",
    [M_TOTALITEMS]="Active products: ",
    [M_TOTALVALUE]="Total stock value (UGX): ",
    [M_LOWSTOCK]="Low stock items (5 or fewer):",
    [M_SAVEERR]="Error: could not save data file!",
    [M_BYE]="Data saved. Goodbye!",
    [M_CURRENT]="Current record:",
    [M_NEWQTY]="New quantity", [M_NEWPRICE]="New price (UGX)",
    [M_NEWNAME]="New name"
  },
  { /* LUGANDA */
    [M_TITLE]="===== OBUWANGUZI BW'EBINTU MU DDUUKA =====",
    [M_MENU1]="1. Yongera ekintu",
    [M_MENU2]="2. Laba ebintu byonna",
    [M_MENU3]="3. Noonya ekintu",
    [M_MENU4]="4. Kyusa ebikwata ku kintu",
    [M_MENU5]="5. Aggyawo ekintu",
    [M_MENU6]="6. Tunda ekintu",
    [M_MENU7]="7. Lipoota y'ebintu",
    [M_MENU0]="0. Tereka ofulume",
    [M_CHOICE]="Londa ekyo ky'oyagala: ",
    [M_BADCHOICE]="Kikyamu: okulonda kwo si kutuufu. Ddamu.",
    [M_NAME]="Erinnya ly'ekintu", [M_QTY]="Obungi", [M_PRICE]="Ebbeeyi (UGX)",
    [M_ID]="Nnamba y'ekintu", [M_STATUS]="Embeera",
    [M_ACTIVE]="Kiriwo", [M_INACTIVE]="Kiggyiddwawo",
    [M_BADNUM]="Kikyamu: wandiika nnamba entuufu mu bbanga eriweereddwa.",
    [M_BADNAME]="Kikyamu: erinnya teriyinza kuba kyereere wadde okubaamu '|'.",
    [M_FULL]="Kikyamu: sitoko ejjudde.",
    [M_NOFILE]="Tewali bitereke bizuuliddwa. Tutandika n'sitoko enkalu.",
    [M_LOADED]="Ebiwandiiko ebizeemu: ",
    [M_ADDED]="Kiyise: ekintu kiyongeddwa.",
    [M_UPDATED]="Kiyise: ekintu kikyusiddwa.",
    [M_REMOVED]="Kiyise: ekintu kiggyiddwawo.",
    [M_SOLD]="Kiyise: okutunda kuwandiikiddwa. Ssente (UGX): ",
    [M_NOSTOCK]="Kikyamu: ebintu tebimala.",
    [M_NOTFOUND]="Kikyamu: ekintu tekizuuliddwa.",
    [M_NONE]="Tewali bintu bya kulaga.",
    [M_SEARCHFOR]="Noonya erinnya: ",
    [M_SELLQTY]="Obungi bw'ogenda okutunda",
    [M_REPORT]="----- LIPOOTA Y'EBINTU -----",
    [M_TOTALITEMS]="Ebintu ebiriwo: ",
    [M_TOTALVALUE]="Omuwendo gwonna ogw'ebintu (UGX): ",
    [M_LOWSTOCK]="Ebintu ebisigadde ebitono (5 oba kitono):",
    [M_SAVEERR]="Kikyamu: tekisobose kutereka fayiro!",
    [M_BYE]="Ebiwandiiko bitereddwa. Weebale!",
    [M_CURRENT]="Ebiriwo kati:",
    [M_NEWQTY]="Obungi obupya", [M_NEWPRICE]="Ebbeeyi empya (UGX)",
    [M_NEWNAME]="Erinnya eppya"
  }
};

/* t points to the table of the chosen language (pointer to array of strings) */
typedef const char * const *Lang;

/* ---------- Global data ---------- */
static Product stock[MAX_PRODUCTS];
static int     count  = 0;      /* number of records used */
static int     nextId = 1;

/* ---------- Input helpers ---------- */
static void readLine(char *buf, int n) {
    if (!fgets(buf, n, stdin)) { buf[0] = '\0'; return; }
    buf[strcspn(buf, "\n")] = '\0';
}

/* Reads an integer in [lo,hi]; repeats until valid. */
static int readInt(Lang t, const char *label, int lo, int hi) {
    char buf[64], *end; long v;
    for (;;) {
        printf("%s [%d-%d]: ", label, lo, hi);
        readLine(buf, sizeof buf);
        v = strtol(buf, &end, 10);
        if (end != buf && *end == '\0' && v >= lo && v <= hi) return (int)v;
        puts(t[M_BADNUM]);
    }
}

static double readMoney(Lang t, const char *label) {
    char buf[64], *end; double v;
    for (;;) {
        printf("%s: ", label);
        readLine(buf, sizeof buf);
        v = strtod(buf, &end);
        if (end != buf && *end == '\0' && v > 0 && v < 1e9) return v;
        puts(t[M_BADNUM]);
    }
}

static void readName(Lang t, const char *label, char *out) {
    char buf[NAME_LEN + 20];
    for (;;) {
        printf("%s: ", label);
        readLine(buf, sizeof buf);
        if (buf[0] != '\0' && strlen(buf) < NAME_LEN && !strchr(buf, '|')) {
            strcpy(out, buf);
            return;
        }
        puts(t[M_BADNAME]);
    }
}

/* ---------- Language selection ---------- */
static int selectLanguage(void) {
    char buf[16];
    for (;;) {
        puts("=================================");
        puts("  1. English");
        puts("  2. Luganda");
        puts("=================================");
        printf("Select language / Londa olulimi: ");
        readLine(buf, sizeof buf);
        if (strcmp(buf, "1") == 0) return 0;
        if (strcmp(buf, "2") == 0) return 1;
        puts("Invalid choice / Okulonda kwo si kutuufu.");
    }
}

/* ---------- Helpers ---------- */
/* Finds an ACTIVE product by id; returns pointer or NULL. */
static Product *findById(int id) {
    int i;
    for (i = 0; i < count; i++)
        if (stock[i].id == id && stock[i].active) return &stock[i];
    return NULL;
}

static int containsIgnoreCase(const char *text, const char *word) {
    size_t i, j, n = strlen(text), m = strlen(word);
    if (m == 0) return 1;
    for (i = 0; i + m <= n; i++) {
        for (j = 0; j < m; j++)
            if (tolower((unsigned char)text[i + j]) != tolower((unsigned char)word[j])) break;
        if (j == m) return 1;
    }
    return 0;
}

/* Prints one product. Uses a pointer so the record is not copied. */
static void printProduct(Lang t, const Product *p) {
    printf("%-4d %-22s %-6d %-10.0f %s\n", p->id, p->name, p->qty, p->price,
           p->active ? t[M_ACTIVE] : t[M_INACTIVE]);
}

static void printHeader(Lang t) {
    printf("%-4s %-22s %-6s %-10s %s\n", "ID", t[M_NAME], "Qty", "UGX", t[M_STATUS]);
    puts("----------------------------------------------------------");
}

/* ---------- File handling ---------- */
static int saveData(void) {
    int i;
    FILE *f = fopen(DATA_FILE, "w");
    if (!f) return 0;
    for (i = 0; i < count; i++)
        fprintf(f, "%d|%s|%d|%.2f|%d\n", stock[i].id, stock[i].name,
                stock[i].qty, stock[i].price, stock[i].active);
    fclose(f);
    return 1;
}

static void loadData(Lang t) {
    char line[128];
    Product p;
    FILE *f = fopen(DATA_FILE, "r");
    if (!f) { puts(t[M_NOFILE]); return; }
    while (count < MAX_PRODUCTS && fgets(line, sizeof line, f)) {
        if (sscanf(line, "%d|%39[^|]|%d|%lf|%d",
                   &p.id, p.name, &p.qty, &p.price, &p.active) == 5) {
            stock[count++] = p;
            if (p.id >= nextId) nextId = p.id + 1;
        }
    }
    fclose(f);
    printf("%s%d\n", t[M_LOADED], count);
}

/* ---------- Operations ---------- */
static void addProduct(Lang t) {
    Product *p;
    if (count >= MAX_PRODUCTS) { puts(t[M_FULL]); return; }
    p = &stock[count];
    p->id = nextId;
    readName(t, t[M_NAME], p->name);
    p->qty    = readInt(t, t[M_QTY], 0, 100000);
    p->price  = readMoney(t, t[M_PRICE]);
    p->active = 1;
    count++; nextId++;
    if (!saveData()) puts(t[M_SAVEERR]);
    puts(t[M_ADDED]);
}

static void viewProducts(Lang t) {
    int i, shown = 0;
    for (i = 0; i < count; i++) {
        if (!stock[i].active) continue;
        if (!shown) printHeader(t);
        printProduct(t, &stock[i]);
        shown++;
    }
    if (!shown) puts(t[M_NONE]);
}

static void searchProduct(Lang t) {
    char key[NAME_LEN + 20];
    int i, shown = 0;
    printf("%s", t[M_SEARCHFOR]);
    readLine(key, sizeof key);
    for (i = 0; i < count; i++) {
        if (stock[i].active && containsIgnoreCase(stock[i].name, key)) {
            if (!shown) printHeader(t);
            printProduct(t, &stock[i]);
            shown++;
        }
    }
    if (!shown) puts(t[M_NOTFOUND]);
}

static void updateProduct(Lang t) {
    int id = readInt(t, t[M_ID], 1, 1000000);
    Product *p = findById(id);
    if (!p) { puts(t[M_NOTFOUND]); return; }
    puts(t[M_CURRENT]); printHeader(t); printProduct(t, p);
    readName(t, t[M_NEWNAME], p->name);
    p->qty   = readInt(t, t[M_NEWQTY], 0, 100000);
    p->price = readMoney(t, t[M_NEWPRICE]);
    if (!saveData()) puts(t[M_SAVEERR]);
    puts(t[M_UPDATED]);
}

static void removeProduct(Lang t) {
    int id = readInt(t, t[M_ID], 1, 1000000);
    Product *p = findById(id);
    if (!p) { puts(t[M_NOTFOUND]); return; }
    p->active = 0;                      /* status change, record kept */
    if (!saveData()) puts(t[M_SAVEERR]);
    puts(t[M_REMOVED]);
}

static void sellProduct(Lang t) {
    int id = readInt(t, t[M_ID], 1, 1000000), q;
    Product *p = findById(id);
    if (!p) { puts(t[M_NOTFOUND]); return; }
    q = readInt(t, t[M_SELLQTY], 1, 100000);
    if (q > p->qty) { puts(t[M_NOSTOCK]); return; }
    p->qty -= q;
    if (!saveData()) puts(t[M_SAVEERR]);
    printf("%s%.0f\n", t[M_SOLD], q * p->price);
}

static void showReport(Lang t) {
    int i, items = 0, low = 0;
    double total = 0;
    for (i = 0; i < count; i++)
        if (stock[i].active) { items++; total += stock[i].qty * stock[i].price; }
    puts(t[M_REPORT]);
    printf("%s%d\n", t[M_TOTALITEMS], items);
    printf("%s%.0f\n", t[M_TOTALVALUE], total);
    puts(t[M_LOWSTOCK]);
    for (i = 0; i < count; i++) {
        if (stock[i].active && stock[i].qty <= LOW_STOCK) {
            if (!low) printHeader(t);
            printProduct(t, &stock[i]);
            low++;
        }
    }
    if (!low) puts(t[M_NONE]);
}

static void showMenu(Lang t) {
    puts("");
    puts(t[M_TITLE]);
    puts(t[M_MENU1]); puts(t[M_MENU2]); puts(t[M_MENU3]); puts(t[M_MENU4]);
    puts(t[M_MENU5]); puts(t[M_MENU6]); puts(t[M_MENU7]); puts(t[M_MENU0]);
}

/* ---------- Main ---------- */
int main(void) {
    int lang = selectLanguage();
    Lang t = TXT[lang];
    char buf[16];
    int running = 1;

    loadData(t);
    while (running) {
        showMenu(t);
        printf("%s", t[M_CHOICE]);
        readLine(buf, sizeof buf);
        if (strlen(buf) != 1) { puts(t[M_BADCHOICE]); continue; }
        switch (buf[0]) {
            case '1': addProduct(t);    break;
            case '2': viewProducts(t);  break;
            case '3': searchProduct(t); break;
            case '4': updateProduct(t); break;
            case '5': removeProduct(t); break;
            case '6': sellProduct(t);   break;
            case '7': showReport(t);    break;
            case '0':
                if (!saveData()) puts(t[M_SAVEERR]);
                else puts(t[M_BYE]);
                running = 0;
                break;
            default: puts(t[M_BADCHOICE]);
        }
    }
    return 0;
}
