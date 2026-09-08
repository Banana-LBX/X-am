#ifndef CIMP_H
#define CIMP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

// Configuration
#ifndef CIMP_INITIAL_CAPACITY
#define CIMP_INITIAL_CAPACITY 8
#endif

// Math Macros
#ifndef CIMP_PI
#define CIMP_PI 3.14159265358979323846
#endif

#define min(a, b) \
    ((a) < (b) ? (a) : (b))

#define max(a, b) \
    ((a) > (b) ? (a) : (b))

#define clamp(x, min_value, max_value) \
    (max((min_value), min((x), (max_value))))

#define lerp(a, b, t) \
    ((a) + ((b) - (a)) * (t))

#define map(value, in_min, in_max, out_min, out_max) \
    ((out_min) + \
    (((value) - (in_min)) * ((out_max) - (out_min))) / \
    ((in_max) - (in_min)))

#define sign(x) \
    (((x) > 0) - ((x) < 0))

#define deg2rad(degrees) \
    ((degrees) * (CIMP_PI / 180.0))

#define rad2deg(radians) \
    ((radians) * (180.0 / CIMP_PI))

// Dynamic Array
typedef struct {
    void *items;
    size_t item_size;
    size_t count;
    size_t capacity;
} List;

#define list_new(type) \
    cimp_list_new(sizeof(type))

List cimp_list_new(size_t item_size);

bool list_append(List *list, const void *item);

bool list_insert(
    List *list,
    size_t index,
    const void *item
);

bool list_remove(
    List *list,
    size_t index
);

bool list_pop(
    List *list,
    void *out_item
);

void list_shuffle(List *list);

void list_clear(List *list);

void list_free(List *list);

// Get item from list
#define list_get(type, list, index) \
    ((type *)((char *)(list)->items + \
    ((index) * (list)->item_size)))

// Append to list
#define list_push(list, value) \
    do { \
        __typeof__(value) cimp_temp_value = (value); \
        list_append((list), &cimp_temp_value); \
    } while (0)


// Strings
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} String;

String string_new(void);

String string_from(const char *str);

bool string_append(
    String *string,
    const char *text
);

bool string_append_char(
    String *string,
    char character
);

bool string_insert(
    String *string,
    size_t index,
    const char *text
);

bool string_remove(
    String *string,
    size_t index,
    size_t count
);

List string_split(
    const String *string,
    const char *delimiter
);

void string_upper(String *string);

void string_lower(String *string);

void string_free(String *string);

// Hash map
/*
    Generic values with string keys.

    Example:

        HashMap map = hashmap_new(int);

        int health = 100;

        hashmap_set(
            &map,
            "health",
            &health
        );

        int *result =
            hashmap_get(int, &map, "health");
*/
typedef struct {
    char *key;
    void *value;
    bool occupied;
} HashMapEntry;

typedef struct {
    HashMapEntry *entries;
    size_t value_size;
    size_t count;
    size_t capacity;
} HashMap;

#define hashmap_new(type) \
    cimp_hashmap_new(sizeof(type))

HashMap cimp_hashmap_new(size_t value_size);

bool hashmap_set(
    HashMap *map,
    const char *key,
    const void *value
);

void *cimp_hashmap_get(
    HashMap *map,
    const char *key
);

#define hashmap_get(type, map, key) \
    ((type *)cimp_hashmap_get((map), (key)))

bool hashmap_remove(
    HashMap *map,
    const char *key
);

void hashmap_free(HashMap *map);


// Stack
typedef struct {
    void *items;
    size_t item_size;
    size_t count;
    size_t capacity;
} Stack;

#define stack_new(type) \
    cimp_stack_new(sizeof(type))

Stack cimp_stack_new(size_t item_size);

bool stack_push(
    Stack *stack,
    const void *item
);

bool stack_pop(
    Stack *stack,
    void *out_item
);

void *stack_peek(Stack *stack);

void stack_free(Stack *stack);

#define stack_push_value(stack, value) \
    do { \
        __typeof__(value) cimp_temp_value = (value); \
        stack_push((stack), &cimp_temp_value); \
    } while (0)

#define stack_peek_as(type, stack) \
    ((type *)stack_peek((stack)))


// Sorting / Search
typedef int (*CimpCompare)(
    const void *a,
    const void *b
);

void quick_sort(
    void *base,
    size_t count,
    size_t item_size,
    CimpCompare compare
);

void merge_sort(
    void *base,
    size_t count,
    size_t item_size,
    CimpCompare compare
);

const void *binary_search(
    const void *key,
    const void *base,
    size_t count,
    size_t item_size,
    CimpCompare compare
);


// File Handling
typedef struct {
    void *data;
    size_t size;
} FileData;

/*
    Read an entire file.

        FileData file = file_read("test.txt");

        printf("%s\n", (char *)file.data);

        file_free(&file);
*/

FileData file_read(const char *filename);

bool file_write(
    const char *filename,
    const void *data,
    size_t size
);

size_t file_size(const char *filename);

void file_free(FileData *file);


// Better Random
void random_seed(unsigned int seed);

int random_int(
    int min_value,
    int max_value
);

float random_float(
    float min_value,
    float max_value
);


// Vectors
typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

// Vec2
Vec2 vec2_add(Vec2 a, Vec2 b);

Vec2 vec2_sub(Vec2 a, Vec2 b);

Vec2 vec2_scale(Vec2 vector, float scalar);

float vec2_dot(Vec2 a, Vec2 b);

float vec2_length(Vec2 vector);

Vec2 vec2_normalize(Vec2 vector);

float vec2_distance(Vec2 a, Vec2 b);

Vec2 vec2_lerp(Vec2 a, Vec2 b, float t);

// Vec3
Vec3 vec3_add(Vec3 a, Vec3 b);

Vec3 vec3_sub(Vec3 a, Vec3 b);

Vec3 vec3_scale(Vec3 vector, float scalar);

float vec3_dot(Vec3 a, Vec3 b);

float vec3_length(Vec3 vector);

Vec3 vec3_normalize(Vec3 vector);

float vec3_distance(Vec3 a, Vec3 b);

Vec3 vec3_lerp(Vec3 a, Vec3 b, float t);

// Logging
void cimp_log_info(
    const char *file,
    int line,
    const char *format,
    ...
);

void cimp_log_warn(
    const char *file,
    int line,
    const char *format,
    ...
);

void cimp_log_error(
    const char *file,
    int line,
    const char *format,
    ...
);

#define log_info(...) \
    cimp_log_info(__FILE__, __LINE__, __VA_ARGS__)

#define log_warn(...) \
    cimp_log_warn(__FILE__, __LINE__, __VA_ARGS__)

#define log_error(...) \
    cimp_log_error(__FILE__, __LINE__, __VA_ARGS__)


// IMPLEMENTATION
#ifdef CIMP_IMPLEMENTATION

#include <stdarg.h>

// Internal Helpers
static bool cimp_resize(
    void **data,
    size_t *capacity,
    size_t item_size,
    size_t required
)
{
    if (required <= *capacity)
        return true;

    size_t new_capacity =
        (*capacity == 0)
        ? CIMP_INITIAL_CAPACITY
        : *capacity;

    while (new_capacity < required)
        new_capacity *= 2;

    void *new_data = realloc(
        *data,
        new_capacity * item_size
    );

    if (!new_data)
        return false;

    *data = new_data;
    *capacity = new_capacity;

    return true;
}

static char *cimp_strdup(const char *str) {
    if (!str)
        return NULL;

    size_t length = strlen(str);

    char *copy = malloc(length + 1);

    if (!copy)
        return NULL;

    memcpy(copy, str, length + 1);

    return copy;
}

// List Implemenatation
List cimp_list_new(size_t item_size) {
    List list = {0};

    list.item_size = item_size;

    return list;
}

bool list_append(
    List *list,
    const void *item
)
{
    if (!list || !item || list->item_size == 0)
        return false;

    if (!cimp_resize(
        &list->items,
        &list->capacity,
        list->item_size,
        list->count + 1
    ))
        return false;

    memcpy(
        (char *)list->items +
        list->count * list->item_size,
        item,
        list->item_size
    );

    list->count++;

    return true;
}

bool list_insert(
    List *list,
    size_t index,
    const void *item
)
{
    if (!list || !item)
        return false;

    if (index > list->count)
        return false;

    if (!cimp_resize(
        &list->items,
        &list->capacity,
        list->item_size,
        list->count + 1
    ))
        return false;

    void *position =
        (char *)list->items +
        index * list->item_size;

    size_t move_count =
        list->count - index;

    memmove(
        (char *)position + list->item_size,
        position,
        move_count * list->item_size
    );

    memcpy(
        position,
        item,
        list->item_size
    );

    list->count++;

    return true;
}

bool list_remove(
    List *list,
    size_t index
)
{
    if (!list)
        return false;

    if (index >= list->count)
        return false;

    void *position =
        (char *)list->items +
        index * list->item_size;

    size_t move_count =
        list->count - index - 1;

    memmove(
        position,
        (char *)position + list->item_size,
        move_count * list->item_size
    );

    list->count--;

    return true;
}

bool list_pop(
    List *list,
    void *out_item
)
{
    if (!list || list->count == 0)
        return false;

    size_t index = list->count - 1;

    void *item =
        (char *)list->items +
        index * list->item_size;

    if (out_item) {
        memcpy(
            out_item,
            item,
            list->item_size
        );
    }

    list->count--;

    return true;
}

void list_shuffle(List *list) {
    if (!list || list->count < 2)
        return;

    char *temp = malloc(list->item_size);

    if (!temp)
        return;

    for (
        size_t i = list->count - 1;
        i > 0;
        i--
    )
    {
        size_t j =
            (size_t)random_int(0, (int)i);

        void *a =
            (char *)list->items +
            i * list->item_size;

        void *b =
            (char *)list->items +
            j * list->item_size;

        memcpy(temp, a, list->item_size);
        memcpy(a, b, list->item_size);
        memcpy(b, temp, list->item_size);
    }

    free(temp);
}

void list_clear(List *list) {
    if (!list)
        return;

    list->count = 0;
}

void list_free(List *list) {
    if (!list)
        return;

    free(list->items);

    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
    list->item_size = 0;
}

// String Implementation
static bool cimp_string_reserve(
    String *string,
    size_t required
)
{
    return cimp_resize(
        (void **)&string->data,
        &string->capacity,
        sizeof(char),
        required
    );
}

String string_new(void) {
    String string = {0};

    cimp_string_reserve(&string, 1);

    if (string.data)
        string.data[0] = '\0';

    return string;
}

String string_from(const char *str) {
    String string = {0};

    if (!str)
        return string;

    size_t length = strlen(str);

    if (!cimp_string_reserve(
        &string,
        length + 1
    ))
        return string;

    memcpy(
        string.data,
        str,
        length + 1
    );

    string.length = length;

    return string;
}

bool string_append(
    String *string,
    const char *text
)
{
    if (!string || !text)
        return false;

    size_t text_length = strlen(text);

    if (!cimp_string_reserve(
        string,
        string->length +
        text_length +
        1
    ))
        return false;

    memcpy(
        string->data + string->length,
        text,
        text_length + 1
    );

    string->length += text_length;

    return true;
}

bool string_append_char(
    String *string,
    char character
)
{
    if (!string)
        return false;

    if (!cimp_string_reserve(
        string,
        string->length + 2
    ))
        return false;

    string->data[string->length] = character;

    string->length++;

    string->data[string->length] = '\0';

    return true;
}

bool string_insert(
    String *string,
    size_t index,
    const char *text
)
{
    if (!string || !text)
        return false;

    if (index > string->length)
        return false;

    size_t text_length = strlen(text);

    if (!cimp_string_reserve(
        string,
        string->length +
        text_length +
        1
    ))
        return false;

    memmove(
        string->data +
        index +
        text_length,

        string->data + index,

        string->length -
        index +
        1
    );

    memcpy(
        string->data + index,
        text,
        text_length
    );

    string->length += text_length;

    return true;
}

bool string_remove(
    String *string,
    size_t index,
    size_t count
)
{
    if (!string)
        return false;

    if (index >= string->length)
        return false;

    if (index + count > string->length)
        count = string->length - index;

    memmove(
        string->data + index,

        string->data +
        index +
        count,

        string->length -
        index -
        count +
        1
    );

    string->length -= count;

    return true;
}

List string_split(
    const String *string,
    const char *delimiter
)
{
    List result =
        cimp_list_new(sizeof(String));

    if (!string ||
        !string->data ||
        !delimiter ||
        delimiter[0] == '\0')
    {
        return result;
    }

    size_t delimiter_length =
        strlen(delimiter);

    char *start = string->data;

    while (1)
    {
        char *found =
            strstr(start, delimiter);

        size_t length;

        if (found)
            length =
                (size_t)(found - start);
        else
            length =
                strlen(start);

        String part = {0};

        if (cimp_string_reserve(
            &part,
            length + 1
        ))
        {
            memcpy(
                part.data,
                start,
                length
            );

            part.data[length] = '\0';
            part.length = length;

            list_append(
                &result,
                &part
            );
        }

        if (!found)
            break;

        start =
            found +
            delimiter_length;
    }

    return result;
}

void string_upper(String *string) {
    if (!string || !string->data)
        return;

    for (
        size_t i = 0;
        i < string->length;
        i++
    )
    {
        if (
            string->data[i] >= 'a' &&
            string->data[i] <= 'z'
        )
        {
            string->data[i] -=
                ('a' - 'A');
        }
    }
}

void string_lower(String *string) {
    if (!string || !string->data)
        return;

    for (
        size_t i = 0;
        i < string->length;
        i++
    )
    {
        if (
            string->data[i] >= 'A' &&
            string->data[i] <= 'Z'
        )
        {
            string->data[i] +=
                ('a' - 'A');
        }
    }
}

void string_free(String *string) {
    if (!string)
        return;

    free(string->data);

    string->data = NULL;
    string->length = 0;
    string->capacity = 0;
}

// Hash Map Implementation
static uint64_t cimp_hash(
    const char *string
)
{
    uint64_t hash =
        1469598103934665603ULL;

    while (*string) {
        hash ^=
            (unsigned char)*string++;

        hash *=
            1099511628211ULL;
    }

    return hash;
}

HashMap cimp_hashmap_new(
    size_t value_size
)
{
    HashMap map = {0};

    map.value_size = value_size;

    map.capacity = CIMP_INITIAL_CAPACITY;

    map.entries = calloc(
        map.capacity,
        sizeof(HashMapEntry)
    );

    if (!map.entries)
        map.capacity = 0;

    return map;
}

static bool cimp_hashmap_resize(
    HashMap *map,
    size_t new_capacity
)
{
    HashMapEntry *old_entries =
        map->entries;

    size_t old_capacity =
        map->capacity;

    HashMapEntry *new_entries =
        calloc(
            new_capacity,
            sizeof(HashMapEntry)
        );

    if (!new_entries)
        return false;

    map->entries = new_entries;
    map->capacity = new_capacity;
    map->count = 0;

    for (
        size_t i = 0;
        i < old_capacity;
        i++
    )
    {
        if (!old_entries[i].occupied)
            continue;

        uint64_t hash =
            cimp_hash(
                old_entries[i].key
            );

        size_t index =
            hash % map->capacity;

        while (
            map->entries[index].occupied
        )
        {
            index =
                (index + 1) %
                map->capacity;
        }

        map->entries[index] =
            old_entries[i];

        map->count++;
    }

    free(old_entries);

    return true;
}

bool hashmap_set(
    HashMap *map,
    const char *key,
    const void *value
)
{
    if (!map ||
        !key ||
        !value ||
        map->value_size == 0)
    {
        return false;
    }

    if (!map->entries)
    {
        *map =
            cimp_hashmap_new(
                map->value_size
            );

        if (!map->entries)
            return false;
    }

    if (
        (map->count + 1) * 100 /
        map->capacity > 70
    )
    {
        if (!cimp_hashmap_resize(
            map,
            map->capacity * 2
        ))
            return false;
    }

    uint64_t hash =
        cimp_hash(key);

    size_t index =
        hash % map->capacity;

    while (
        map->entries[index].occupied
    )
    {
        if (
            strcmp(
                map->entries[index].key,
                key
            ) == 0
        )
        {
            memcpy(
                map->entries[index].value,
                value,
                map->value_size
            );

            return true;
        }

        index =
            (index + 1) %
            map->capacity;
    }

    char *key_copy =
        cimp_strdup(key);

    void *value_copy =
        malloc(map->value_size);

    if (!key_copy || !value_copy) {
        free(key_copy);
        free(value_copy);

        return false;
    }

    memcpy(
        value_copy,
        value,
        map->value_size
    );

    map->entries[index].key =
        key_copy;

    map->entries[index].value =
        value_copy;

    map->entries[index].occupied =
        true;

    map->count++;

    return true;
}

void *cimp_hashmap_get(
    HashMap *map,
    const char *key
)
{
    if (!map ||
        !map->entries ||
        !key)
    {
        return NULL;
    }

    uint64_t hash =
        cimp_hash(key);

    size_t index =
        hash % map->capacity;

    size_t start = index;

    while (
        map->entries[index].occupied
    )
    {
        if (
            strcmp(
                map->entries[index].key,
                key
            ) == 0
        )
        {
            return
                map->entries[index].value;
        }

        index =
            (index + 1) %
            map->capacity;

        if (index == start)
            break;
    }

    return NULL;
}

bool hashmap_remove(
    HashMap *map,
    const char *key
)
{
    if (!map ||
        !map->entries ||
        !key)
    {
        return false;
    }

    void *value =
        cimp_hashmap_get(map, key);

    if (!value)
        return false;

    uint64_t hash =
        cimp_hash(key);

    size_t index =
        hash % map->capacity;

    while (
        map->entries[index].occupied
    )
    {
        if (
            strcmp(
                map->entries[index].key,
                key
            ) == 0
        )
        {
            free(
                map->entries[index].key
            );

            free(
                map->entries[index].value
            );

            map->entries[index].occupied =
                false;

            map->entries[index].key =
                NULL;

            map->entries[index].value =
                NULL;

            map->count--;

            /*
                Rehash everything after this
                entry.
            */

            size_t next =
                (index + 1) %
                map->capacity;

            while (
                map->entries[next].occupied
            )
            {
                HashMapEntry entry =
                    map->entries[next];

                map->entries[next].occupied =
                    false;

                map->count--;

                hashmap_set(
                    map,
                    entry.key,
                    entry.value
                );

                free(entry.key);
                free(entry.value);

                next =
                    (next + 1) %
                    map->capacity;
            }

            return true;
        }

        index =
            (index + 1) %
            map->capacity;
    }

    return false;
}

void hashmap_free(HashMap *map) {
    if (!map)
        return;

    for (
        size_t i = 0;
        i < map->capacity;
        i++
    )
    {
        if (map->entries[i].occupied) {
            free(
                map->entries[i].key
            );

            free(
                map->entries[i].value
            );
        }
    }

    free(map->entries);

    map->entries = NULL;
    map->value_size = 0;
    map->count = 0;
    map->capacity = 0;
}

// Stack Implemenation
Stack cimp_stack_new(
    size_t item_size
)
{
    Stack stack = {0};

    stack.item_size = item_size;

    return stack;
}

bool stack_push(
    Stack *stack,
    const void *item
)
{
    if (!stack || !item)
        return false;

    if (!cimp_resize(
        &stack->items,
        &stack->capacity,
        stack->item_size,
        stack->count + 1
    ))
        return false;

    memcpy(
        (char *)stack->items +
        stack->count *
        stack->item_size,

        item,
        stack->item_size
    );

    stack->count++;

    return true;
}

bool stack_pop(
    Stack *stack,
    void *out_item
)
{
    if (!stack ||
        stack->count == 0)
    {
        return false;
    }

    stack->count--;

    void *item =
        (char *)stack->items +
        stack->count *
        stack->item_size;

    if (out_item) {
        memcpy(
            out_item,
            item,
            stack->item_size
        );
    }

    return true;
}

void *stack_peek(Stack *stack) {
    if (!stack ||
        stack->count == 0)
    {
        return NULL;
    }

    return
        (char *)stack->items +
        (stack->count - 1) *
        stack->item_size;
}

void stack_free(Stack *stack) {
    if (!stack)
        return;

    free(stack->items);

    stack->items = NULL;
    stack->item_size = 0;
    stack->count = 0;
    stack->capacity = 0;
}

// Sorting Implementation
void quick_sort(
    void *base,
    size_t count,
    size_t item_size,
    CimpCompare compare
)
{
    if (!base ||
        !compare ||
        count < 2)
    {
        return;
    }

    qsort(
        base,
        count,
        item_size,
        compare
    );
}

void merge_sort(
    void *base,
    size_t count,
    size_t item_size,
    CimpCompare compare
)
{
    if (!base ||
        !compare ||
        count < 2)
    {
        return;
    }

    char *temp =
        malloc(count * item_size);

    if (!temp)
        return;

    char *array = base;

    for (
        size_t width = 1;
        width < count;
        width *= 2
    )
    {
        for (
            size_t left = 0;
            left < count;
            left += width * 2
        )
        {
            size_t mid =
                min(left + width, count);

            size_t right =
                min(left + width * 2, count);

            size_t i = left;
            size_t j = mid;
            size_t k = left;

            while (
                i < mid &&
                j < right
            )
            {
                if (
                    compare(
                        array + i * item_size,
                        array + j * item_size
                    ) <= 0
                )
                {
                    memcpy(
                        temp + k * item_size,
                        array + i * item_size,
                        item_size
                    );

                    i++;
                } else {
                    memcpy(
                        temp + k * item_size,
                        array + j * item_size,
                        item_size
                    );

                    j++;
                }

                k++;
            }

            while (i < mid) {
                memcpy(
                    temp + k * item_size,
                    array + i * item_size,
                    item_size
                );

                i++;
                k++;
            }

            while (j < right) {
                memcpy(
                    temp + k * item_size,
                    array + j * item_size,
                    item_size
                );

                j++;
                k++;
            }
        }

        memcpy(
            array,
            temp,
            count * item_size
        );
    }

    free(temp);
}

const void *binary_search(
    const void *key,
    const void *base,
    size_t count,
    size_t item_size,
    CimpCompare compare
)
{
    if (!key ||
        !base ||
        !compare)
    {
        return NULL;
    }

    return bsearch(
        key,
        base,
        count,
        item_size,
        compare
    );
}


/* ============================================================
   FILE IMPLEMENTATION
   ============================================================ */

FileData file_read(
    const char *filename
)
{
    FileData result = {0};

    if (!filename)
        return result;

    FILE *file =
        fopen(filename, "rb");

    if (!file)
        return result;

    fseek(file, 0, SEEK_END);

    long size = ftell(file);

    rewind(file);

    if (size < 0) {
        fclose(file);
        return result;
    }

    result.data =
        malloc((size_t)size + 1);

    if (!result.data) {
        fclose(file);
        return result;
    }

    size_t read =
        fread(
            result.data,
            1,
            (size_t)size,
            file
        );

    fclose(file);

    if (read != (size_t)size) {
        free(result.data);

        result.data = NULL;

        return result;
    }

    ((char *)result.data)[size] =
        '\0';

    result.size = (size_t)size;

    return result;
}

bool file_write(
    const char *filename,
    const void *data,
    size_t size
)
{
    if (!filename || !data)
        return false;

    FILE *file =
        fopen(filename, "wb");

    if (!file)
        return false;

    size_t written =
        fwrite(
            data,
            1,
            size,
            file
        );

    fclose(file);

    return written == size;
}

size_t file_size(
    const char *filename
)
{
    if (!filename)
        return 0;

    FILE *file =
        fopen(filename, "rb");

    if (!file)
        return 0;

    fseek(file, 0, SEEK_END);

    long size = ftell(file);

    fclose(file);

    if (size < 0)
        return 0;

    return (size_t)size;
}

void file_free(FileData *file) {
    if (!file)
        return;

    free(file->data);

    file->data = NULL;
    file->size = 0;
}


// Random Implemenation
void random_seed(unsigned int seed) {
    srand(seed);
}

int random_int(
    int min_value,
    int max_value
)
{
    if (min_value > max_value) {
        int temp = min_value;

        min_value = max_value;
        max_value = temp;
    }

    return min_value +
        rand() %
        (max_value - min_value + 1);
}

float random_float(
    float min_value,
    float max_value
)
{
    if (min_value > max_value) {
        float temp = min_value;

        min_value = max_value;
        max_value = temp;
    }

    float t =
        (float)rand() /
        (float)RAND_MAX;

    return
        min_value +
        (max_value - min_value) * t;
}

// Vec2 Implementation
Vec2 vec2_add(Vec2 a, Vec2 b) {
    return (Vec2) {
        a.x + b.x,
        a.y + b.y
    };
}

Vec2 vec2_sub(Vec2 a, Vec2 b) {
    return (Vec2) {
        a.x - b.x,
        a.y - b.y
    };
}

Vec2 vec2_scale(
    Vec2 vector,
    float scalar
)
{
    return (Vec2) {
        vector.x * scalar,
        vector.y * scalar
    };
}

float vec2_dot(Vec2 a, Vec2 b) {
    return
        a.x * b.x +
        a.y * b.y;
}

float vec2_length(Vec2 vector) {
    return sqrtf(
        vector.x * vector.x +
        vector.y * vector.y
    );
}

Vec2 vec2_normalize(Vec2 vector) {
    float length =
        vec2_length(vector);

    if (length == 0.0f) {
        return (Vec2) {
            0.0f,
            0.0f
        };
    }

    return vec2_scale(
        vector,
        1.0f / length
    );
}

float vec2_distance(
    Vec2 a,
    Vec2 b
)
{
    return vec2_length(
        vec2_sub(a, b)
    );
}

Vec2 vec2_lerp(
    Vec2 a,
    Vec2 b,
    float t
)
{
    return (Vec2) {
        lerp(a.x, b.x, t),
        lerp(a.y, b.y, t)
    };
}

// Vec3 Implemenation
Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3) {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3) {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

Vec3 vec3_scale(
    Vec3 vector,
    float scalar
)
{
    return (Vec3) {
        vector.x * scalar,
        vector.y * scalar,
        vector.z * scalar
    };
}

float vec3_dot(Vec3 a, Vec3 b) {
    return
        a.x * b.x +
        a.y * b.y +
        a.z * b.z;
}

float vec3_length(Vec3 vector) {
    return sqrtf(
        vector.x * vector.x +
        vector.y * vector.y +
        vector.z * vector.z
    );
}

Vec3 vec3_normalize(Vec3 vector) {
    float length =
        vec3_length(vector);

    if (length == 0.0f) {
        return (Vec3) {
            0.0f,
            0.0f,
            0.0f
        };
    }

    return vec3_scale(
        vector,
        1.0f / length
    );
}

float vec3_distance(
    Vec3 a,
    Vec3 b
)
{
    return vec3_length(
        vec3_sub(a, b)
    );
}

Vec3 vec3_lerp(
    Vec3 a,
    Vec3 b,
    float t
)
{
    return (Vec3) {
        lerp(a.x, b.x, t),
        lerp(a.y, b.y, t),
        lerp(a.z, b.z, t)
    };
}

// Logging Implemenation
static void cimp_log(
    const char *level,
    const char *file,
    int line,
    const char *format,
    va_list args
)
{
    fprintf(
        stderr,
        "[%s] %s:%d: ",
        level,
        file,
        line
    );

    vfprintf(
        stderr,
        format,
        args
    );

    fprintf(stderr, "\n");
}

void cimp_log_info(
    const char *file,
    int line,
    const char *format,
    ...
)
{
    va_list args;

    va_start(args, format);

    cimp_log(
        "INFO",
        file,
        line,
        format,
        args
    );

    va_end(args);
}

void cimp_log_warn(
    const char *file,
    int line,
    const char *format,
    ...
)
{
    va_list args;

    va_start(args, format);

    cimp_log(
        "WARN",
        file,
        line,
        format,
        args
    );

    va_end(args);
}

void cimp_log_error(
    const char *file,
    int line,
    const char *format,
    ...
)
{
    va_list args;

    va_start(args, format);

    cimp_log(
        "ERROR",
        file,
        line,
        format,
        args
    );

    va_end(args);
}

#endif /* CIMP_IMPLEMENTATION */

#endif /* CIMP_H */
