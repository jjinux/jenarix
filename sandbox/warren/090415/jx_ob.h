

/* must be public */

typedef struct xx__ob xx_ob;
typedef struct xx__string xx_string;
typedef struct xx__list xx_list;
typedef struct xx__hash xx_hash;


typedef    xx_float;
typedef    double  xx_float;


struct xx__ob {
  xx_uint32 meta;
  union {
    xx_bool  bool_;
    xx_int   int_;
    xx_float  float_;
    xx_string *str;
    xx_list   *list;
    xx_hash   *hash;
  } data;
}

/* could be private */

struct xx__string {
  xx_word size;
  xx_char data[1];
};

struct xx__hash {
...
};

struct xx__list {
...
};


[ [ ...data... ], [10,20] ]   # two dimensional matrix

[ [ ...data... ], [10,20,30] ]   # three dimensional matrix









