#include "jx_public.h"

int show_ob(jx_ob ob, int depth);
int show_list(jx_ob ob, int depth) {
    int lsize = jx_list_size(ob);
    printf("list size = %d [\n", lsize);
    int i=0;
    for (i=0; i<lsize; ++i) {
      //jx_ob item = jx_list_get(ob,i);
      jx_ob item = jx_list_shift(ob);
      show_ob(item, depth+1);
    }
    printf ("]\n\n");
    lsize = jx_list_size(ob);
    printf("final list size = %d\n", lsize);
    return lsize;
}
int show_str(jx_ob ob) {
    int isize = jx_ob_as_int(jx_ob_size(ob));
    printf("str size = %d; ", isize);
    printf("%s\n", jx_ob_as_str(&ob));
    return isize;
}
int show_null(jx_ob ob) {
    int isize = jx_ob_as_int(jx_ob_size(ob));
    printf("null size = %d\n", isize);
    return isize;
}
int show_bool(jx_ob ob) {
    int isize = jx_ob_as_int(jx_ob_size(ob));
    printf("bool size = %d; ", isize);
    printf("%d\n", jx_ob_as_bool(ob));
    return isize;
}
int show_int(jx_ob ob) {
    int isize = jx_ob_as_int(jx_ob_size(ob));
    printf("int size = %d; ", isize);
    printf("%d\n", jx_ob_as_int(ob));
    return isize;
}
int show_float(jx_ob ob) {
    int isize = jx_ob_as_int(jx_ob_size(ob));
    printf("float size = %d; ", isize);
    printf("%f\n", jx_ob_as_float(ob));
    return isize;
}
int show_hash(jx_ob ob, int depth) {
    int hsize = jx_hash_size(ob);
    printf("hash size = %d {\n", hsize);
    jx_ob keys = jx_hash_keys(ob);
    //show_ob(keys, depth+1);
    int i=0;
    for (i=0; i<hsize; ++i) {
      //jx_ob item = jx_list_get(keys,i);
      jx_ob item = jx_list_shift(keys);
      printf("key ");
      show_ob(item, depth+1);
      //show_ob(jx_hash_get(ob, item), depth+1);
      show_ob(jx_hash_remove(ob, item), depth+1);
    }
    printf ("}\n");
    jx_ob_free(keys);
    hsize = jx_hash_size(ob);
    printf("final hash size = %d\n", hsize);
    return hsize;
}
int show_ident(jx_ob ob) {
    int isize = jx_ob_as_int(jx_ob_size(ob));
    printf("ident size = %d; ", isize);
    printf("%s\n", jx_ob_as_ident(&ob));
    return isize;
}
int show_builtin(jx_ob ob) {
    int isize = jx_ob_as_int(jx_ob_size(ob));
    printf("builtin size = %d; ", isize);
    return isize;
}
/*
int show_builtin_fn(jx_ob ob) {
    int isize = jx_ob_as_int(jx_ob_size(ob));
    printf("builtin_fn size = %d; ", isize);
    printf("%s\n", jx_ob_as_builtin_fn(&ob));
    return isize;
}
*/
int show_ob(jx_ob ob, int depth) {

    if (jx_null_check(ob)) {
        return show_null(ob);
    } else if (jx_bool_check(ob)) {
        return show_bool(ob);
    } else if (jx_int_check(ob)) {
        return show_int(ob);
    } else if (jx_float_check(ob)) {
        return show_float(ob);
    } else if (jx_str_check(ob)) {
        return show_str(ob);
    } else if (jx_list_check(ob)) {
        return show_list(ob, depth);
    } else if (jx_hash_check(ob)) {
        return show_hash(ob, depth);
    } else if (jx_ident_check(ob)) {
        return show_ident(ob);
    } else if (jx_builtin_check(ob)) {
        return show_builtin(ob);
/*
    } else if (jx_builtin_fn_check(ob)) {
        return show_builtin_fn(ob);
*/
    }
    return -1;
}
