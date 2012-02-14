#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "glean.h"
#include "table.h"

#include "greatest.h"

#define HASH_MULTIPLIER 251

hash_t dumb_hash(void *v) {
    char *s = (char *) v;
    hash_t h = 0;
    for (uint i=0; s[i]; i++) {
        h = HASH_MULTIPLIER * h + s[i];
    }
    return h;
}

int cmp(void *a, void *b) {
    char *as = (char *) a;
    char *bs = (char *) b;
    return strcmp(as, bs);
}

static char *words[] = { /* These are from http://veggieipsum.com/. */
    "radish", "salad", "maize", "caulie", "yarrow", "coriander", "endive",
    "spinach", "welsh", "onion", "gourd", "amaranth", "nori", "courgette",
    "radicchio", "chicory", "parsnip", "sorrel", "welsh", "onion",
    "shallot", "mung", "bean", "fennel", "yarrow", "salad", "green", "bean",
    "courgette", "sea", "lettuce", "broccoli", "scallion", "kombu",
    "parsnip", "eggplant", "kombu", "prairie", "turnip", "brussels",
    "sprout", "chicory", "wattle", "seed", "wakame", "artichoke", "squash",
    "rock", "melon", "sweet", "pepper", "garbanzo", "caulie", "spinach",
    "dulse", "fennel", "garbanzo", "green", "bean", "taro", "soko", "nori",
    "zucchini", "soybean", "desert", "raisin", "corn", "spinach",
    "broccoli", "avocado", "onion", "broccoli", "rabe", "shallot", "beet",
    "greens", "garlic", "daikon", "broccoli", "collard", "greens",
    "cauliflower", "kohlrabi", "salad", "earthnut", "pea", "chickweed",
    "celery", "radicchio", "quandong", "ricebean", "salad", "garbanzo",
    "kale", "kakadu", "plum", "swiss", "chard", "tomato", "horseradish",
    "yarrow", "catsear", "zucchini", "ricebean", "garbanzo", "kohlrabi",
    "cabbage", "grape", "celery", "lentil", "okra", "kale", "maize",
    "radicchio", "silver", "beet", "melon", "groundnut", "water", "spinach",
    "wakame", "broccoli", "rabe", "brussels", "sprout", "sweet", "pepper",
    "tatsoi", "lentil", "fennel", "scallion", "parsley", "sierra", "leone",
    "bologi", "celery", "garbanzo", "beetroot", "cauliflower", "courgette",
    "dandelion", "prairie", "turnip", "shallot", "bamboo", "shoot",
    "rutabaga", "chicory", "celery", "maize", "swiss", "chard", "napa",
    "cabbage", "spinach", "bunya", "nuts", "rutabaga", "gram", "soybean",
    "gourd", "beet", "greens", "azuki", "bean", "sierra", "leone", "bologi",
    "celery", "broccoli", "celtuce", "aubergine", "rock", "melon",
    "courgette", "water", "chestnut", "water", "spinach", "leek", "salsify",
    "broccoli", "earthnut", "pea", "lettuce", "chickweed", "water",
    "chestnut", "garbanzo", "napa", "cabbage", "potato", "salad", "fennel",
    NULL
};

/* Word count, not including the terminal NULL. */
static int word_count = sizeof(words) / sizeof(words[0]) - 1;

/* 0 files to go -> "0s " */
TEST add_words_find_dupes() {
    table *t = table_init(2, dumb_hash, cmp);
    int duplicates[word_count];
    bzero(duplicates, word_count * sizeof(int));

    for (int i=0; i<word_count; i++) {
        char *w = words[i];
        ASSERT(w);
        void *cur = table_get(t, w);
        if (cur == NULL) {
            ASSERT(!table_known(t, w));
        } else {
            duplicates[i] = 1;
            ASSERT(table_known(t, w));
        }

        if (table_set(t, (void *) w) == TABLE_SET_FAIL) FAIL();
    }

    for (int i=0; i<word_count; i++) {
        if (0) printf("%d - %s %d\n", i, words[i], duplicates[i]);
    }

    for (int i=0; i<18; i++) {
        ASSERT_EQm("none of the first 18 are duplicates", 0, duplicates[i]);
    }

    ASSERT_EQm("words[19] (\"onion\") is a dup", 1, duplicates[19]);
    ASSERT_EQm("the last word (\"fennel\") is a dup", 1, duplicates[word_count - 1]);

    table_free(t, NULL);
    PASS();
}

void print_cb(void *v) {
    char *s = (char *) v;
    if (greatest_info.verbose) printf("got word: %s\n", s);
}

TEST print_words_if_verbose() {
    table *t = table_init(2, dumb_hash, cmp);
    int duplicates[word_count];
    bzero(duplicates, word_count * sizeof(int));

    for (int i=0; i<word_count; i++) {
        char *w = words[i];
        if (table_set(t, (void *) w) == TABLE_SET_FAIL) FAIL();
    }

    table_apply(t, print_cb);

    table_free(t, NULL);
    PASS();
}

SUITE(table_suite) {
    RUN_TEST(add_words_find_dupes);
    RUN_TEST(print_words_if_verbose);
}
