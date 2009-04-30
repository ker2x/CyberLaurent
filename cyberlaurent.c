/*
 * Copyright (c) 2002-2009 Frank DENIS <j at pureftpd dot org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef enum Type_ {
    T_WHAT = 1 << 0,
    T_MALE = 1 << 1,
    T_MALES = 1 << 2,        
    T_FEMALE = 1 << 3,
    T_FEMALES = 1 << 4,
    T_SUBJ = 1 << 5,
    T_SUBJP = 1 << 6,
    T_ACT = 1 << 7,
    T_ACT1 = 1 << 8,
    T_ACT1P = 1 << 9,        
    T_ACT2 = 1 << 10,
    T_ACT2P = 1 << 11,
    T_ADJ = 1 << 12,
    T_LINK = 1 << 13,
    T_VERB = 1 << 14,
    T_SING = 1 << 15,
    T_PLURAL = 1 << 16,
    T_CC = 1 << 17,
    T_CCBEG = 1 << 18,
    T_CCFIN = 1 << 19,
    T_OPTION = 1 << 31    
} Type;

typedef struct Word_ {
    const char *text;
    const Type type;
    int usage;
} Word;

typedef Type Grammar[64];

Word words[] = {
    { "l'iron", T_WHAT | T_SUBJ | T_MALE | T_SING, 0 },
    { "le firmware", T_WHAT | T_SUBJ | T_MALE | T_SING, 0 },
    { "les ports", T_SUBJ | T_MALES | T_PLURAL, 0 },
    { "des serveurs", T_SUBJP | T_MALES | T_PLURAL, 0 },
    { "sont passes en mode", T_ACT1 | T_MALES | T_PLURAL, 0 },
    { "a ete switche en mode", T_ACT1 | T_MALE | T_SING, 0 },    
    { "testing", T_ACT1P | T_MALE | T_MALES | T_FEMALE | T_FEMALES, 0 },
    { "pooling", T_ACT1P | T_MALE | T_MALES | T_FEMALE | T_FEMALES, 0 },    
    { "corporate", T_ACT1P | T_MALE | T_MALES | T_FEMALE | T_FEMALES, 0 },    
    { "actif", T_ADJ | T_ACT1P | T_MALE, 0 },
    { "actives", T_ADJ | T_ACT1P | T_FEMALES, 0 },
    { "mais", T_LINK, 0 },
    { "donc", T_LINK, 0 },    
    { "or", T_LINK, 0 },
    { "pourtant", T_LINK, 0 },
    { "cependant", T_LINK, 0 },    
    { "pour couronner le tout", T_LINK, 0 },    
    { "si bien que", T_LINK, 0 },
    { "a priori,", T_LINK, 0 },
    { "au passage,", T_LINK, 0 },
    { "d'ailleurs,", T_LINK, 0 },
    { "au fait,", T_LINK, 0 },
    { "et", T_LINK, 0 },
    { "c'est pour ca que", T_LINK, 0 },    
    { "le serveur", T_WHAT | T_SUBJ | T_MALE | T_SING, 0 },
    { "ne renvoit pas", T_VERB | T_SING, 0 },
    { "n'envoient pas", T_VERB | T_PLURAL, 0 },    
    { "les requetes", T_WHAT | T_SUBJ | T_FEMALES | T_PLURAL, 0 },
    { "rollback", T_ACT1P, 0 },
    { "un rollback", T_WHAT | T_SUBJ | T_MALE | T_SING, 0 },
    { "des warm restart", T_WHAT | T_SUBJ | T_MALES | T_PLURAL, 0 },
    { "de spare", T_ADJ | T_MALE | T_FEMALE | T_MALES | T_FEMALES, 0 },
    { "virtuel", T_ADJ | T_MALE, 0 },
    { "est toujours", T_ACT2 | T_SING | T_MALE | T_FEMALE, 0 },
    { "sont encore", T_ACT2 | T_PLURAL | T_MALES | T_FEMALES, 0 },
    { "down", T_ACT2P | T_MALE | T_FEMALE | T_MALES | T_FEMALES, 0 },
    { "up", T_ACT2P | T_MALE | T_FEMALE | T_MALES | T_FEMALES, 0 },
    { "off", T_ACT2P | T_MALE | T_FEMALE | T_MALES | T_FEMALES, 0 },
    { "du flapping", T_WHAT | T_MALE | T_SING, 0 },
    { "du pointcasting", T_WHAT | T_MALE | T_SING, 0 },
    { "un debriefing", T_SUBJ | T_MALE | T_SING, 0 },
    { "les logs", T_SUBJ | T_MALES | T_PLURAL, 0 },
    { "des logs", T_WHAT | T_MALES | T_PLURAL, 0 },
    { "le filer", T_SUBJ | T_WHAT | T_MALE | T_SING, 0 },
    { "le fw", T_SUBJ | T_WHAT | T_MALE | T_SING, 0 },
    { "le switch", T_SUBJ | T_WHAT | T_MALE | T_SING, 0 },        
    { "le peering", T_SUBJ | T_WHAT | T_MALE | T_SING, 0 },
    { "la politique de routage", T_SUBJ | T_WHAT | T_FEMALE | T_SING, 0 },
    { "reboote", T_VERB | T_SING, 0 },
    { "le failovering", T_SUBJ | T_WHAT | T_MALE | T_SING, 0 },
    { "l'accounting", T_SUBJ | T_WHAT | T_MALE | T_SING, 0 },
    { "un test", T_SUBJ | T_WHAT | T_MALE | T_SING, 0 },
    { "mounté en raid5", T_ADJ | T_MALE, 0 },
    { "de /parisdata", T_ADJ | T_MALE | T_FEMALE | T_MALES | T_FEMALES, 0 },    
    { "la todo list", T_SUBJ | T_WHAT | T_FEMALE | T_SING, 0 },
    { "la BDD", T_SUBJ | T_WHAT | T_FEMALE | T_SING, 0 },
    { "finale", T_ADJ | T_FEMALE, 0 },
    { "de backup", T_ADJ | T_MALE, 0 },
    { "une procedure d'urgence", T_SUBJ | T_WHAT | T_FEMALE | T_SING, 0 },
    { "nous a pete a la gueule", T_ACT | T_MALE | T_FEMALE | T_SING, 0 },
    { "commencent a merder", T_ACT | T_MALES | T_FEMALES | T_PLURAL, 0 },    
    { "des service dependency definition", T_SUBJ | T_WHAT | T_MALES | T_PLURAL, 0 },
    { "mysql2", T_SUBJ | T_WHAT | T_FEMALE | T_SING, 0 },
    { "ping", T_VERB | T_SING, 0 },
    { "route", T_VERB | T_SING, 0 },
    { "routent", T_VERB | T_PLURAL, 0 },
    { "montre", T_VERB | T_SING, 0 },
    { "un broadcast", T_WHAT | T_MALE | T_SING, 0 },
    { "le mrtg", T_WHAT | T_MALE | T_SING, 0 },
    { "les altertes du mon", T_WHAT | T_FEMALES | T_PLURAL, 0 },
    { "remet", T_VERB | T_SING, 0 },
    { "resettent", T_VERB | T_SING, 0 },    
    { "forwardent", T_VERB | T_PLURAL, 0 },
    { "les autres hosts", T_SUBJ | T_WHAT | T_MALES | T_PLURAL, 0 },
    { "des stats", T_SUBJ | T_WHAT | T_MALES | T_PLURAL, 0 },
    { "la gw", T_SUBJ | T_WHAT | T_FEMALE | T_SING, 0 },
    { "le hosting", T_SUBJ | T_WHAT | T_FEMALE | T_SING, 0 },        
    { "la machine de William", T_SUBJ | T_WHAT | T_FEMALE | T_SING, 0 },
    { "une configuration foireuse", T_SUBJ | T_WHAT | T_FEMALE | T_SING, 0 },
    { "un truc bizarre", T_SUBJ | T_WHAT | T_MALE | T_SING, 0 },
    { "depuis quelques jours", T_CC | T_CCFIN, 0 },
    { "et ca se produit surtout sur la baie 13", T_CC | T_CCFIN, 0 },    
    { "d'apres nos partenaires", T_CC | T_CCFIN, 0 },
    { "comme l'a dit Const", T_CC | T_CCFIN, 0 },
    { "j'ai remarque que", T_CC | T_CCBEG, 0 },
    { "dans les logs du c7206 on voit que", T_CC | T_CCBEG, 0 },    
    { "je constate que", T_CC | T_CCBEG, 0 },    
    { "il se passe un truc space:", T_CC | T_CCBEG, 0 },
    { "faut qu'on m'explique pourquoi", T_CC, 0 },
    { "tu me parles sur un autre ton, car", T_CC | T_CCBEG, 0 },
    { "le mec de Colt m'a dit que", T_CC | T_CCBEG, 0 },
    { "il se trouve que", T_CC | T_CCBEG, 0 },
    { "c'est bizarre:", T_CC | T_CCBEG, 0 },    
    { "c'est pas normal...", T_CC | T_CCFIN, 0 },
    { "et je vais t'assigner une tache pour ca", T_CC | T_CCFIN, 0 },    
    { NULL, (Type) -1, 0 },
};

const Grammar grammar[] = {
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALE, T_VERB | T_SING, T_WHAT, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALE, T_VERB | T_SING, T_WHAT, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALES, T_VERB | T_PLURAL, T_WHAT, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALES, T_VERB | T_PLURAL, T_WHAT, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALE, T_VERB | T_SING, T_WHAT | T_MALE, T_ADJ | T_MALE, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALE, T_VERB | T_SING, T_WHAT | T_MALES, T_ADJ | T_MALES, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALE, T_VERB | T_SING, T_WHAT | T_FEMALE, T_ADJ | T_FEMALE, T_OPTION | T_CC | T_CCFIN, (Type) -1 },    
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALE, T_VERB | T_SING, T_WHAT | T_FEMALES, T_ADJ | T_FEMALES, T_OPTION | T_CC | T_CCFIN, (Type) -1 },

    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALE, T_VERB | T_SING, T_WHAT | T_MALE, T_ADJ | T_MALE, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALE, T_VERB | T_SING, T_WHAT | T_MALES, T_ADJ | T_MALES, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALE, T_VERB | T_SING, T_WHAT | T_FEMALE, T_ADJ | T_FEMALE, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALE, T_VERB | T_SING, T_WHAT | T_FEMALES, T_ADJ | T_FEMALES, T_OPTION | T_CC | T_CCFIN, (Type) -1 },    
    
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALES, T_VERB | T_PLURAL, T_WHAT | T_MALE, T_ADJ | T_MALE, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALES, T_VERB | T_PLURAL, T_WHAT | T_MALES, T_ADJ | T_MALES, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALES, T_VERB | T_PLURAL, T_WHAT | T_FEMALE, T_ADJ | T_FEMALE, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALES, T_VERB | T_PLURAL, T_WHAT | T_FEMALES, T_ADJ | T_FEMALES, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALES, T_VERB | T_PLURAL, T_WHAT | T_MALE, T_ADJ | T_MALE, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALES, T_VERB | T_PLURAL, T_WHAT | T_MALES, T_ADJ | T_MALES, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALES, T_VERB | T_PLURAL, T_WHAT | T_FEMALE, T_ADJ | T_FEMALE, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALES, T_VERB | T_PLURAL, T_WHAT | T_FEMALES, T_ADJ | T_FEMALES, T_OPTION | T_CC | T_CCFIN, (Type) -1 },    

    /* new style */
    
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALE, T_ACT1 | T_MALE | T_SING, T_ACT1P, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALE, T_ACT1 | T_FEMALE | T_SING, T_ACT1P, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALES, T_ACT1 | T_MALES | T_PLURAL, T_ACT1P, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALES, T_ACT1 | T_FEMALES | T_PLURAL, T_ACT1P, T_OPTION | T_CC | T_CCFIN, (Type) -1 },

    /* snip snip */
    
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALE, T_ACT1 | T_SING | T_MALE, T_ACT1P, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALE, T_ACT1 | T_SING | T_FEMALE, T_ACT1P, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALES, T_ACT1 | T_PLURAL | T_MALES, T_ACT1P, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALES, T_ACT1 | T_PLURAL | T_FEMALES, T_ACT1P, T_OPTION | T_CC | T_CCFIN, (Type) -1 },

    /* new style */
    
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALE, T_ACT2 | T_SING | T_MALE, T_ACT2P, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALE, T_ACT2 | T_SING | T_FEMALE, T_ACT2P, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALES, T_ACT2 | T_PLURAL | T_MALES , T_ACT2P, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALES, T_ACT2 | T_PLURAL | T_FEMALES, T_ACT2P, T_OPTION | T_CC | T_CCFIN, (Type) -1 },

    /* new-new style */
    
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALE, T_ACT | T_SING | T_MALE, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALE | T_SING, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALE, T_ACT | T_SING | T_FEMALE, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_MALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_MALES, T_ACT | T_PLURAL | T_MALES, T_OPTION | T_CC | T_CCFIN, (Type) -1 },
    { T_OPTION | T_CC | T_CCBEG, T_SUBJ | T_FEMALES | T_PLURAL, T_OPTION | T_SUBJP, T_OPTION | T_ADJ | T_FEMALES, T_ACT | T_PLURAL | T_FEMALES, T_OPTION | T_CC | T_CCFIN, (Type) -1 },

    
    { (Type) -1 },
};

size_t mystrlcat(char *dst, const char *src, size_t siz)
{
    register char *d = dst;
    register const char *s = src;
    register size_t n = siz;
    size_t dlen;
    
    while (n-- != 0 && *d != 0) {
        d++;
    }
    dlen = d - dst;
    n = siz - dlen;    
    if (n == 0) {
        return dlen + strlen(s);
    }
    while (*s != 0) {
        if (n != 1) {
            *d++ = *s;
            n--;
        }
        s++;
    }
    *d = 0;
    
    return dlen + (s - src);
}

void reset(void)
{
    Word *wordpnt = words;

    do {
        if (wordpnt->usage > 0) {
            wordpnt->usage--;
        }
        wordpnt++;
    } while (wordpnt->text != NULL);
}

int get(Type type, char * const buf, const size_t buflen, int * const initialp)
{
    Word *wordpnt = &words[rand() % (sizeof words / sizeof words[0] - 1)];
    const Word * const start = wordpnt;

    if ((type & T_OPTION) != (Type) 0) {
        if (rand() > rand()) {
            return 1;
        }
    }
    type &= ~T_OPTION;    
    do {
        if ((wordpnt->type & type) == type && wordpnt->usage == 0) {
            wordpnt->usage += 8;
            if (*initialp != 0) {
                char wi[strlen(wordpnt->text) + (size_t) 1U];
                
                *initialp = 0;
                strcpy(wi, wordpnt->text);
                wi[0] = toupper(wi[0]);
                mystrlcat(buf, wi, buflen);
            } else {
                mystrlcat(buf, wordpnt->text, buflen);
            }
            return 0;
        }
        wordpnt++;
        if (wordpnt->text == NULL) {
            wordpnt = words;
        }
    } while (wordpnt != start);
    
    return -1;
}

void laurentize(char *buf)
{
    const size_t s = strlen(buf);
    int sw = s / 32;
    int p;
    int c;
    
    do {
        p = rand() % s;
        if (isalpha(buf[p]) && !isupper(buf[p]) && isalpha(buf[p + 1])) {
            c = buf[p];
            buf[p] = buf[p + 1];
            buf[p + 1] = c;
        }
        sw--;
    } while (sw != 0);
}

void head(char * const buf, const size_t buflen)
{
    (void) buf;
    (void) buflen;
}

void sig(char * const buf, const size_t buflen)
{
    mystrlcat(buf, "\n\nLL\n", buflen);
}

int main(void)
{
    char buf[32768];
    const Grammar *gpnt;
    const Type *lpnt;
    char *z;
    char *bpb = buf;
    size_t pointbreak = (size_t) 0U;
    int initial = 1;
    
    srand((unsigned int) time(NULL) % (unsigned int) getpid());
    head(buf, sizeof buf);
    do {
        gpnt = &grammar[rand() % (sizeof grammar / sizeof grammar[0] - 1)];
        lpnt = *gpnt;
        reset();
        bpb = buf + strlen(buf);
        do {
            switch (get(*lpnt, buf, sizeof buf, &initial)) {
            case -1:
                *bpb = 0;
                goto skip;
            case 1:
                goto nxt;
            }
            mystrlcat(buf, " ", sizeof buf);            
            nxt:
            lpnt++;
        } while (*lpnt != (Type) -1);        
        z = buf + (strlen(buf) - (size_t) 1U);        
        while (z != buf && isspace(*z)) {
            *z-- = 0;
        }
        if (rand() > rand()) {
            if (rand() > rand()) {
                mystrlcat(buf, ". ", sizeof buf);
            } else {
                mystrlcat(buf, ".\n\n", sizeof buf);
            }
            initial = 1;
            if (rand() > rand()) {
                pointbreak = (size_t) 0U;
            } else {
                pointbreak = strlen(buf);
                get(T_LINK, buf, sizeof buf, &initial);
                mystrlcat(buf, " ", sizeof buf);
            }
        } else {
            pointbreak = strlen(buf);
            if (rand() > rand()) {
                mystrlcat(buf, ", ", sizeof buf);
            } else {
                mystrlcat(buf, " ", sizeof buf);
            }
            get(T_LINK, buf, sizeof buf, &initial);
            mystrlcat(buf, " ", sizeof buf);            
        }
        skip:
        gpnt++;
    } while (**gpnt != (Type) -1);    
    if (pointbreak > (size_t) 0U) {
        buf[pointbreak] = 0;
    }
    laurentize(buf);
    sig(buf, sizeof buf);
    puts(buf);
    
    return 0;
}


