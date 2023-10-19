//
//  main.c
//  seq.c
//
//  Created by Michał Korniak on 07/04/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "seq.h" 
#include "memory_tests.h"

typedef struct seq {
    struct seq *left;
    struct seq *mid;
    struct seq *right;
    struct seq *equPrev;
    struct seq *equNext;
    struct equClass *class;
    
} seq_t;

typedef struct equClass {
    char const *name;
    seq_t *first;
    seq_t *last;
} equClass_t;

seq_t * seq_new(void)
{
    seq_t *seq = (seq_t *) malloc(sizeof(seq_t));
    if (seq != NULL) {
        seq->left = NULL;
        seq->mid = NULL;
        seq->right = NULL;
        seq->equPrev = NULL;
        seq->equNext = NULL;
        seq->class = NULL;
    }
    else
        errno = ENOMEM;
    return seq;
}

void seq_delete(seq_t *p)
{
    if (p != NULL) {
        seq_delete(p->left);
        seq_delete(p->mid);
        seq_delete(p->right);
        if (p->class != NULL) {
            seq_t *ptr = p->class->first;
            free(p->class);
            while (ptr != NULL) {
                ptr->class = NULL;
                ptr = ptr->equNext;
            }
        }
        free(p);
    }
}

static int check_char(char const *s){
    if (s == NULL || s[0] == '\0')
        return 0;
    int ok = 1;
    int i = 0;
    while (ok && s[i] != '\0') {
        if (!(s[i] == '0' || s[i] == '1' || s[i] == '2'))
            ok = 0;
        i++;
    }
    return ok;
}

static void seq_initialize(seq_t *seq)
{
    seq->left = NULL;
    seq->mid = NULL;
    seq->right = NULL;
    seq->equPrev = NULL;
    seq->equNext = NULL;
    seq->class = NULL;
}

static void class_initialize(equClass_t* c, seq_t *seq)
{
    c->name = "";
    c->first = seq;
    c->last = seq;
}

static seq_t * seq_add_aux(int* ok)
{
    seq_t *ptr = (seq_t *) malloc(sizeof(seq_t));
    if (ptr == NULL) {
        (*ok) = -1;
        return NULL;
    }
    else {
        seq_initialize(ptr);
        ptr->class = (equClass_t *) malloc(sizeof(equClass_t));
        if (ptr->class == NULL) {
            (*ok) = -1;
            free(ptr);
            return NULL;
        }
        else {
            class_initialize(ptr->class, ptr);
            return ptr;
        }
    }
}

int seq_add(seq_t *p, char const *s)
{
    if (p == NULL || !check_char(s)) {
        errno = EINVAL;
        return -1;
    }
    int i = 0;
    int changed = 0;
    seq_t *ptr = p;
    while (s[i] != '\0') {
        if (s[i] == '0') {
            if (ptr->left == NULL) {
                int ok = 1;
                ptr->left = seq_add_aux(&ok);
                if (ok == -1) {
                    errno = ENOMEM;
                    return -1;
                }
                changed = 1;
            }
            ptr = ptr->left;
        }
        else if (s[i] == '1') {
            if (ptr->mid == NULL) {
                int ok = 1;
                ptr->mid = seq_add_aux(&ok);
                if (ok == -1) {
                    errno = ENOMEM;
                    return -1;
                }
                changed = 1;
            }
            ptr = ptr->mid;
        }
        else if (s[i] == '2') {
            if (ptr->right == NULL) {
                int ok = 1;
                ptr->right = seq_add_aux(&ok);
                if (ok == -1) {
                    errno = ENOMEM;
                    return -1;
                }
                changed = 1;
            }
            ptr = ptr->right;
        }
        i++;
    }
    return changed;
}

static void seq_remove_aux(seq_t *p)
{
    if (p != NULL) {
        seq_remove_aux(p->left);
        seq_remove_aux(p->mid);
        seq_remove_aux(p->right);
        if (p->equPrev == NULL && p->equNext == NULL)
            free(p->class);
        else if (p->equPrev == NULL) {
            p->class->first = p->equNext;
            p->equNext->equPrev = NULL;
        }
        else if (p->equNext == NULL) {
            p->class->last = p->equPrev;
            p->equPrev->equNext = NULL;
        }
        else {
            p->equPrev->equNext = p->equNext;
            p->equNext->equPrev = p->equPrev;
        }
        free(p);
    }
}

static seq_t * is_seq_valid(seq_t *p, char const *s)
{
    int i = 0;
    seq_t *ptr = p;
    while (ptr != NULL && s[i] != '\0') {
        if (s[i] == '0')
            ptr = ptr->left;
        else if (s[i] == '1')
            ptr = ptr->mid;
        else if (s[i] == '2')
            ptr = ptr->right;
        i++;
    }
    return ptr;
}
int seq_remove(seq_t *p, char const *s)
{
    if (p == NULL || !check_char(s)) {
        errno = EINVAL;
        return -1;
    }
    int i = 0;
    seq_t *ptr = p;
    seq_t *ptrPrev = NULL;
    while (ptr != NULL && s[i] != '\0') {
        ptrPrev = ptr;
        if (s[i] == '0')
            ptr = ptr->left;
        else if (s[i] == '1')
            ptr = ptr->mid;
        else if (s[i] == '2')
            ptr = ptr->right;
        i++;
    }

    if (ptr == NULL)
        return 0;
    else {
        seq_remove_aux(ptr);
        if (s[i - 1] == '0')
            ptrPrev->left = NULL;
        else if (s[i - 1] == '1')
            ptrPrev->mid = NULL;
        else if (s[i - 1] == '2')
            ptrPrev->right = NULL;
        return 1;
    }
}

int seq_valid(seq_t *p, char const *s)
{
    if (p == NULL || !check_char(s)) {
        errno = EINVAL;
        return -1;
    }
    seq_t *ptr = is_seq_valid(p, s);
    if (ptr == NULL)
        return 0;
    else
        return 1;
}


static int compare_names(char const *n, char const *c)
{
    int i = 0;
    while (n[i] != '\0' && c[i] != '\0' && n[i] == c[i])
        i++;
    return n[i] == '\0' && c[i] == '\0';
}

int seq_set_name(seq_t *p, char const *s, char const *n)
{
    if (p == NULL || !check_char(s) || n == NULL || n[0] == '\0') {
        errno = EINVAL;
        return -1;
    }
    seq_t *ptr = is_seq_valid(p, s);
    if (ptr == NULL || compare_names(n, ptr->class->name))
        return 0;
    else {
        ptr->class->name = n;
        return 1;
    }
}

char const * seq_get_name(seq_t *p, char const *s)
{
    if (p == NULL || !check_char(s)) {
        errno = EINVAL;
        return NULL;
    }
    seq_t *ptr = is_seq_valid(p, s);
    if (ptr == NULL || ptr->class->name == NULL ||
        ptr->class->name[0] == '\0') {
        errno = 0;
        return NULL;
    }
    else
        return ptr->class->name;
}

static void count_length(char const *s1, char const *s2, int *i)
{
    while(s1[*i] != '\0') {
        (*i)++;
    }
    int j = 0;
    while(s2[j] != '\0') {
        (j)++;
    }
    (*i) += j + 1;
}

static void concatenate(char const *s1, char const *s2, char s[])
{
    int i = 0;
    while(s1[i] != '\0') {
        s[i] = s1[i];
        i++;
    }
    int j = 0;
    while(s2[j] != '\0') {
        s[i] = s2[j];
        i++;
        j++;
    }
    s[i] = '\0';
}

int seq_equiv(seq_t *p, char const *s1, char const *s2)
{
    if (p == NULL || !check_char(s1) || !check_char(s2)) {
        errno = EINVAL;
        return -1;
    }
    seq_t *ptr1 = is_seq_valid(p, s1);
    seq_t *ptr2 = is_seq_valid(p, s2);
    if (ptr1 == NULL || ptr2 == NULL || ptr1->class == ptr2->class)
        return 0;
    else {
        char const *n2 = ptr2->class->name;
        ptr1->class->last->equNext = ptr2->class->first;
        ptr2->class->first->equPrev = ptr1->class->last;
        ptr1->class->last = ptr2->class->last;
        equClass_t *c = ptr2->class;
        seq_t *ptr3 = ptr2->class->first;
        while (ptr3 != NULL) {
            ptr3->class = ptr1->class;
            ptr3 = ptr3->equNext;
        }
        free(c);
        if (n2[0] != '\0' && ptr1->class->name[0] == '\0')
            ptr1->class->name = n2;
        else if (ptr1->class->name[0] != '\0' && n2[0] != '\0'
                 && !compare_names(ptr1->class->name, n2)) {
            int i = 0;
            count_length(ptr1->class->name, n2, &i);
            char s[i];
            concatenate(ptr1->class->name, n2, s);
            ptr1->class->name = s;
        }
        return 1;
    }
}

int main (void) {
   char *str;

   /* Initial memory allocation */
   str = (char *) malloc(15);
   strcpy(str, "tutorialspoint");
   printf("String = %s,  Address = %u\n", str, str);

   /* Reallocating memory */
   str = (char *) realloc(str, 25);
   strcat(str, ".com");
   printf("String = %s,  Address = %u\n", str, str);

   /* Deallocate allocated memory */
   free(str);
   
   return(0);
}
