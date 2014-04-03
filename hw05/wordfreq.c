#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

typedef struct intvalue
{
    gint val;
} IntValue;


GHashTable *makeNewMap()
{
    GHashTable *map = g_hash_table_new(g_str_hash, g_str_equal);
    if (map == NULL)
    {
        fprintf(stderr, "failed to create the hash table\n");
        exit(EXIT_FAILURE);
    }
    return map;
}

void addWord(GHashTable *map, gchar *word)
{
    IntValue *val = malloc(sizeof(IntValue));
    if (val == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    val->val = 1;
    g_hash_table_insert(map, word, &val);
}

void countWord(GHashTable *map, gchar *word)
{
    gpointer key, value;
    gboolean keyFound;
    keyFound = g_hash_table_lookup_extended(map, word, &key, &value);

    IntValue *val = malloc(sizeof(IntValue));
    val->val = 1;

    if (val == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    if (keyFound)
    {
        val->val = ((IntValue *)value)->val + 1;
        g_hash_table_insert(map, word, val);
    }
    else
    {
        g_hash_table_insert(map, word, val);
    }
}

gchar *removePunct(gchar *word)
{
    int len = strlen((char *)word);
    int numpunc = 0;
    gchar *dest = malloc(sizeof(gchar) * len);
    for (; NULL != *word; ++word)
    {
        gchar c = (gchar) * word;
        if (!g_ascii_ispunct(c))
        {
            *dest = c;
            dest++;
        }
        else
        {
            numpunc++;
        }
    }
    int newlen = len - numpunc;
    dest = dest - newlen;
    return dest;
}

void printMap(GHashTable *map)
{
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init (&iter, map);
    while (g_hash_table_iter_next (&iter, &key, &value))
    {
        g_print("%s: %d\n", (gchar *)key, *(int *)value);
    }
}

gchar *openFile(gchar *filename)
{
    gchar *contents = malloc(sizeof(gchar *));
    gsize *length = malloc(sizeof(gsize *));
    GError **error = malloc(sizeof(GError *));

    if (contents == NULL || length == NULL || error == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    gboolean results = g_file_get_contents(filename, &contents, length, error);
    if (contents == NULL || !results)
    {
        fprintf(stderr, "File Reading Failed.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        return contents;
    }
}

GHashTable *makeHistogram(gchar **str_array)
{
    GHashTable *map = makeNewMap();
    for (; NULL != *str_array; str_array++)
    {
        g_strstrip(*str_array);
        gchar *no_punc = removePunct(*str_array);
        GString *gstr = g_string_new(no_punc);
        if (gstr == NULL)
        {
            fprintf(stderr, "failled to create a new string key\n");
            exit(EXIT_FAILURE);
        }
        if ((gstr->str != NULL) && (gstr->len > 0))
        {
            countWord(map, gstr->str);
        }
    };
    return map;
}

gchar **split(gchar *str)
{
    gchar **str_array = g_strsplit(str, " ", 0);
    return str_array;
}

gint get_freq(gconstpointer a, gconstpointer b, gpointer user_data)
{
    GHashTable *map = (GHashTable *) user_data;
    IntValue *a_freq = (IntValue *)g_hash_table_lookup(map, (gpointer) a);
    IntValue *b_freq = (IntValue *)g_hash_table_lookup(map, (gpointer) b);
    return a_freq->val - b_freq->val;
}

void printSortedMap(GList *list, GHashTable *map)
{
    GList *node = list;
    while (node->next != NULL)
    {
        gchar *key = (gchar *)(node->data);
        IntValue *val = (IntValue *)g_hash_table_lookup(map, node->data);
        g_print("%s : %d\n", key, val->val);
        node = node->next;
    }
}

GList *sortWords(GHashTable *map)
{
    GList *words = g_hash_table_get_keys(map);
    GList *sorted_words = g_list_sort_with_data(words, (GCompareDataFunc) get_freq, (gpointer) map);
    return sorted_words;
}

int main(int argc, char **argv)
{
    gchar *file_contents = openFile("huckfinn.txt");
    gchar **split_file = split(file_contents);
    GHashTable *map = makeHistogram(split_file);
    GList *sorted = sortWords(map);
    printSortedMap(sorted,map);
    return 0;
}