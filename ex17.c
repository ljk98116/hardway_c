#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

//#define MAX_DATA 512
//#define MAX_ROWS 100

//结构体压缩，指针对齐的成员放在前面，从大到小排序
struct Address {
    //char name[MAX_DATA];
    //char email[MAX_DATA];
    char *name;
    char *email;
    int id;
    int set;
};

struct Database {
    //struct Address rows[MAX_ROWS];
    struct Address *rows;
    int MAX_ROWS;
    int MAX_DATA;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

void die(const char *message, struct Connection *conn)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }
    /* 清理连接资源 */
    if(conn) {
        if(conn->file) fclose(conn->file);
        if(conn->db) free(conn->db);
        free(conn);
    }
    exit(1);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n",
            addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1) die("Failed to load database.", conn);
    /* 加载数据库行 */
    /* allocate memory */
    conn->db->rows = (struct Address *)malloc(sizeof(struct Address) * conn->db->MAX_ROWS);
    /* 加载Adress结构体信息 */
    rc = fread(conn->db->rows, sizeof(struct Address), conn->db->MAX_ROWS, conn->file);
    if(rc != conn->db->MAX_ROWS) die("Failed to load database.", conn);
    /* 加载Address内名称信息 */
    for(int i=0;i<conn->db->MAX_ROWS;++i){
        if(conn->db->rows[i].name){
            conn->db->rows[i].name = (char *)malloc(conn->db->MAX_DATA);
            int rc_ = fread(conn->db->rows[i].name, conn->db->MAX_DATA, 1, conn->file);
            if(rc_ != 1) die("load address name error", conn);
        }
        if(conn->db->rows[i].email){
            conn->db->rows[i].email = (char *)malloc(conn->db->MAX_DATA);
            int rc_ = fread(conn->db->rows[i].email, conn->db->MAX_DATA, 1, conn->file);
            if(rc_ != 1) die("load address email error", conn);  
        }      
    }
}

struct Connection *Database_open(const char *filename, char mode)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(!conn) die("Memory error", conn);

    conn->db = malloc(sizeof(struct Database));

    if(!conn->db) die("Memory error", conn);

    if(mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if(conn->file) {
            Database_load(conn);
        }
    }

    if(!conn->file) die("Failed to open the file", conn);

    return conn;
}

void Database_close(struct Connection *conn)
{
    if(conn) {
        if(conn->file) fclose(conn->file);
        if(conn->db) {
            for(int i=0;i<conn->db->MAX_ROWS;++i){
                struct Address addr = conn->db->rows[i];
                free(addr.name);
                free(addr.email);

            }
            free(conn->db->rows);
            free(conn->db);
        }
        free(conn);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1) die("Failed to write database.", conn);

    rc = fwrite(conn->db->rows, sizeof(struct Address), conn->db->MAX_ROWS, conn->file);
    if(rc != conn->db->MAX_ROWS) die("Failed to write database.", conn);

    /* 保存Address内名称信息 */
    for(int i=0;i<conn->db->MAX_ROWS;++i){
        if(conn->db->rows[i].name != NULL){
            int rc_ = fwrite(conn->db->rows[i].name, conn->db->MAX_DATA, 1, conn->file);
            if(rc_ != 1) die("write address name error", conn);
        }
        if(conn->db->rows[i].email != NULL){
            int rc_ = fwrite(conn->db->rows[i].email, conn->db->MAX_DATA, 1, conn->file);
            if(rc_ != 1) die("write address email error", conn);  
        }
    }    
    rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn, int max_rows, int max_data)
{
    int i = 0;
    /* 设置database属性 */
    conn->db->MAX_DATA = max_data;
    conn->db->MAX_ROWS = max_rows;
    conn->db->rows = (struct Address*)malloc(sizeof(struct Address) * max_rows);
    for(i = 0; i < max_rows; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0, .name = NULL, .email = NULL};
        // then just assign it
        conn->db->rows[i] = addr;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die("Already set, delete it first", conn);

    addr->set = 1;
    addr->name = (char*)malloc(conn->db->MAX_DATA);
    memset(addr->name, '\0', conn->db->MAX_DATA);//需要初始化，避免valgrind警告
    char *res = strncpy(addr->name, name, strlen(name) + 1);
    if(!res) die("Name copy failed", conn);

    addr->email = (char*)malloc(conn->db->MAX_DATA);
    memset(addr->email, '\0', conn->db->MAX_DATA);
    res = strncpy(addr->email, email, strlen(email) + 1);
    if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    struct Address addr = {.id = id, .set = 0};
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < conn->db->MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];

        if(cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

    char *filename = argv[1];
    char action = argv[2][0];

    /* 默认值 */
    int max_data = 512;
    int max_rows = 100;
    /* 创建db，如果给定参数 */
    if(action == 'c'){
        if(argc == 5) max_rows = atoi(argv[4]);
        if(argc >= 4) max_data = atoi(argv[3]);
    }

    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    //if(argc > 3) id = atoi(argv[3]);
    if(action != 'c' && argc > 3) id = atoi(argv[3]);

    if(id >= max_rows) die("There's not that many records.", conn);

    switch(action) {
        case 'c':
            Database_create(conn, max_rows, max_data);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 4) die("Need an id to get", conn);

            Database_get(conn, id);
            break;

        case 's':
            if(argc != 6) die("Need id, name, email to set", conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4) die("Need id to delete", conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
        case 'f':{
            break;
        }
        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
    }

    Database_close(conn);

    return 0;
}